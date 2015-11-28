LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := fkcw_sceneexmgr_msgmanager_static

LOCAL_MODULE_FILENAME := libfkcwsceneexmgrmsgmanager

LOCAL_SRC_FILES := Source/FKCW_SceneExMgr_MsgManager.cpp \
Source/FKCW_SceneExMgr_SceneEx.cpp \
Source/FKCW_SceneExMgr_SceneExMgr.cpp \
Source/FKCW_SceneExMgr_SceneExTransition.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionCrossFade.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionFadeColor.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionFadeDir.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionFlip.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionJumpZoom.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionMoveIn.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionOriented.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionPageTurn.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionProgress.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionRotoZoom.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionShrinkGrow.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionSlideIn.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionSplit.cpp \
Source/FKCW_SceneExMgr_SceneExTransitionTurnOffTiles.cpp \




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