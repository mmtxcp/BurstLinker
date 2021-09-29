#pragma once


// GIF_EXPORT
#if defined(GIF_STATICLIB) || defined(GIF_SOURCE)
#define GIF_EXPORT
#elif defined(_MSC_VER)
#if defined(GIF_DYNAMICLIB) || defined(GIF_EXPORTS) || defined(GIF_EXPORTS)
#define GIF_EXPORT  __declspec(dllexport)
#else
#define GIF_EXPORT  __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#define GIF_EXPORT  __attribute__((visibility("default")))
#else
#define GIF_EXPORT
#endif