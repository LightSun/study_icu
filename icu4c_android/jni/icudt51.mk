LOCAL_PATH  := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := icudt51
LOCAL_C_INCLUDES := ../icu4c/source/common
LOCAL_SRC_FILES := ../icu4c/source/stubdata/stubdata.c

include $(BUILD_SHARED_LIBRARY)

