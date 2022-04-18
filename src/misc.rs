use core::arch::asm;
use core::num::Wrapping;

use crate::ppb::{SCS, DWT};

// FPU

// [STM32+CortexM4 specific] performs the procedure to enable the FPU
pub fn enable_fpu() {
    // enable coprocessors 10 and 11
    SCS.reg_scb_cpacr().set_bits(0b11, 2, 10*2);  // CP10 = 0b11 (full access)
    SCS.reg_scb_cpacr().set_bits(0b11, 2, 11*2);  // CP11 = 0b11 (full access)

    unsafe { asm!(
        "dsb", // wait for the store to complete
        "isb", // reset pipeline now that the FPU is enabled
    ) };
}

// SLEEP

pub fn enable_cyccnt(enabled: bool) {
    // enable tracing modules (we need DWT)
    SCS.reg_demcr().set_bits(1, 1, 24); // TRCENA=1
    // unlock the DWT registers
    let _lock = DWT.coresight().unlock();
    // set CYCCNTENA bit
    DWT.reg_ctrl().set_bits(u32::from(enabled), 1, 0);
}

// returns current cycle count
// (requires calling `enable_cyccnt` first)
#[inline]
pub fn cyccnt() -> Wrapping<u32> {
    Wrapping(DWT.reg_cyccnt().read())
}

const SYSCLK_FREQ_HZ: u32 = 16000000;
const NS_PER_CYCLE: f32 = (1e9 / (SYSCLK_FREQ_HZ as f64)) as f32;

// short pause (busy wait)
// (requires calling `enable_cyccnt` first)
#[inline]
pub fn sleep_ns(ns: u32) {
    // sample register as soon as possible
    let now = cyccnt();
    // calculate target value (compensate by loop iteration time)
    let target = now + Wrapping((ns as f32 / NS_PER_CYCLE) as u32) - Wrapping(4);
    // wait for target value reached
    while ((cyccnt() - target).0 as i32) < 0 {}
}

// very short pause (clock cycle)
#[inline(always)]
pub fn clock_cycle() {
    unsafe { asm!("and r0, r0") };
}
