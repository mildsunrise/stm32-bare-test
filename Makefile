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

# 'openocd' has to be running in the background
boot: code.elf
	echo ramboot > /dev/tcp/localhost/4444

flash: code.elf
	openocd -f openocd.cfg -c init -c romboot -c exit

clean:
	rm -f *.o *.tmp code.elf
