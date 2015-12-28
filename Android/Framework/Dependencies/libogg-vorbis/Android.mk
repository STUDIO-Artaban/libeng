LOCAL_PATH:= $(call my-dir)

# libogg ###############################

include $(CLEAR_VARS)

LOCAL_MODULE:= libogg
LOCAL_SRC_FILES:= $(LOCAL_PATH)/libs/armeabi/libogg.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/jni/include

include $(PREBUILT_SHARED_LIBRARY)

# libvorbis ############################

include $(CLEAR_VARS)

LOCAL_MODULE:= libvorbis
LOCAL_SRC_FILES:= $(LOCAL_PATH)/libs/armeabi/libvorbis.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/jni/include

include $(PREBUILT_SHARED_LIBRARY)

