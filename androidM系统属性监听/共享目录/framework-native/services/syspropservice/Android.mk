LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    SysProperty.cpp \
    ISysPropertiesListener.cpp \
    ISysPropertiesRegistrar.cpp

LOCAL_STATIC_LIBRARIES := \
    libutils \
    libbinder

LOCAL_MODULE:= libsyspropservice

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += -Wall -Werror -Wunused -Wunreachable-code

include $(BUILD_STATIC_LIBRARY)

