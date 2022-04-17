#include "entry.h"

// RCC registers
#define BASE_RCC (BASE_AHB1 + 0x3800)
#define RCC_CR            (*((volatile uint32_t*)(BASE_RCC) + 0))
#define RCC_PLLCFGR       (*((volatile uint32_t*)(BASE_RCC) + 1))
#define RCC_CFGR          (*((volatile uint32_t*)(BASE_RCC) + 2))
#define RCC_CIR           (*((volatile uint32_t*)(BASE_RCC) + 3))
#define RCC_AHB1RSTR      (*((volatile uint32_t*)(BASE_RCC) + 4))
#define RCC_AHB2RSTR      (*((volatile uint32_t*)(BASE_RCC) + 5))
#define RCC_AHB3RSTR      (*((volatile uint32_t*)(BASE_RCC) + 6))
/* reserved: 7 */
#define RCC_APB1RSTR      (*((volatile uint32_t*)(BASE_RCC) + 8))
#define RCC_APB2RSTR      (*((volatile uint32_t*)(BASE_RCC) + 9))
/* reserved: 10 */
/* reserved: 11 */
#define RCC_AHB1ENR       (*((volatile uint32_t*)(BASE_RCC) + 12))
#define RCC_AHB2ENR       (*((volatile uint32_t*)(BASE_RCC) + 13))
#define RCC_AHB3ENR       (*((volatile uint32_t*)(BASE_RCC) + 14))
/* reserved: 15 */
#define RCC_APB1ENR       (*((volatile uint32_t*)(BASE_RCC) + 16))
#define RCC_APB2ENR       (*((volatile uint32_t*)(BASE_RCC) + 17))
/* reserved: 18 */
/* reserved: 19 */
#define RCC_AHB1LPENR     (*((volatile uint32_t*)(BASE_RCC) + 20))
#define RCC_AHB2LPENR     (*((volatile uint32_t*)(BASE_RCC) + 21))
#define RCC_AHB3LPENR     (*((volatile uint32_t*)(BASE_RCC) + 22))
/* reserved: 23 */
#define RCC_APB1LPENR     (*((volatile uint32_t*)(BASE_RCC) + 24))
#define RCC_APB2LPENR     (*((volatile uint32_t*)(BASE_RCC) + 25))
/* reserved: 26 */
/* reserved: 27 */
#define RCC_BDCR          (*((volatile uint32_t*)(BASE_RCC) + 28))
#define RCC_CSR           (*((volatile uint32_t*)(BASE_RCC) + 29))
/* reserved: 30 */
/* reserved: 31 */
#define RCC_SSCGR         (*((volatile uint32_t*)(BASE_RCC) + 32))
#define RCC_PLLI2SCFGR    (*((volatile uint32_t*)(BASE_RCC) + 33))

// USART registers
#define BASE_USART1 (BASE_APB2 + 0x1000)
#define BASE_USART2 (BASE_APB1 + 0x4400)
#define BASE_USART3 (BASE_APB1 + 0x4800)
#define BASE_UART4  (BASE_APB1 + 0x4C00)
#define BASE_UART5  (BASE_APB1 + 0x5000)
#define BASE_USART6 (BASE_APB2 + 0x1400)
#define BASE_UART7  (BASE_APB1 + 0x7800)
#define BASE_UART8  (BASE_APB1 + 0x7C00)
#define USART_SR(port)   (*((volatile uint32_t*)(port) + 0))
#define USART_DR(port)   (*((volatile uint32_t*)(port) + 1))
#define USART_BRR(port)  (*((volatile uint32_t*)(port) + 2))
#define USART_CR1(port)  (*((volatile uint32_t*)(port) + 3))
#define USART_CR2(port)  (*((volatile uint32_t*)(port) + 4))
#define USART_CR3(port)  (*((volatile uint32_t*)(port) + 5))
#define USART_GTPR(port) (*((volatile uint32_t*)(port) + 6))

// GPIO registers
#define BASE_GPIO (BASE_AHB1)
#define OFFSET_GPIO_PORT 0x400
#define BASE_GPIOA (BASE_GPIO + OFFSET_GPIO_PORT * 0)
#define BASE_GPIOB (BASE_GPIO + OFFSET_GPIO_PORT * 1)
#define BASE_GPIOC (BASE_GPIO + OFFSET_GPIO_PORT * 2)
#define BASE_GPIOD (BASE_GPIO + OFFSET_GPIO_PORT * 3)
#define BASE_GPIOE (BASE_GPIO + OFFSET_GPIO_PORT * 4)
#define BASE_GPIOF (BASE_GPIO + OFFSET_GPIO_PORT * 5)
#define BASE_GPIOG (BASE_GPIO + OFFSET_GPIO_PORT * 6)
#define BASE_GPIOH (BASE_GPIO + OFFSET_GPIO_PORT * 7)
#define BASE_GPIOI (BASE_GPIO + OFFSET_GPIO_PORT * 8)
#define GPIO_MODER(port)   (*((volatile uint32_t*)(port) + 0))
#define GPIO_OTYPER(port)  (*((volatile uint32_t*)(port) + 1))
#define GPIO_OSPEEDR(port) (*((volatile uint32_t*)(port) + 2))
#define GPIO_PUPDR(port)   (*((volatile uint32_t*)(port) + 3))
#define GPIO_IDR(port)     (*((volatile uint32_t*)(port) + 4))
#define GPIO_ODR(port)     (*((volatile uint32_t*)(port) + 5))
#define GPIO_BSRR(port)    (*((volatile uint32_t*)(port) + 6))
#define GPIO_LCKR(port)    (*((volatile uint32_t*)(port) + 7))
#define GPIO_AFRL(port)    (*((volatile uint32_t*)(port) + 8))
#define GPIO_AFRH(port)    (*((volatile uint32_t*)(port) + 9))

#define PUPDR_NONE     0b00
#define PUPDR_PULLUP   0b01
#define PUPDR_PULLDOWN 0b10
// 0b11 is reserved

void gpio_make_af(uintptr_t port, int pin, int af) {
    volatile uint32_t* afReg = (pin / 8) ? &GPIO_AFRH(port) : &GPIO_AFRL(port);
    SET_BITS(*afReg, af, 0b1111, (pin % 8) * 4); // set AF number
    SET_BITS(GPIO_MODER(port), 0b10, 0b11, pin * 2); // put into AF mode
}

void gpio_make_output_push_pull(uintptr_t port, int pin) {
    SET_BITS(GPIO_PUPDR(port), PUPDR_NONE, 0b11, pin * 2); // disable pull-up / pull-down
    SET_BITS(GPIO_OTYPER(port), 0b0, 0b1, pin); // set push-pull
    SET_BITS(GPIO_MODER(port), 0b01, 0b11, pin * 2); // set output
}

void gpio_make_output_open_drain(uintptr_t port, int pin, int pullups) {
    SET_BITS(GPIO_OTYPER(port), 0b1, 0b1, pin); // set open-drain
    SET_BITS(GPIO_PUPDR(port), pullups, 0b11, pin * 2); // set pullups
    SET_BITS(GPIO_MODER(port), 0b01, 0b11, pin * 2); // set output
}

void gpio_make_input(uintptr_t port, int pin, int pullups) {
    SET_BITS(GPIO_MODER(port), 0b00, 0b11, pin * 2); // set input
    SET_BITS(GPIO_PUPDR(port), pullups, 0b11, pin * 2);
}

void gpio_write(uintptr_t port, int pin, int value) {
    GPIO_BSRR(port) = 1 << (pin + (value ? 0 : 16));
}

int gpio_read(uintptr_t port, int pin) {
    return (GPIO_IDR(port) >> pin) & 1;
}

void init_serial_console() {
    // enable clock for GPIOA and USART2
    RCC_AHB1ENR |= (1 << 0);
    RCC_APB1ENR |= (1 << 17);

    // pass PA2 and PA3 to USART2
    gpio_make_af(BASE_GPIOA, 2, 7);
    gpio_make_af(BASE_GPIOA, 3, 7);

    // enable UART, set signal parameters
    USART_CR1(BASE_USART2) |= (1 << 13); // UE (USART enable)
    USART_CR1(BASE_USART2) &= ~(1 << 12); // M (word length) = 8 data bits
    USART_CR1(BASE_USART2) &= ~(1 << 10); // PCE (parity control enable) = no parity
    SET_BITS(USART_CR2(BASE_USART2), 0b00, 0b11, 12); // STOP = 1 stop bit

    // set up a 115200 baud rate
    // 1. we only need oversampling by 8, since 16 doesn't improve the error
    USART_CR1(BASE_USART2) |= (1 << 15); // OVER8
    // 2. assuming default SYSCLK selection, AHB & APB1 prescaler values,
    //    we are running at 16MHz. therefore 16MHz * 8 / 115200 = 17.361
    // 3. mantissa = 17; fractional = 0.361 * 8 = 2.88 -> 3
    USART_BRR(BASE_USART2) = (17 << 4) | (3 << 0);

    // enable transmitter (sends idle frame)
    USART_CR1(BASE_USART2) |= (1 << 3); // TE (transmitter enable)
}

void transmit_buffer(const uint8_t* data, size_t length) {
    if (!length) return;
    for (size_t i = 0; i < length; i++) {
        USART_DR(BASE_USART2) = data[i];
        while (!((USART_SR(BASE_USART2) >> 7) & 1)); // wait for TXE=1
    }
    while (!((USART_SR(BASE_USART2) >> 6) & 1)); // wait for TC=1
}

void transmit_string(const char* data) {
    if (!*data) return;
    for (; *data; data++) {
        USART_DR(BASE_USART2) = (uint8_t)*data;
        while (!((USART_SR(BASE_USART2) >> 7) & 1)); // wait for TXE=1
    }
    while (!((USART_SR(BASE_USART2) >> 6) & 1)); // wait for TC=1
}

#define PIN_USER_BTN  BASE_GPIOA, 0

#define PIN_LD3  BASE_GPIOD, 13  // up, orange
#define PIN_LD4  BASE_GPIOD, 12  // left, green
#define PIN_LD5  BASE_GPIOD, 14  // right, red
#define PIN_LD6  BASE_GPIOD, 15  // down, blue

#define PIN_LCD_DB4  BASE_GPIOE, 10
#define PIN_LCD_DB5  BASE_GPIOE, 11
#define PIN_LCD_DB6  BASE_GPIOE, 12
#define PIN_LCD_DB7  BASE_GPIOE, 13
#define PIN_LCD_RS   BASE_GPIOE, 14
#define PIN_LCD_E    BASE_GPIOE, 15

void lcd_raw_init() {
    // enable clock to GPIOE
    RCC_AHB1ENR |= (1 << 4);

    // set up pins
    gpio_write(PIN_LCD_E, 0);
    gpio_make_output_push_pull(PIN_LCD_E);
    gpio_make_output_push_pull(PIN_LCD_RS);
    gpio_make_output_push_pull(PIN_LCD_E);
    gpio_make_output_push_pull(PIN_LCD_E);
    gpio_make_output_push_pull(PIN_LCD_E);
    gpio_make_output_push_pull(PIN_LCD_E);
}

// assumes RS, R/W already set
void lcd_raw_write_nibble(uint8_t nibble) {
    NOP_CYCLE; // >= 30ns
    gpio_write(PIN_LCD_DB4, (nibble >> 0) & 1);
    gpio_write(PIN_LCD_DB5, (nibble >> 1) & 1);
    gpio_write(PIN_LCD_DB6, (nibble >> 2) & 1);
    gpio_write(PIN_LCD_DB7, (nibble >> 3) & 1);
    gpio_write(PIN_LCD_E, 1);
    NOP_CYCLE;
    NOP_CYCLE;
    NOP_CYCLE; // >= 150ns
    gpio_write(PIN_LCD_E, 0);
    NOP_CYCLE;
    NOP_CYCLE;
    NOP_CYCLE; // whole cycle >= 400ns
}

void lcd_raw_write_byte(char rs, uint8_t byte) {
    gpio_write(PIN_LCD_RS, rs);
    // here we would set R/W if we had it
    lcd_raw_write_nibble(byte >> 4);
    lcd_raw_write_nibble(byte & 0xF);
}

#define LCD_WAIT_TIME_1 4100000 // ns
#define LCD_WAIT_TIME_2  100000 // ns
#define LCD_INIT_N 1 // two lines
#define LCD_INIT_F 0 // 5x8 dots

void lcd_cmd_clear_display() {
    lcd_raw_write_byte(0, (1 << 0));
    sleep_ns(LCD_WAIT_TIME_1);
}

void lcd_cmd_return_home() {
    lcd_raw_write_byte(0, (1 << 1));
    sleep_ns(LCD_WAIT_TIME_1);
}

void lcd_cmd_set_entry_mode(int i_d, int s) {
    lcd_raw_write_byte(0, (1 << 2) | ((!!i_d) << 1) | ((!!s) << 0));
    sleep_ns(LCD_WAIT_TIME_2);
}

void lcd_cmd_set_display(int d, int c, int b) {
    lcd_raw_write_byte(0, (1 << 3) | ((!!d) << 2) | ((!!c) << 1) | ((!!b) << 0));
    sleep_ns(LCD_WAIT_TIME_2);
}

void lcd_cmd_set_display_shift(int s_c, int r_l) {
    lcd_raw_write_byte(0, (1 << 4) | ((!!s_c) << 3) | ((!!r_l) << 2));
    sleep_ns(LCD_WAIT_TIME_2);
}

void lcd_cmd_set_function(int dl, int n, int f) {
    lcd_raw_write_byte(0, (1 << 5) | ((!!dl) << 4) | ((!!n) << 3) | ((!!f) << 2));
    sleep_ns(LCD_WAIT_TIME_2);
}

void lcd_cmd_set_cgram_addr(uint8_t addr) {
    // FIXME: assert(!(addr & ~0b111111))
    lcd_raw_write_byte(0, (1 << 6) | addr);
    sleep_ns(LCD_WAIT_TIME_2);
}

void lcd_cmd_set_ddram_addr(uint8_t addr) {
    // FIXME: assert(!(addr & ~0b1111111))
    lcd_raw_write_byte(0, (1 << 7) | addr);
    sleep_ns(LCD_WAIT_TIME_2);
}

void lcd_cmd_write(uint8_t value) {
    // FIXME: assert(!(addr & ~0b1111111))
    lcd_raw_write_byte(1, value);
    sleep_ns(LCD_WAIT_TIME_2);
}

void lcd_init() {
    lcd_raw_init();
    sleep_ns(15000000);
    gpio_write(PIN_LCD_RS, 0);
    lcd_raw_write_nibble((1 << 1) | (1 << 0)); // set_function
    sleep_ns(LCD_WAIT_TIME_1);
    lcd_raw_write_nibble((1 << 1) | (1 << 0)); // set_function
    sleep_ns(LCD_WAIT_TIME_2);
    lcd_raw_write_nibble((1 << 1) | (1 << 0)); // set_function
    lcd_raw_write_nibble((1 << 1)); // set_function with 4 bit
    lcd_cmd_set_function(0, LCD_INIT_N, LCD_INIT_F);
    lcd_cmd_set_display(0, 0, 0);
    lcd_cmd_clear_display();
    lcd_cmd_set_entry_mode(1, 0); // FIXME
}

void lcd_goto(uint32_t row, uint32_t col) {
    //FIXME: assert(row < 2 && col < 0x10)
    lcd_cmd_set_ddram_addr(row * 0x40 + col);
}

void lcd_write_buffer(const uint8_t* data, size_t length) {
    for (size_t i = 0; i < length; i++)
        lcd_cmd_write(data[i]);
}

void lcd_write_string(const char* data) {
    for (; *data; data++)
        lcd_cmd_write(*data);
}

void main() {
    // SET UP BUSES / PERIPHERALS

    // enable clock for GPIOA and GPIOD
    RCC_AHB1ENR |= (1 << 0) | (1 << 3);
    init_serial_console();
    transmit_string("loading LCD...\r\n");
    lcd_init();

    // SET UP GPIO

    gpio_make_input(PIN_USER_BTN, PUPDR_PULLDOWN);

    gpio_make_output_push_pull(PIN_LD3);
    gpio_make_output_push_pull(PIN_LD4);
    gpio_make_output_push_pull(PIN_LD5);
    gpio_make_output_push_pull(PIN_LD6);

    // CODE

    transmit_string("Hello from STM32! 🦊💕✨\r\n");
    lcd_write_string("Hello from STM32");

    const int led_sequence [] = { 12, 13, 14, 15 };
    const int led_sequence_size = sizeof(led_sequence) / sizeof(*led_sequence);

    int sequence_idx = 0;
    while (1) {
        sleep_ns(400 * 1000000);
        while (!gpio_read(PIN_USER_BTN));
        gpio_write(BASE_GPIOD, led_sequence[sequence_idx], 0);
        sequence_idx = (sequence_idx + 1) % led_sequence_size;
        gpio_write(BASE_GPIOD, led_sequence[sequence_idx], 1);
    }
}
