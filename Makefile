all: code.elf

# FIXME: enable the FPU
LDFLAGS := -nostdlib --gc-sections

SOURCES = $(PWD)/target/thumbv7em-none-eabi/release/libcode.a
-include $(SOURCES:%.a=%.d)

code.lds.tmp: code.lds
	arm-none-eabi-cpp -P -C $< -o $@

$(SOURCES):
	cargo build --release

code.elf: code.lds.tmp $(SOURCES)
	arm-none-eabi-ld $(LDFLAGS) $(SOURCES) -T code.lds.tmp -o code.elf

# 'openocd' has to be running in the background
boot: code.elf
	echo ramboot > /dev/tcp/localhost/4444

flash: code.elf
	openocd -f openocd.cfg -c init -c romboot -c exit

clean:
	rm -r *.tmp code.elf; cargo clean
