LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := fkdblib_static

LOCAL_MODULE_FILENAME := libfkdblib

LOCAL_SRC_FILES := Source/Animation.cpp \
Source/AnimationEvent.cpp \
Source/AnimationState.cpp \
Source/Armature.cpp \
Source/ArmatureData.cpp \
Source/ArmatureEvent.cpp \
Source/BaseFactory.cpp \
Source/Bone.cpp \
Source/DargonBonesObject.cpp \
Source/DargonBonesXMLParser.cpp \
Source/EventDispatcher.cpp \
Source/FrameEvent.cpp \
Source/Matrix.cpp \
Source/Slot.cpp \
Source/SoundEvent.cpp \
Source/SoundEventManager.cpp \
Source/TimeLineState.cpp \
Source/TinyXML2.cpp \
Source/Transform.cpp \
Source/TransformTImeLine.cpp \




LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include	\
										$(LOCAL_PATH)/Interface

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