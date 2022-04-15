#include "entry.h"

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
