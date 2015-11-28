LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := fkcw_effectnode_static

LOCAL_MODULE_FILENAME := libfkcweffectnode

LOCAL_SRC_FILES := Source/FKCW_EffecNode_2DSoftShadow.cpp \
Source/FKCW_EffectNode_BreakSprite.cpp \
Source/FKCW_EffectNode_ColorAdjustSprite.cpp \
Source/FKCW_EffectNode_GhostLikeSprite.cpp \
Source/FKCW_EffectNode_GLProgramWithUnifos.cpp \
Source/FKCW_EffectNode_IndexVBO.cpp \
Source/FKCW_EffectNode_Laser.cpp \
Source/FKCW_EffectNode_LightingBoltNode.cpp \
Source/FKCW_EffectNode_NormalMappedSprite.cpp \
Source/FKCW_EffectNode_Ripper2DSprite.cpp \
Source/FKCW_EffectNode_RippleSprite.cpp \
Source/FKCW_EffectNode_Shatter.cpp \
Source/FKCW_EffectNode_TailSprite.cpp \
Source/FKCW_EffectNode_TinyWingsTerrainSprite.cpp \



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