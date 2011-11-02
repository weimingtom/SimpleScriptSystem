CC := /home/Administrator/android-ndk-r5b/toolchains/arm-linux-androideabi-4.4.3/prebuilt/windows/bin/arm-linux-androideabi-gcc.exe --sysroot="c:/cygwin/home/Administrator/android-ndk-r5b/platforms/android-8/arch-arm"
# CC := gcc
LD := ld
# AR := ar rcu  
AR := /home/Administrator/android-ndk-r5b/toolchains/arm-linux-androideabi-4.4.3/prebuilt/windows/bin/arm-linux-androideabi-ar.exe rcu 
# RANLIB := ranlib  
RANLIB := /home/Administrator/android-ndk-r5b/toolchains/arm-linux-androideabi-4.4.3/prebuilt/windows/bin/arm-linux-androideabi-ranlib
RM := rm -f  

CFLAGS := -DNDEBUG -DHAVE_CONFIG_H -DPLATFORM='"cygwin"'
CFLAGS += -DPYTHONPATH='":plat-cygwin:lib-tk"' -DPREFIX='"/usr/local"' 
CFLAGS += -DEXEC_PREFIX='"/usr/local"'
CFLAGS += -DVERSION='"2.2"' -DVPATH='""' -DBUILD='"2.2"'
# -g
CFLAGS += -O2 
CFLAGS += -Wall -Wstrict-prototypes 
CFLAGS += -I. -I./Include 

LDFLAGS += -ldl -lm

OBJECTS := 

# OBJECTS += Modules/python.o

OBJECTS += Parser/acceler.o Parser/grammar1.o Parser/listnode.o
OBJECTS += Parser/node.o Parser/parser.o Parser/parsetok.o
OBJECTS += Parser/tokenizer.o Parser/bitset.o Parser/metagrammar.o
OBJECTS += Parser/myreadline.o

OBJECTS += Objects/abstract.o Objects/bufferobject.o Objects/cellobject.o
OBJECTS += Objects/classobject.o Objects/cobject.o Objects/complexobject.o
OBJECTS += Objects/descrobject.o Objects/fileobject.o Objects/floatobject.o
OBJECTS += Objects/frameobject.o Objects/funcobject.o Objects/intobject.o
OBJECTS += Objects/iterobject.o Objects/listobject.o Objects/longobject.o
OBJECTS += Objects/dictobject.o Objects/methodobject.o Objects/moduleobject.o
OBJECTS += Objects/object.o Objects/rangeobject.o Objects/sliceobject.o
OBJECTS += Objects/stringobject.o Objects/structseq.o Objects/tupleobject.o
OBJECTS += Objects/typeobject.o Objects/weakrefobject.o Objects/unicodeobject.o
OBJECTS += Objects/unicodectype.o

OBJECTS += Python/bltinmodule.o Python/exceptions.o Python/ceval.o
OBJECTS += Python/mysnprintf.o

# OBJECTS += Parser/firstsets.o Parser/grammar.o Parser/pgen.o
# OBJECTS += Parser/printgrammar.o

OBJECTS += Python/compile.o Python/codecs.o Python/errors.o
OBJECTS += Python/frozen.o Python/frozenmain.o Python/future.o
OBJECTS += Python/getargs.o Python/getcompiler.o Python/getcopyright.o
OBJECTS += Python/getmtime.o Python/getplatform.o Python/getversion.o
OBJECTS += Python/graminit.o Python/import.o Python/importdl.o
OBJECTS += Python/marshal.o Python/modsupport.o Python/mystrtoul.o
OBJECTS += Python/pyfpe.o Python/pystate.o Python/pythonrun.o
OBJECTS += Python/structmember.o Python/symtable.o Python/sysmodule.o
OBJECTS += Python/traceback.o Python/getopt.o Python/dynload_shlib.o
OBJECTS += Python/thread.o 
 
OBJECTS += Modules/config.o Modules/getpath.o Modules/main.o
OBJECTS += Modules/gcmodule.o 
OBJECTS += Modules/threadmodule.o Modules/signalmodule.o
OBJECTS += Modules/posixmodule.o Modules/errnomodule.o
OBJECTS += Modules/_sre.o Modules/newmodule.o
OBJECTS += Modules/symtablemodule.o Modules/xxsubtype.o

OBJECTS += Modules/getbuildinfo.o

#all : libpython.a python.exe
all : libpython.a

python.exe : libpython.a 
	$(CC) $(CFLAGS) -o $@ Modules/python.c libpython.a -lm -ldl

libpython.a : $(OBJECTS)
	$(AR) $@ $(OBJECTS) 
	$(RANLIB) $@
	
Modules/%.o : Modules/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

Parser/%.o : Parser/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

Python/%.o : Python/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

Objects/%.o : Objects/%.c
	$(CC) $(CFLAGS) -o $@ -c $<
	
clean :
	$(RM) $(OBJECTS) *.a *.exe
