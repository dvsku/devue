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
	"src/systems/dv_sys_scene.cpp"
	"src/utilities/dv_util_uuid.cpp"
	"src/systems/dv_systems_bundle.cpp"
	"src/importers/dv_file_type.cpp"
	"src/scene/dv_scene.cpp"
	"src/scene/dv_camera.cpp"
)

# COPY RESOURCES TO OUTPUT DIR

SET(RESOURCES_SRC_DIR "${CMAKE_SOURCE_DIR}/resources")
SET(RESOURCES_DST_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")

ADD_CUSTOM_TARGET(copy_resources ALL
    COMMAND robocopy ${RESOURCES_SRC_DIR} ${RESOURCES_DST_DIR} /MIR /NP > NUL 2>&1 || exit 0
)

ADD_DEPENDENCIES(copy_resources devue_core)

TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/glad/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/khr/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/glfw/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/imgui/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/glm/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib")

TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/include/devue/devue_core")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/include/devue_shared")