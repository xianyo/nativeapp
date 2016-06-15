LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= ndk_helper
LOCAL_SRC_FILES:= $(LOCAL_PATH)/JNIHelper.cpp \
 $(LOCAL_PATH)/interpolator.cpp \
 $(LOCAL_PATH)/tapCamera.cpp \
 $(LOCAL_PATH)/gestureDetector.cpp \
 $(LOCAL_PATH)/perfMonitor.cpp \
 $(LOCAL_PATH)/vecmath.cpp \
 $(LOCAL_PATH)/GLContext.cpp \
 $(LOCAL_PATH)/shader.cpp \
 $(LOCAL_PATH)/gl3stub.cpp \
 $(LOCAL_PATH)/sensorManager.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -latomic -llog -landroid -lEGL -lGLESv2

LOCAL_LDLIBS := -latomic -llog -landroid -lEGL -lGLESv2

LOCAL_STATIC_LIBRARIES := cpufeatures android_native_app_glue

LOCAL_CFLAGS += -std=c++11

#hard-fp setting
ifneq ($(filter %armeabi-v7a,$(TARGET_ARCH_ABI)),)
#For now, only armeabi-v7a is supported for hard-fp
#adding compiler/liker flags specifying hard float ABI for user code and math library
LOCAL_CFLAGS += -mhard-float -D_NDK_MATH_NO_SOFTFP=1
LOCAL_EXPORT_CFLAGS += -mhard-float -D_NDK_MATH_NO_SOFTFP=1
LOCAL_EXPORT_LDLIBS += -lm_hard
ifeq (,$(filter -fuse-ld=mcld,$(APP_LDFLAGS) $(LOCAL_LDFLAGS)))
#Supressing warn-mismatch warnings
LOCAL_EXPORT_LDFLAGS += -Wl,--no-warn-mismatch
endif
endif

include $(BUILD_SHARED_LIBRARY)
