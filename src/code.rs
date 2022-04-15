use core::panic::PanicInfo;

#[panic_handler]
fn panic_impl(_info: &PanicInfo) -> ! {
    loop {}
}

use crate::gpio::{GpioPin, GPIOA, GPIOD, GPIOE, PUPDR_PULLDOWN};
use crate::usart::{Usart, USART2};
use crate::rcc::{RCC};

const PIN_USER_BTN  : GpioPin = GPIOA.pin(0);

const PIN_LD3  : GpioPin = GPIOD.pin(13);  // up, orange
const PIN_LD4  : GpioPin = GPIOD.pin(12);  // left, green
const PIN_LD5  : GpioPin = GPIOD.pin(14);  // right, red
const PIN_LD6  : GpioPin = GPIOD.pin(15);  // down, blue

// const PIN_LCD_DB4  : GpioPin = GPIOE.pin(10);
// const PIN_LCD_DB5  : GpioPin = GPIOE.pin(11);
// const PIN_LCD_DB6  : GpioPin = GPIOE.pin(12);
// const PIN_LCD_DB7  : GpioPin = GPIOE.pin(13);
// const PIN_LCD_RS   : GpioPin = GPIOE.pin(14);
// const PIN_LCD_E    : GpioPin = GPIOE.pin(15);

const SERIAL: Usart = USART2;

fn init_serial_console() {
    // enable clock for GPIOA and USART2
    RCC.reg_ahb1enr().set_bits(1, 1, 0);
    RCC.reg_apb1enr().set_bits(1, 1, 17);

    // pass PA2 and PA3 to USART2
    GPIOA.pin(2).make_af(7);
    GPIOA.pin(3).make_af(7);

    SERIAL.init_tx_115200_at_16mhz();
}

pub fn main() -> ! {
    // SET UP BUSES / PERIPHERALS

    // enable clock for GPIOA and GPIOD
    RCC.reg_ahb1enr().set_bits(1, 1, 0);
    RCC.reg_ahb1enr().set_bits(1, 1, 3);
    init_serial_console();
    SERIAL.transmit_utf8("loading LCD...\r\n");
    //lcd_init();

    // SET UP GPIO

    PIN_USER_BTN.make_input(PUPDR_PULLDOWN);

    for pin in [ PIN_LD3, PIN_LD4, PIN_LD5, PIN_LD6 ] {
        pin.make_output_push_pull();
    }

    // CODE

    SERIAL.transmit_utf8("Hello from STM32! 🦊💕✨\r\n");
    //lcd_write_string("Hello from STM32");

    let led_sequence = [ PIN_LD4, PIN_LD3, PIN_LD5, PIN_LD6 ];
    let mut sequence_idx = 0;
    loop {
        //sleep_ns(400 * 1000000);
        while !PIN_USER_BTN.read() {}
        led_sequence[sequence_idx].write(false);
        sequence_idx = (sequence_idx + 1) % led_sequence.len();
        led_sequence[sequence_idx].write(true);
    }
}
