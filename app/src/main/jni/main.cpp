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

#include <jni.h>
#include <errno.h>

#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>
#include <android/native_activity.h>
#include <cpu-features.h>
#include <sstream>
#include <algorithm>
#include <thread>
#include <dlfcn.h>

#include <JavaUI.h>
#include <u/libu.h>
#include <NDKHelper.h>
#include <JNIHelper.hpp>

typedef void (*android_main_app_func)(android_app *state);

/*
 * Preprocessors
 */

// Class name of helper function
#define HELPER_CLASS_NAME "com.sample.helper.NDKHelper"
// Class name of JUIhelper function
#define JUIHELPER_CLASS_NAME "com.sample.helper.JUIHelper"
// Share object name of helper function library
#define HELPER_CLASS_SONAME "nativeapp"

enum AndroidAppActivityResults {
    ANDROID_APP_ACTIVITY_RESULT_CANCELED = 0,
    ANDROID_APP_ACTIVITY_RESULT_FIRST_USER = 1,
    ANDROID_APP_ACTIVITY_RESULT_OK = -1,
};

int result = 0;
void* pHandle;

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(android_app *state) {
  app_dummy();
  LOGI("android_main");

  // Init helper functions
    ndk_helper::JNIHelper::Init(state->activity, HELPER_CLASS_NAME);
    char sopath[256];

    memset(sopath,0,256);

    ANativeActivity * const activity = state->activity;
    {
        jobject nativeActivityObject = activity->clazz;
        jclass nativeActivityClass;
        jmethodID getIntentApp;
        JNIEnv *env = activity->env;
        activity->vm->AttachCurrentThread(&env, NULL);
        nativeActivityClass = env->GetObjectClass(nativeActivityObject);
        getIntentApp = env->GetMethodID(nativeActivityClass, "getIntentApp",
                                     "()Ljava/lang/String;");
        jstring appName = (jstring) env->CallObjectMethod(nativeActivityObject, getIntentApp);
        const char *appname = env->GetStringUTFChars(appName, NULL);
        strncpy(sopath,appname,256);
        LOGI("path %s",sopath);
        env->ReleaseStringUTFChars(appName, appname);
        env->DeleteLocalRef(appName);
        activity->vm->DetachCurrentThread();
    }

    pHandle = dlopen(sopath, RTLD_LAZY);  //调用驱动

    if (pHandle != NULL) {

        android_main_app_func func = (android_main_app_func) dlsym(pHandle, "android_main");  //得到函数句柄

        if (func != NULL) {
            LOGI("execute startup_appmain!!");
            func(state);
        }
        else{
            result = 1;
            LOGE("dlsym err! (%s)", dlerror());
        }
        dlclose(pHandle);
    }else{
        LOGE("dlopen err! (%s)", dlerror());
        result = 2;
    }

    LOGI("app exit");

    if(result==0)return;

    // Pass the return code from main back to the Activity.
    {
        jobject nativeActivityObject = activity->clazz;
        jclass nativeActivityClass;
        jmethodID setResult;
        JNIEnv *env = activity->env;
        int returnResult = result == 0 ? ANDROID_APP_ACTIVITY_RESULT_OK :
                           result > 0 ? result + ANDROID_APP_ACTIVITY_RESULT_FIRST_USER :
                           ANDROID_APP_ACTIVITY_RESULT_CANCELED;
        activity->vm->AttachCurrentThread(&env, NULL);
        nativeActivityClass = env->GetObjectClass(nativeActivityObject);
        setResult = env->GetMethodID(nativeActivityClass, "setResult",
                                        "(I)V");
        env->CallVoidMethod(nativeActivityObject, setResult, returnResult);
        activity->vm->DetachCurrentThread();
    }

    // Finish the activity and exit the app.
    for ( ; ; ) {
        struct android_poll_source* source = NULL;
        int looperId;
        int events;
        // Signal app completion.
        ANativeActivity_finish(state->activity);
        // Pump the event loop.
        while ((looperId = ALooper_pollAll(-1, NULL, &events,
                                           (void**)&source)) >= 0) {
            switch (looperId) {
                case LOOPER_ID_MAIN:
                    // drop through
                case LOOPER_ID_INPUT:
                    if (source) {
                        source->process(state, source);
                    }
                    break;
                default:
                    // >= LOOPER_ID_USER so this is a user data source which this doesn't
                    // know how to process.
                    break;
            }
            // If the NativeActivity is waiting for the application thread to
            // complete, exit this function.
            if (state->destroyRequested) {
                return;
            }
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

    LOGI("main OnPauseHandler");
    jui_helper::JUIWindow::GetInstance()->Suspend(APP_CMD_PAUSE);

}
}

