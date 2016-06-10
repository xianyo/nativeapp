package com.zhuxy.android.nativeapp;

import android.app.NativeActivity;

/**
 * Created by zhuxy on 16-6-11.
 */
public class LauncherNativeActivity extends NativeActivity {
    static {
        System.loadLibrary("nativeapp");
    }

    @Override
    protected void onPause() {
        super.onPause();
        OnPauseHandler();
    }

    native public void OnPauseHandler();
}
