# Copyright 2013 The Android Open Source Project

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	syspropbin.cpp \
	run_mode_android.cpp \
	SysPropertiesRegistrar.cpp

LOCAL_MODULE := syspropbin
LOCAL_MODULE_TAGS := optional
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)

LOCAL_CFLAGS := -D__STDC_LIMIT_MACROS -Werror


LOCAL_STATIC_LIBRARIES := libsyspropservice libbinder libminui  libz libutils libstdc++ libcutils liblog libm libc

ifeq ($(strip $(BOARD_CHARGER_ENABLE_SUSPEND)),true)
LOCAL_STATIC_LIBRARIES += libsuspend
endif


include $(BUILD_EXECUTABLE)


