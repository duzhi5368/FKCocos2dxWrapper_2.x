LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := fkcw_action_static

LOCAL_MODULE_FILENAME := libfkcwaction

LOCAL_SRC_FILES := Source/FKCW_Action_Blur.cpp \
Source/FKCW_Action_ClipIn.cpp \
Source/FKCW_Action_ClipOut.cpp \
Source/FKCW_Action_Count.cpp \
Source/FKCW_Action_Curl.cpp \
Source/FKCW_Action_Flash.cpp \
Source/FKCW_Action_Missile.cpp \
Source/FKCW_Action_Shake.cpp \
Source/FKCW_Action_Shine.cpp \
Source/FKCW_Action_Suck.cpp \
Source/FKCW_Action_TrailMoveTo.cpp \
Source/FKCW_Action_TreeFadeIn.cpp \
Source/FKCW_Action_TreeFadeOut.cpp \


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include	\

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