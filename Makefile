
all: vcdump

vcdump: vcdump.c
	gcc -g $^ -o $@

d: vcdump
	gdb --args vcdump bootcode.bin
