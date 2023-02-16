
all: vcdump

vcdump: vcdump.c
	gcc -g $^ -o $@
