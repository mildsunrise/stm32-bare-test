// API
// ---

static inline void dwt_enable_cyccnt(int enabled) {
    // enable tracing modules (we need DWT)
    SCS_DEMCR |= (1 << 24); // TRCENA=1
    // unlock the DWT registers
    uint32_t oldLAR = CoreSight_LAR(BASE_DWT);
    CoreSight_LAR(BASE_DWT) = CoreSight_UNLOCK_VALUE;
    // set CYCCNTENA bit
    enabled ? (DWT_CTRL |= (1 << 0)) : (DWT_CTRL &= ~(1 << 0));
    // lock registers again
    CoreSight_LAR(BASE_DWT) = oldLAR;
}

#define NOP_CYCLE __asm__ __volatile__ ("and r0, r0")

#define SYSCLK_FREQ_HZ (16000000)
#define NS_PER_CYCLE (1e9f / SYSCLK_FREQ_HZ)

static inline void sleep_ns(uint32_t ns) {
    // sample register as soon as possible
    uint32_t now = DWT_CYCCNT;
    // calculate target value (compensate by loop iteration time)
    uint32_t target = now + ((uint32_t)(((float)ns) / NS_PER_CYCLE) - 4);
    // wait for target value reached
    while (((int32_t)(DWT_CYCCNT - target)) < 0);
}
