#include "systems/dv_sys_model.hpp"
#include "model/dv_plugin_model.hpp"
#include "obj_loader/include/obj_loader.h"
#include "utilities/dv_util_string.hpp"

#include <algorithm>
#include <filesystem>

using namespace devue::core;
using namespace devue::shared;

static dv_plugin_model _import_obj(const std::filesystem::path& path);

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

size_t dv_sys_model::count() const {
    return models.size();
}

dv_model* dv_sys_model::get(const devue::uuid& uuid) {
    if (!models.contains(uuid)) return nullptr;
    return &models[uuid];
}

const std::vector<dv_file_filter>& dv_sys_model::get_import_filters() {
    return m_import_filters;
}

dv_model& dv_sys_model::import(const std::string& path) {
    std::filesystem::path filepath = path;
    std::string ext				   = filepath.extension().string();
    
    auto cmp_fn = [&](const dv_file_type& type) {
    	return dv_util_string::contains(type.extensions, ext);
    };

    for (auto& importer : m_importers) {
    	if (std::none_of(importer.types.begin(), importer.types.end(), cmp_fn))
    		continue;
    	
    	dv_plugin_model pmodel = importer.fn(path);
    	dv_model model;

    	// Create uuid from path
    	devue::uuid uuid = devue::core::dv_util_uuid::create(path);

    	model.uuid = uuid;
    	model.name = filepath.filename().replace_extension("").string();
    	
    	for (auto& pmesh : pmodel.meshes) {
    		devue::uuid mesh_uuid	  = dv_util_uuid::create(DV_FORMAT("{}_{}", path, pmesh.name));
    		devue::uuid material_uuid = dv_util_uuid::create(DV_FORMAT("{}_{}", mesh_uuid, pmesh.material.name));

    		model.materials[material_uuid] = dv_material();
    		dv_material& material	 = model.materials[material_uuid];
    		material.diffuse_texture = pmesh.material.diffuse_texture;

    		model.meshes[mesh_uuid] = dv_mesh();
    		dv_mesh& mesh		= model.meshes[mesh_uuid];
    		mesh.name			= pmesh.name;
    		mesh.material_uuid	= material_uuid;
    		
    		for (size_t i = 0; i < pmesh.indices.size(); i += 3) {
    			mesh.faces.push_back({
    				pmesh.indices[i],
    				pmesh.indices[i + 1],
    				pmesh.indices[i + 2],
    			});
    		}

    		for (auto& pvertex : pmesh.vertices) {
    			dv_vertex vertex;
    			vertex.position = pvertex.position;
    			vertex.normal = pvertex.normal;
    			vertex.uv = pvertex.uv;
    			mesh.vertices.push_back(vertex);
    		}
    	}

    	set_min_y(model);

    	models[uuid] = std::move(model);
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

dv_plugin_model _import_obj(const std::filesystem::path& path) {
    objl::Loader parser;

    if (!parser.LoadFile(path.string()))
    	throw std::runtime_error("Failed to load obj.");

    dv_plugin_model model;
    
    for (auto& lmesh : parser.LoadedMeshes) {
    	if ((lmesh.Indices.size() % 3) != 0)
    		throw std::runtime_error("Not a triangle mesh, unsupported");

    	model.meshes.push_back({});
    	dv_plugin_mesh& mesh = model.meshes.back();
    	mesh.name = lmesh.MeshName;

    	mesh.material.name			  = lmesh.MeshMaterial.name;
    	mesh.material.diffuse_texture = lmesh.MeshMaterial.map_Kd;

    	for (auto& index : lmesh.Indices)
    		mesh.indices.push_back(index);

    	for (auto& vertex : lmesh.Vertices) {
    		mesh.vertices.push_back({});
    		dv_plugin_vertex& pv = mesh.vertices.back();

    		pv.position = { vertex.Position.X, vertex.Position.Y, vertex.Position.Z };
    		pv.normal	= { vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z };
    		pv.uv		= { vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y };
    	}
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