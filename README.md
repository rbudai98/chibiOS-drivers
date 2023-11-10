# ChibiOS and no-OS drivers

## Containing folders:
* [RT-STM32F469I-EVAL-SDP-CK1Z-ADXL355](https://github.com/rbudai98/chibiOS-drivers/tree/main/RT-STM32F469I-EVAL-SDP-CK1Z-ADXL355)
* [RT-STM32F469I-EVAL-SDP-CK1Z-BUTTON](https://github.com/rbudai98/chibiOS-drivers/tree/main/RT-STM32F469I-EVAL-SDP-CK1Z-BUTTON)
* [RT-STM32F469I-EVAL-SDP-CK1Z-GPIO](https://github.com/rbudai98/chibiOS-drivers/tree/main/RT-STM32F469I-EVAL-SDP-CK1Z-GPIO)
* [RT-STM32F469I-EVAL-SDP-CK1Z-I2C](https://github.com/rbudai98/chibiOS-drivers/tree/main/RT-STM32F469I-EVAL-SDP-CK1Z-I2C)

## Usage:
1. Install ChibiOS: https://www.chibios.org/dokuwiki/doku.php
2. After extracting the OS, in the workspace add the directory examples and modify in the main Makefile the path towards os [```$(CHIBIOS)```] and the path toward no-os repository [```$(NOOS)```]

## Pre-requirements
* For system setup make sure to download ChibiOS from: https://sourceforge.net/projects/chibios/files/
and install it according to the provided guide.
* Download No-OS from https://github.com/analogdevicesinc/no-OS github repository. (Code -> Download ZIP) \
Alternatively by using the following command: 
```console 
git clone https://github.com/analogdevicesinc/no-OS.git
```

* Download and extract the provided examples from https://github.com/rbudai98/chibiOS-drivers (Code -> Download ZIP) and copy the project folders into ```C:/ChibiOS/``` \
Alternatively one can clone the repository directly into ```C:/ChibiOS/``` with command 
```console
git clone https://github.com/rbudai98/chibiOS-drivers.git
```
### Testing
* For serial testing install [Putty](https://www.putty.org/) or [Terra Term](https://tera-term.en.softonic.com/).

## Projects
### [RT-STM32F469I-EVAL-SDP-CK1Z-ADXL355](https://github.com/rbudai98/chibiOS-drivers/tree/main/RT-STM32F469I-EVAL-SDP-CK1Z-ADXL355)
### [RT-STM32F469I-EVAL-SDP-CK1Z-BUTTON](https://github.com/rbudai98/chibiOS-drivers/tree/main/RT-STM32F469I-EVAL-SDP-CK1Z-BUTTON)
### [RT-STM32F469I-EVAL-SDP-CK1Z-GPIO](https://github.com/rbudai98/chibiOS-drivers/tree/main/RT-STM32F469I-EVAL-SDP-CK1Z-GPIO)
### [RT-STM32F469I-EVAL-SDP-CK1Z-I2C](https://github.com/rbudai98/chibiOS-drivers/tree/main/RT-STM32F469I-EVAL-SDP-CK1Z-I2C)
