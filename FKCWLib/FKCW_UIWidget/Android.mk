LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := fkcw_uiwidget_static

LOCAL_MODULE_FILENAME := libfkcwtuiwidget

LOCAL_SRC_FILES := Source/FKCW_UIWidget_Button.cpp \
Source/FKCW_UIWidget_CheckBox.cpp \
Source/FKCW_UIWidget_ColorView.cpp \
Source/FKCW_UIWidget_ControlView.cpp \
Source/FKCW_UIWidget_ExpandableListView.cpp \
Source/FKCW_UIWidget_GradientView.cpp \
Source/FKCW_UIWidget_GridPageView.cpp \
Source/FKCW_UIWidget_GridView.cpp \
Source/FKCW_UIWidget_ImageView.cpp \
Source/FKCW_UIWidget_ImageViewScale9.cpp \
Source/FKCW_UIWidget_Label.cpp \
Source/FKCW_UIWidget_LabelAtlas.cpp \
Source/FKCW_UIWidget_LabelBMFont.cpp \
Source/FKCW_UIWidget_Layout.cpp \
Source/FKCW_UIWidget_ListView.cpp \
Source/FKCW_UIWidget_Lua.cpp \
Source/FKCW_UIWidget_PageView.cpp \
Source/FKCW_UIWidget_ProgressBar.cpp \
Source/FKCW_UIWidget_Protocol.cpp \
Source/FKCW_UIWidget_Scale9Sprite.cpp \
Source/FKCW_UIWidget_ScrollView.cpp \
Source/FKCW_UIWidget_Slider.cpp \
Source/FKCW_UIWidget_TableView.cpp \
Source/FKCW_UIWidget_TextRich.cpp \
Source/FKCW_UIWidget_ToggleView.cpp \
Source/FKCW_UIWidget_Widget.cpp \
Source/FKCW_UIWidget_WidgetWindow.cpp \










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