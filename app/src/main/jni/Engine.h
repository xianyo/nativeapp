#pragma once

#include <jni.h>
#include <errno.h>

#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
#include <cpu-features.h>
#include <sstream>
#include <algorithm>
#include <thread>

#include <JavaUI.h>
#include <u/libu.h>
#include <NDKHelper.h>
#include <JNIHelper.hpp>

/*
 * Preprocessors
 */

// Class name of helper function
#define HELPER_CLASS_NAME "com.sample.helper.NDKHelper"
// Class name of JUIhelper function
#define JUIHELPER_CLASS_NAME "com.sample.helper.JUIHelper"
// Share object name of helper function library
#define HELPER_CLASS_SONAME "nativeapp"


const int32_t CHOICES_PER_QUESTION = 4;

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

 private:


  void InitUI();
  void EnableUI(bool enable);

  jui_helper::JUIButton *CreateChoiceButton(const char *cap,
                                            jui_helper::JUIButton *left,
                                            float fontSize = 17.f);
  void CheckChoice(jui_helper::JUIButton *selection);

  mutable std::mutex mutex_;

  // GLContext instance
  ndk_helper::GLContext *gl_context_;

  bool initialized_resources_;
  bool has_focus_;

  // Native activity app instance
  android_app *app_;

  // JUI dialog-related UI stuff here
  jui_helper::JUIDialog *dialog_;

  enum BUTTON_INDEX {
    BUTTON_ADVERTISE = 0,
    BUTTON_DISCOVER,
    BUTTON_PLAY_GAME,
    BUTTON_STOP,
    UI_BUTTON_COUNT
  };
  jui_helper::JUIButton *ui_buttons_[UI_BUTTON_COUNT];
  jui_helper::JUITextView *status_text_;

  jui_helper::JUITextView *time_text_;
  jui_helper::JUITextView *math_formula_;
  jui_helper::JUITextView *scores_text_;
  jui_helper::JUIButton *game_buttons_[CHOICES_PER_QUESTION];

};

