#![no_main]
#![no_std]
#![feature(asm_const)]

mod memory;
use memory::*;

// VECTOR TABLE

global_asm!(
    ".pushsection VECTOR_TABLE, \"aw\"",
    ".global __VECTOR_TABLE",
    "__VECTOR_TABLE:",
        ".4byte reset",
        ".4byte {initial_sp}",
        ".rept 256 - 2",
        ".4byte 0",
        ".endr",
    ".popsection",
    initial_sp = const (BASE_SRAM1 + SIZE_SRAM1),
);

// not needed, but in case we want to access the vector table

#[allow(dead_code)]
#[repr(C, packed)]
struct VectorTable {
    initial_sp: usize,
    reset: usize,
    nmi: usize,
    hard_fault: usize,
    memory_management_fault: usize,
    bus_fault: usize,
    usage_fault: usize,
    __reserved_1: [usize; 4],
    sv_call: usize,
    __reserved_debug: usize,
    __reserved_2: usize,
    pend_sv: usize,
    systick: usize,
    irq: [usize; 240],
}

extern "C" {
    static mut __VECTOR_TABLE: VectorTable;
}

// ENTRY POINT

#[no_mangle]
fn reset() {
    loop {}
}

use core::panic::PanicInfo;
use core::arch::global_asm;

#[panic_handler]
fn panic_impl(_info: &PanicInfo) -> ! {
    loop {}
}
