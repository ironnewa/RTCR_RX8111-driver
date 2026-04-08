
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef I2CDRIVER_H
#define I2CDRIVER_H

//Only use test environment unless board is connected.
//#define PHYSICAL_ENVIRONMENT
#define TEST_ENVIRONMENT

#ifdef PHYSICAL_ENVIRONMENT
	#include "stm32h7xx_hal.h"
#endif

#include <stdint.h>

typedef signed char     SHORTINT;
typedef unsigned char   BYTE;
typedef signed short    INTEGER;
typedef unsigned short  WORD;
typedef signed int      LONGINT;
typedef unsigned int    LONGWORD;

#ifdef PHYSICAL_ENVIRONMENT
	//i2c functions
	extern I2C_HandleTypeDef hi2c1;
	extern I2C_HandleTypeDef hi2c2;

	void MX_I2C1_Init(void);
	void MX_I2C2_Init(void);

	//i2cr functions
	void MngI2CR_Init(void);
	HAL_StatusTypeDef WriteI2CR_ToRegister(uint16_t device_address, uint16_t register_address, uint8_t* tx_data, uint16_t size);
	HAL_StatusTypeDef ReadI2CR_FromRegister(uint16_t device_address, uint16_t register_address, uint8_t* rx_data, uint16_t size);
#endif

#endif //I2CDRIVER_H
