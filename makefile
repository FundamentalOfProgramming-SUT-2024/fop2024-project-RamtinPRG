all: main run

main:
	gcc ./src/*.c -lncursesw -I./include/ -o rogue

run:
	./rogue