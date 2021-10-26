#include "constants.h"

typedef unsigned int uintptr_t;
typedef unsigned int uint32_t;

#define BASE_GPIO BASE_AHB1
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
#define BASE_GPIOJ (BASE_GPIO + OFFSET_GPIO_PORT * 9)
#define BASE_GPIOK (BASE_GPIO + OFFSET_GPIO_PORT * 10)
// Look, I get to use 'volatile' for its intended purpose!
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

#define SET_BITS(ref, bits, mask, shift) \
    (ref) = ((ref) & ~((bits) << (shift))) | ((mask) << (shift))

#define PUPDR_NONE     0b00
#define PUPDR_PULLUP   0b01
#define PUPDR_PULLDOWN 0b10
// 0b11 is reserved

void gpio_make_output_push_pull(uintptr_t port, int pin) {
    SET_BITS(GPIO_PUPDR(port), PUPDR_NONE, 0b11, pin * 2); // disable pull-up / pull-down
    SET_BITS(GPIO_OTYPER(port), 0b0, 0b1, pin); // set push-pull
    SET_BITS(GPIO_MODER(port), 0b01, 0b11, pin * 2); // set output
}

void gpio_make_input(uintptr_t port, int pin, int pullups) {
    SET_BITS(GPIO_MODER(port), 0b00, 0b11, pin * 2); // set output
    SET_BITS(GPIO_PUPDR(port), pullups, 0b11, pin * 2);
}

void gpio_write(uintptr_t port, int pin, int value) {
    GPIO_BSRR(port) = 1 << (pin + (value ? 0 : 16));
}

int gpio_read(uintptr_t port, int pin) {
    return (GPIO_IDR(port) >> pin) & 1;
}

#define PIN_USER_BTN  BASE_GPIOA, 0

#define PIN_LD3  BASE_GPIOD, 13  // up, orange
#define PIN_LD4  BASE_GPIOD, 12  // left, green
#define PIN_LD5  BASE_GPIOD, 14  // right, red
#define PIN_LD6  BASE_GPIOD, 15  // down, blue

void main() {
    gpio_make_input(PIN_USER_BTN, PUPDR_PULLUP);

    gpio_make_output_push_pull(PIN_LD3);
    gpio_make_output_push_pull(PIN_LD4);
    gpio_make_output_push_pull(PIN_LD5);
    gpio_make_output_push_pull(PIN_LD6);

    gpio_write(PIN_LD3, 1);
    gpio_write(PIN_LD4, 0);
    gpio_write(PIN_LD5, 0);
    gpio_write(PIN_LD6, 1);

    while (1);
}
