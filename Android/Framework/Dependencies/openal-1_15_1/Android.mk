LOCAL_PATH:= $(call my-dir)

# openal ###############################

include $(CLEAR_VARS)

LOCAL_MODULE := openal
LOCAL_SRC_FILES := $(LOCAL_PATH)/build/libopenal.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

include $(PREBUILT_SHARED_LIBRARY)

