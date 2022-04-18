#![allow(dead_code)]

// This module exposes the peripherals in the Private Peripheral Bus.
// Except for the parts explicitely marked as 'XXX specific', this
// applies to ARMv7-M cores in general

use crate::memory::{Register, BASE_PPB};


// CoreSight management registers
// ------------------------------

// CoreSight doesn't have a (single) base address, it's a sub-component
// implemented in many of the peripherals in the PPB (SCS, DWT, etc.)

pub struct CoreSight(usize);

pub const CORESIGHT_UNLOCK_VALUE: u32 = 0xC5ACCE55;

impl CoreSight {
    // Accesses CoreSight on a certain peripheral, given the 12-bit
    // block it's mapped in (or the last block if it's mapped to
    // multiple blocks)
    pub const fn new(last_block: usize) -> CoreSight {
        // CoreSight regs sit on the last 0x100 section of the block
        CoreSight(last_block + 0xF00)
    }

    // registers

    // [start of space]
    pub fn reg_itctrl      (&self) -> Register { unsafe { Register::new(self.0, 0) } }

    pub fn reg_claimset    (&self) -> Register { unsafe { Register::new(self.0 + 0xA0,  0) } }
    pub fn reg_claimclr    (&self) -> Register { unsafe { Register::new(self.0 + 0xA0,  1) } }
    pub fn reg_devaff0     (&self) -> Register { unsafe { Register::new(self.0 + 0xA0,  2) } }
    pub fn reg_devaff1     (&self) -> Register { unsafe { Register::new(self.0 + 0xA0,  3) } }
    pub fn reg_lar         (&self) -> Register { unsafe { Register::new(self.0 + 0xA0,  4) } }
    pub fn reg_lsr         (&self) -> Register { unsafe { Register::new(self.0 + 0xA0,  5) } }
    pub fn reg_authstatus  (&self) -> Register { unsafe { Register::new(self.0 + 0xA0,  6) } }
    pub fn reg_devarch     (&self) -> Register { unsafe { Register::new(self.0 + 0xA0,  7) } }
    pub fn reg_devid2      (&self) -> Register { unsafe { Register::new(self.0 + 0xA0,  8) } }
    pub fn reg_devid1      (&self) -> Register { unsafe { Register::new(self.0 + 0xA0,  9) } }
    pub fn reg_devid       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 10) } }
    pub fn reg_devtype     (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 11) } }
    pub fn reg_pidr4       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 12) } }
    pub fn reg_pidr5       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 13) } }
    pub fn reg_pidr6       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 14) } }
    pub fn reg_pidr7       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 15) } }
    pub fn reg_pidr0       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 16) } }
    pub fn reg_pidr1       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 17) } }
    pub fn reg_pidr2       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 18) } }
    pub fn reg_pidr3       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 19) } }
    pub fn reg_cidr0       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 20) } }
    pub fn reg_cidr1       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 21) } }
    pub fn reg_cidr2       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 22) } }
    pub fn reg_cidr3       (&self) -> Register { unsafe { Register::new(self.0 + 0xA0, 23) } }
    // [end of space]

    // high-level methods

    // (not thread-safe)
    pub fn unlock(&self) -> CoreSightLock { CoreSightLock::new(self.reg_lar()) }
}

pub struct CoreSightLock(Register, u32);

impl CoreSightLock {
    fn new(lar: Register) -> Self {
        let old_value = lar.read();
        lar.write(CORESIGHT_UNLOCK_VALUE);
        CoreSightLock(lar, old_value)
    }
}

impl Drop for CoreSightLock {
    fn drop(&mut self) {
        let CoreSightLock(lar, old_value) = *self;
        lar.write(old_value);
    }
}


// System Control Space
// --------------------

pub const SCS: Scs = Scs(BASE_PPB + 0xE000);

pub struct Scs(usize);

impl Scs {
    // System control and ID
    //  [reserved: Master Control register]
    pub fn reg_scb_ictr       (&self) -> Register { unsafe { Register::new(self.0 + 0x0, 1) } }
    pub fn reg_scb_actlr      (&self) -> Register { unsafe { Register::new(self.0 + 0x0, 2) } }

    // SysTick
    pub fn reg_syst_csr       (&self) -> Register { unsafe { Register::new(self.0 + 0x10, 0) } }
    pub fn reg_syst_rvr       (&self) -> Register { unsafe { Register::new(self.0 + 0x10, 1) } }
    pub fn reg_syst_cvr       (&self) -> Register { unsafe { Register::new(self.0 + 0x10, 2) } }
    pub fn reg_syst_calib     (&self) -> Register { unsafe { Register::new(self.0 + 0x10, 3) } }

    // NVIC
    pub fn reg_nvic_iser      (&self, n: u8) -> Register { debug_assert!(n < 16); unsafe { Register::new(self.0 + 0x100, n.into()) } }
    pub fn reg_nvic_icer      (&self, n: u8) -> Register { debug_assert!(n < 16); unsafe { Register::new(self.0 + 0x180, n.into()) } }
    pub fn reg_nvic_ispr      (&self, n: u8) -> Register { debug_assert!(n < 16); unsafe { Register::new(self.0 + 0x200, n.into()) } }
    pub fn reg_nvic_icpr      (&self, n: u8) -> Register { debug_assert!(n < 16); unsafe { Register::new(self.0 + 0x280, n.into()) } }
    pub fn reg_nvic_iabr      (&self, n: u8) -> Register { debug_assert!(n < 16); unsafe { Register::new(self.0 + 0x300, n.into()) } }
    pub fn reg_nvic_ipr       (&self, n: u8) -> Register { debug_assert!(n < 16); unsafe { Register::new(self.0 + 0x400, n.into()) } }

    // System control and ID
    pub fn reg_scb_cpuid      (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 0) } }
    pub fn reg_scb_icsr       (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 1) } }
    pub fn reg_scb_vtor       (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 2) } }
    pub fn reg_scb_aircr      (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 3) } }
    pub fn reg_scb_scr        (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 4) } }
    pub fn reg_scb_ccr        (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 5) } }
    pub fn reg_scb_shpr1      (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 6) } }
    pub fn reg_scb_shpr2      (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 7) } }
    pub fn reg_scb_shpr3      (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 8) } }
    pub fn reg_scb_shcsr      (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 9) } }
    pub fn reg_scb_cfsr       (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 10) } }
    pub fn reg_scb_hfsr       (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 11) } }
    pub fn reg_scb_dfsr       (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 12) } }
    pub fn reg_scb_mmfar      (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 13) } }
    pub fn reg_scb_bfar       (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 14) } }
    pub fn reg_scb_afsr       (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 15) } }
    //  [reserved: CPUID table]
    pub fn reg_scb_cpacr      (&self) -> Register { unsafe { Register::new(self.0 + 0xD00, 34) } }

    // MPU
    pub fn reg_mpu_type       (&self) -> Register { unsafe { Register::new(self.0 + 0xD90, 0) } }
    pub fn reg_mpu_ctrl       (&self) -> Register { unsafe { Register::new(self.0 + 0xD90, 1) } }
    pub fn reg_mpu_rnr        (&self) -> Register { unsafe { Register::new(self.0 + 0xD90, 2) } }
    pub fn reg_mpu_rbar       (&self) -> Register { unsafe { Register::new(self.0 + 0xD90, 3) } }
    pub fn reg_mpu_rasr       (&self) -> Register { unsafe { Register::new(self.0 + 0xD90, 4) } }
    pub fn reg_mpu_rbar_a1    (&self) -> Register { unsafe { Register::new(self.0 + 0xD90, 5) } }
    pub fn reg_mpu_rasr_a1    (&self) -> Register { unsafe { Register::new(self.0 + 0xD90, 6) } }
    pub fn reg_mpu_rbar_a2    (&self) -> Register { unsafe { Register::new(self.0 + 0xD90, 7) } }
    pub fn reg_mpu_rasr_a2    (&self) -> Register { unsafe { Register::new(self.0 + 0xD90, 8) } }
    pub fn reg_mpu_rbar_a3    (&self) -> Register { unsafe { Register::new(self.0 + 0xD90, 9) } }
    pub fn reg_mpu_rasr_a3    (&self) -> Register { unsafe { Register::new(self.0 + 0xD90, 10) } }

    // Debug
    pub fn reg_dhcsr      (&self) -> Register { unsafe { Register::new(self.0 + 0xDF0, 0) } }
    pub fn reg_dcrsr      (&self) -> Register { unsafe { Register::new(self.0 + 0xDF0, 1) } }
    pub fn reg_dcrdr      (&self) -> Register { unsafe { Register::new(self.0 + 0xDF0, 2) } }
    pub fn reg_demcr      (&self) -> Register { unsafe { Register::new(self.0 + 0xDF0, 3) } }

    // This last 8-bit section overlaps with CoreSight:
    pub fn reg_scb_stir       (&self) -> Register { unsafe { Register::new(self.0 + 0xF00, 0) } }
    /// [STM32+CortexM4 specific] FPU
    pub fn reg_fpccr          (&self) -> Register { unsafe { Register::new(self.0 + 0xF00, 13) } }
    pub fn reg_fpcar          (&self) -> Register { unsafe { Register::new(self.0 + 0xF00, 14) } }
    pub fn reg_fpdscr         (&self) -> Register { unsafe { Register::new(self.0 + 0xF00, 15) } }

    pub fn coresight(&self) -> CoreSight { CoreSight::new(self.0) }
}


// Data Watchpoint and Trace
// -------------------------

pub const DWT: Dwt = Dwt(BASE_PPB + 0x1000);

pub struct Dwt(usize);

impl Dwt {
    pub fn reg_ctrl      (&self) -> Register { unsafe { Register::new(self.0, 0) } }
    pub fn reg_cyccnt    (&self) -> Register { unsafe { Register::new(self.0, 1) } }
    pub fn reg_cpicnt    (&self) -> Register { unsafe { Register::new(self.0, 2) } }
    pub fn reg_exccnt    (&self) -> Register { unsafe { Register::new(self.0, 3) } }
    pub fn reg_sleepcnt  (&self) -> Register { unsafe { Register::new(self.0, 4) } }
    pub fn reg_lsucnt    (&self) -> Register { unsafe { Register::new(self.0, 5) } }
    pub fn reg_foldcnt   (&self) -> Register { unsafe { Register::new(self.0, 6) } }
    pub fn reg_pcsr      (&self) -> Register { unsafe { Register::new(self.0, 7) } }
    pub unsafe fn reg_comp      (&self, n: u8) -> Register { Register::new(self.0, 8 + isize::from(n)*4 + 0) }
    pub unsafe fn reg_mask      (&self, n: u8) -> Register { Register::new(self.0, 8 + isize::from(n)*4 + 1) }
    pub unsafe fn reg_function  (&self, n: u8) -> Register { Register::new(self.0, 8 + isize::from(n)*4 + 2) }

    pub fn coresight(&self) -> CoreSight { CoreSight::new(self.0) }
}
