LOCAL_PATH := $(call my-dir)

# libeng ######################################

include $(CLEAR_VARS)

LOCAL_MODULE            := libeng
LOCAL_SHARED_LIBRARIES  := libogg libvorbis openal
LOCAL_SRC_FILES         := $(LOCAL_PATH)/libs/$(TARGET_ARCH_ABI)/libeng.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/jni

include $(PREBUILT_SHARED_LIBRARY)
