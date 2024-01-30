#include "devue_plugin_model.hpp"
#include "systems/dv_sys_model.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "utilities/dv_util_log.hpp"
#include "utilities/dv_util_string.hpp"

#include <algorithm>
#include <filesystem>
#include <sstream>

using namespace devue::core;
using namespace devue::plugins;

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

static void calculate_bounding_box(dv_model& model);

static bool compare_file_filters(const dv_file_filter& a, const dv_file_filter& b) {
    return a.name < b.name;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_sys_model::dv_sys_model(dv_systems_bundle* systems) 
    : m_systems(systems) {}

const std::vector<dv_file_filter>& dv_sys_model::get_supported_file_types() const {
    return m_supported_file_types;
}

void dv_sys_model::update_supported_file_types() {
    for (auto& [uuid, plugin] : m_systems->plugin.plugins) {
        for (auto& file_type : plugin.supported_model_types) {
            m_supported_file_types.emplace_back(dv_file_filter(file_type));
        }
    }

    std::sort(m_supported_file_types.begin(), m_supported_file_types.end(), compare_file_filters);

    m_supported_file_types.push_back({ L"All files (*.*)\0", L"*.*\0" });
}

bool dv_sys_model::is_supported_file_type(const std::string& path) {
    std::filesystem::path filepath = path;

    if (!std::filesystem::is_regular_file(filepath))
        return false;

    std::string ext = filepath.extension().string();

    auto cmp_fn = [&](const dv_file_type& type) {
        return dv_util_string::contains(type.extensions, ext);
    };

    for (auto& [plugin_uuid, plugin] : m_systems->plugin.plugins) {
        if (std::none_of(plugin.supported_model_types.begin(), plugin.supported_model_types.end(), cmp_fn))
            continue;
        return true;
    }

    return false;
}

bool dv_sys_model::import(const std::string& path, const std::string& material_path) {
    std::filesystem::path filepath = path;
    std::string ext				   = filepath.extension().string();
    
    // Create uuid from path
    devue::uuid uuid = devue::core::dv_util_uuid::create(path);

    if (models.contains(uuid))
        return true;

    auto cmp_fn = [&](const dv_file_type& type) {
    	return dv_util_string::contains(type.extensions, ext);
    };

    // Flag to check if we tried importing but all importers failed
    bool tried_importing = false;

    // Error messages for all tried importers that failed
    // Ignored if one of them succeeded
    std::stringstream accumulated_errors;

    for (auto& [plugin_uuid, plugin] : m_systems->plugin.plugins) {
        if (std::none_of(plugin.supported_model_types.begin(), plugin.supported_model_types.end(), cmp_fn))
            continue;

        // We have tried importing
        tried_importing = true;

        devue_plugin_model pmodel;
        try {
            pmodel = plugin.import_model(path);
        }
        catch (const std::exception& e) {
            accumulated_errors << DV_FORMAT("\t`{}`: {}", plugin.name, e.what());
            plugin.cleanup();
            continue;
        }
        catch (...) {
            accumulated_errors << DV_FORMAT("\t`{}`: critical failure", plugin.name);
            plugin.cleanup();
            continue;
        }

        plugin.cleanup();

    	dv_model model;
    	model.uuid        = uuid;
    	model.name        = filepath.filename().replace_extension("").string();
        model.texture_dir = material_path;
    	
        for (auto& pvertex : pmodel.vertices) {
            dv_vertex vertex;
            vertex.position = pvertex.position;
            vertex.normal   = pvertex.normal;
            vertex.uv       = pvertex.uv;
            model.vertices.push_back(vertex);
        }

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
    	}

        calculate_bounding_box(model);

    	models[uuid] = std::move(model);
    	return true;
    }

    std::string errors = accumulated_errors.str();
    if (tried_importing && !errors.empty()) {
        DV_LOG("Tried importing model with following importers:\n{}", errors);
    }
    
    DV_LOG("Failed to import `{}` model | No suitable importer found.", filepath.filename().string());
    return false;
}

void dv_sys_model::remove(dv_model& model) {
    if (!models.contains(model.uuid)) return;

    // Mark for removal scene models that use this model asset
    if (m_systems->scene.current_scene) {
        for (auto& [uuid, smodel] : m_systems->scene.current_scene->models) {
            if (smodel.model_uuid != model.uuid) continue;
            m_systems->scene.remove_from_scene(smodel);
        }
    }

    model.marked_for_removal = true;
}

void dv_sys_model::remove_marked_models() {
    try {
        auto it = models.begin();
        while (it != models.end()) {
            if (it->second.marked_for_removal) {
                it = models.erase(it);
            }
            else {
                ++it;
            }
        }
    }
    catch (...) {}
}

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

void calculate_bounding_box(dv_model& model) {
    model.bounding_box.minimum = { FLT_MAX, FLT_MAX, FLT_MAX };
    model.bounding_box.maximum = { std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest() };

    for (const auto& vertex : model.vertices) {
        model.bounding_box.minimum.x = min(model.bounding_box.minimum.x, vertex.position.x);
        model.bounding_box.minimum.y = min(model.bounding_box.minimum.y, vertex.position.y);
        model.bounding_box.minimum.z = min(model.bounding_box.minimum.z, vertex.position.z);

        model.bounding_box.maximum.x = max(model.bounding_box.maximum.x, vertex.position.x);
        model.bounding_box.maximum.y = max(model.bounding_box.maximum.y, vertex.position.y);
        model.bounding_box.maximum.z = max(model.bounding_box.maximum.z, vertex.position.z);
    }
}
