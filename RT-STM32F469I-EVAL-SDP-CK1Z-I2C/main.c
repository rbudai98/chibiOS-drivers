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

#include "chibios_i2c.h"
#include "no_os_i2c.h"


static BaseSequentialStream* chp = (BaseSequentialStream*) &SD5;

static THD_WORKING_AREA(waThreadBlinker, 256);

static THD_FUNCTION(ThreadBlinker, arg) {

  (void)arg;

  chRegSetThreadName("blinker");

  while (true) {

    palToggleLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(200);
  }
}

/*
 * Application entry point.
 */

static I2CConfig chI2CConfig = {
     OPMODE_I2C,
     100000,
     STD_DUTY_CYCLE,
};

static struct no_os_i2c_init_param testCfg ={
     .device_id=1,
     .slave_address = (uint8_t)0b1001000,
     .extra = &chI2CConfig,
     .platform_ops = &chibios_i2c_ops,
};


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

  /* Configuring I2C SCK and I2C SDA related GPIOs .*/
  palSetLineMode(LINE_ARD_D15, PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OSPEED_HIGHEST | PAL_STM32_OTYPE_OPENDRAIN);
  palSetLineMode(LINE_ARD_D14, PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OSPEED_HIGHEST | PAL_STM32_OTYPE_OPENDRAIN);

  chThdCreateStatic(waThreadBlinker, sizeof(waThreadBlinker), NORMALPRIO + 1, ThreadBlinker, NULL);


  uint8_t configBuff[3] = {0x01, 0b00000010, 0b00010000};
  uint8_t tmpRegAddr = 0x00;
  uint8_t rxBuffer[2] = {0,0};

  //NO-OS driver for MAX31875PMB1
  struct no_os_i2c_desc *idesc;
  no_os_i2c_init(&idesc, &testCfg);
  no_os_i2c_write(idesc, &configBuff, 3, 1);
  no_os_i2c_write(idesc, &tmpRegAddr, 1, 0);
  no_os_i2c_read(idesc, &rxBuffer, 2, 1);


  sdStart(&SD5, NULL);

  chprintf(chp, "Starting ...\n\r");

  while(1) {
    no_os_i2c_write(idesc, &tmpRegAddr, 1, 0);
    no_os_i2c_read(idesc, &rxBuffer, 2, 1);

    int8_t tmp_val = rxBuffer[0];

    chprintf(chp, "Temperature: %d celsius\n\r", tmp_val);
    chThdSleepMilliseconds(1000);
  }

}
