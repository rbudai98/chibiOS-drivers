# ChibiOS and no-OS drivers

## Containing folders:
* RT-STM32F469I-EVAL-SDP-CK1Z-ADXL355
* RT-STM32F469I-EVAL-SDP-CK1Z-BUTTON
* RT-STM32F469I-EVAL-SDP-CK1Z-GPIO
* RT-STM32F469I-EVAL-SDP-CK1Z-I2C

## Usage:
1. Install ChibiOS: https://www.chibios.org/dokuwiki/doku.php
2. After extracting the OS, in the workspace add the directory examples and modify in the main Mkefile the path towards os [```$(CHIBIOS)```] and the path toward no-os repository [```$(NOOS)```]