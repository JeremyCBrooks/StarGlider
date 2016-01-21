#ifndef _____MY_LOG_H_
#define _____MY_LOG_H_

#ifdef ANDROID
#include <android/log.h>
#define LOG(...)

#ifdef NDEBUG
#define LOGD(...)
#else
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "StarGliderNative", __VA_ARGS__)
#endif


#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "StarGliderNative", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "StarGliderNative", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, "StarGliderNative", __VA_ARGS__)
#else
#define LOG(...)
#define LOGD(...) fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define LOGI(...) fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define LOGE(...) fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#define LOGW(...) fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n")
#endif

#endif /* MY_LOG_H_ */
