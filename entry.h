#include "constants.h"

typedef unsigned int uintptr_t;
typedef unsigned int size_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned char uint8_t;

#define MASK(nbits) (~((~0) << (nbits)))

#define SET_BITS(ref, bits, mask, shift) \
    (ref) = ((ref) & ~((mask) << (shift))) | ((bits) << (shift))


// CoreSight management registers
// ------------------------------

// (implemented in many components such as SCS, DWT, etc.)

#define BASE_CoreSight(last_block) ((last_block) + 0xF00)

#define CoreSight_ITCTRL(last_block)      (*((volatile uint32_t*)BASE_CoreSight(last_block) +  0))
#define CoreSight_CLAIMSET(last_block)    (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) +  0))
#define CoreSight_CLAIMCLR(last_block)    (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) +  1))
#define CoreSight_DEVAFF0(last_block)     (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) +  2))
#define CoreSight_DEVAFF1(last_block)     (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) +  3))
#define CoreSight_LAR(last_block)         (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) +  4))
#define CoreSight_LSR(last_block)         (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) +  5))
#define CoreSight_AUTHSTATUS(last_block)  (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) +  6))
#define CoreSight_DEVARCH(last_block)     (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) +  7))
#define CoreSight_DEVID2(last_block)      (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) +  8))
#define CoreSight_DEVID1(last_block)      (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) +  9))
#define CoreSight_DEVID(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 10))
#define CoreSight_DEVTYPE(last_block)     (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 11))
#define CoreSight_PIDR4(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 12))
#define CoreSight_PIDR5(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 13))
#define CoreSight_PIDR6(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 14))
#define CoreSight_PIDR7(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 15))
#define CoreSight_PIDR0(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 16))
#define CoreSight_PIDR1(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 17))
#define CoreSight_PIDR2(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 18))
#define CoreSight_PIDR3(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 19))
#define CoreSight_CIDR0(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 20))
#define CoreSight_CIDR1(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 21))
#define CoreSight_CIDR2(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 22))
#define CoreSight_CIDR3(last_block)       (*((volatile uint32_t*)(BASE_CoreSight(last_block) + 0xA0) + 23))

#define CoreSight_UNLOCK_VALUE (0xC5ACCE55)


// SCS registers
// -------------

#define BASE_SCS (BASE_PPB + 0xE000)

// System control and ID
//  [reserved: Master Control register]
#define SCB_ICTR       (*((volatile uint32_t*)(BASE_SCS + 0x0) + 1))
#define SCB_ACTLR      (*((volatile uint32_t*)(BASE_SCS + 0x0) + 2))

// SysTick
#define SYST_CSR       (*((volatile uint32_t*)(BASE_SCS + 0x10) + 0))
#define SYST_RVR       (*((volatile uint32_t*)(BASE_SCS + 0x10) + 1))
#define SYST_CVR       (*((volatile uint32_t*)(BASE_SCS + 0x10) + 2))
#define SYST_CALIB     (*((volatile uint32_t*)(BASE_SCS + 0x10) + 3))

// NVIC
#define NVIC_ISER0(n)  (*((volatile uint32_t*)(BASE_SCS + 0x100) + n))
#define NVIC_ICER0(n)  (*((volatile uint32_t*)(BASE_SCS + 0x180) + n))
#define NVIC_ISPR0(n)  (*((volatile uint32_t*)(BASE_SCS + 0x200) + n))
#define NVIC_ICPR0(n)  (*((volatile uint32_t*)(BASE_SCS + 0x280) + n))
#define NVIC_IABR0(n)  (*((volatile uint32_t*)(BASE_SCS + 0x300) + n))
#define NVIC_IPR0(n)   (*((volatile uint32_t*)(BASE_SCS + 0x400) + n))

// System control and ID
#define SCB_CPUID      (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 0))
#define SCB_ICSR       (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 1))
#define SCB_VTOR       (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 2))
#define SCB_AIRCR      (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 3))
#define SCB_SCR        (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 4))
#define SCB_CCR        (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 5))
#define SCB_SHPR1      (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 6))
#define SCB_SHPR2      (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 7))
#define SCB_SHPR3      (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 8))
#define SCB_SHCSR      (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 9))
#define SCB_CFSR       (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 10))
#define SCB_HFSR       (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 11))
#define SCB_DFSR       (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 12))
#define SCB_MMFAR      (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 13))
#define SCB_BFAR       (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 14))
#define SCB_AFSR       (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 15))
//  [reserved: CPUID table]
#define SCB_CPACR      (*((volatile uint32_t*)(BASE_SCS + 0xD00) + 34))

// MPU
#define MPU_TYPE       (*((volatile uint32_t*)(BASE_SCS + 0xD90) + 0))
#define MPU_CTRL       (*((volatile uint32_t*)(BASE_SCS + 0xD90) + 1))
#define MPU_RNR        (*((volatile uint32_t*)(BASE_SCS + 0xD90) + 2))
#define MPU_RBAR       (*((volatile uint32_t*)(BASE_SCS + 0xD90) + 3))
#define MPU_RASR       (*((volatile uint32_t*)(BASE_SCS + 0xD90) + 4))
#define MPU_RBAR_A1    (*((volatile uint32_t*)(BASE_SCS + 0xD90) + 5))
#define MPU_RASR_A1    (*((volatile uint32_t*)(BASE_SCS + 0xD90) + 6))
#define MPU_RBAR_A2    (*((volatile uint32_t*)(BASE_SCS + 0xD90) + 7))
#define MPU_RASR_A2    (*((volatile uint32_t*)(BASE_SCS + 0xD90) + 8))
#define MPU_RBAR_A3    (*((volatile uint32_t*)(BASE_SCS + 0xD90) + 9))
#define MPU_RASR_A3    (*((volatile uint32_t*)(BASE_SCS + 0xD90) + 10))

// Debug
#define SCS_DHCSR      (*((volatile uint32_t*)(BASE_SCS + 0xDF0) + 0))
#define SCS_DCRSR      (*((volatile uint32_t*)(BASE_SCS + 0xDF0) + 1))
#define SCS_DCRDR      (*((volatile uint32_t*)(BASE_SCS + 0xDF0) + 2))
#define SCS_DEMCR      (*((volatile uint32_t*)(BASE_SCS + 0xDF0) + 3))

// System control and ID
#define SCB_STIR       (*((volatile uint32_t*)(BASE_SCS + 0xF00) + 0))


// DWT registers
// -------------

#define BASE_DWT (BASE_PPB + 0x1000)

#define DWT_CTRL         (*((volatile uint32_t*)(BASE_DWT) + 0))
#define DWT_CYCCNT       (*((volatile uint32_t*)(BASE_DWT) + 1))
#define DWT_CPICNT       (*((volatile uint32_t*)(BASE_DWT) + 2))
#define DWT_EXCCNT       (*((volatile uint32_t*)(BASE_DWT) + 3))
#define DWT_SLEEPCNT     (*((volatile uint32_t*)(BASE_DWT) + 4))
#define DWT_LSUCNT       (*((volatile uint32_t*)(BASE_DWT) + 5))
#define DWT_FOLDCNT      (*((volatile uint32_t*)(BASE_DWT) + 6))
#define DWT_PCSR         (*((volatile uint32_t*)(BASE_DWT) + 7))
#define DWT_COMP(n)      (*((volatile uint32_t*)(BASE_DWT) + 8 + (n)*4 + 0))
#define DWT_MASK(n)      (*((volatile uint32_t*)(BASE_DWT) + 8 + (n)*4 + 1))
#define DWT_FUNCTION(n)  (*((volatile uint32_t*)(BASE_DWT) + 8 + (n)*4 + 2))


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
