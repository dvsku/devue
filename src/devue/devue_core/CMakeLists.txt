CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

ADD_LIBRARY(devue_core STATIC
	"src/graphics/dv_opengl_window.cpp"
	"src/systems/dv_sys_model.cpp"
	"src/systems/dv_sys_material.cpp"
	"src/systems/dv_sys_rendering.cpp"
	"src/systems/dv_sys_texture.cpp"
	"src/utilities/dv_util_uuid.cpp"
	"src/systems/dv_systems_bundle.cpp"
)

TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/glad/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/khr/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/glfw/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/imgui/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/glm/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib")

TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/include/devue/devue_core")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/include/devue_shared")