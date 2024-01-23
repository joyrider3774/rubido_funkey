SRC_DIR = src
OBJ_DIR = ./obj
EXE=rubido

SRC=$(wildcard *.c $(foreach fd, $(SRC_DIR), $(fd)/*.cpp)) 
OBJS=$(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))


CXX ?= g++
PREFIX ?=/usr
SDLCONFIG ?= sdl-config
DESTDIR ?=
PREFIX ?= /usr
OPT_LEVEL ?= -O2 
CXXFLAGS ?= -g -Wall -Wextra -I$(PREFIX)/include
LDFLAGS ?= -L$(PREFIX)/lib -lSDL_image -lSDL_ttf -lSDL_mixer -lmikmod -lSDL_gfx -lm

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