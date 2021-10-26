/* GENERAL MEMORY LAYOUT */

#define BASE_BOOT          (0x0000 << 16)  /* (aliased to flash or SRAM) */

/* memory */
#define BASE_FLASH_MAIN    (0x0800 << 16)
    #define SIZE_FLASH_MAIN      (0x100000)
#define BASE_CCMRAM        (0x1000 << 16)
#define BASE_FLASH_OTHER   (0x1FFF << 16)
#define BASE_SRAM1         (0x2000 << 16)
    #define SIZE_SRAM1            (0x1C000)
#define BASE_SRAM2        ((0x2001 << 16) + 0xC000)
    #define SIZE_SRAM2             (0x4000)

/* peripherals */
#define BASE_APB1          (0x4000 << 16)
#define BASE_APB2          (0x4001 << 16)
#define BASE_AHB1          (0x4002 << 16)  /* until 0x4008_0000 */
#define BASE_AHB2          (0x5000 << 16)  /* until 0x5007_0000 */
#define BASE_AHB3          (0xA000 << 16)
