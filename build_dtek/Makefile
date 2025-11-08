SRC_DIR ?= ./ ../src
OBJ_DIR ?= ./
SOURCES ?= $(shell find $(SRC_DIR) -name '*.c' -or -name '*.S')
OBJECTS ?= $(addsuffix .o, $(basename $(notdir $(SOURCES))))
LINKER ?= ./dtekv-script.lds

TOOLCHAIN ?= riscv32-unknown-elf-

# add DTEK platform flag macro
# add the dtek build directory and all files in /src as an include path
CFLAGS ?= -Wall -nostdlib -O3 -mabi=ilp32 -march=rv32imzicsr -fno-builtin \
	-D__DTEK__=1 \
	-I. \
	-I../src/**


build: clean main.bin

main.elf: 
	$(TOOLCHAIN)gcc -c $(CFLAGS) $(SOURCES)
	$(TOOLCHAIN)ld -o $@ -T $(LINKER) $(filter-out boot.o, $(OBJECTS)) softfloat.a

main.bin: main.elf
	$(TOOLCHAIN)objcopy --output-target binary $< $@
	$(TOOLCHAIN)objdump -D $< > $<.txt

clean:
	rm -f *.o *.elf *.bin *.txt

TOOL_DIR ?= ./tools
run: main.bin
	make -C $(TOOL_DIR) "FILE_TO_RUN=$(CURDIR)/$<"
