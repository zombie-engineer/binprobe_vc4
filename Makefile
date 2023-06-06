all: vcdump

CFLAGS := -c -g
OBJS := filereader \
  symbol_table \
  vc4_scalar_op_table \
  vc4_scalar_op \
  vc4_decode \
  vc4_instructions \
  vcdump

OBJS := $(addsuffix .o, $(OBJS))

%.o: %.c
	gcc $(CFLAGS) $^ -o $@

vcdump: $(OBJS)
	gcc -g $^ -o $@

d: vcdump
	gdb --args vcdump bootcode.bin

clean:
	rm -f -v *.o vcdump
