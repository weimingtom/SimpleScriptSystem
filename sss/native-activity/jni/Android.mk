LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := native-activity

# look at other folder, the root is ./JNI
LOCAL_SRC_FILES := ../../src/mainframe_ana.c
LOCAL_SRC_FILES += ../../src/canvas_ana.c
LOCAL_SRC_FILES += ../../src/canvas_dfb.c
LOCAL_SRC_FILES += ../../src/canvas_gdi.c
LOCAL_SRC_FILES += ../../src/canvas_sdl.c
LOCAL_SRC_FILES += ../../src/canvas_wgl.c
LOCAL_SRC_FILES += ../../src/keyboard.c
LOCAL_SRC_FILES += ../../src/mainframe_dfb.c
LOCAL_SRC_FILES += ../../src/mainframe_gdi.c
LOCAL_SRC_FILES += ../../src/mainframe_sdl.c
LOCAL_SRC_FILES += ../../src/mainframe_wgl.c
LOCAL_SRC_FILES += ../../src/misc.c
LOCAL_SRC_FILES += ../../src/mouse.c
LOCAL_SRC_FILES += ../../src/script.c

# NOTE:the root is on project path
LOCAL_CFLAGS    := -I../python-2.2.2-android
LOCAL_CFLAGS    += -I../python-2.2.2-android/Include

# lib
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM 
# prebuilt static lib
LOCAL_LDLIBS    += ../python-2.2.2-android/libpython.a

# see below $(call import-module,android/native_app_glue)
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

# native_app_glue is in NDK
$(call import-module,android/native_app_glue)
