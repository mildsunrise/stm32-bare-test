# stm32-bare-test

This is an attempt at programming an STM32F4 with nothing but an ARM GCC toolchain.  
OpenOCD is used to flash the compiled firmware to the MCU.

In particular, this is for the [STM32F4DISCOVERY board][stm32f4-discovery].  
If you have one of these and want to try, install the toolchain and OpenOCD:

~~~ bash
sudo apt install arm-none-eabi-gcc openocd
~~~

Then, compile `code.elf` with:

~~~ bash
make
~~~

Plug in your board, and flash it with:

~~~ bash
make flash
~~~

This is a personal experiment to exercise my low level skills, see [this thread][thread].  
Don't expect fantastic code quality, or meaningful functionality.


## Structure

 - `code.c`: Main application code
 - `constants.h`: Memory layout constants
 - `entry.{h,c}`: Core CPU initialization (vector table, stack, entry point, enable FPU, ...)
 - `Makefile`: The 'buildsystem'
 - `code.lds`: Linker script (set up memory regions, etc.)
 - `openocd.cfg`: Programming procedures (flash, RAM boot, etc.)

Other:

 - `docs/`: Manufacturer documentation (board, MCU, core)


[stm32f4-discovery]: https://www.st.com/en/evaluation-tools/stm32f4discovery.html
[thread]: https://twitter.com/mild_sunrise/status/1452296814770769920
