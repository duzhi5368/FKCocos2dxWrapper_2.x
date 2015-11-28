LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := fkcw_3d_static

LOCAL_MODULE_FILENAME := libfkcw3d

LOCAL_SRC_FILES := Source/FKCW_3D_Actor.cpp \
Source/FKCW_3D_Camera.cpp \
Source/FKCW_3D_DefaultMeshes.cpp \
Source/FKCW_3D_DefaultPassUniformCallback.cpp \
Source/FKCW_3D_DefaultPrograms.cpp \
Source/FKCW_3D_FKSimpleModelLoader.cpp \
Source/FKCW_3D_GLMath.cpp \
Source/FKCW_3D_IDTriangle.cpp \
Source/FKCW_3D_IndexVBO3D.cpp \
Source/FKCW_3D_LightSource.cpp \
Source/FKCW_3D_Macro.cpp \
Source/FKCW_3D_Material.cpp \
Source/FKCW_3D_Math.cpp \
Source/FKCW_3D_Matrix4.cpp \
Source/FKCW_3D_MatStackInfoGetor.cpp \
Source/FKCW_3D_Mesh.cpp \
Source/FKCW_3D_Model.cpp \
Source/FKCW_3D_Node.cpp \
Source/FKCW_3D_Programs.cpp \
Source/FKCW_3D_Range.cpp \
Source/FKCW_3D_Root.cpp \
Source/FKCW_3D_SubMesh.cpp \
Source/FKCW_3D_SubMeshData.cpp \
Source/FKCW_3D_Vector.cpp \


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/Include \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/Include	\
                    $(LOCAL_PATH)/../../../cocos2dx/include \
                    $(LOCAL_PATH)/../../../cocos2dx/platform \
                    $(LOCAL_PATH)/../../../cocos2dx/platform/android \
                    $(LOCAL_PATH)/../../../cocos2dx/kazmath/include \


LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static 
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static 
LOCAL_WHOLE_STATIC_LIBRARIES += libiconv_static


LOCAL_CFLAGS := -fexceptions 
LOCAL_CFLAGS += -Wno-psabi -DUSE_FILE32API
LOCAL_EXPORT_CFLAGS := -Wno-psabi -DUSE_FILE32API


include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,project/FKCWLib/FKCW_Depend/libiconv)