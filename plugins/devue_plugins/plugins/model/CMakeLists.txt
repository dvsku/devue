CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

ADD_LIBRARY(dv_plg_mod_devue SHARED
	"dv_model_plugin.cpp"
)

TARGET_INCLUDE_DIRECTORIES(dv_plg_mod_devue PRIVATE "${PLUGINS_LIB_DIR}/assimp/include")
TARGET_INCLUDE_DIRECTORIES(dv_plg_mod_devue PRIVATE "${DEVUE_LIB_DIR}/glm/include")

TARGET_LINK_LIBRARIES(dv_plg_mod_devue assimp)