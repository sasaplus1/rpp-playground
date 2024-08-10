#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "ssd1306.h"

// LED {{{
const uint LED = 25;

static struct repeating_timer timer;

bool repeating_timer_callback(struct repeating_timer *t);

/**
 * setup LED on board
 */
void setup_led() {
  gpio_init(LED);
  gpio_set_dir(LED, GPIO_OUT);
  gpio_put(LED, 0);

  // blink timer
  add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);
}

void toggle_led() {
  int led = gpio_get(LED);
  gpio_put(LED, led > 0 ? 0 : 1);
}

bool repeating_timer_callback(struct repeating_timer *t) {
  toggle_led();
  return true; // continue timer
}
/// }}}

// OLED : SSD1306 {{{
static ssd1306_t disp;

const uint OLED_SDA = 0; // GPIO0
const uint OLED_SCL = 1; // GPIO1
const uint OLED_WIDTH = 128;
const uint OLED_HEIGHT = 64;

/**
 * setup OLED with SSD1306
 */
void setup_oled() {
  // i2c_init(i2c0, 400000);
  i2c_init(i2c0, 1000000);
  gpio_set_function(OLED_SDA, GPIO_FUNC_I2C);
  gpio_set_function(OLED_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(OLED_SDA);
  gpio_pull_up(OLED_SCL);

  // use 3.3V
  disp.external_vcc = false;
  ssd1306_init(&disp, OLED_WIDTH, OLED_HEIGHT, 0x3C, i2c0);
}

/**
 * update OLED display
 */
void loop_oled() {
  static int y = 0;

  ssd1306_clear(&disp);

  if (y >= 64) {
    y = 0;
  } else {
    y++;
  }

  // draw horizontal line
  // ssd1306_draw_line(&disp, 0, y, 127, y);

  char str[5 + 4];
  sprintf(str, "%s%d", "Hello", y);

  ssd1306_draw_string(&disp, 8, y, 2, str);
  ssd1306_show(&disp);
}
// }}}

int main() {
  stdio_init_all();

  setup_led();
  setup_oled();

  while (1) {
    loop_oled();
  }

  return 0;
}

// vim:fdm=marker:
