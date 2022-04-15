#![allow(dead_code)]

use crate::memory::{Register, BASE_APB1, BASE_APB2};

// USART registers
pub const USART1: Usart = Usart(BASE_APB2 + 0x1000);
pub const USART2: Usart = Usart(BASE_APB1 + 0x4400);
pub const USART3: Usart = Usart(BASE_APB1 + 0x4800);
pub const UART4:  Usart = Usart(BASE_APB1 + 0x4C00);
pub const UART5:  Usart = Usart(BASE_APB1 + 0x5000);
pub const USART6: Usart = Usart(BASE_APB2 + 0x1400);
pub const UART7:  Usart = Usart(BASE_APB1 + 0x7800);
pub const UART8:  Usart = Usart(BASE_APB1 + 0x7C00);

#[derive(Copy, Clone, Debug)]
pub struct Usart(usize);

impl Usart {
    fn reg_sr   (&self) -> Register { unsafe { Register::new(self.0, 0) } }
    fn reg_dr   (&self) -> Register { unsafe { Register::new(self.0, 1) } }
    fn reg_brr  (&self) -> Register { unsafe { Register::new(self.0, 2) } }
    fn reg_cr1  (&self) -> Register { unsafe { Register::new(self.0, 3) } }
    fn reg_cr2  (&self) -> Register { unsafe { Register::new(self.0, 4) } }
    fn reg_cr3  (&self) -> Register { unsafe { Register::new(self.0, 5) } }
    fn reg_gtpr (&self) -> Register { unsafe { Register::new(self.0, 6) } }

    pub fn init_tx_115200_at_16mhz(&self) {
        // enable UART, set signal parameters
        self.reg_cr1().set_bits(1, 1, 13); // UE (USART enable)
        self.reg_cr1().set_bits(0, 1, 12); // M (word length) = 8 data bits
        self.reg_cr1().set_bits(0, 1, 10); // PCE (parity control enable) = no parity
        self.reg_cr2().set_bits(0b00, 2, 12); // STOP = 1 stop bit

        // set up a 115200 baud rate
        // 1. we only need oversampling by 8, since 16 doesn't improve the error
        self.reg_cr1().set_bits(1, 1, 15); // OVER8
        // 2. assuming default SYSCLK selection, AHB & APB1 prescaler values,
        //    we are running at 16MHz. therefore 16MHz * 8 / 115200 = 17.361
        // 3. mantissa = 17; fractional = 0.361 * 8 = 2.88 -> 3
        self.reg_brr().write((17 << 4) | (3 << 0));

        // enable transmitter (sends idle frame)
        self.reg_cr1().set_bits(1, 1, 3); // TE (transmitter enable)
    }

    pub fn transmit(&self, data: &[u8]) {
        for &byte in data.iter() {
            self.reg_dr().write(byte.into());
            while self.reg_sr().get_bits(1, 7) == 0 {} // wait for TXE=1
        }
        while self.reg_sr().get_bits(1, 6) == 0 {} // wait for TC=1
    }

    pub fn transmit_utf8(&self, data: &str) {
        self.transmit(data.as_bytes())
    }
}
