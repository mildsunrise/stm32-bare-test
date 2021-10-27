#include "constants.h"

MEMORY {
  /* this section is called flash_main but we also want to support
     boot from RAM, so make it fit in the first half of SRAM1 */
  flash_main (rx) : ORIGIN = BASE_BOOT, LENGTH = SIZE_SRAM1 / 2
  /* to support boot from RAM, we'll put data in the lower half of SRAM1 */
  sram1 (!ir) : ORIGIN = BASE_SRAM1 + SIZE_SRAM1 / 2, LENGTH = SIZE_SRAM1 / 2
}

SECTIONS {
  .text :{ *(VECTOR_TABLE) *(.text) }
  .noinit :{ *(.noinit) }
}
