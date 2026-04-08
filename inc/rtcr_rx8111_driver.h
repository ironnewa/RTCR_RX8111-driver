
#ifndef RTCR_RX8111_DRIVER_H
#define RTCR_RX8111_DRIVER_H

#include "i2cDriver.h"

#define KeRTCR_B_RTCRX8111Addr 0b10

typedef enum TeRTCR_B_RX8111CE_Registers
{
	RX8111CE_REG_Sec = 0x10,
	RX8111CE_REG_Min = 0x11,
	RX8111CE_REG_Hour = 0x12,
	RX8111CE_REG_Week = 0x13,
	RX8111CE_REG_Day = 0x14,
	RX8111CE_REG_Month = 0x15,
	RX8111CE_REG_Year = 0x16,
	RX8111CE_REG_Extension = 0x1D,
	RX8111CE_REG_Flag = 0x1E,
	RX8111CE_REG_Control = 0x1F,
	RX8111CE_REG_Cmd_Trigger = 0x2F,
	RX8111CE_REG_Pwr_Sw_Ctrl = 0x32,
	RX8111CE_REG_EVIN_Monitor = 0x33
} TeRTCR_B_RX8111CE_Registers;

//Chip struct
typedef struct TsRTCR_h_RX8111CE
{
	BYTE i2c_address;
	TeRTCR_B_RX8111CE_Registers register_address;
	BYTE register_value;
	BYTE register_expected_values[7] = {0,0,0,0,0,0,0};
} TsRTCR_h_RX8111CE;

/******************************************************************************
 * Function Declarations
 *****************************************************************************/
BYTE CalcRTCR_DecToBCD(BYTE LeRTCR_y_Decimal_Val);
BYTE CalcRTCR_BCDToDec(BYTE LeRTCR_y_BCD_Val);
LONGWORD CalcRTCR_CompileBCDToSecondsDec(BYTE* LaRTCR_t_Time_Data_Array);
void MngRTCR_Set_Epoch(void);
LONGWORD GetRTCR_SecondsSinceEpoch(void);
void WriteRTCR_RX8111CE_Register(void);
BYTE ReadRTCR_RX8111CE_Register(void);
void SetRTCR_RX8111CE_Register(TeRTCR_B_RX8111CE_Registers LeRTCR_y_Reg_Sel, BYTE LeRTCR_y_Reg_Val);
void MngRTCR_RX8111CE_Setup_Epoch(BYTE* LpRTCR_t_Epoch_Data);
void MngRTCR_RX8111CE_Init(void);
void displayChipData(void);

#endif /* RTCR_H_ */