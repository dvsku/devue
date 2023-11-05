CMAKE_MINIMUM_REQUIRED (VERSION 3.8)

ADD_LIBRARY(dv_plg_tex_stb SHARED
	"src/dv_plugin_texture_stb.cpp"
)

TARGET_INCLUDE_DIRECTORIES(dv_plg_tex_stb PRIVATE "${DEVUE_INCLUDE_DIR}/devue_plugins")