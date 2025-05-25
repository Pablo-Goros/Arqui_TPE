include Makefile.inc

MODULE    := ../0000-sampleCodeModule.bin
C_SRCS    := $(wildcard [^_]*.c)
ASM_SRCS  := $(wildcard assembly/*.asm)
OBJS      := $(C_SRCS:%.c=%.o) $(ASM_SRCS:assembly/%.asm=assembly/%.o)

CFLAGS    := $(GCCFLAGS) -std=gnu11 -ffreestanding -O2 -Iinclude
ASFLAGS   := $(ASMFLAGS) -felf64

all: $(MODULE)

$(MODULE): $(OBJS)
	$(GCC) $(CFLAGS) -nostdlib -T userCodeModule.ld _loader.c -o $(MODULE) $(OBJS)

%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

assembly/%.o: assembly/%.asm
	$(ASM) $(ASFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(MODULE)
