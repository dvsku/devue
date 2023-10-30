CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

ADD_LIBRARY(imgui STATIC
	"src/imgui.cpp"
	"src/imgui_demo.cpp"
	"src/imgui_draw.cpp"
	"src/imgui_tables.cpp"
	"src/imgui_widgets.cpp"
	"src/backends/imgui_impl_glfw.cpp"
	"src/backends/imgui_impl_opengl3.cpp"
	"src/misc/cpp/imgui_stdlib.cpp"
)

TARGET_INCLUDE_DIRECTORIES(imgui PRIVATE "${CMAKE_SOURCE_DIR}/lib/imgui/include")
TARGET_INCLUDE_DIRECTORIES(imgui PRIVATE "${CMAKE_SOURCE_DIR}/lib/imgui/include/backends")
TARGET_INCLUDE_DIRECTORIES(imgui PRIVATE "${CMAKE_SOURCE_DIR}/lib/imgui/include/misc/cpp")
TARGET_INCLUDE_DIRECTORIES(imgui PRIVATE "${CMAKE_SOURCE_DIR}/lib/glfw/include")
TARGET_INCLUDE_DIRECTORIES(imgui PRIVATE "${CMAKE_SOURCE_DIR}/lib/glad/include")
TARGET_INCLUDE_DIRECTORIES(imgui PRIVATE "${CMAKE_SOURCE_DIR}/lib/khr/include")

TARGET_COMPILE_DEFINITIONS(imgui PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM)
