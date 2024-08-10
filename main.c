#include "pico/stdlib.h"

const uint LED = 25;
const uint PIN = 16;

int
main() {
  stdio_init_all();

  gpio_init(LED);
  gpio_set_dir(LED, GPIO_OUT);
  gpio_put(LED, 0);

  gpio_init(PIN);
  gpio_set_dir(PIN, GPIO_OUT);
  gpio_pull_up(PIN);
  gpio_put(PIN, 1);

  while (1) {
    int led = gpio_get(LED);
    int pin = gpio_get(PIN);

    gpio_put(LED, led > 0 ? 0 : 1);
    gpio_put(PIN, pin > 0 ? 0 : 1);

    sleep_ms(500);
  }

  return 0;
}
