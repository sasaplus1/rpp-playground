#include "pico/stdlib.h"

const uint LED = 25;

void
setup_led() {
  gpio_init(LED);
  gpio_set_dir(LED, GPIO_OUT);
  gpio_put(LED, 0);
}

void
toggle_led() {
  int led = gpio_get(LED);
  gpio_put(LED, led > 0 ? 0 : 1);
}

int
main() {
  stdio_init_all();

  setup_led();

  while (1) {
    toggle_led();
    sleep_ms(500);
  }

  return 0;
}
