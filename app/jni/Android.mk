LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := my_lib

LOCAL_SRC_FILES := my_jni.cpp

include $(BUILD_SHARED_LIBRARY)
