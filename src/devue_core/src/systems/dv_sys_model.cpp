#include "systems/dv_sys_model.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "dv_plugin_model.hpp"
#include "utilities/dv_util_string.hpp"

#include <algorithm>
#include <filesystem>

using namespace devue::core;
using namespace devue::plugins;

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

static void set_min_y(dv_model& model);

static bool compare_file_filters(const dv_file_filter& a, const dv_file_filter& b) {
    return a.name < b.name;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_sys_model::dv_sys_model(dv_systems_bundle* systems) 
    : m_systems(systems) {}

dv_model* dv_sys_model::get(const devue::uuid& uuid) {
    if (!models.contains(uuid)) return nullptr;
    return &models[uuid];
}

size_t dv_sys_model::count() const {
    return models.size();
}

const std::vector<dv_file_filter>& dv_sys_model::get_supported_file_types() const {
    return m_supported_file_types;
}

void dv_sys_model::update_supported_file_types() {
    for (auto& [uuid, plugin] : m_systems->plugin.model_plugins) {
        for (auto& file_type : plugin.supported_file_types) {
            m_supported_file_types.emplace_back(dv_file_filter(file_type));
        }
    }

    std::sort(m_supported_file_types.begin(), m_supported_file_types.end(), compare_file_filters);

    m_supported_file_types.push_back({ L"All files (*.*)\0", L"*.*\0" });
}

dv_model& dv_sys_model::import(const std::string& path, const std::string& texture_path) {
    std::filesystem::path filepath = path;
    std::string ext				   = filepath.extension().string();
    
    // Create uuid from path
    devue::uuid uuid = devue::core::dv_util_uuid::create(path);

    if (models.contains(uuid))
        return models[uuid];

    auto cmp_fn = [&](const dv_file_type& type) {
    	return dv_util_string::contains(type.extensions, ext);
    };

    for (auto& [plugin_uuid, plugin] : m_systems->plugin.model_plugins) {
        if (std::none_of(plugin.supported_file_types.begin(), plugin.supported_file_types.end(), cmp_fn))
            continue;

        dv_plugin_model pmodel;

        try {
            pmodel = plugin.import(path);
        }
        catch (...) {
            plugin.cleanup();
            continue;
        }

        plugin.cleanup();

    	dv_model model;
    	model.uuid        = uuid;
    	model.name        = filepath.filename().replace_extension("").string();
        model.texture_dir = texture_path;
    	
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

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

void set_min_y(dv_model& model) {
    float min_y = 0.0f;
    for (auto& mesh : model.meshes)
    	for (auto& vertex : mesh.second.vertices)
    		if (vertex.position.y < min_y)
    			min_y = vertex.position.y;

    model.min_y = min_y;
}