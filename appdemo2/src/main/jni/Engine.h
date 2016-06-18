#ifndef ENGINE_H_
#define ENGINE_H_

#pragma once

#include <jni.h>
#include <errno.h>

#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
//#include <cpu-features.h>
#include <sstream>
#include <algorithm>
#include <thread>
#include <dlfcn.h>

#include <JavaUI.h>
#include <u/libu.h>
#include <NDKHelper.h>
#include <JNIHelper.hpp>
#include <android/input.h>

#include "log.h"
/*
 * Preprocessors
 */

// Class name of helper function
#define HELPER_CLASS_NAME "com.sample.helper.NDKHelper"
// Class name of JUIhelper function
#define JUIHELPER_CLASS_NAME "com.sample.helper.JUIHelper"
// Share object name of helper function library
#define HELPER_CLASS_SONAME "nativeapp"

/*
 * Engine class of the sample: my class should be IEndpointDiscoverListener()
 */

class Engine {
 public:

  // Event handling
  static void HandleCmd(struct android_app *app, int32_t cmd);
  static int32_t HandleInput(android_app *app, AInputEvent *event);

  // Engine life cycles
  Engine();
  ~Engine();
  void SetState(android_app *state);
  void InitDisplay(const int32_t cmd);
  void DrawFrame();
  void TermDisplay(const int32_t cmd);
  bool IsReady();
  void InitUI();
  bool running_;
 private:

  void EnableUI(bool enable);

  mutable std::mutex mutex_;

  // GLContext instance
  ndk_helper::GLContext *gl_context_;

  bool initialized_resources_;
  bool has_focus_;

  // Native activity app instance
  android_app *app_;

  // JUI dialog-related UI stuff here
  jui_helper::JUIDialog *dialog_;

  jui_helper::JUIButton *ui_button;
};

#ifdef __cplusplus
extern "C" {
#endif
void android_main_app(android_app *state);
void android_main(android_app *state);
#ifdef __cplusplus
}
#endif

#endif