#![no_main]
#![no_std]
#![feature(asm_const)]

mod memory;
mod gpio;
mod usart;
mod code;
mod rcc;
mod ppb;

use core::arch::global_asm;

use crate::memory::*;

// VECTOR TABLE

global_asm!(
    ".pushsection VECTOR_TABLE, \"a\"",
    ".global __VECTOR_TABLE",
    "__VECTOR_TABLE: .type __VECTOR_TABLE, #object",
        ".4byte {initial_sp}",
        ".4byte reset",
        ".4byte __nmi",
        ".4byte __hard_fault",
        ".rept 256 - 4",
        ".4byte 0",
        ".endr",
        ".size __VECTOR_TABLE, . - __VECTOR_TABLE",
    ".popsection",

    ".global __nmi",
    "__nmi: .type __nmi, #function",
        "bkpt",
        ".size __nmi, . - __nmi",
    ".global __hard_fault",
    "__hard_fault: .type __hard_fault, #function",
        "bkpt",
        ".size __hard_fault, . - __hard_fault",
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
    static __VECTOR_TABLE: VectorTable;
}

// ENTRY POINT

#[no_mangle]
fn reset() -> ! {
    crate::code::main()
}
