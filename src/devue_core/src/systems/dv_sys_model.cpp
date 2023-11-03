#include "systems/dv_sys_model.hpp"
#include "obj_loader/include/obj_loader.h"
#include "utilities/dv_util_string.hpp"

#include <algorithm>

using namespace devue::core;

static dv_model _import_obj(const std::filesystem::path& path);

static void set_min_y(dv_model& model);

void dv_sys_model::prepare() {
	// TEMP
	m_importers.push_back({
		{ 
			{"Wavefront object", ".obj"}
		},
		_import_obj
	});

	create_filters();
}

dv_model* dv_sys_model::get(const devue::uuid& uuid) {
	if (!models.contains(uuid)) return nullptr;
	return &models[uuid];
}

const std::vector<dv_file_filter>& dv_sys_model::get_import_filters() {
	return m_import_filters;
}

dv_model& dv_sys_model::import(const FILE_PATH& path) {
	std::string ext = path.extension().string();
	
	auto cmp_fn = [&](const dv_file_type& type) {
		return dv_util_string::contains(type.extensions, ext);
	};

	for (auto& importer : m_importers) {
		if (std::none_of(importer.types.begin(), importer.types.end(), cmp_fn))
			continue;
		
		devue::uuid uuid = devue::core::dv_util_uuid::create();

		dv_model model = importer.fn(path);
		models[uuid] = std::move(model);
		models[uuid].uuid = uuid;

		set_min_y(models[uuid]);

		return models[uuid];
	}

	throw std::runtime_error("Unsupported model type");
}

bool compare_file_filters(const dv_file_filter& a, const dv_file_filter& b) {
	return a.name < b.name;
}

void dv_sys_model::create_filters() {
	for (auto& importer : m_importers)
		for (auto& file_type : importer.types)
			m_import_filters.emplace_back(dv_file_filter(file_type));

	std::sort(m_import_filters.begin(), m_import_filters.end(), compare_file_filters);

	m_import_filters.push_back({ L"All files (*.*)\0", L"*.*\0" });
}

dv_model _import_obj(const std::filesystem::path& path) {
	objl::Loader parser;

	if (!parser.LoadFile(path.string()))
		throw std::runtime_error("Failed to load obj.");

	dv_model model;
	model.name = path.filename().string();

	for (auto& material : parser.LoadedMaterials) {
		dv_material m;
		m.name = material.name;
		m.diffuse_texture = material.map_Kd;

		devue::uuid uuid = devue::core::dv_util_uuid::create();
		model.materials[uuid] = m;
	}

	for (auto& lmesh : parser.LoadedMeshes) {
		if ((lmesh.Indices.size() % 3) != 0)
			throw std::runtime_error("Not a triangle mesh, unsupported");

		dv_mesh mesh;
		mesh.name = lmesh.MeshName;

		// Link material
		for (auto& kvp : model.materials) {
			if (kvp.second.name == lmesh.MeshMaterial.name) {
				mesh.material_uuid = kvp.first;
				break;
			}
		}

		for (size_t i = 0; i < lmesh.Indices.size(); i += 3) {
			mesh.faces.push_back({
				lmesh.Indices[i],
				lmesh.Indices[i + 1],
				lmesh.Indices[i + 2],
			});
		}

		for (auto& v : lmesh.Vertices) {
			dv_vertex vertex;

			vertex.position = { v.Position.X, v.Position.Y, v.Position.Z };
			vertex.normal = { v.Normal.X, v.Normal.Y, v.Normal.Z };
			vertex.uv = { v.TextureCoordinate.X, v.TextureCoordinate.Y };

			mesh.vertices.emplace_back(std::move(vertex));
		}

		devue::uuid uuid = devue::core::dv_util_uuid::create(mesh.name);
		model.meshes[uuid] = std::move(mesh);
	}

	return model;
}

void set_min_y(dv_model& model) {
	float min_y = 0.0f;
	for (auto& mesh : model.meshes)
		for (auto& vertex : mesh.second.vertices)
			if (vertex.position.y < min_y)
				min_y = vertex.position.y;

	model.min_y = min_y;
}