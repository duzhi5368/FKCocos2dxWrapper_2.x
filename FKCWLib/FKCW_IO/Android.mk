LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := fkcw_io_static

LOCAL_MODULE_FILENAME := libfkcwio

LOCAL_SRC_FILES := Platform/FKCW_IO_AssetInputStream_android.cpp \
Platform/FKCW_IO_AssetOutputStream_Win.cpp \
Source/FKCW_IO_AssetInputStream.cpp \
Source/FKCW_IO_AssetOutputStream.cpp \
Source/FKCW_IO_MemoryInputStream.cpp \
Source/FKCW_IO_MemoryOutputStream.cpp \



LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include	\
										$(LOCAL_PATH)/Platform	\

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Include	\
                    $(LOCAL_PATH)/../../../cocos2dx/include \
                    $(LOCAL_PATH)/../../../cocos2dx/platform \
                    $(LOCAL_PATH)/../../../cocos2dx/platform/android \
                    $(LOCAL_PATH)/../../../cocos2dx/kazmath/include \


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static 
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static 


LOCAL_CFLAGS := -fexceptions 
LOCAL_CFLAGS += -Wno-psabi -DUSE_FILE32API
LOCAL_EXPORT_CFLAGS := -Wno-psabi -DUSE_FILE32API


include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)