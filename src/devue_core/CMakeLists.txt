CMAKE_MINIMUM_REQUIRED (VERSION 3.12)

# INCLUDE ALL HEADER FILES SO VS
# CAN STOP FREAKING OUT
FILE(GLOB_RECURSE DEVUE_CORE_HEADERS CONFIGURE_DEPENDS 
	"${CMAKE_SOURCE_DIR}/include/devue_core/*.hpp"
)

ADD_LIBRARY(devue_core STATIC
	${DEVUE_CORE_HEADERS}
	"src/graphics/dv_opengl_window.cpp"
	"src/systems/dv_systems_bundle.cpp"
	"src/systems/dv_sys_model.cpp"
	"src/systems/dv_sys_material.cpp"
	"src/systems/dv_sys_rendering.cpp"
	"src/systems/dv_sys_texture.cpp"
	"src/systems/dv_sys_scene.cpp"
	"src/utilities/dv_util_uuid.cpp"
	"src/utilities/dv_util_diag.cpp"
	"src/rendering/dv_shader.cpp"
	"src/rendering/dv_render_target.cpp"
	"src/rendering/dv_multisample_frame_buffer.cpp"
	"src/importers/dv_file_type.cpp"
	"src/scene/dv_scene.cpp"
	"src/scene/dv_camera.cpp"
	"src/scene/dv_scene_grid.cpp"
)

# COPY RESOURCES TO OUTPUT DIR

SET(RESOURCES_SRC_DIR "${CMAKE_SOURCE_DIR}/resources")
SET(RESOURCES_DST_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")

ADD_CUSTOM_TARGET(copy_resources ALL
    COMMAND robocopy ${RESOURCES_SRC_DIR} ${RESOURCES_DST_DIR} /MIR /NP > NUL 2>&1 || exit 0
)

ADD_DEPENDENCIES(copy_resources devue_core)

TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${DEVUE_LIB_DIR}/glad/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${DEVUE_LIB_DIR}/khr/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${DEVUE_LIB_DIR}/glfw/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${DEVUE_LIB_DIR}/imgui/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${DEVUE_LIB_DIR}/glm/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${DEVUE_LIB_DIR}")

TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${DEVUE_INCLUDE_DIR}/devue_core")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${DEVUE_INCLUDE_DIR}/devue_shared")