#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include <stdio.h>

int main() {
  stdio_init_all();
  printf("Hello, world!\n");

  if (cyw43_arch_init()) {
    printf("Wi-Fi init failed");
    return -1;
  }
  while (true) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(700);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(700);
  }
}
