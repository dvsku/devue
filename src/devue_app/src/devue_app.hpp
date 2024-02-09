#pragma once

#define DV_TO_STRING_1(s) #s
#define DV_TO_STRING_2(s) DV_TO_STRING_1(s)

#if _WIN64
    #define DV_ENV64
    #define DV_ENV_STR "64-bit"
#else
    #define DV_ENV32
    #define DV_ENV_STR "32-bit"
#endif

#define DV_APP_NAME "devue"
#define DV_WEBSITE  "https://github.com/dvsku/devue"

#define DV_VERSION_MAJOR 0
#define DV_VERSION_MINOR 1
#define DV_VERSION_REV   0

#define DV_VERSION     DV_VERSION_MAJOR, DV_VERSION_MINOR, DV_VERSION_REV, 0
#define DV_VERSION_STR DV_TO_STRING_2(DV_VERSION_MAJOR) "." DV_TO_STRING_2(DV_VERSION_MINOR) "." DV_TO_STRING_2(DV_VERSION_REV) ".0"
