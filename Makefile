SRC_DIR = src
OBJ_DIR = ./obj
EXE=rubido

SRC=$(wildcard *.c $(foreach fd, $(SRC_DIR), $(fd)/*.cpp)) 
OBJS=$(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))


CXX ?= g++
SDLCONFIG ?= sdl-config
DESTDIR ?=
PREFIX ?= /usr
OPT_LEVEL ?= -O2 
CXXFLAGS ?= -g -DWIN32 -DSHOW_FPS -Wall -Wextra
LDFLAGS ?=  -lSDL_image -lSDL_ttf -lSDL_mixer -lSDL -lSDL_gfx -lm

#MINGW does not have X11 and does not require it
#dont know about cygwin
ifneq ($(OS),Windows_NT)
ifeq ($(NOX11),)
LDFLAGS += -lX11
endif
endif

ifdef DEBUG
CXXFLAGS += -g
endif

ifdef TARGET
include $(TARGET).mk
endif

CXXFLAGS += `$(SDLCONFIG) --cflags`
LDFLAGS += `$(SDLCONFIG) --libs`

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(TARGET_ARCH) $^ $(LDFLAGS) -o $@ 

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -rv *~ $(OBJS) $(EXE)