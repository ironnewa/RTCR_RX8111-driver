
#include "i2cDriver.h"

#ifdef PHYSICAL_ENVIRONMENT

    I2C_HandleTypeDef hi2c1;
    I2C_HandleTypeDef hi2c2;


    //Blank defines for testing
    #define I2C_BUS 0
    #define I2C_READ_ADDR 0
    #define I2C_WRITE_ADDR 0


    void MX_I2C1_Init(void)
    {

        hi2c1.Instance;
        hi2c1.Init.Timing = 0x009034B6;
        hi2c1.Init.OwnAddress1 = 0;
        hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        hi2c1.Init.OwnAddress2 = 0;
        hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
        hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
        if (HAL_I2C_Init(&hi2c1) != HAL_OK)
        {
            //Error_Handler();
        }


        if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
        {
            //Error_Handler();
        }

        if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
        {
            //Error_Handler();
        }
    

    }

    void MX_I2C2_Init(void)
    {


        hi2c2.Instance;
        hi2c2.Init.Timing = 0x009034B6;
        hi2c2.Init.OwnAddress1 = 0;
        hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        hi2c2.Init.OwnAddress2 = 0;
        hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
        hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
        if (HAL_I2C_Init(&hi2c2) != HAL_OK)
        {
            //Error_Handler();
        }

        if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
        {
            //Error_Handler();
        }


        if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
        {
            //Error_Handler();
        }


    }


    void MngI2CR_Init()
    {
        HAL_StatusTypeDef bus_ready;

        bus_ready = HAL_I2C_Init(I2C_BUS);

        if (bus_ready != HAL_OK)
        {
            // TODO: report bus ready error if bus is not ready reset or retry
        }

        // TODO: check if slave devices are ready with HAL_I2C_IsDeviceReady()
    }

    HAL_StatusTypeDef WriteI2CR_ToRegister(uint16_t device_address, uint16_t register_address, uint8_t* tx_data, uint16_t size)
    {
        HAL_StatusTypeDef result;

        uint8_t data[6];
        uint8_t tx_size;

        if (register_address > 0xFF)
        {
            data[0] = (register_address >> 8) & 0xFF;
            data[1] = (register_address & 0xFF);
            for (int idx = 0; idx < size; ++idx)
            {
                data[idx + 2] = tx_data[idx];
            }
            tx_size = size + 2;
        }
        else
        {
            data[0] = register_address;
            for (int idx = 0; idx < size; ++idx)
            {
                data[idx + 1] = tx_data[idx];
            }
            tx_size = size + 1;
        }

        result = HAL_I2C_Master_Transmit(I2C_BUS, I2C_WRITE_ADDR(device_address), data, tx_size, HAL_MAX_DELAY);

        return result;
    }

    HAL_StatusTypeDef WriteI2CR_RegisterAddress(uint16_t device_address, uint16_t register_address)
    {
        HAL_StatusTypeDef result;
        uint8_t data[2];
        uint8_t size;

        if (register_address > 0xFF)
        {
            data[0] = (register_address >> 8) & 0xFF;
            data[1] = (register_address & 0xFF);
            size = 2;
        }
        else
        {
            data[0] = register_address;
            size = 1;
        }
        result = HAL_I2C_Master_Transmit(I2C_BUS, I2C_WRITE_ADDR, data, size, HAL_MAX_DELAY);

        return result;
    }

    HAL_StatusTypeDef ReadI2CR_FromRegister(uint16_t device_address, uint16_t register_address, uint8_t* rx_data, uint16_t size)
    {
        HAL_StatusTypeDef result;

        result = WriteI2CR_RegisterAddress(device_address, register_address);

        if (result != HAL_OK)
        {
            return result;
        }

    
        result = HAL_I2C_Master_Receive(I2C_BUS, I2C_READ_ADDR, rx_data, size, HAL_MAX_DELAY);

        return result;
    }

#endif