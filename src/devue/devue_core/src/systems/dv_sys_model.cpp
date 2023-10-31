#include "systems/dv_sys_model.hpp"

using namespace devue::core;

int*dv_sys_model::get(const devue::uuid& uuid) {
	if (!models.contains(uuid)) return nullptr;
	return &models[uuid];
}
