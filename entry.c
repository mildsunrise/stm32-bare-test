#include "entry.h"

struct vector_table {
    void *initial_sp;
    void *reset;
    void *nmi;
    void *hard_fault;
    void *memory_management_fault;
    void *bus_fault;
    void *usage_fault;
    void *__reserved_1[4];
    void *sv_call;
    void *__reserved_debug;
    void *__reserved_2;
    void *pend_sv;
    void *systick;
    void *irq[240];
};

static void _start();

// must be the first definition in this file
const struct vector_table __VECTOR_TABLE __attribute__((section("VECTOR_TABLE"))) = {
    .initial_sp = (void*)(BASE_SRAM1 + SIZE_SRAM1),
    .reset = _start,
    // rest zeroed for now
};

void main();

static void _start() {
    // enable CYCCNT for sleep_ns() to operate
    dwt_enable_cyccnt(1);
    // application code
    main();
    // control should never reach back here,
    // but if it does, idle loop
    while (1);
}
