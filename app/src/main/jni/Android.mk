LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := HelloJNI
LOCAL_SRC_FILES := EncryptionCoreWrapper.c
LOCAL_LDLIBS    := -llog -landroid
include $(BUILD_SHARED_LIBRARY)