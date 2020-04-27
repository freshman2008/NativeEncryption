LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/ \
$(LOCAL_PATH)/openssl/$(TARGET_ARCH_ABI)/include/

LOCAL_MODULE := HelloJNI
LOCAL_SRC_FILES := EncryptionCoreWrapper.c
LOCAL_STATIC_LIBRARIES	:= ssl crypto
LOCAL_LDLIBS    := -llog -landroid
include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH))
$(call import-module,openssl)