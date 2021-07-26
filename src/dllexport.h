#ifndef __DLLEXPORT_H__
#define __DLLEXPORT_H__
#if defined(_WIN32)
#ifndef CODEC2_DLL_IMPORT
#    if defined(__GNUC__)
#        define CODEC2_PUBLIC  __attribute__((dllexport))
#        define CODEC2_PRIVATE
#    else
#        define CODEC2_PUBLIC  __declspec(dllexport)
#        define CODEC2_PRIVATE
#    endif
#else
#    if defined(__GNUC__)
#        define CODEC2_PUBLIC  __attribute__((dllimport))
#        define CODEC2_PRIVATE
#    else
#        define CODEC2_PUBLIC  __declspec(dllimport)
#        define CODEC2_PRIVATE
#    endif
#endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#    define CODEC2_PUBLIC  __attribute__((visibility("default")))
#    define CODEC2_PRIVATE __attribute__((visibility("hidden")))
#else
#    define CODEC2_PUBLIC
#    define CODEC2_PRIVATE
#endif
#endif
