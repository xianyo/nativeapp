package com.zhuxy.android.nativeapp;

import android.app.NativeActivity;

/**
 * Created by zhuxy on 16-6-11.
 */
public class LauncherNativeActivity extends NativeActivity {
    static {
        System.loadLibrary("ndk_helper");
        System.loadLibrary("jui_helper");
        System.loadLibrary("JNIHelper");
        System.loadLibrary("u");
        System.loadLibrary("nativeapp");
    }

    @Override
    protected void onPause() {
        super.onPause();
        OnPauseHandler();
    }

    native public void OnPauseHandler();
}
