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

#include "chibios_spi.h"
#include "no_os_spi.h"

#include "adxl355.h"

static THD_WORKING_AREA(waThreadBlinker, 256);

static BaseSequentialStream* chp = (BaseSequentialStream*) &SD5;

static struct chibios_gpio_init_param chibios_gpio_extra_ip_6 = {
  .port = GPIOG,
  .pad = 11U,
  .mode = PAL_MODE_OUTPUT_OPENDRAIN,
};

static struct no_os_gpio_init_param chibios_GPIO_6 = {
  .platform_ops = &chibios_gpio_ops,
  .extra = &chibios_gpio_extra_ip_6,
};

static struct no_os_gpio_desc *gpio_desc_6;

static struct adxl355_dev *adxl355_desc;
static struct chibios_spi_init_param chibios_spi_extra_ip  = {
    .circular         = false,
    .slave            = false,
    .data_cb          = NULL,
    .error_cb         = NULL,
    .ssline           = LINE_ARD_D10,
    .cr1              = SPI_CR1_BR_2,
    .cr2              = 0
};
static struct no_os_spi_init_param adxl355_spi_ip = {
    .device_id = 1,
    .max_speed_hz = 4000000,
    .bit_order = NO_OS_SPI_BIT_ORDER_MSB_FIRST,
    .mode = NO_OS_SPI_MODE_0,
    .platform_ops = &chibios_spi_ops,
    .chip_select = 15,
    .extra = &chibios_spi_extra_ip,
};
static struct adxl355_init_param adxl355_ip = {
    .comm_type = ADXL355_SPI_COMM,
    .dev_type = ID_ADXL355,
};


static THD_FUNCTION(ThreadBlinker, arg) {

  (void)arg;

  chRegSetThreadName("blinker");
  palSetLineMode(LINE_ARD_D9, PAL_MODE_OUTPUT_OPENDRAIN);
  no_os_gpio_get(&gpio_desc_6, &chibios_GPIO_6);

  while (true) {
    no_os_gpio_set_value(gpio_desc_6, PAL_LOW);
    chThdSleepMilliseconds(1000);
    no_os_gpio_set_value(gpio_desc_6, PAL_HIGH);
    chThdSleepMilliseconds(1000);
  }
}

static struct adxl355_frac_repr x[32] = {0};
static struct adxl355_frac_repr y[32] = {0};
static struct adxl355_frac_repr z[32] = {0};


static struct chibios_gpio_init_param chibios_gpio_extra_ip_1 = {
  .port = GPIOB,
  .pad = 3U,
  .mode = PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST,
};
static struct no_os_gpio_init_param chibios_GPIO_1 = {
  .platform_ops = &chibios_gpio_ops,
  .extra = &chibios_gpio_extra_ip_1,
};
static struct chibios_gpio_init_param chibios_gpio_extra_ip_2 = {
  .port = GPIOB,
  .pad = 4U,
  .mode = PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST,
};
static struct no_os_gpio_init_param chibios_GPIO_2 = {
  .platform_ops = &chibios_gpio_ops,
  .extra = &chibios_gpio_extra_ip_2,
};
static struct chibios_gpio_init_param chibios_gpio_extra_ip_3 = {
  .port = GPIOA,
  .pad = 7U,
  .mode = PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST,
};
static struct no_os_gpio_init_param chibios_GPIO_3 = {
  .platform_ops = &chibios_gpio_ops,
  .extra = &chibios_gpio_extra_ip_3,
};
static struct chibios_gpio_init_param chibios_gpio_extra_ip_4 = {
  .port = GPIOA,
  .pad = 15U,
  .mode = PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST,
};
static struct no_os_gpio_init_param chibios_GPIO_4 = {
  .platform_ops = &chibios_gpio_ops,
  .extra = &chibios_gpio_extra_ip_4,
};

static struct no_os_gpio_desc *gpio_desc_1;
static struct no_os_gpio_desc *gpio_desc_2;
static struct no_os_gpio_desc *gpio_desc_3;
static struct no_os_gpio_desc *gpio_desc_4;

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

  no_os_gpio_get(&gpio_desc_1, &chibios_GPIO_1);
  no_os_gpio_get(&gpio_desc_2, &chibios_GPIO_2);
  no_os_gpio_get(&gpio_desc_3, &chibios_GPIO_3);
  no_os_gpio_get(&gpio_desc_4, &chibios_GPIO_4);

  int ret;
  adxl355_ip.comm_init.spi_init = adxl355_spi_ip;
  ret = adxl355_init(&adxl355_desc, adxl355_ip);
  if (ret)
      goto error;
  ret = adxl355_soft_reset(adxl355_desc);
  if (ret)
      goto error;
  ret = adxl355_set_odr_lpf(adxl355_desc, ADXL355_ODR_3_906HZ);
  if (ret)
      goto error;
  ret = adxl355_set_op_mode(adxl355_desc, ADXL355_MEAS_TEMP_ON_DRDY_OFF);
  if (ret)
      goto error;

  sdStart(&SD5, NULL);

  while(1) {

    chprintf(chp, "Single read \n\r");
      ret = adxl355_get_xyz(adxl355_desc,&x[0], &y[0], &z[0]);
      if (ret)
          goto error;
      chprintf(chp, " x=%d"".%09u", (int)x[0].integer, (abs)(x[0].fractional));
      chprintf(chp, " y=%d"".%09u", (int)y[0].integer, (abs)(y[0].fractional));
      chprintf(chp, " z=%d"".%09u \n\r", (int)z[0].integer,(abs)(z[0].fractional));

  }
error:
  {
    while(true) {
      chThdSleepMilliseconds(1000);
    }
  }
}
