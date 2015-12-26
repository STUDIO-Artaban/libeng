LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LS_CPP = $(subst $(1)/,,$(wildcard $(1)/$(2)/*.cpp))

LOCAL_MODULE         := libeng
LOCAL_C_INCLUDES     := $(LOCAL_PATH)
LOCAL_SRC_FILES      := $(call LS_CPP,$(LOCAL_PATH),libeng)                             \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Game)                     \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Game/2D)                  \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Graphic)                  \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Graphic/Bounds)           \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Graphic/Bounds/2D)        \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Graphic/Object)           \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Graphic/Object/2D)        \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Graphic/Text)             \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Graphic/Text/2D)          \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Graphic/Scrolling)        \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Graphic/Scrolling/Panels) \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Graphic/Progress)         \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Inputs)                   \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Inputs/Touch)             \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Inputs/Accel)             \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Gamepads)                 \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Player)                   \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Render)                   \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Render/2D)                \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Textures)                 \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Tools)                    \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Storage)                  \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Intro)                    \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Features)                 \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Features/Camera)          \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Features/Mic)             \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Features/Internet)        \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Features/Bluetooth)       \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Advertising)              \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Social)                   \
                           $(call LS_CPP,$(LOCAL_PATH),libeng/Social/Networks)
LOCAL_CFLAGS           := -fPIC -fexceptions -Wmultichar -ffunction-sections -fdata-sections -std=c++98 -std=gnu++98 -frtti #-fno-rtti
LOCAL_STATIC_LIBRARIES := boost_system boost_thread boost_math_c99f boost_regex
LOCAL_SHARED_LIBRARIES := libogg libvorbis openal
LOCAL_LDLIBS           := -llog -landroid -lEGL -lGLESv2
LOCAL_LDFLAGS          := -shared -Wl -gc-sections

include $(BUILD_SHARED_LIBRARY)

$(call import-module, boost_1_53_0)
$(call import-module, libogg-vorbis)
$(call import-module, openal-1_15_1)
