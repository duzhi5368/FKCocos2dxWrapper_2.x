LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := fkcw_renderex_static

LOCAL_MODULE_FILENAME := libfkcwrenderex

LOCAL_SRC_FILES := Source/FKCW_RenderEx_BlurTextureFilter.cpp \
Source/FKCW_RenderEx_CatmullRomSprite.cpp \
Source/FKCW_RenderEx_EraserSprite.cpp \
Source/FKCW_RenderEx_GradientSprite.cpp \
Source/FKCW_RenderEx_JoystickNode.cpp \
Source/FKCW_RenderEx_LaserSprite.cpp \
Source/FKCW_RenderEx_NumberCountNode.cpp \
Source/FKCW_RenderEx_PredrawArmature.cpp \
Source/FKCW_RenderEx_PredrawSprite.cpp \
Source/FKCW_RenderEx_RenderTextureMutable.cpp \
Source/FKCW_RenderEx_ScrollingBackgroundNode.cpp \
Source/FKCW_RenderEx_SimpleShaderColorPainter.cpp \
Source/FKCW_RenderEx_SpotLightSprite.cpp \
Source/FKCW_RenderEx_SwayNode.cpp \
Source/FKCW_RenderEx_Texture2DMutable.cpp \
Source/FKCW_RenderEx_TiledSprite.cpp \
Source/FKCW_RenderEx_TurnCardSprite.cpp \
Source/FKCW_RenderEx_ZoomControllerLayer.cpp \








LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include	\
										$(LOCAL_PATH)/../../../cocos2dx/include \
                    $(LOCAL_PATH)/../../../cocos2dx/platform \
                    $(LOCAL_PATH)/../../../cocos2dx/platform/android \
                    $(LOCAL_PATH)/../../../cocos2dx/kazmath/include \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Include	\
                    $(LOCAL_PATH)/../../../cocos2dx/include \
                    $(LOCAL_PATH)/../../../cocos2dx/platform \
                    $(LOCAL_PATH)/../../../cocos2dx/platform/android \
                    $(LOCAL_PATH)/../../../cocos2dx/kazmath/include \


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static 
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static 


LOCAL_LDLIBS := -lGLESv2 \
								-lGLESv1_CM \
                -llog \
                -lz \

LOCAL_EXPORT_LDLIBS := -lGLESv2 \
											 -lGLESv1_CM \
                       -llog \
                       -lz \

LOCAL_CFLAGS := -fexceptions
LOCAL_CFLAGS += -Wno-psabi -DUSE_FILE32API
LOCAL_EXPORT_CFLAGS := -Wno-psabi -DUSE_FILE32API


include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)