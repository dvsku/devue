#pragma once

#include "utilities/dv_util_uuid.hpp"

#include <cstdint>
#include <typeindex>
#include <unordered_map>

namespace devue {
	class dv_selected {
	public:
		enum class obj_type : uint8_t {
			none		= 0x0,
			model		= 0x1,
			scene_model = 0x2,
			camera		= 0x3
		};

		obj_type type	 = obj_type::none;
		devue::uuid uuid = 0U;

	public:
		dv_selected();

		template<typename T>
		void select(T& obj) {
			if (!m_selection_types.contains(typeid(obj)))
				return;

			select(m_selection_types[typeid(obj)], obj.uuid);
		}

		template<typename T>
		bool in_selected(T& obj) {
			if (type == obj_type::none)						return false;
			if (!m_selection_types.contains(typeid(obj)))	return false;

			return m_selection_types[typeid(obj)] == type && obj.uuid == uuid;
		}

	private:
		std::unordered_map<std::type_index, obj_type> m_selection_types;

	private:
		void select(obj_type type = obj_type::none, devue::uuid uuid = 0U);
	};
}