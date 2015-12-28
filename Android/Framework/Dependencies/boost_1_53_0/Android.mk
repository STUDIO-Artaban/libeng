LOCAL_PATH:= $(call my-dir)

# boost_atomic ######################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_atomic
LOCAL_SRC_FILES:= android/lib/libboost_atomic.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_chrono ######################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_chrono
LOCAL_SRC_FILES:= android/lib/libboost_chrono.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_context #####################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_context
LOCAL_SRC_FILES:= android/lib/libboost_context.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_date_time ###################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_date_time
LOCAL_SRC_FILES:= android/lib/libboost_date_time.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_exception ###################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_exception
LOCAL_SRC_FILES:= android/lib/libboost_exception.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_filesystem ##################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_filesystem
LOCAL_SRC_FILES:= android/lib/libboost_filesystem.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_graph #######################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_graph
LOCAL_SRC_FILES:= android/lib/libboost_graph.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_iostreams ###################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_iostreams
LOCAL_SRC_FILES:= android/lib/libboost_iostreams.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_math_c99 ####################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_math_c99
LOCAL_SRC_FILES:= android/lib/libboost_math_c99.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_math_c99f ###################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_math_c99f
LOCAL_SRC_FILES:= android/lib/libboost_math_c99f.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_math_c99l ###################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_math_c99l
LOCAL_SRC_FILES:= android/lib/libboost_math_c99l.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_math_tr1 ####################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_math_tr1
LOCAL_SRC_FILES:= android/lib/libboost_math_tr1.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_math_tr1f ###################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_math_tr1f
LOCAL_SRC_FILES:= android/lib/libboost_math_tr1f.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_math_tr1l ###################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_math_tr1l
LOCAL_SRC_FILES:= android/lib/libboost_math_tr1l.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_prg_exec_monitor ############################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_prg_exec_monitor
LOCAL_SRC_FILES:= android/lib/libboost_prg_exec_monitor.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_program_options #############################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_program_options
LOCAL_SRC_FILES:= android/lib/libboost_program_options.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_random ######################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_random
LOCAL_SRC_FILES:= android/lib/libboost_random.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_regex #######################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_regex
LOCAL_SRC_FILES:= android/lib/libboost_regex.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_signals #####################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_signals
LOCAL_SRC_FILES:= android/lib/libboost_signals.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_system ######################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_system
LOCAL_SRC_FILES:= android/lib/libboost_system.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_test_exec_monitor ###########################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_test_exec_monitor
LOCAL_SRC_FILES:= android/lib/libboost_test_exec_monitor.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_thread ######################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_thread
LOCAL_SRC_FILES:= android/lib/libboost_thread.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_timer #######################################

include $(CLEAR_VARS)

LOCAL_MODULE:= boost_timer
LOCAL_SRC_FILES:= android/lib/libboost_timer.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(PREBUILT_STATIC_LIBRARY)

# boost_unit_test_framework #########################

#include $(CLEAR_VARS)

#LOCAL_MODULE:= boost_unit_test_framework
#LOCAL_SRC_FILES:= android/lib/libboost_unit_test_framework.a
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

#include $(PREBUILT_STATIC_LIBRARY)

# boost_wave ########################################

#include $(CLEAR_VARS)

#LOCAL_MODULE:= boost_wave
#LOCAL_SRC_FILES:= android/lib/libboost_wave.a
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

#include $(PREBUILT_STATIC_LIBRARY)

