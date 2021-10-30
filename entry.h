#include "constants.h"

typedef unsigned int uintptr_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

#define MASK(nbits) (~((~0) << (nbits)))

#define SET_BITS(ref, bits, mask, shift) \
    (ref) = ((ref) & ~((mask) << (shift))) | ((bits) << (shift))

#define NOP_CYCLE __asm__ __volatile__ ("and r0, r0")
