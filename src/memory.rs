#![allow(dead_code)]

// GENERAL MEMORY LAYOUT

pub const BASE_BOOT: usize =          0x0000 << 16;  // (aliased to flash or SRAM)

/* memory */
pub const BASE_FLASH_MAIN: usize =    0x0800 << 16;
    pub const SIZE_FLASH_MAIN: usize =              0x100000;
pub const BASE_CCMRAM: usize =        0x1000 << 16;
pub const BASE_FLASH_OTHER: usize =   0x1FFF << 16;
pub const BASE_SRAM1: usize =         0x2000 << 16;
    pub const SIZE_SRAM1: usize =                    0x1C000;
pub const BASE_SRAM2: usize =        (0x2001 << 16) + 0xC000;
    pub const SIZE_SRAM2: usize =                     0x4000;

/* peripherals */
pub const BASE_APB1: usize =          0x4000 << 16;
pub const BASE_APB2: usize =          0x4001 << 16;
pub const BASE_AHB1: usize =          0x4002 << 16; // until 0x4008_0000
pub const BASE_AHB2: usize =          0x5000 << 16; // until 0x5007_0000
pub const BASE_AHB3: usize =          0xA000 << 16;

/* system peripherals */
pub const BASE_PPB: usize =           0xE000 << 16;
    pub const SIZE_PPB: usize =                     0x100000;

// REGISTER WRAPPER

#[derive(Copy, Clone, Debug)]
pub struct Register(*mut u32);

impl Register {
    pub const unsafe fn new(base: usize, offset: isize) -> Self {
        Register((base as *mut u32).offset(offset))
    }
    pub fn read(&self) -> u32 {
        unsafe { self.0.read_volatile() }
    }
    pub fn write(&self, value: u32) {
        unsafe { self.0.write_volatile(value) }
    }
    pub fn update<F: FnOnce(u32) -> u32>(&self, f: F) {
        self.write(f(self.read()))
    }

    // for convenience
    pub fn set_mask(&self, value: u32, mask: u32) {
        debug_assert!((value & !mask) == 0);
        self.update(|x| (x & !mask) | value)
    }
    pub fn set_bits(&self, value: u32, nbits: u8, offset: u8) {
        debug_assert!(nbits.checked_add(offset).unwrap() <= 32);
        let mask = !(!0 << nbits);
        debug_assert!((value & !mask) == 0);
        self.set_mask(value << offset, mask << offset)
    }
    pub fn get_bits(&self, nbits: u8, offset: u8) -> u32 {
        debug_assert!(nbits.checked_add(offset).unwrap() <= 32);
        let mask = !(!0 << nbits);
        (self.read() >> offset) & mask
    }
}
