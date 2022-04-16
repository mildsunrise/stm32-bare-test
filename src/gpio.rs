#![allow(dead_code)]

use crate::memory::{Register, BASE_AHB1};

const BASE_GPIO: usize = BASE_AHB1;
const OFFSET_GPIO_PORT: usize = 0x400;

pub const GPIOA: Gpio = Gpio(BASE_GPIO + OFFSET_GPIO_PORT * 0);
pub const GPIOB: Gpio = Gpio(BASE_GPIO + OFFSET_GPIO_PORT * 1);
pub const GPIOC: Gpio = Gpio(BASE_GPIO + OFFSET_GPIO_PORT * 2);
pub const GPIOD: Gpio = Gpio(BASE_GPIO + OFFSET_GPIO_PORT * 3);
pub const GPIOE: Gpio = Gpio(BASE_GPIO + OFFSET_GPIO_PORT * 4);
pub const GPIOF: Gpio = Gpio(BASE_GPIO + OFFSET_GPIO_PORT * 5);
pub const GPIOG: Gpio = Gpio(BASE_GPIO + OFFSET_GPIO_PORT * 6);
pub const GPIOH: Gpio = Gpio(BASE_GPIO + OFFSET_GPIO_PORT * 7);
pub const GPIOI: Gpio = Gpio(BASE_GPIO + OFFSET_GPIO_PORT * 8);

#[derive(Copy, Clone, Debug)]
pub struct Gpio(usize);

#[derive(Copy, Clone, Debug)]
pub struct GpioPin(pub Gpio, u8);

pub const PUPDR_NONE: u8 =     0b00;
pub const PUPDR_PULLUP: u8 =   0b01;
pub const PUPDR_PULLDOWN: u8 = 0b10;
// 0b11 is reserved

impl Gpio {
    fn reg_moder    (&self) -> Register { unsafe { Register::new(self.0, 0) } }
    fn reg_otyper   (&self) -> Register { unsafe { Register::new(self.0, 1) } }
    fn reg_ospeedr  (&self) -> Register { unsafe { Register::new(self.0, 2) } }
    fn reg_pupdr    (&self) -> Register { unsafe { Register::new(self.0, 3) } }
    fn reg_idr      (&self) -> Register { unsafe { Register::new(self.0, 4) } }
    fn reg_odr      (&self) -> Register { unsafe { Register::new(self.0, 5) } }
    fn reg_bsrr     (&self) -> Register { unsafe { Register::new(self.0, 6) } }
    fn reg_lckr     (&self) -> Register { unsafe { Register::new(self.0, 7) } }
    fn reg_afrl     (&self) -> Register { unsafe { Register::new(self.0, 8) } }
    fn reg_afrh     (&self) -> Register { unsafe { Register::new(self.0, 9) } }

    pub const fn pin(&self, pin: u8) -> GpioPin {
        debug_assert!(pin < 16);
        GpioPin(*self, pin)
    }
}

impl GpioPin {
    pub fn make_af(&self, af: u8) {
        let GpioPin(gpio, pin) = *self;
        debug_assert!(af < 16);
        let af_reg = if pin < 8 { gpio.reg_afrl() } else { gpio.reg_afrh() };
        af_reg.set_bits(af.into(), 4, (pin % 8) * 4); // set AF number
        gpio.reg_moder().set_bits(0b10, 2, pin * 2); // put into AF mode
    }

    pub fn make_output_push_pull(&self) {
        let GpioPin(gpio, pin) = *self;
        gpio.reg_pupdr().set_bits(PUPDR_NONE.into(), 2, pin * 2); // disable pull-up / pull-down
        gpio.reg_otyper().set_bits(0b0, 1, pin); // set push-pull
        gpio.reg_moder().set_bits(0b01, 2, pin * 2); // set output
    }

    pub fn make_output_open_drain(&self, pullups: u8) {
        let GpioPin(gpio, pin) = *self;
        debug_assert!(pullups <= 0b11);
        gpio.reg_otyper().set_bits(0b1, 1, pin); // set open-drain
        gpio.reg_pupdr().set_bits(pullups.into(), 2, pin * 2); // set pullups
        gpio.reg_moder().set_bits(0b01, 2, pin * 2); // set output
    }

    pub fn make_input(&self, pullups: u8) {
        let GpioPin(gpio, pin) = *self;
        debug_assert!(pullups <= 0b11);
        gpio.reg_moder().set_bits(0b00, 2, pin * 2); // set input
        gpio.reg_pupdr().set_bits(pullups.into(), 2, pin * 2);
    }

    pub fn write(&self, value: bool) {
        let GpioPin(gpio, pin) = *self;
        let bit = pin + if value { 0 } else { 16 };
        gpio.reg_bsrr().write(1 << bit)
    }

    pub fn read(&self) -> bool {
        let GpioPin(gpio, pin) = *self;
        (gpio.reg_idr().read() >> pin) & 1 != 0
    }
}
