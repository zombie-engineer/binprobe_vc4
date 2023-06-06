
all: vcdump

vc4_scalar_op_table.o: vc4_scalar_op_table.c
	gcc -c -g $^ -o $@

vc4_scalar_op.o: vc4_scalar_op.c
	gcc -c -g $^ -o $@

filereader.o: filereader.c
	gcc -c -g $^ -o $@

symbol_table.o: symbol_table.c
	gcc -c -g $^ -o $@

vc4_decode.o: vc4_decode.c
	gcc -c -g $^ -o $@

vc4_instructions.o: vc4_instructions.c
	gcc -c -g $^ -o $@

vcdump: vcdump.c vc4_decode.o vc4_instructions.o symbol_table.o filereader.o vc4_scalar_op_table.o vc4_scalar_op.o
	gcc -g $^ -o $@

d: vcdump
	gdb --args vcdump bootcode.bin
