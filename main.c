#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"
#include "tusb.h"
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

// HID keyboard {{{
const uint KEY1 = 2;
const uint KEY2 = 3;
const uint KEY3 = 4;
const uint KEY4 = 5;

const uint key_a = 0x04;
const uint key_b = 0x05;
const uint key_c = 0x06;
const uint key_d = 0x07;

void gpio_callback(uint gpio, uint32_t events);

void setup_hid() {
  gpio_init(KEY1);
  gpio_set_dir(KEY1, GPIO_IN);
  gpio_pull_up(KEY1);

  gpio_init(KEY2);
  gpio_set_dir(KEY2, GPIO_IN);
  gpio_pull_up(KEY2);

  gpio_init(KEY3);
  gpio_set_dir(KEY3, GPIO_IN);
  gpio_pull_up(KEY3);

  gpio_init(KEY4);
  gpio_set_dir(KEY4, GPIO_IN);
  gpio_pull_up(KEY4);

  gpio_set_irq_enabled_with_callback(KEY1, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(KEY2, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(KEY3, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
  gpio_set_irq_enabled_with_callback(KEY4, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
}

void gpio_callback(uint gpio, uint32_t events) {
  static bool curr_keycode[4] = {0};
  static bool prev_keycode[4] = {0};

  if (!tud_hid_ready() || !events) {
    return;
  }

  // FALL => keydown / RISE => keyup
  switch (gpio) {
    case KEY1:
      curr_keycode[0] = GPIO_IRQ_EDGE_FALL ? true : false;
      break;
    case KEY2:
      curr_keycode[1] = GPIO_IRQ_EDGE_FALL ? true : false;
      break;
    case KEY3:
      curr_keycode[2] = GPIO_IRQ_EDGE_FALL ? true : false;
      break;
    case KEY4:
      curr_keycode[3] = GPIO_IRQ_EDGE_FALL ? true : false;
      break;
  }

  if (memcmp(curr_keycode, prev_keycode, sizeof(curr_keycode)) != 0) {
    for (int i = 0; i <= 4; i++) {
      prev_keycode[i] = curr_keycode[i];
    }

    uint8_t keycode[4] = {0};

    int i = 0;

    if (curr_keycode[0]) {
      keycode[i] = key_a;
      i++;
    }
    if (curr_keycode[1]) {
      keycode[i] = key_b;
      i++;
    }
    if (curr_keycode[2]) {
      keycode[i] = key_c;
      i++;
    }
    if (curr_keycode[3]) {
      keycode[i] = key_d;
      i++;
    }

    tud_hid_keyboard_report(0, 0, keycode);
  }
}
// }}}

int main() {
  stdio_init_all();
  tusb_init();

  setup_led();
  setup_oled();
  setup_hid();

  while (1) {
    tud_task();
    hid_task();
    loop_oled();
  }

  return 0;
}

// vim:fdm=marker:
