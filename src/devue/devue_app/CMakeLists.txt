CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

ADD_EXECUTABLE(devue 
	"src/devue_app.cpp"
	"src/gui/dv_gui.cpp"
	"src/gui/base/dv_comp.cpp"
	"src/gui/components/dv_comp_scene.cpp"
	"src/gui/components/dv_comp_assets.cpp"
	"src/gui/components/dv_comp_hierarchy.cpp"
	"src/gui/components/dv_comp_properties.cpp"
	"src/gui/components/dv_comp_console.cpp"
	"src/gui/components/dv_comp_dockspace.cpp"
	"src/gui/components/dv_components.cpp"
	"src/systems/dv_systems.cpp"
)

TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${CMAKE_SOURCE_DIR}/lib/glad/include")
TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${CMAKE_SOURCE_DIR}/lib/khr/include")
TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${CMAKE_SOURCE_DIR}/lib/glfw/include")
TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${CMAKE_SOURCE_DIR}/lib/imgui/include")
TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${CMAKE_SOURCE_DIR}/lib/glm/include")

TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${CMAKE_SOURCE_DIR}/include/devue/devue_app")
TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${CMAKE_SOURCE_DIR}/include/devue/devue_core")
TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${CMAKE_SOURCE_DIR}/include/devue_shared")
TARGET_INCLUDE_DIRECTORIES(devue PRIVATE "${CMAKE_SOURCE_DIR}/src/devue/devue_app/src")

TARGET_LINK_LIBRARIES(devue devue_core)

TARGET_LINK_LIBRARIES(devue glad)
TARGET_LINK_LIBRARIES(devue glfw)
TARGET_LINK_LIBRARIES(devue imgui)