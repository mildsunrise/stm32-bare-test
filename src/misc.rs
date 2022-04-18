use core::arch::asm;

use crate::ppb::SCS;

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
