#pragma once

#include <unordered_map>
#include <typeindex>

namespace devue {
    struct dv_systems;

    struct inspectable {
        enum class type : unsigned char {
            none = 0x0,
            model,
            scene_model,
            camera,
            ambient_light,
            directional_light
        };

        inspectable::type inspected_type = inspectable::type::none;
        uint64_t          inspected_id   = 0U;
    };

    class dv_sys_properties {
    public:
        dv_sys_properties() = delete;
        dv_sys_properties(dv_systems* systems);

        dv_sys_properties(const dv_sys_properties&) = delete;
        dv_sys_properties(dv_sys_properties&&)      = delete;

        dv_sys_properties& operator=(const dv_sys_properties&) = delete;
        dv_sys_properties& operator=(dv_sys_properties&&)      = delete;

    public:
        template<typename T>
        void set_inspected(T& obj) {
            if (!m_supported_types.contains(typeid(obj))) return;
            inspect(m_supported_types[typeid(obj)], obj.id);
        }

        template<typename T>
        bool is_inspected(T& obj) {
            if (m_current_inspectable.inspected_type == inspectable::type::none) return false;
            if (!m_supported_types.contains(typeid(obj)))                        return false;

            return m_supported_types[typeid(obj)] == m_current_inspectable.inspected_type
                && obj.id == m_current_inspectable.inspected_id;
        }

        void unset_inspected();
        const inspectable& get_inspected();

    private:
        dv_systems* m_systems = nullptr;

        inspectable m_current_inspectable;
        std::unordered_map<std::type_index, inspectable::type> m_supported_types;

    private:
        void inspect(inspectable::type type = inspectable::type::none, uint64_t id = 0U);
    };
}