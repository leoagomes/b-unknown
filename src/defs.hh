#pragma once

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_WEB, PLATFORM_ANDROID
#define GLSL_VERSION 100
#endif
