CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

ADD_LIBRARY(dv_plg_devue_official SHARED
	"${CMAKE_CURRENT_SOURCE_DIR}/resources/plugin.rc"
	"devue_plugin.cpp"
)

TARGET_LINK_LIBRARIES(dv_plg_devue_official devue_plugin_core)
TARGET_LINK_LIBRARIES(dv_plg_devue_official assimp)

TARGET_INCLUDE_DIRECTORIES(dv_plg_devue_official PRIVATE "${PLUGINS_LIB_DIR}")
TARGET_INCLUDE_DIRECTORIES(dv_plg_devue_official PRIVATE "${PLUGINS_LIB_DIR}/assimp/include")
TARGET_INCLUDE_DIRECTORIES(dv_plg_devue_official PRIVATE "${DEVUE_LIB_DIR}/glm/include")
TARGET_INCLUDE_DIRECTORIES(dv_plg_devue_official PRIVATE "${DEVUE_LIB_DIR}/json/include")