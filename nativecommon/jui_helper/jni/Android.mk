LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= jui_helper
LOCAL_SRC_FILES:= $(LOCAL_PATH)/JavaUI.cpp \
 $(LOCAL_PATH)/JavaUI_Toast.cpp \
 $(LOCAL_PATH)/JavaUI_Window.cpp \
 $(LOCAL_PATH)/JavaUI_Dialog.cpp \
 $(LOCAL_PATH)/JavaUI_Layouts.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -llog -landroid

LOCAL_SHARED_LIBRARIES := ndk_helper

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

$(call import-module,ndk_helper/jni)
