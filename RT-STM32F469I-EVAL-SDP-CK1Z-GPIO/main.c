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

static THD_WORKING_AREA(waThreadBlinker, 128);

static struct chibios_gpio_init_param chibios_gpio_extra_ip = {
  .port = GPIOB,
  .pad = 15U,
  .mode = PAL_MODE_OUTPUT_OPENDRAIN,
};

static struct no_os_gpio_init_param chibios_GPIO = {
  .platform_ops = &chibios_gpio_ops,
  .extra = &chibios_gpio_extra_ip,
};

static struct no_os_gpio_desc *gpio_desc;

static THD_FUNCTION(ThreadBlinker, arg) {

  (void)arg;

  chRegSetThreadName("blinker");
  no_os_gpio_get(&gpio_desc, &chibios_GPIO);

  while (true) {
    no_os_gpio_set_value(gpio_desc, PAL_LOW);
    chThdSleepMilliseconds(1000);
    no_os_gpio_set_value(gpio_desc, PAL_HIGH);
    chThdSleepMilliseconds(1000);
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

  chThdCreateStatic(waThreadBlinker, sizeof(waThreadBlinker), NORMALPRIO + 1, ThreadBlinker, NULL);

  while(true) {
    chThdSleepMilliseconds(1000);
  }
}
