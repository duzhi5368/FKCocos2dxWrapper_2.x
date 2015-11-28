LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := fkcw_base_static

LOCAL_MODULE_FILENAME := libfkcwbase

LOCAL_SRC_FILES := Source/FKCW_Base_AStar.cpp \
Source/FKCW_Base_Base64.cpp \
Source/FKCW_Base_Bundle.cpp \
Source/FKCW_Base_ByteBuffer.cpp \
Source/FKCW_Base_CSVLoader.cpp \
Source/FKCW_Base_Data.cpp \
Source/FKCW_Base_DecodeHtmlEntitiesUTF8.cpp \
Source/FKCW_Base_DrawingPrimitives.cpp \
Source/FKCW_Base_FileZip.cpp \
Source/FKCW_Base_Geometry.cpp \
Source/FKCW_Base_Localization.cpp \
Source/FKCW_Base_MD5.cpp \
Source/FKCW_Base_Memory.cpp \
Source/FKCW_Base_PinYinUtils.cpp \
Source/FKCW_Base_PlatformStringParser.cpp \
Source/FKCW_Base_PointList.cpp \
Source/FKCW_Base_ResourceLoader.cpp \
Source/FKCW_Base_SecureUserDefault.cpp \
Source/FKCW_Base_SHA1.cpp \
Source/FKCW_Base_StringConvert.cpp \
Source/FKCW_Base_Unicode.cpp \
Source/FKCW_Base_Utils.cpp \


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include	\

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Include	\
                    $(LOCAL_PATH)/../../../cocos2dx/include \
                    $(LOCAL_PATH)/../../../cocos2dx/platform \
                    $(LOCAL_PATH)/../../../cocos2dx/platform/android \
                    $(LOCAL_PATH)/../../../cocos2dx/kazmath/include \


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static 
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static 
LOCAL_WHOLE_STATIC_LIBRARIES += libiconv_static


LOCAL_CFLAGS := -fexceptions -std=c++11 -Wno-literal-suffix 
LOCAL_CFLAGS += -Wno-psabi -DUSE_FILE32API
LOCAL_EXPORT_CFLAGS := -Wno-psabi -DUSE_FILE32API


include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,project/FKCWLib/FKCW_Depend/libiconv)