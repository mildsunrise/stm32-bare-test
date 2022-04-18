# stm32-bare-test

This is an attempt at programming an STM32F4 with nothing but a Rust toolchain (with Cargo).  
OpenOCD is used to flash the compiled firmware to the MCU.

In particular, this is for the [STM32F4DISCOVERY board][stm32f4-discovery].  
If you have one of these and want to try, install:

 - the Rust toolchain (currently nightly is required), with e.g. [Rustup](https://rustup.rs):

   ~~~ bash
   curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
   rustup toolchain install nightly
   rustup default nightly

   rustup target add thumbv7em-none-eabihf  # support for our target platform
   ~~~

 - OpenOCD, e.g. `apt install openocd`

Then, compile with:

~~~ bash
cargo build
~~~

Plug in your board, and flash it with:

~~~ bash
make flash
~~~

This is a personal experiment to exercise my low level skills, see [this thread][thread].  
Don't expect fantastic code quality, or meaningful functionality.


## Structure

Main files (application-specific code is highlighted in bold):

 - `Cargo.toml`: Cargo package manifest
 - `src/`: Application code
   - `memory.rs`: General layout of the address space
   - `entry.rs`: Firmware entrypoint (vector table, entry point, fault handlers, ...)
   - `code.rs`: **Main application code**
   - `ppb.rs`: Internal processor peripherals (SCS / SCB, NVIC, MPU, SysTick, CoreSight, debug features, ...)
   - `misc.rs`: **Miscellaneous processor operations (FPU, sleep, etc.)**
   - `rcc.rs`: Reset and Clock Control peripheral
   - `gpio.rs`: General Purpose Input/Output peripheral
   - `usart.rs`: Universal Serial Asynchronous Receiver/Transmitter peripheral

Buildsystem:

 - `.cargo/config.toml`: Compilation flags
 - `code.lds`: Linker script (set up memory regions, etc.)
 - `build.rs`: Build script to preprocess the linker script

Uploading:

 - `openocd.cfg`: Programming procedures (flash, RAM boot, etc.)
 - `Makefile`: Shortcuts to build + upload, for convenience

Other:

 - `docs/`: Manufacturer documentation (board, MCU, core)


[stm32f4-discovery]: https://www.st.com/en/evaluation-tools/stm32f4discovery.html
[thread]: https://twitter.com/mild_sunrise/status/1452296814770769920
