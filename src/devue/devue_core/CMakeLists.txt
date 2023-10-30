CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

ADD_LIBRARY(devue_core STATIC
	"src/graphics/dv_opengl_window.cpp"
)

TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/glad/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/khr/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/glfw/include")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/lib/imgui/include")

TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/include/devue/devue_core")
TARGET_INCLUDE_DIRECTORIES(devue_core PRIVATE "${CMAKE_SOURCE_DIR}/include/devue_shared")