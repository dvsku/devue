CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

ADD_LIBRARY(devue_plugin_core STATIC
	"devue_plugin_base.cpp"
)

TARGET_INCLUDE_DIRECTORIES(devue_plugin_core PRIVATE "${DEVUE_LIB_DIR}/glm/include")
TARGET_INCLUDE_DIRECTORIES(devue_plugin_core PRIVATE "${DEVUE_LIB_DIR}/json/include")