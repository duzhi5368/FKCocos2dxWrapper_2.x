LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := fkcw_database_static

LOCAL_MODULE_FILENAME := libfkcwdatabase

LOCAL_SRC_FILES := Source/FKCW_Database_Database.cpp \
Source/FKCW_Database_ResultSet.cpp \
Source/FKCW_Database_Statement.cpp \


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include	\

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Include	\
                    $(LOCAL_PATH)/../../../cocos2dx/include \
                    $(LOCAL_PATH)/../../../cocos2dx/platform \
                    $(LOCAL_PATH)/../../../cocos2dx/platform/android \
                    $(LOCAL_PATH)/../../../cocos2dx/kazmath/include \
                    


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static 
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += sqlite 


LOCAL_CFLAGS := -fexceptions 
LOCAL_CFLAGS += -Wno-psabi -DUSE_FILE32API
LOCAL_EXPORT_CFLAGS := -Wno-psabi -DUSE_FILE32API


include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,project/FKCWLib/FKCW_Depend/Sqlite/mobile)
