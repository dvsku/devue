#include "systems/dv_sys_model.hpp"
#include "obj_loader/include/obj_loader.h"

#include <algorithm>

using namespace devue::core;

static dv_model _import_obj(const std::filesystem::path& path);

static void set_min_y(dv_model& model);

dv_sys_model::dv_sys_model() {
	attach_importer({
		{devue::core::dv_util_uuid::create(".obj")}, 
		_import_obj
	});
}

bool dv_sys_model::attach_importer(dv_model_importer&& importer) {
	if (!importer.types.size()) return false;
	if (!importer.fn)			return false;

	m_importers.emplace_back(std::move(importer));
	return true;
}

dv_model* dv_sys_model::get(const devue::uuid& uuid) {
	if (!models.contains(uuid)) return nullptr;
	return &models[uuid];
}

dv_model& dv_sys_model::import(const FILE_PATH& path) {
	devue::uuid ex_uuid = devue::core::dv_util_uuid::create(path.extension().string());
	
	auto cmp_fn = [&](const devue::uuid& type) {
		return type == ex_uuid;
	};

	for (auto& importer : m_importers) {
		if (std::none_of(importer.types.begin(), importer.types.end(), cmp_fn))
			continue;
		
		devue::uuid uuid = devue::core::dv_util_uuid::create();

		dv_model model = importer.fn(path);
		models[uuid] = std::move(model);
		models[uuid].uuid = uuid;

		return models[uuid];
	}

	throw std::runtime_error("Unsupported model type");
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