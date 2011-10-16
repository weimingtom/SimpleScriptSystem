--------------------------
I. Build & run platform
* Visual Studio 6.0 on Windows XP
* MinGW 3.4.5 (mingw-vista special r3) on Windows XP
* GCC 4.4.3 on Ubuntu 10.04

--------------------------
II. How to build

* Build or install Python .lib/.dll file (2.2.2 / 2.6)
1) VC6
	Open Python-2.2.2\PCbuild\pcbuild.dsw.
2) MinGW
	Not need. (see VC6)
3) Ubuntu
	$ sudo apt-get install python-dev

* Build or install SDL .lib/.dll file (1.2)
1) VC6
	Download them development and runtime libraries from
	http://www.libsdl.org/download-1.2.php
2) MinGW
	Not need. (see VC6)
3) Ubuntu
	$ sudo apt-get install libsdl-dev

* Build Simple Script System
1) VC6
	Open SimpleScriptSystem.dsw.
2) MinGW
	$ make -f Makefile.mingw clean
	$ make -f Makefile.mingw
3) Ubuntu
	$ make clean
	$ make

--------------------------
III. Build Configure
1) Modify config.h
	USE_GDI : use Windows GDI
	USE_SDL : use SDL

--------------------------
IV. Status
* Sorry, it's not stable now, only for test.

