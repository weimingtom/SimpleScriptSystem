--------------------------
I. Build & run platform
* Visual Studio 6.0 on Windows XP (Microsoft Windows XP [version 5.1.2600])
* MinGW 3.4.5 (mingw-vista special r3) on Windows XP (Microsoft Windows XP [version 5.1.2600])
* GCC 4.4.3 on Ubuntu 10.04 (Linux ubuntu 2.6.32-21-generic)
* QEMU for Windows, Linux (linux 2.6.39.2)
* Android emulator for windows, Android native activity (Android 2.3.3 API Level 10) (WARNING: for test)
* OpenGL ES emulator for windows (Microsoft Windows XP [version 5.1.2600])

--------------------------
II. How to build

* Build or install Python .lib/.dll file (2.2.2 / 2.6)
1) VC6
	Open Python-2.2.2\PCbuild\pcbuild.dsw.
2) MinGW
	Not need. (see VC6)
3) Ubuntu
	$ sudo apt-get install python-dev
4) Android NDK
	(1) modify python-2.2.2-android/Makefile
		CC
		AR
		RANLIB
	(2) get libpython.a
		$ cd python-2.2.2-android
		$ make clean all
	
* Build or install SDL .lib/.dll file (1.2)
1) VC6
	Download them development and runtime libraries from
	http://www.libsdl.org/download-1.2.php
2) MinGW
	Not need. (see VC6)
3) Ubuntu
	$ sudo apt-get install libsdl-dev

* Build or install DirectFB
1) Ubuntu
	Download source from http://www.directfb.org/
	or 
	$ sudo apt-get install libdirectfb-dev

* Build Simple Script System
1) VC6
	Open SimpleScriptSystem.dsw.
	(if need to build WGLES port, please switch to SimpleScriptSystemES.dsp)
2) MinGW
	$ make -f Makefile.mingw clean
	$ make -f Makefile.mingw
3) Ubuntu
	* for SDL
		$ make clean
		$ make
	* for DirectFB
		$ make -f Makefile.dfb clean
		$ make -f Makefile.dfb
	* for QEMU on Windows (only for me)
		$ make -f Makefile.qemu clean
		$ make -f Makefile.qemu 
4) Android NDK (Android Native Activity)
	See below (IV) (WARNING: for test)
	
--------------------------
III. Build Configure
1) Modify config.h
	USE_GDI : use Windows GDI (Windows)
	USE_SDL : use SDL (Windows / Ubuntu)
	USE_WGL : use OpenGL (Windows)
	USE_DFB : use DirectFB (QEMU on Windows / Ubuntu)
	USE_WGLES : use OpenGL ES emulator (Windows)
--------------------------
IV. Android native activity build
1) Modify config.h
	USE_ANA
2) Execute in Cygwin
	$ export PATH=/cygdrive/C/cygwin/home/Administrator/android-ndk-r5b:$PATH
	$ export PATH=/cygdrive/D/java/apache-ant-1.8.1/bin:$PATH
	$ cd /cygdrive/D/ugame_c/SimpleScriptSystem/sss
	$ cd python-2.2.2-android/
	$ make clean all
	$ cd ..
	$ cd native-activity
	$ ndk-build clean all
	$ ant clean debug
	(Now, start android emulator, for example,
	> start emulator -avd add4
	)
	$ ant install
3) Execute build-install-ana.bat (not available now)
	
--------------------------

V. Status
* Sorry, it's not stable now, only for test.
