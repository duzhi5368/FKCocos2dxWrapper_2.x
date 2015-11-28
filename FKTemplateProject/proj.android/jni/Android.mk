LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := fktemplate_shared

LOCAL_MODULE_FILENAME := libfktemplate

LOCAL_SRC_FILES := fkcw/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/BaseScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static 
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += libiconv_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_freetype2_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_base_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_3d_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_action_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_database_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_effectnode_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_font_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_gr_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_io_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_json_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_network_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_pixel_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_renderex_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_richhtml_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_sceneexmgr_msgmanager_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_shader_shadermanager_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_simplenet_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_story_clicklayer_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_tmx_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_ui_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkcw_uiwidget_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkdbcocos2dxlib_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkdblib_static
LOCAL_WHOLE_STATIC_LIBRARIES += fkdbpiclib_static


include $(BUILD_SHARED_LIBRARY)

LOCAL_CFLAGS := -fexceptions -DCOCOS2D_DEBUG=0

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,project/FKCWLib/FKCW_Depend/libiconv)
$(call import-module,project/FKCWLib/FKCW_Depend/freetype2/android/prebuilt/libfreetype2)
$(call import-module,project/FKCWLib/FKCW_3D)
$(call import-module,project/FKCWLib/FKCW_Base)
$(call import-module,project/FKCWLib/FKCW_Action)
$(call import-module,project/FKCWLib/FKCW_Database)
$(call import-module,project/FKCWLib/FKCW_EffectNode)
$(call import-module,project/FKCWLib/FKCW_Font)
$(call import-module,project/FKCWLib/FKCW_GR)
$(call import-module,project/FKCWLib/FKCW_IO)
$(call import-module,project/FKCWLib/FKCW_Json)
$(call import-module,project/FKCWLib/FKCW_Network)
$(call import-module,project/FKCWLib/FKCW_Pixel)
$(call import-module,project/FKCWLib/FKCW_RenderEx)
$(call import-module,project/FKCWLib/FKCW_RichHtml)
$(call import-module,project/FKCWLib/FKCW_SceneExMgr)
$(call import-module,project/FKCWLib/FKCW_Shader)
$(call import-module,project/FKCWLib/FKCW_SimpleNet)
$(call import-module,project/FKCWLib/FKCW_Story)
$(call import-module,project/FKCWLib/FKCW_TMX)
$(call import-module,project/FKCWLib/FKCW_UI)
$(call import-module,project/FKCWLib/FKCW_UIWidget)
$(call import-module,project/FKCoreLib/FKDBCocos2dxLib)
$(call import-module,project/FKCoreLib/FKDBLib)
$(call import-module,project/FKCoreLib/FKPicLib)


