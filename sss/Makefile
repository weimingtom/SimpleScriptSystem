CC := gcc
LD := ld
RM := rm -f

CFLAGS := -g -O2
CFLAGS += `sdl-config --cflags` 
CFLAGS += `python-config --cflags`

BIN := sss
LIBS := 
LIBS += `sdl-config --libs` 
LIBS += `python-config --libs`

OBJS :=
OBJS += src/canvas_gdi.o src/canvas_sdl.o
OBJS += src/mainframe_gdi.o src/mainframe_sdl.o 
OBJS += src/misc.o src/keyboard.o
OBJS += src/mouse.o src/script.o
OBJS += src/canvas_wgl.o src/mainframe_wgl.o 

all : $(BIN)

$(BIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
	
src/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean :
	$(RM) $(OBJS) $(BIN)

