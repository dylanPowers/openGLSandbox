LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := renderer
LOCAL_SRC_FILES := renderer.cpp jni.cpp
LOCAL_LDLIBS    := -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)