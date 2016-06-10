/**
 * Copyright 2015 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*
 * Include files
 */
#include "Engine.h"

const int SELECTION_PER_QUESTION = 4;
const int MATH_GRADE_LEVEL = 2;
/*
 * Ctor
 */
Engine::Engine()
    : initialized_resources_(false),
      has_focus_(false),
      app_(nullptr),
      dialog_(nullptr),
      status_text_(nullptr) {
  gl_context_ = ndk_helper::GLContext::GetInstance();
  memset(ui_buttons_, 0, sizeof(ui_buttons_));
}

/*
 * Dtor
 */
Engine::~Engine() {

  jui_helper::JUIWindow::GetInstance()->Close();
  delete dialog_;
}

/**
 * Initialize an EGL context for the current display.
 */
void Engine::InitDisplay(const int32_t cmd) {
  if (!initialized_resources_) {
    gl_context_->Init(app_->window);
    InitUI();
    initialized_resources_ = true;
  } else {
    if (EGL_SUCCESS != gl_context_->Resume(app_->window)) {
      LOGE("Failed To Re-Initialize OpenGL functions");
    }

    jui_helper::JUIWindow::GetInstance()->Resume(app_->activity, cmd);
  }

  // Enable culling OpenGL state
  glEnable(GL_CULL_FACE);

  // Enabled depth test OpenGL state
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Note that screen size might have been changed
  glViewport(0, 0, gl_context_->GetScreenWidth(),
             gl_context_->GetScreenHeight());
}

/**
 * Just the current frame in the display.
 */
void Engine::DrawFrame() {
  float bkColor = .5f;
  glClearColor(bkColor, bkColor, bkColor, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  gl_context_->Swap();
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void Engine::TermDisplay(const int32_t cmd) {
  gl_context_->Suspend();
  jui_helper::JUIWindow::GetInstance()->Suspend(cmd);
}

/**
 * HandleInput(): processing any gesture event we are interested in. For this
 * App, all event are handled on Java side
 */
int32_t Engine::HandleInput(android_app *app, AInputEvent *event) {
  Engine *eng = reinterpret_cast<Engine *> (app->userData);
  return 0;
}

/**
 * Process the next main command.
 */
void Engine::HandleCmd(struct android_app *app, int32_t cmd) {
  Engine *eng = reinterpret_cast<Engine *> (app->userData);
  LOGI("message %d", cmd);
  switch (cmd) {
    case APP_CMD_INIT_WINDOW:
      if (app->window != NULL) {
        eng->InitDisplay(APP_CMD_INIT_WINDOW);
        eng->DrawFrame();
      }
      break;
    case APP_CMD_TERM_WINDOW:
      // Disconnect all connections before going down

      // Note that JUI helper needs to know if a window has been terminated
      eng->TermDisplay(APP_CMD_TERM_WINDOW);
      eng->has_focus_ = false;
      break;
    case APP_CMD_RESUME:
      jui_helper::JUIWindow::GetInstance()->Resume(app->activity,
                                                   APP_CMD_RESUME);
      // Players need re-connect since we teared down the connection when we
      // went suspension
      eng->EnableUI(true);
      break;
    case APP_CMD_GAINED_FOCUS:
      // Start animation
      eng->has_focus_ = true;
      jui_helper::JUIWindow::GetInstance()->Resume(app->activity,
                                                   APP_CMD_GAINED_FOCUS);
      break;
    case APP_CMD_LOST_FOCUS:
      // Also stop animating.
      eng->has_focus_ = false;
      eng->DrawFrame();
      break;
    case APP_CMD_CONFIG_CHANGED:
      // Configuration changes
      eng->TermDisplay(APP_CMD_CONFIG_CHANGED);
      eng->InitDisplay(APP_CMD_CONFIG_CHANGED);
      break;
    case APP_CMD_DESTROY:
      ndk_helper::JNIHelper::GetInstance()->DetachCurrentThread();
      break;
  }
}

/*
 * Misc
 */
void Engine::SetState(android_app *state) { app_ = state; }

/*
 * disable 3D rendering for now
 */
bool Engine::IsReady() {
  if (has_focus_) return true;

  return false;
}


/*
 * Initialize game management UI,
 * invoking jui_helper functions to create java UIs
 */
void Engine::InitUI() {

  const int32_t LEFT_MARGIN = 20;

  // The window initialization
  jui_helper::JUIWindow::Init(app_->activity, JUIHELPER_CLASS_NAME);

  // Using jui_helper, a support library, to create and bind game management UIs
  int32_t win_width = ANativeWindow_getWidth(app_->window);
  int32_t win_height = ANativeWindow_getHeight(app_->window);

  if (win_height <= 0 || win_width <= 0) {
    LOGE("Failed to get native window size");
    return;
  }
  if (win_height > win_width) {
    int32_t tmp = win_width;
    win_width = win_height;
    win_height = tmp;
  }

  int32_t button_raw_width = win_width / 4;  // we have 4 buttons
  int32_t button_height = win_height / 4;
  int cur_idx = 0;

  // Create 4 buttons to control nearby sign-in
  // The sequence is dictated by enum BUTTON_INDEX,
  // it MUST match the button titles array defined here
  const char *titles[UI_BUTTON_COUNT] = {"Advertise", "Discover", "Play Game",
                                         "Stop"};
  std::function<void(jui_helper::JUIView *, const int32_t)> button_handlers[] = {
          [this](jui_helper::JUIView *button, const int32_t msg) {
            if (msg == jui_helper::JUICALLBACK_BUTTON_UP) {
              LOGE("button 0 ");
            }
          },
          [this](jui_helper::JUIView *button, const int32_t msg) {
            if (msg == jui_helper::JUICALLBACK_BUTTON_UP) {
              LOGE("button 1 ");
            }
          },
          [this](jui_helper::JUIView *button, const int32_t msg) {
            if (msg == jui_helper::JUICALLBACK_BUTTON_UP) {
              LOGE("button 2  ");
            }
          },
          [this](jui_helper::JUIView *button, const int32_t msg) {
            if (msg == jui_helper::JUICALLBACK_BUTTON_UP) {
              LOGE("button 3  ");
            }
          },
      };

  for (cur_idx = 0; cur_idx < UI_BUTTON_COUNT; cur_idx++) {

    jui_helper::JUIButton *button = new jui_helper::JUIButton(titles[cur_idx]);
    button->AddRule(jui_helper::LAYOUT_PARAMETER_CENTER_VERTICAL,
                    jui_helper::LAYOUT_PARAMETER_TRUE);
    button->AddRule(jui_helper::LAYOUT_PARAMETER_ALIGN_PARENT_LEFT,
                    jui_helper::LAYOUT_PARAMETER_TRUE);
    button->SetAttribute("MinimumWidth", button_raw_width - LEFT_MARGIN);
    button->SetAttribute("MinimumHeight", button_height);
    button->SetMargins(LEFT_MARGIN + cur_idx * button_raw_width, 0, 0, 0);
    button->SetCallback(button_handlers[cur_idx]);

    jui_helper::JUIWindow::GetInstance()->AddView(button);
    ui_buttons_[cur_idx] = button;

  }

  status_text_ = new jui_helper::JUITextView("Nearby Connection is Idle");
  status_text_->AddRule(jui_helper::LAYOUT_PARAMETER_ALIGN_PARENT_BOTTOM,
                        jui_helper::LAYOUT_PARAMETER_TRUE);
  status_text_->AddRule(jui_helper::LAYOUT_PARAMETER_CENTER_IN_PARENT,
                        jui_helper::LAYOUT_PARAMETER_TRUE);
  status_text_->SetAttribute("TextSize", jui_helper::ATTRIBUTE_UNIT_SP, 17.f);
  jui_helper::JUIWindow::GetInstance()->AddView(status_text_);

  EnableUI(true);
  LOGI("ui end ... ");
  return;
}

/*
 * Enable/Disable management UI
 */
void Engine::EnableUI(bool enable) {
  LOGI("Updating UI:%d", enable);
  ndk_helper::JNIHelper::GetInstance()->RunOnUiThread([this, enable]() {
    ui_buttons_[BUTTON_ADVERTISE]->SetAttribute(
        "Enabled",
        enable);
    ui_buttons_[BUTTON_DISCOVER]->SetAttribute(
        "Enabled",
        enable );
    ui_buttons_[BUTTON_PLAY_GAME]->SetAttribute(
        "Enabled", enable);
    /*
     * For experimental purpose, Stop button is always enabled
     */
    ui_buttons_[BUTTON_STOP]->SetAttribute("Enabled", true);

    std::string str;
    str += "Nearby Connection Status: Connected Clients = ";

    str = str.substr(0, str.size() - 2);
    status_text_->SetAttribute("Text", const_cast<const char *>(str.c_str()));
  });
}

/*
 * Help function to create(multiple choice buttons)
 */
jui_helper::JUIButton *Engine::CreateChoiceButton(const char *cap,
                                                  jui_helper::JUIButton *left,
                                                  float fontSize) {
  jui_helper::JUIButton *button = new jui_helper::JUIButton(cap);
  if (!button) {
    LOGE("Out of Memory in %s @ line %d", __FILE__, __LINE__);
    return NULL;
  }
  button->SetCallback([this](jui_helper::JUIView *view, const int32_t msg) {
    switch (msg) {
      case jui_helper::JUICALLBACK_BUTTON_UP:
        CheckChoice(static_cast<jui_helper::JUIButton *>(view));

    }
  });
  if (left) button->AddRule(jui_helper::LAYOUT_PARAMETER_RIGHT_OF, left);
  button->AddRule(jui_helper::LAYOUT_PARAMETER_BELOW, math_formula_);
  button->SetAttribute("TextSize", jui_helper::ATTRIBUTE_UNIT_SP, fontSize);
  button->SetAttribute("Padding", 2, 5, 2, 5);
  button->SetMargins(0, 0, 0, 0);
  return button;
}

/*
 * Check the selection is the correct answer and update our local score
 */
void Engine::CheckChoice(jui_helper::JUIButton *selection) {
  int idx = 0;
  while (idx < CHOICES_PER_QUESTION && game_buttons_[idx] != selection) {
    ++idx;
  }
}

/*
 * Our global instance for Game engine
 */
Engine g_engine;

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(android_app *state) {
  app_dummy();

  g_engine.SetState(state);
  u_con("Hello LibU world !");
  // Init helper functions
  ndk_helper::JNIHelper::Init(state->activity, HELPER_CLASS_NAME,
                              HELPER_CLASS_SONAME);

  state->userData = &g_engine;
  state->onAppCmd = Engine::HandleCmd;
  state->onInputEvent = Engine::HandleInput;

  // loop waiting for stuff to do.
  while (1) {
    // Read all pending events.
    int id;
    int events;
    android_poll_source *source;

    // If not animating, we will block forever waiting for events.
    // If animating, we loop until all events are read, then continue
    // to draw the next frame of animation.
    while ((id = ALooper_pollAll(g_engine.IsReady() ? 0 : -1, NULL, &events,
                                 reinterpret_cast<void **>(&source))) >= 0) {
      // Process this event.
      if (source != NULL) source->process(state, source);

      // Check if we are exiting.
      if (state->destroyRequested != 0) {
        g_engine.TermDisplay(APP_CMD_TERM_WINDOW);
        return;
      }
    }

    if (g_engine.IsReady()) {
      // Drawing is throttled to the screen update rate, so there
      // is no need to do timing here.
      g_engine.DrawFrame();
    }
  }
}

extern "C" {
JNIEXPORT void
Java_com_zhuxy_android_nativeapp_LauncherNativeActivity_OnPauseHandler(
    JNIEnv *env) {
  // This call is to suppress 'E/WindowManager(): android.view.WindowLeaked...'
  // errors.
  // Since orientation change events in NativeActivity comes later than
  // expected, we can not dismiss
  // popupWindow gracefully from NativeActivity.
  // So we are releasing popupWindows explicitly triggered from Java callback
  // through JNI call.
  jui_helper::JUIWindow::GetInstance()->Suspend(APP_CMD_PAUSE);
}
}

