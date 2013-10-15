#ifndef LOG_H
#define LOG_H

#include <android/log.h>

#define LOGD(_tag, ...) ((void) __android_log_print(ANDROID_LOG_DEBUG, _tag, __VA_ARGS__))
#define LOGE(_tag, ...) ((void) __android_log_print(ANDROID_LOG_ERROR, _tag, __VA_ARGS__))

#endif