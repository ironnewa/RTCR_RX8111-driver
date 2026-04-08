# RTCR_RX8111-driver
I2C driver for rx8111 real time clock

This codebase is a section of a larger battery management system. 
It is the driver for the RX8111CE RTC chip using i2c.
It can be used independently of the BMS codebase.

No direct console input is needed for the driver.

If you wish to test a different test case or function, define a new test case scenario in main.cpp
If you wish to test a physical board, ensure TEST_ENVIRONMENT is not defined and PHYSICAL_ENVIRONMENT is.

This driver is testing the functionality of each RX8111 RTC i2c driver.

It is expected to have a full stm32H745xx driver before implementation on a chip.
It is expected to have a CMSIS core_cm7 driver before implementation on a chip.

If testing beyond the simulated chipset is desired, an i2c RX8111CE model is needed.

**
