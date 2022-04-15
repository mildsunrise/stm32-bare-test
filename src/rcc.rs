#![allow(dead_code)]

use crate::memory::{Register, BASE_AHB1};

pub const RCC: Rcc = Rcc(BASE_AHB1 + 0x3800);

#[derive(Copy, Clone, Debug)]
pub struct Rcc(usize);

impl Rcc {
    pub fn reg_cr            (&self) -> Register { unsafe { Register::new(self.0, 0) } }
    pub fn reg_pllcfgr       (&self) -> Register { unsafe { Register::new(self.0, 1) } }
    pub fn reg_cfgr          (&self) -> Register { unsafe { Register::new(self.0, 2) } }
    pub fn reg_cir           (&self) -> Register { unsafe { Register::new(self.0, 3) } }
    pub fn reg_ahb1rstr      (&self) -> Register { unsafe { Register::new(self.0, 4) } }
    pub fn reg_ahb2rstr      (&self) -> Register { unsafe { Register::new(self.0, 5) } }
    pub fn reg_ahb3rstr      (&self) -> Register { unsafe { Register::new(self.0, 6) } }
    /* reserved: 7 */
    pub fn reg_apb1rstr      (&self) -> Register { unsafe { Register::new(self.0, 8) } }
    pub fn reg_apb2rstr      (&self) -> Register { unsafe { Register::new(self.0, 9) } }
    /* reserved: 10 */
    /* reserved: 11 */
    pub fn reg_ahb1enr       (&self) -> Register { unsafe { Register::new(self.0, 12) } }
    pub fn reg_ahb2enr       (&self) -> Register { unsafe { Register::new(self.0, 13) } }
    pub fn reg_ahb3enr       (&self) -> Register { unsafe { Register::new(self.0, 14) } }
    /* reserved: 15 */
    pub fn reg_apb1enr       (&self) -> Register { unsafe { Register::new(self.0, 16) } }
    pub fn reg_apb2enr       (&self) -> Register { unsafe { Register::new(self.0, 17) } }
    /* reserved: 18 */
    /* reserved: 19 */
    pub fn reg_ahb1lpenr     (&self) -> Register { unsafe { Register::new(self.0, 20) } }
    pub fn reg_ahb2lpenr     (&self) -> Register { unsafe { Register::new(self.0, 21) } }
    pub fn reg_ahb3lpenr     (&self) -> Register { unsafe { Register::new(self.0, 22) } }
    /* reserved: 23 */
    pub fn reg_apb1lpenr     (&self) -> Register { unsafe { Register::new(self.0, 24) } }
    pub fn reg_apb2lpenr     (&self) -> Register { unsafe { Register::new(self.0, 25) } }
    /* reserved: 26 */
    /* reserved: 27 */
    pub fn reg_bdcr          (&self) -> Register { unsafe { Register::new(self.0, 28) } }
    pub fn reg_csr           (&self) -> Register { unsafe { Register::new(self.0, 29) } }
    /* reserved: 30 */
    /* reserved: 31 */
    pub fn reg_sscgr         (&self) -> Register { unsafe { Register::new(self.0, 32) } }
    pub fn reg_plli2scfgr    (&self) -> Register { unsafe { Register::new(self.0, 33) } }
}
