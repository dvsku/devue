#pragma once

#include "glm/vec3.hpp"
#include "glm/matrix.hpp"

#include <string>

namespace devue::core {
	class dv_shader {
	public:
		uint32_t shader_id = 0U;

	public:
		dv_shader()					= default;
		dv_shader(const dv_shader&) = delete;
		dv_shader(dv_shader&&)		= default;

		dv_shader& operator=(const dv_shader&) = delete;
		dv_shader& operator=(dv_shader&&)	   = default;

		void use();

		void set(const std::string& name, bool value)		const;
		void set(const std::string& name, int value)		const;
		void set(const std::string& name, float value)		const;
		void set(const std::string& name, glm::vec3 vec)	const;
		void set(const std::string& name, glm::ivec3 vec)	const;
		void set(const std::string& name, glm::vec4 vec)	const;
		void set(const std::string& name, glm::mat3 mat)	const;
		void set(const std::string& name, glm::mat4 mat)	const;
	};
}