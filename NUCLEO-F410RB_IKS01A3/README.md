
---
pagetitle: Readme
lang: en
---
::: {.row}
::: {.col-sm-12 .col-lg-8}

## <b>NUCLEO-F410RB_IKS01A3 Example Description</b>

Main function is to show how to create firmware template for MEMS Studio and AlgoBuilder applications.

### <b>Keywords</b>

MEMS, I2C, VCOM, ALGOBUILDER

### <b>Directory contents</b>

  - Drivers - contains all drivers for corresponding HW
  - EWARM - contains project files for IAR toolchain
  - Inc - contains header files
  - MDK-ARM - contains project files for Keil µVision toolchain
  - Middlewares - contains middleware libraries
  - Src - contains source files
  - STM32CubeIDE - contains project files for STM32CubeIDE toolchain

### <b>Hardware and Software environment</b>

  - This example runs on STM32 Nucleo board with MEMS sensor expansion board.
  - If you power the Nucleo board via USB 3.0 port, please check that you have flashed the last version of
    the firmware of ST-Link v2 inside the Nucleo board. In order to flash the last available firmware of the
	  ST-Link v2, you can use the STM32 ST Link Utility.
  - This example has been tested with STMicroelectronics NUCLEO-F410RB RevC and X-NUCLEO-IKS01A3 and
    can be easily tailored to any other supported device and development board.

ADDITIONAL_BOARD : X-NUCLEO-IKS01A3 https://www.st.com/en/ecosystems/x-nucleo-iks01a3.html
ADDITIONAL_COMP : LSM6DSO https://www.st.com/en/mems-and-sensors/lsm6dso.html
ADDITIONAL_COMP : LIS2DW12 https://www.st.com/en/mems-and-sensors/lis2dw12.html
ADDITIONAL_COMP : LIS2MDL https://www.st.com/en/mems-and-sensors/lis2mdl.html
ADDITIONAL_COMP : LPS22HH https://www.st.com/en/mems-and-sensors/lps22hh.html
ADDITIONAL_COMP : HTS221 https://www.st.com/en/mems-and-sensors/hts221.html
ADDITIONAL_COMP : STTS751 https://www.st.com/en/mems-and-sensors/stts751.html

### <b>How to use it?</b>

In order to make the program work, you must do the following:

  - WARNING: before opening the project with any toolchain be sure your folder
    installation path is not too in-depth since the toolchain may report errors
    after building
  - Open your preferred toolchain
  - Rebuild all files and load your image into target memory
  - Run the example

### <b>Author</b>

STMicroelectronics

### <b>License</b>

Copyright (c) 2024 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

:::
:::
