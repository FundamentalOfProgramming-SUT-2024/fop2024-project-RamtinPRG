all: main run

main: main.c
	gcc main.c -lpanel -lncursesw -o rogue

run:
	./rogue