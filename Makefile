SRC_DIR = src
OBJ_DIR = ./obj
EXE=rubido

SRC=$(wildcard *.c $(foreach fd, $(SRC_DIR), $(fd)/*.c)) 
OBJS=$(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))


CC ?= gcc
SDLCONFIG ?= sdl-config
DESTDIR ?=
PREFIX ?= /usr
OPT_LEVEL ?= -O2 
CFLAGS ?= -g -DWIN32 -DSHOW_FPS -Wall -Wextra
LDFLAGS ?=  -lSDL_image -lSDL_ttf -lSDL_mixer -lSDL -lSDL_gfx -lm

#MINGW does not have X11 and does not require it
#dont know about cygwin
ifneq ($(OS),Windows_NT)
ifeq ($(NOX11),)
LDFLAGS += -lX11
endif
endif

ifdef DEBUG
CFLAGS += -g
endif

ifdef TARGET
include $(TARGET).mk
endif

CFLAGS += `$(SDLCONFIG) --cflags`
LDFLAGS += `$(SDLCONFIG) --libs`

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(TARGET_ARCH) $^ $(LDFLAGS) -o $@ 

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -rv *~ $(OBJS) $(EXE)