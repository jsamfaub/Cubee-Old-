OBJS = main.cpp background.cpp close.cpp init.cpp checkCollision.cpp entity.cpp level.cpp LTimer.cpp LWindow.cpp player.cpp rect.cpp Tile.cpp ui.cpp
OBJ_NAME = cubee
CC = g++
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer
all : $(OBJS)
	$(CC) --no-warnings $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)
