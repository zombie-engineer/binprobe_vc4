MAINBIN=vc4dump
all: $(MAINBIN)

CFLAGS := -c -g
OBJS := filereader \
  symbol_table \
  vc4_scalar_op_table \
  vc4_scalar_op \
  vc4_decode \
  vc4_instructions \
  vc4_dump

OBJS := $(addsuffix .o, $(OBJS))
OBJS := $(addprefix build/,$(OBJS))

$(info $(OBJS))
build/%.o: %.c
	gcc $(CFLAGS) $^ -o $@

$(MAINBIN): $(OBJS)
	gcc -g $^ -o $@

d: $(MAINBIN)
	gdb --args $@ bootcode.bin

clean:
	rm -f -v *.o $(MAINBIN)
