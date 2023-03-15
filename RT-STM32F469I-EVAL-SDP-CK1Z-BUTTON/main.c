/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio
    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
        http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include <stdio.h>

#include "chibios_gpio.h"
#include "no_os_gpio.h"

static THD_WORKING_AREA(waThreadButton, 256);

static struct chibios_gpio_init_param chibios_gpio_extra_ip_5 = {
  .port = GPIOA,
  .pad = 1U,
  .mode = PAL_MODE_OUTPUT_PUSHPULL,
};
static struct no_os_gpio_init_param chibios_GPIO_5 = {
  .platform_ops = &chibios_gpio_ops,
  .extra = &chibios_gpio_extra_ip_5,
};
static struct no_os_gpio_desc *gpio_desc_5;

static BaseSequentialStream* chp = (BaseSequentialStream*) &SD5;

static THD_FUNCTION(ThreadButton, arg){
  (void)arg;
  sdStart(&SD5, NULL);
  chRegSetThreadName("button");

  no_os_gpio_get(&gpio_desc_5, &chibios_GPIO_5);
  //no-os gpio test
  uint8_t tmp;

  while (true) {
        no_os_gpio_get_value(gpio_desc_5, &tmp);
        chprintf(chp, "Value: %d, ", tmp);
        /* Checking the button status. */
        if (tmp == 0) {
          /* Button is pressed: turning the LED on. */
          chprintf(chp, "button pushed\n\r");
        }
        else {
          /* Button is released: turning the LED off. */
          chprintf(chp, "button not pushed\n\r");
        }
  }
}

/*
 * Application entry point.
 */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */

  halInit();
  chSysInit();

  chThdCreateStatic(waThreadButton, sizeof(waThreadButton), NORMALPRIO + 1, ThreadButton, NULL);

  while(true) {
    chThdSleepMilliseconds(1000);
  }
}
