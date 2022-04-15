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
