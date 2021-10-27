all: code.elf

# FIXME: enable the FPU
CFLAGS := -g -O2 -mcpu=cortex-m4+nofp
LDFLAGS := -nostdlib

SOURCES = code.o entry.o

code.lds.tmp: code.lds
	arm-none-eabi-cpp -P -C $< -o $@

%.o: %.c
	arm-none-eabi-gcc -c $(CFLAGS) $< -o $@

code.elf: code.lds.tmp $(SOURCES)
	arm-none-eabi-ld $(LDFLAGS) $(SOURCES) -T code.lds.tmp -o code.elf

flash: code.elf
	openocd -f board/stm32f4discovery.cfg \
		-c 'program code.elf preverify verify reset exit 0x08000000'

clean:
	rm *.o *.tmp code.elf
