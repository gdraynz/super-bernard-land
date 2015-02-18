GCC ?= g++ -W -Wall -Werror -ansi -pedantic -Isrc -Iinclude -std=c++11
UNAME := $(shell uname)

ifeq ($(UNAME),Darwin)
	GCC += -Qunused-arguments
	LIBS ?= -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer
else ifeq ($(UNAME), Linux)
	LIBS ?= -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer
else
	LIBS ?= -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
endif

ifeq ($(UNAME),Darwin)
	DELCMD ?= rm -rf src/*.o
else ifeq ($(UNAME), Linux)
	DELCMD ?= rm -rf src/*.o
else
	DELCMD ?= del .\src\*.o
endif

SRC ?= src/main.cpp src/Logger.cpp src/Application.cpp src/Image.cpp src/Font.cpp src/Player.cpp src/Menu.cpp src/Options.cpp src/Collisions.cpp src/LevelGenerator.cpp src/Monster.cpp src/Music.cpp
OBJ ?= $(SRC:.cpp=.o)

all: main

main: $(OBJ)
	$(GCC) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(GCC) -o $@ -c $< $(CFLAGS)

clean:
	$(DELCMD)
