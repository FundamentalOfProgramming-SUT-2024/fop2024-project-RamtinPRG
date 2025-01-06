all: main run

main:
	gcc ./src/*.c -lncursesw -lSDL2 -lSDL2_mixer -I./include/ -o rogue

run:
	./rogue