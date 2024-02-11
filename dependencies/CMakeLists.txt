INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET OFF)
SET(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(
	dv_gui_opengl
	GIT_REPOSITORY https://github.com/dvsku/dv_gui_opengl.git
	GIT_TAG master
)

FetchContent_MakeAvailable(dv_gui_opengl)

SET(DV_GUI_INCLUDES "${dv_gui_opengl_SOURCE_DIR}/include" PARENT_SCOPE)

FetchContent_Declare(
	devue_plugin_core
	GIT_REPOSITORY https://github.com/dvsku/devue_plugin_core.git
	GIT_TAG master
)

FetchContent_MakeAvailable(devue_plugin_core)

SET(DEVUE_PLUGIN_CORE_INCLUDES "${devue_plugin_core_SOURCE_DIR}/include" PARENT_SCOPE)
