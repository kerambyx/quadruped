#include <stdio.h>

#include <cstdint>

#include "pico/stdlib.h"

int main()
{

    // FRONT LEFT
    gpio_set_function(0, GPIO_FUNC_PWM);
    gpio_set_function(1, GPIO_FUNC_PWM);
    gpio_set_function(2, GPIO_FUNC_PWM);

    // REAR LEFT
    gpio_set_function(12, GPIO_FUNC_PWM);
    gpio_set_function(13, GPIO_FUNC_PWM);
    gpio_set_function(14, GPIO_FUNC_PWM);

    // FRONT RIGHT
    gpio_set_function(28, GPIO_FUNC_PWM);
    gpio_set_function(27, GPIO_FUNC_PWM);
    gpio_set_function(26, GPIO_FUNC_PWM);

    // REAR RIGHT
    gpio_set_function(18, GPIO_FUNC_PWM);
    gpio_set_function(17, GPIO_FUNC_PWM);
    gpio_set_function(16, GPIO_FUNC_PWM);

    stdio_init_all();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
