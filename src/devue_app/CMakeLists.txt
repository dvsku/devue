SET(DEVUE_APP_RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc")

ADD_EXECUTABLE(devue 
	${DEVUE_APP_RESOURCES}

    "src/devue_app.cpp"
    "src/gui/dv_gui.cpp"
    "src/gui/base/dv_comp.cpp"   
    "src/gui/components/dv_comp_scene.cpp"
    "src/gui/components/dv_comp_assets.cpp"
    "src/gui/components/dv_comp_hierarchy.cpp"
    "src/gui/components/dv_comp_properties.cpp"
    "src/gui/components/dv_comp_console.cpp"
    "src/gui/components/dv_comp_dockspace.cpp"
	"src/gui/components/dv_comp_meshes.cpp"
	"src/gui/components/dv_comp_texture.cpp"
	"src/gui/modals/dv_modal_import.cpp"
	"src/gui/modals/dv_modal_plugins.cpp"
	"src/gui/modals/dv_modal_about.cpp"
    "src/gui/dv_components.cpp"
    "src/systems/dv_sys_command.cpp"
	"src/systems/dv_sys_properties.cpp"
	"src/systems/dv_systems.cpp"
	"src/utilities/dv_util_command.cpp"
	"src/utilities/dv_util_imgui.cpp"
)

TARGET_LINK_OPTIONS(devue PRIVATE "/SUBSYSTEM:WINDOWS")

TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${DV_GUI_INCLUDES}")
TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${DEVUE_LIB_DIR}/glm/include")

TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${DEVUE_INCLUDE_DIR}/devue_core")
TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${DEVUE_LIB_DIR}/plugin_core/include")
TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

TARGET_LINK_LIBRARIES(devue dv_gui_opengl)
TARGET_LINK_LIBRARIES(devue devue_core)
TARGET_LINK_LIBRARIES(devue comctl32.lib)
TARGET_LINK_LIBRARIES(devue propsys.lib)
TARGET_LINK_LIBRARIES(devue shlwapi.lib)