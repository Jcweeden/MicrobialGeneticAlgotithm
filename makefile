#OBJS specifies which files to compile as part of the project
OBJS = main.cpp Game.cpp GameObject.cpp InputHandler.cpp Environment.cpp Microbe.cpp FoodSource.cpp Obstacle.cpp Node.cpp Grid.cpp PathFinder.cpp Heap.cpp UI.cpp MicrobialGA.cpp SoundMixer.cpp


#CC specifies which compiler we're using
CC = g++ -std=c++11

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -g

#LINKER_FLAGS specifies the libraries we're linking against 
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_gfx

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = MicrobialGA

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
