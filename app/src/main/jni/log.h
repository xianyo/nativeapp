#ifndef LOG_ENGINE_H_
#define LOG_ENGINE_H_

#include <jni.h>
#include <errno.h>

#include <android/log.h>

#define ENG_LOG_TAG  "main_engine"//(ndk_helper::JNIHelper::GetInstance()->GetAppName())
#undef LOGI
#undef LOGW
#undef LOGE
#define LOGI(...)                                                              \
  ((void) __android_log_print(                                                 \
      ANDROID_LOG_INFO, ENG_LOG_TAG,    \
      __VA_ARGS__))
#define LOGW(...)                                                              \
  ((void) __android_log_print(                                                 \
      ANDROID_LOG_WARN, ENG_LOG_TAG,    \
      __VA_ARGS__))
#define LOGE(...)                                                              \
  ((void) __android_log_print(                                                 \
      ANDROID_LOG_ERROR, ENG_LOG_TAG,   \
      __VA_ARGS__))


#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

#endif