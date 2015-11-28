LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := fkengine_static

LOCAL_MODULE_FILENAME := libfkengine

LOCAL_SRC_FILES := Audio/FKAudioSystem.cpp \
Camera/FKCamera.cpp \
Debug/FKDebugSystem.cpp \
Event/FKEventSystem.cpp \
Framework/FKFramework.cpp \
Input/FKInputSystem.cpp \
MsgDispatch/FKMsgDispatchSystem.cpp \
Network/FKNetworkCommand.cpp \
Network/FKNetworkSocket.cpp \
Network/FKNetworkSocketConnecter.cpp \
Network/FKNetworkSystem.cpp \
Render/FKRenderSystem.cpp \
Render/FKAnimateEx.cpp \
Render/FKAnimateExRes.cpp \
Render/FKAnimateExResMgr.cpp \
Render/FKBackgroundLayer.cpp \
Render/FKBarrierChunk.cpp \
Render/FKBarrierLayer.cpp \
Render/FKCameraChunk.cpp \
Render/FKCameraLayer.cpp \
Render/FKCameraMaskLayer.cpp \
Render/FKDBSpriteObj.cpp \
Render/FKEventChunk.cpp \
Render/FKEventLayer.cpp \
Render/FKFarLayer.cpp \
Render/FKFrameSpriteObj.cpp \
Render/FKFrontLayer.cpp \
Render/FKImageChunk.cpp \
Render/FKImageLayer.cpp \
Render/FKMapChunk.cpp \
Render/FKMapLayer.cpp \
Render/FKMapMgr.cpp \
Render/FKMapObj.cpp \
Render/FKMaskChunk.cpp \
Render/FKMaskLayer.cpp \
Render/FKNormalLayer.cpp \
Render/FKRenderableMgr.cpp \
Render/FKRenderObj.cpp \
Render/FKRenderObjMgr.cpp \
Render/FKRenderObjModel.cpp \
Render/FKRenderText.cpp \
Render/FKRenderTextMgr.cpp \
Render/FKSpriteEx.cpp \
Render/FKSpriteObj.cpp \
Scene/FKSceneSystem.cpp \
Timer/FKTimerSystem.cpp \
UI/FKUISystem.cpp \





LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Common	\
										$(LOCAL_PATH)/Interface \
										$(LOCAL_PATH)/../FKCWLib/FKCW_Depend/png \
										$(LOCAL_PATH)/../FKCWLib \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Interface \
                    $(LOCAL_PATH)/../../../cocos2dx/include \
                    $(LOCAL_PATH)/../../../cocos2dx/platform \
                    $(LOCAL_PATH)/../../../cocos2dx/platform/android \
                    $(LOCAL_PATH)/../../../cocos2dx/kazmath/include \
                    $(LOCAL_PATH)/../FKCWLib/FKCW_Depend/png \
                    $(LOCAL_PATH)/../FKCWLib \


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static 
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static 
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_freetype2_static


LOCAL_CFLAGS := -fexceptions 
LOCAL_CFLAGS += -Wno-psabi -DUSE_FILE32API
LOCAL_EXPORT_CFLAGS := -Wno-psabi -DUSE_FILE32API


include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,project/FKCWLib/FKCW_Depend/freetype2/android/prebuilt/libfreetype2)