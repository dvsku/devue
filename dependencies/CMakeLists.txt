INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET OFF)
SET(FETCHCONTENT_UPDATES_DISCONNECTED ON)

###############################################################################
# dv_gui_opengl

SET(DV_GUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
SET(DV_GUI_BUILD_OPENGL2  OFF CACHE BOOL "" FORCE)
SET(DV_GUI_BUILD_OPENGL3   ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    dv_gui_opengl
    URL https://github.com/dvsku/dv_gui_opengl/archive/d8a0c3459843a86e6f09a05df67fe8526b6d883c.zip
)

FetchContent_MakeAvailable(dv_gui_opengl)

SET(DV_GUI_INCLUDES "${dv_gui_opengl_SOURCE_DIR}/include" PARENT_SCOPE)

###############################################################################
# devue_plugin_core

FetchContent_Declare(
    devue_plugin_core
    URL https://github.com/dvsku/devue_plugin_core/archive/0b98f9c842ff28c4afc6a29b0ce96429ae05f209.zip
)

FetchContent_MakeAvailable(devue_plugin_core)

SET(DEVUE_PLUGIN_CORE_INCLUDES "${devue_plugin_core_SOURCE_DIR}/include" PARENT_SCOPE)

###############################################################################
## cmrc

SET(BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    cmrc
    URL https://github.com/vector-of-bool/cmrc/archive/refs/heads/master.zip
)

FetchContent_MakeAvailable(cmrc)

###############################################################################
## devue_official_plugin

IF(DEVUE_BUILD_PLUGINS)
    FetchContent_Declare(
        devue_official_plugin
        URL https://github.com/dvsku/devue_official_plugin/archive/27dbeca6b0af99f5ac84882e2a09ff4da637a410.zip
    )

    FetchContent_MakeAvailable(devue_official_plugin)
ENDIF()
