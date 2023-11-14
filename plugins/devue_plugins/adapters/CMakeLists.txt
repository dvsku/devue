CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

ADD_LIBRARY(dv_plg_tex_adapter STATIC
	"dv_plugin_adapter.cpp"
	"dv_texture_plugin_adapter.cpp"
	"dv_model_plugin_adapter.cpp"
)

TARGET_INCLUDE_DIRECTORIES(dv_plg_tex_adapter PRIVATE "${DEVUE_LIB_DIR}/glm/include")