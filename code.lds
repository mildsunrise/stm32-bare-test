#include "constants.h"

MEMORY {
  flash_main (rx) : ORIGIN = BASE_BOOT, LENGTH = SIZE_FLASH_MAIN
  sram1 (!ir) : ORIGIN = BASE_SRAM1, LENGTH = SIZE_SRAM1
}

SECTIONS {
  .text :{ *(VECTOR_TABLE) *(.text) }
  .noinit :{ *(.noinit) }
}
