all: main run

main:
	gcc ./*.c -lncursesw -I./include/ -o rogue

run:
	./rogue