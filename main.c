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
#include "rt_test_root.h"
#include "oslib_test_root.h"


/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
uint8_t write;
uint8_t* message[10];

static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
//    palSetLine(LINE_LED_GREEN);
//    chThdSleepMilliseconds(50);
    palSetLine(LINE_LED_ORANGE);
    chThdSleepMilliseconds(50);
    palSetLine(LINE_LED_RED);
//    chThdSleepMilliseconds(200);
//    palClearLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(50);
    palClearLine(LINE_LED_ORANGE);
    chThdSleepMilliseconds(50);
    palClearLine(LINE_LED_RED);
    chThdSleepMilliseconds(200);
  }
}



static THD_WORKING_AREA(waThread2, 128);
static THD_FUNCTION(Thread2, arg) {

  (void)arg;
  chRegSetThreadName("Serial");
  while (true) {
      if (write == 1)
      {
        //write on serial message
         write = 0;
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

//  palSetLineMode(LINE_ARD_D0, PAL_MODE_INPUT);

      palSetLine(LINE_LED_GREEN);
      chThdSleepMilliseconds(2000);
      palClearLine(LINE_LED_GREEN);
      chThdSleepMilliseconds(2000);

  /*
   * Activates the serial driver 5 using the driver default configuration.
   */

      /* Configuring PC6 as AF8 assigning it to USART6_TX. */
      palSetPadMode(GPIOC, 6, PAL_MODE_ALTERNATE(8));
      /* Configuring PC7 as AF8 assigning it to USART6_RX. */
      palSetPadMode(GPIOC, 7, PAL_MODE_ALTERNATE(8));

      sdStart(&SD3, NULL);

      chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

      while (true) {

        sdPut(&SD3, (int8_t)'a');

        /* Checking the button status. */
        if (palReadLine(LINE_ARD_D0) == PAL_HIGH) {
          /* Button is pressed: turning the LED on. */
          palSetLine(LINE_LED_GREEN);
        }
        else {
          /* Button is released: turning the LED off. */
          palClearLine(LINE_LED_GREEN);
        }
        chThdSleepMilliseconds(500);
      }
}
