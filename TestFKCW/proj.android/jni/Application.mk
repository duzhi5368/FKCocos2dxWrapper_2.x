APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -DCOCOS2D_DEBUG=1 -DCC_ENABLE_BOX2D_INTEGRATION=1
APP_CPPFLAGS += -Wno-error=format-security 
APP_CPPFLAGS += -std=c++0x -Wno-deprecated -Wno-format-security 

APP_PLATFORM := android-10
APP_ABI := armeabi