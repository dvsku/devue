#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

namespace devue::core {
    struct dv_transform {
    	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
    	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
    	glm::vec3 scale    = { 1.0f, 1.0f, 1.0f };

    	glm::mat4 get_transform_matrix() {
    		glm::quat combined_rotation = glm::quat({
    			glm::radians(rotation.x),
    			glm::radians(rotation.y),
    			glm::radians(rotation.z),
    		});

    		glm::mat4 transform_matrix = glm::mat4(1.0f);
    		transform_matrix = glm::translate(transform_matrix, position);
    		transform_matrix = transform_matrix * glm::mat4_cast(combined_rotation);
    		return glm::scale(transform_matrix, scale);
    	}

    	glm::mat4 get_normal_matrix() {
    		glm::mat4 transform_matrix = get_transform_matrix();

    		glm::mat3 normal_matrix {
    			glm::normalize(glm::vec3(transform_matrix[0])),
    			glm::normalize(glm::vec3(transform_matrix[1])),
    			glm::normalize(glm::vec3(transform_matrix[2]))
    		};

    		return glm::transpose(glm::inverse(normal_matrix));
    	}
    };
}