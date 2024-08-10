#include "pico/stdlib.h"

const uint LED = 25;

int
main() {
  stdio_init_all();

  gpio_init(LED);
  gpio_set_dir(LED, GPIO_OUT);
  gpio_put(LED, 0);

  while (1) {
    int led = gpio_get(LED);

    gpio_put(LED, led > 0 ? 0 : 1);

    sleep_ms(500);
  }

  return 0;
}
