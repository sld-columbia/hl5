
ifndef V
        QUIET_CC            = @echo '   ' CC $@;
        QUIET_LINK          = @echo '   ' LINK $@;
        QUIET_MKDIR         = @echo '   ' MKDIR $@;
        QUIET_RUN           = @echo '   ' RUN $@;
        QUIET_MAKE          = @echo '   ' MAKE $@;
        QUIET_OBJDUMP       = @echo '   ' OBJDUMP $@;
        QUIET_OBJCOPY       = @echo '   ' OBJCOPY $@;
        QUIET_CLEAN         = @echo '   ' CLEAN $@;
endif
RM = rm -rf

CROSS_COMPILE ?= riscv32-unknown-elf-
CC = $(CROSS_COMPILE)gcc
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy

CFLAGS ?=
CFLAGS += -O2 -march=rv32ima

BOOT_OBJS = crt.o syscalls.o

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
TARGETS = $(SRCS:.c=.riscv)
LSTS = $(SRCS:.c=.lst)
SREC = $(SRCS:.c=.srec)
TEXT = $(SRCS:.c=.txt)

all: $(TARGETS) $(SREC) $(LSTS) $(TEXT)


.SUFFIX: .c .lst .srec .txt .riscv .o

$(LSTS): %.lst : %.riscv
	$(QUIET_OBJDUMP) $(OBJDUMP) -D -S -l -F -Mnumeric,no-aliases $< > $@

$(SREC): %.srec : %.riscv
	$(QUIET_OBJCOPY) $(OBJCOPY) -O srec --gap-fill 0 $< $@

$(TEXT): %.txt : %.srec srec2text.py
	$(QUIET_OBJCOPY) ./srec2text.py $< > $@

$(TARGETS): %.riscv : %.o $(BOOT_OBJS) ./common/test.ld
	$(QUIET_CC) $(CC) -T ./common/test.ld $< syscalls.o crt.o -o $@ -static -nostdlib -nostartfiles -lgcc

$(OBJS): %.o : %.c
	$(QUIET_CC) $(CC) $(CFLAGS) -mcmodel=medany -static -std=gnu99 -O2 -ffast-math -fno-common -fno-builtin-printf -DPREALLOCATE=1 -DHOST_DEBUG=0 -c $< -o $@

crt.o: common/crt.S common/encoding.h
	$(QUIET_CC) $(CC) -mcmodel=medany -static -std=gnu99 -O2 -ffast-math -fno-common -fno-builtin-printf -DPREALLOCATE=1 -DHOST_DEBUG=0 -D__ASSEMBLY__=1 -c -I./common $< -o $@

syscalls.o: common/syscalls.c
	$(QUIET_CC) $(CC) -mcmodel=medany -static -std=gnu99 -O2 -ffast-math -fno-common -fno-builtin-printf -DPREALLOCATE=1 -DHOST_DEBUG=0 -c $< -o $@

clean:
	$(QUIET_CLEAN) $(RM) $(OBJS) $(BOOT_OBJS) *.lst *.srec *.txt

distclean: clean
	$(QUIET_CLEAN) $(RM) $(TARGETS)

.PHONY: all clean distclean
