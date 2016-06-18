LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := nativeappdemo2
LOCAL_SRC_FILES :=  Engine.cpp

LOCAL_CFLAGS :=
LOCAL_CPPFLAGS := -std=c++11 

LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2 -latomic -lz
LOCAL_STATIC_LIBRARIES := cpufeatures android_native_app_glue
LOCAL_SHARED_LIBRARIES := libu ndk_helper jui_helper JNIHelper

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_C_INCLUDES := $(LOCAL_PATH)

#hard-fp setting
ifneq ($(filter %armeabi-v7a,$(TARGET_ARCH_ABI)),)
  #For now, only armeabi-v7a is supported for hard-fp
  #adding compiler/liker flags specifying hard float ABI for user code and math library
  LOCAL_CFLAGS += -mhard-float -D_NDK_MATH_NO_SOFTFP=1
  LOCAL_LDLIBS += -lm_hard
  ifeq (,$(filter -fuse-ld=mcld,$(APP_LDFLAGS) $(LOCAL_LDFLAGS)))
    #Supressing warn-mismatch warnings
    LOCAL_LDFLAGS += -Wl,--no-warn-mismatch
  endif
endif

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path,$(LOCAL_PATH)/../../../../nativecommon)
$(call import-module,ndk_helper/jni)
$(call import-module,jui_helper/jni)
$(call import-module,JNIHelper/jni)
$(call import-module,prebuild/libu)
$(call import-module,android/native_app_glue)
$(call import-module,android/cpufeatures)
