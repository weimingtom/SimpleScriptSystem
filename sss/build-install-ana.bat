@echo off

@cd /d D:\ugame_c\SimpleScriptSystem\sss
@echo CD is "%CD%"

:clean-ana
@echo "Clean all output files"
@rmdir /S /Q obj > nul 2>&1

:make-ana
@REM There is no space character around the equal in set command!
@set ANDROID_NDK_PATH=C:/cygwin/home/Administrator/android-ndk-r5b
@set ANDROID_BIN_PATH=%ANDROID_NDK_PATH%/toolchains/arm-linux-androideabi-4.4.3/prebuilt/windows/bin
@set ANDORID_SYSROOT=%ANDROID_NDK_PATH%/platforms/android-9/arch-arm
@set ANDROID_LIB_PATH=%ANDORID_SYSROOT%/usr/lib
@set ANDROID_INC_PATH=%ANDORID_SYSROOT%/usr/include
@REM =======================================
@set ANDROID_SDK_PATH=D:\java\android-sdk_r10-windows\android-sdk-windows
@set ANDROID_PROJ_PATH=%CD%\native-activity
@REM =======================================
@set ANDROID_PLATFORM=arm-linux-androideabi-
@set ANDROID_GCC=%ANDROID_BIN_PATH%/%ANDROID_PLATFORM%gcc
@set ANDROID_CPP=%ANDROID_BIN_PATH%/%ANDROID_PLATFORM%g++.exe
@set ANDROID_AR=%ANDROID_BIN_PATH%/%ANDROID_PLATFORM%ar.exe
@set ANDROID_STRIP=%ANDROID_BIN_PATH%/%ANDROID_PLATFORM%strip.exe
@REM =======================================
@REM Flags (exclude -MMD -MP -MF)
@set ANDROID_CFLAGS=-fpic -ffunction-sections -funwind-tables -fstack-protector
@set ANDROID_DEFINE=-D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__ -DANDROID
@set ANDROID_WARNING=-Wno-psabi
@set ANDROID_ARCH=-march=armv5te -mtune=xscale -msoft-float -mthumb
@set ANDROID_FLAGS=-Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64
@REM Include path
@set ANDROID_INCLUDE=-I%ANDROID_NDK_PATH%/sources/android/native_app_glue  
@set ANDROID_INCLUDE=%ANDROID_INCLUDE% -I%ANDROID_INC_PATH%
@set ANDROID_INCLUDE=%ANDROID_INCLUDE% -Ipython-2.2.2-android
@set ANDROID_INCLUDE=%ANDROID_INCLUDE% -Ipython-2.2.2-android/Include
@set ANDROID_DEBUG=-Wa,--noexecstack -O2 -DNDEBUG -g
@set ANDROID_SYSROOT=--sysroot=%ANDORID_SYSROOT%
@REM Lib path
@set ANDROID_LIBS=%ANDROID_LIB_PATH%/libc.so %ANDROID_LIB_PATH%/libstdc++.so %ANDROID_LIB_PATH%/libm.so
@set ANDROID_CPPFLAGS=-Wl,--no-undefined -Wl,-z,noexecstack
@set ANDOIRD_LDFLAGS=-L%ANDROID_LIB_PATH% -llog -landroid -lEGL -lGLESv1_CM -llog 
@set ANDOIRD_LDFLAGS=%ANDOIRD_LDFLAGS% -Wl,-rpath-link=%ANDROID_LIB_PATH% -lsupc++
@set ANDOIRD_LDFLAGS=%ANDOIRD_LDFLAGS% python-2.2.2-android/libpython.a
@REM =======================================
@set CFLAGS=%ANDROID_CFLAGS% %ANDROID_DEFINE% %ANDROID_WARNING% 
@set CFLAGS=%CFLAGS% %ANDROID_ARCH% %ANDROID_FLAGS%  
@set CFLAGS=%CFLAGS% %ANDROID_INCLUDE%  %ANDROID_DEBUG%
@REM =======================================
@set APP_GLUE=%ANDROID_NDK_PATH%/sources/android/native_app_glue/android_native_app_glue.c
@REM =======================================

@rmdir /S /Q obj > nul 2>&1
@mkdir obj > nul 2>&1

@echo "Compile thumb  : android_native_app_glue <= android_native_app_glue.c"
@%ANDROID_GCC% %CFLAGS% -c %APP_GLUE% -o obj/android_native_app_glue.o

@echo "StaticLibrary  : libandroid_native_app_glue.a"
@del /S /Q native-activity\obj\local\armeabi\libandroid_native_app_glue.a > nul 2>&1
@%ANDROID_AR% crs obj/libandroid_native_app_glue.a obj/android_native_app_glue.o

@echo "Compile thumb  : native-activity <= mainframe_ana.c"
@%ANDROID_GCC% %CFLAGS%  -ID:/ugame_c/sss-android/native-activity/jni -c src/mainframe_ana.c -o obj/mainframe_ana.o

@echo "SharedLibrary  : libnative-activity.so"
@%ANDROID_CPP% -Wl,-soname,libnative-activity.so -shared %ANDROID_SYSROOT%  obj/mainframe_ana.o obj/libandroid_native_app_glue.a %ANDROID_LIBS% %ANDROID_CPPFLAGS% %ANDOIRD_LDFLAGS% -o obj/libnative-activity.so

@echo "Install        : libnative-activity.so => libs/armeabi/libnative-activity.so"
@mkdir native-activity\libs\armeabi > nul 2>&1
@copy obj\libnative-activity.so native-activity\libs\armeabi\libnative-activity.so > nul 2>&1
@%ANDROID_STRIP% --strip-unneeded native-activity/libs/armeabi/libnative-activity.so

@rmdir /S /Q obj > nul 2>&1

:package-ana
@rmdir /S /Q %ANDROID_PROJ_PATH%\bin > nul 2>&1
@del /Q %ANDROID_PROJ_PATH%\bin\MyAndroidApp.ap_ > nul 2>&1
@del /Q %ANDROID_PROJ_PATH%\bin\MyAndroidApp-debug-unaligned.apk > nul 2>&1
@rmdir /S /Q %ANDROID_PROJ_PATH%\bin > nul 2>&1
@mkdir %ANDROID_PROJ_PATH%\res > nul 2>&1
@mkdir %ANDROID_PROJ_PATH%\libs > nul 2>&1
@mkdir %ANDROID_PROJ_PATH%\bin > nul 2>&1

@echo "Package apk    : bin/MyAndroidApp-debug.apk"
@"%ANDROID_SDK_PATH%\platform-tools\aapt.exe" package -f --debug-mode -M "%ANDROID_PROJ_PATH%\AndroidManifest.xml" -S "%ANDROID_PROJ_PATH%\res" -I "%ANDROID_SDK_PATH%\platforms\android-10\android.jar" -F "%ANDROID_PROJ_PATH%\bin\MyAndroidApp.ap_"
@call "%ANDROID_SDK_PATH%\tools\apkbuilder.bat" "%ANDROID_PROJ_PATH%\bin\MyAndroidApp-debug-unaligned.apk" -nf "%ANDROID_PROJ_PATH%\libs" -z "%ANDROID_PROJ_PATH%\bin\MyAndroidApp.ap_" > nul 2>&1
@"%ANDROID_SDK_PATH%\tools\zipalign.exe" -f 4 "%ANDROID_PROJ_PATH%\bin\MyAndroidApp-debug-unaligned.apk" "%ANDROID_PROJ_PATH%\bin\MyAndroidApp-debug.apk"

:install
@echo "Reinstall apk  : bin/MyAndroidApp-debug.apk"
"%ANDROID_SDK_PATH%\platform-tools\adb.exe" install -r "%ANDROID_PROJ_PATH%\bin\MyAndroidApp-debug.apk"

@echo off
@pause
