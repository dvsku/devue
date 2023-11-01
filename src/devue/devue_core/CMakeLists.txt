CMAKE_MINIMUM_REQUIRED (VERSION 3.12)

# INCLUDE ALL HEADER FILES SO VS
# CAN STOP FREAKING OUT
FILE(GLOB_RECURSE DEVUE_CORE_HEADERS CONFIGURE_DEPENDS 
	"${CMAKE_SOURCE_DIR}/include/devue/devue_core/*.hpp"
)

ADD_LIBRARY(devue_core STATIC
	${DEVUE_CORE_HEADERS}
	"src/graphics/dv_opengl_window.cpp"
	"src/systems/dv_sys_model.cpp"
	"src/systems/dv_sys_material.cpp"
	"src/systems/dv_sys_rendering.cpp"
	"src/systems/dv_sys_texture.cpp"
	"src/utilities/dv_util_uuid.cpp"
	"src/systems/dv_systems_bundle.cpp"
	"src/importers/dv_file_type.cpp"
)

TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/glad/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/khr/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/glfw/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/imgui/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/glm/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib")

TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/include/devue/devue_core")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/include/devue_shared")