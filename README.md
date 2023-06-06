About:
  Research VideoCore IV bytecode structure.

Reason:
  Graphic processor on a Raspberry PI 3b+ executes
  this bytecode to startup a kernel, which is compiled
  for ARM. Arm is easily disassembled and compiled by
  existing tools.
  VideoCore bytecode is not so ready for disassembly.
  This is strange, considering that Raspberry PI is
  selled as hardware for non-professional opensource
  engineering (in schools, universities, by hobbyists,
  etc.), whereas not so much info is given how it is
  bootloader works.
  That leads to outdated documentation, that is present
  all over the internet, which might not reflect actual
  parameters.
  That also makes developers try to tweak config.txt
  and try different parameters there, which is not the
  right approach.
  I intend to understand how Raspberry PI actually
  starts up.

References:
  I am greatly inspired by these two projects, which are
  publicly available.
  1. vc4 emulator, it gave me initial idea how I should
  approach instruction decoding:
  https://github.com/mgottschlag/vctools.git

  2. herman heritage videocoreiv project, that contains
  great effort of reverse engineering the bytecode. I
  find this very interesting and use as reference.
  https://github.com/hermanhermitage/videocoreiv

  No official documentation is used as reference.
  I do not have any broadcom documentation at hand,
  only 2 above links.


Building:
  git clone ...
  cd vc4dump
  make

Running:
  ./vc4dump bootcode.bin
