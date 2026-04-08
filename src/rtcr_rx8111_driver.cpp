#include "rtcr_rx8111_driver.h"
#include "i2cDriver.h"
#include <iostream>
#include <bitset>
#include <iomanip>


//
/******************************************************************************
 * Data-Type:   BYTE Array
 * Name:        KaRTCR_t_RTCRX8111_EPOCH
 * Description: The predetermined epoch used by set epoch function. The epoch
 * 				is written as "[sec,min,hour,day,week,year]" since Jan 1st 2000.
 * 				The predetermined epoch is set to Jan 1st 2020.
 *****************************************************************************/
BYTE KaRTCR_t_RTCRX8111_EPOCH[7] = { 0,0,0,0,4,1,20 };

//Chip instance
/******************************************************************************
 * Data-Type:   TsRTCR_h_RX8111CE
 * Name:        VsRTCR_h_RX8111CE
 * Description: RTC RX8111CE chip instance.
 *****************************************************************************/
TsRTCR_h_RX8111CE VsRTCR_h_RX8111CE;

/******************************************************************************
 * Function:    CalcRTCR_DecToBCD
 * Arguments:   BYTE LeRTCR_y_Decimal_Val : The decimal value to be converted
 * Description: Converts a decimal value to binary converted decimal format.
 *****************************************************************************/
BYTE CalcRTCR_DecToBCD(BYTE LeRTCR_y_Decimal_Val)
{
	BYTE VeRTCR_y_BCD_Val = (LeRTCR_y_Decimal_Val / 10) << 4;
	VeRTCR_y_BCD_Val |= (LeRTCR_y_Decimal_Val % 10);
	return VeRTCR_y_BCD_Val;
}

/******************************************************************************
 * Function:    CalcRTCR_BCDToDec
 * Arguments:   BYTE LeRTCR_y_BCD_Val : The BCD value to be converted
 * Description: Converts a binary converted decimal value to decimal format.
 *****************************************************************************/
BYTE CalcRTCR_BCDToDec(BYTE LeRTCR_y_BCD_Val)
{
	BYTE VeRTCR_y_Decimal_Val = (LeRTCR_y_BCD_Val >> 4) * 10;
	VeRTCR_y_Decimal_Val += (LeRTCR_y_BCD_Val & 0x0F);
	return VeRTCR_y_Decimal_Val;
}

/******************************************************************************
 * Function:    CalcRTCR_CompileBCDToSecondsDec
 * Arguments:   BYTE * LaRTCR_t_Time_Data_Array : Array of BCD time values
 * Description: Compiles an array of BCD time data from min,hr,day,month,year
 * 				into decimal total seconds.
 *****************************************************************************/
LONGWORD CalcRTCR_CompileBCDToSecondsDec(BYTE* LaRTCR_t_Time_Data_Array)
{
	LONGWORD VeRTCR_t_Seconds_Count = 0;
	WORD VeRTCR_d_Days_Count = 0;
	WORD VaRTCR_d_Days_Elapsed_Per_Month[12] = { 0,31,59,90,120,151,181,212,243,273,304,334 };
	BYTE VeRTCR_y_Year_Count = CalcRTCR_BCDToDec(LaRTCR_t_Time_Data_Array[6]) - 20; //current year - 2020 = years passed
	BYTE VeRTCR_y_Current_Month = CalcRTCR_BCDToDec(LaRTCR_t_Time_Data_Array[5]);

	//Days passed in the current year
	VeRTCR_d_Days_Count = (CalcRTCR_BCDToDec(LaRTCR_t_Time_Data_Array[3]) - 1) +
		VaRTCR_d_Days_Elapsed_Per_Month[VeRTCR_y_Current_Month];

	//Amount of leap years
	VeRTCR_d_Days_Count += VeRTCR_y_Year_Count / 4;

	//If current year is a leap year and month is Feb. or earlier
	if ((VeRTCR_y_Year_Count % 4 == 0) && (VeRTCR_y_Current_Month <= 2))
	{
		VeRTCR_d_Days_Count--;
	}

	VeRTCR_d_Days_Count += VeRTCR_y_Year_Count * 365; //365 days per normal year

	VeRTCR_t_Seconds_Count += VeRTCR_d_Days_Count * 24 * 3600; //Convert total days to seconds

	VeRTCR_t_Seconds_Count += CalcRTCR_BCDToDec(LaRTCR_t_Time_Data_Array[0]);	//second
	VeRTCR_t_Seconds_Count += CalcRTCR_BCDToDec(LaRTCR_t_Time_Data_Array[1]) * 60; //minute
	VeRTCR_t_Seconds_Count += CalcRTCR_BCDToDec(LaRTCR_t_Time_Data_Array[2]) * 3600; //hour

	return VeRTCR_t_Seconds_Count;
}

//Sets epoch from predefined time data set
/******************************************************************************
 * Function:    MngRTCR_Set_Epoch
 * Arguments:   void
 * Description: Sets the epoch to count from on the RTC chip. The epoch is
 * 				determined by a preset declaration.
 *****************************************************************************/
void MngRTCR_Set_Epoch(void) {
	MngRTCR_RX8111CE_Setup_Epoch(KaRTCR_t_RTCRX8111_EPOCH);
}

/******************************************************************************
 * Function:    GetRTCR_SecondsSinceEpoch
 * Arguments:   void
 * Description: returns the total seconds elapsed since the preset epoch.
 *****************************************************************************/
LONGWORD GetRTCR_SecondsSinceEpoch(void)
{
	BYTE VaRTCR_t_Time_Data_Array[7];

	//Grab time data
	SetRTCR_RX8111CE_Register(RX8111CE_REG_Sec, 0x00); //Set read register
	VaRTCR_t_Time_Data_Array[0] = ReadRTCR_RX8111CE_Register(); //Update instance
	VsRTCR_h_RX8111CE.register_expected_values[0] = VaRTCR_t_Time_Data_Array[0];
	
	SetRTCR_RX8111CE_Register(RX8111CE_REG_Min, 0x00); //Set read register
	VaRTCR_t_Time_Data_Array[1] = ReadRTCR_RX8111CE_Register(); //Update instance
	VsRTCR_h_RX8111CE.register_expected_values[1] = VaRTCR_t_Time_Data_Array[1];

	SetRTCR_RX8111CE_Register(RX8111CE_REG_Hour, 0x00); //Set read register
	VaRTCR_t_Time_Data_Array[2] = ReadRTCR_RX8111CE_Register(); //Update instance
	VsRTCR_h_RX8111CE.register_expected_values[2] = VaRTCR_t_Time_Data_Array[2];

	SetRTCR_RX8111CE_Register(RX8111CE_REG_Week, 0x00); //Set read register
	VaRTCR_t_Time_Data_Array[3] = ReadRTCR_RX8111CE_Register(); //Update instance
	VsRTCR_h_RX8111CE.register_expected_values[3] = VaRTCR_t_Time_Data_Array[3];

	SetRTCR_RX8111CE_Register(RX8111CE_REG_Day, 0x00); //Set read register
	VaRTCR_t_Time_Data_Array[4] = ReadRTCR_RX8111CE_Register(); //Update instance
	VsRTCR_h_RX8111CE.register_expected_values[4] = VaRTCR_t_Time_Data_Array[4];

	SetRTCR_RX8111CE_Register(RX8111CE_REG_Month, 0x00); //Set read register
	VaRTCR_t_Time_Data_Array[5] = ReadRTCR_RX8111CE_Register(); //Update instance
	VsRTCR_h_RX8111CE.register_expected_values[5] = VaRTCR_t_Time_Data_Array[5];

	SetRTCR_RX8111CE_Register(RX8111CE_REG_Year, 0x00); //Set read register
	VaRTCR_t_Time_Data_Array[6] = ReadRTCR_RX8111CE_Register(); //Update instance
	VsRTCR_h_RX8111CE.register_expected_values[6] = VaRTCR_t_Time_Data_Array[6];

	//Convert time data to decimal and return in terms of seconds.
	return CalcRTCR_CompileBCDToSecondsDec(VaRTCR_t_Time_Data_Array);
}

/******************************************************************************
 * Function:    WriteRTCR_RX8111CE_Register
 * Arguments:   void
 * Description: Writes data from register to chip over i2c
 *****************************************************************************/
void WriteRTCR_RX8111CE_Register(void)
{
	BYTE VeRTCR_y_regData = VsRTCR_h_RX8111CE.register_value;

#ifdef TEST_ENVIRONMENT

	switch (VsRTCR_h_RX8111CE.register_address) {
	case RX8111CE_REG_Sec:
		VsRTCR_h_RX8111CE.register_expected_values[0] = VsRTCR_h_RX8111CE.register_value;
		break;
	case RX8111CE_REG_Min:
		VsRTCR_h_RX8111CE.register_expected_values[1] = VsRTCR_h_RX8111CE.register_value;
		break;
	case RX8111CE_REG_Hour:
		VsRTCR_h_RX8111CE.register_expected_values[2] = VsRTCR_h_RX8111CE.register_value;
		break;
	case RX8111CE_REG_Week:
		VsRTCR_h_RX8111CE.register_expected_values[3] = VsRTCR_h_RX8111CE.register_value;
		break;
	case RX8111CE_REG_Day:
		VsRTCR_h_RX8111CE.register_expected_values[4] = VsRTCR_h_RX8111CE.register_value;
		break;
	case RX8111CE_REG_Month:
		VsRTCR_h_RX8111CE.register_expected_values[5] = VsRTCR_h_RX8111CE.register_value;
		break;
	case RX8111CE_REG_Year:
		VsRTCR_h_RX8111CE.register_expected_values[6] = VsRTCR_h_RX8111CE.register_value;
		break;
	default:
		VsRTCR_h_RX8111CE.register_value = VsRTCR_h_RX8111CE.register_value;
	}
#endif

#ifdef PHYSICAL_ENVIRONMENT
	WriteI2CR_ToRegister(VsRTCR_h_RX8111CE.i2c_address, VsRTCR_h_RX8111CE.register_address,
		&VeRTCR_y_regData, 1);
#endif
}

//Read from i2c
/******************************************************************************
 * Function:    ReadRTCR_RX8111CE_Register
 * Arguments:   void
 * Description: Reads and stores data from chip over i2c
 *****************************************************************************/
BYTE ReadRTCR_RX8111CE_Register(void)
{
	BYTE VeRTCR_y_regData;

#ifdef TEST_ENVIRONMENT
	switch (VsRTCR_h_RX8111CE.register_address) {
	case RX8111CE_REG_Sec:
		VsRTCR_h_RX8111CE.register_value = VsRTCR_h_RX8111CE.register_expected_values[0];
		break;
	case RX8111CE_REG_Min:
		VsRTCR_h_RX8111CE.register_value = VsRTCR_h_RX8111CE.register_expected_values[1];
		break;
	case RX8111CE_REG_Hour:
		VsRTCR_h_RX8111CE.register_value = VsRTCR_h_RX8111CE.register_expected_values[2];
		break;
	case RX8111CE_REG_Week:
		VsRTCR_h_RX8111CE.register_value = VsRTCR_h_RX8111CE.register_expected_values[3];
		break;
	case RX8111CE_REG_Day:
		VsRTCR_h_RX8111CE.register_value = VsRTCR_h_RX8111CE.register_expected_values[4];
		break;
	case RX8111CE_REG_Month:
		VsRTCR_h_RX8111CE.register_value = VsRTCR_h_RX8111CE.register_expected_values[5];
		break;
	case RX8111CE_REG_Year:
		VsRTCR_h_RX8111CE.register_value = VsRTCR_h_RX8111CE.register_expected_values[6];
		break;
	default:
		VsRTCR_h_RX8111CE.register_value = VsRTCR_h_RX8111CE.register_value;
	}
	VeRTCR_y_regData = VsRTCR_h_RX8111CE.register_value;
#endif

#ifdef PHYSICAL_ENVIRONMENT
	ReadI2CR_FromRegister(VsRTCR_h_RX8111CE.i2c_address, VsRTCR_h_RX8111CE.register_address,
		&VeRTCR_y_regData, 1);

	VsRTCR_h_RX8111CE.register_value = VeRTCR_y_regData;
#endif
	return VeRTCR_y_regData;
}

//Set data sent to i2c
/******************************************************************************
 * Function:    SetRTCR_RX8111CE_Register
 * Arguments:   TeRTCR_B_RX8111CE_Registers LeRTCR_y_Reg_Sel :
 * 				BYTE LeRTCR_y_Reg_Val                        :
 * Description: Sets the register address and internal register value for the
 * 				chip over i2c.
 *****************************************************************************/
void SetRTCR_RX8111CE_Register(TeRTCR_B_RX8111CE_Registers LeRTCR_y_Reg_Sel, BYTE LeRTCR_y_Reg_Val)
{
	VsRTCR_h_RX8111CE.register_address = LeRTCR_y_Reg_Sel;
	VsRTCR_h_RX8111CE.register_value = LeRTCR_y_Reg_Val;
}

/******************************************************************************
 * Function:    MngRTCR_RX8111CE_Setup_Epoch
 * Arguments:   BYTE * LpRTCR_t_Epoch_Data : Data array of the format [sec,min,hr,day,week,month,year]
 * Description: Sets the epoch for the chip from an input array. Once an epoch is set,
 * 				the chip will start counting.
 *****************************************************************************/
void MngRTCR_RX8111CE_Setup_Epoch(BYTE* LpRTCR_t_Epoch_Data)
{

	//Stops the chip from counting
	SetRTCR_RX8111CE_Register(RX8111CE_REG_Control, 0x01);
	WriteRTCR_RX8111CE_Register();

	//Grab and place each time data section
	
	SetRTCR_RX8111CE_Register((RX8111CE_REG_Sec),
		CalcRTCR_DecToBCD(LpRTCR_t_Epoch_Data[0]));
	VsRTCR_h_RX8111CE.register_expected_values[0] = LpRTCR_t_Epoch_Data[0];
	WriteRTCR_RX8111CE_Register();
	
	SetRTCR_RX8111CE_Register((RX8111CE_REG_Min),
		CalcRTCR_DecToBCD(LpRTCR_t_Epoch_Data[1]));
	VsRTCR_h_RX8111CE.register_expected_values[1] = LpRTCR_t_Epoch_Data[1];
	WriteRTCR_RX8111CE_Register();

	SetRTCR_RX8111CE_Register((RX8111CE_REG_Hour),
		CalcRTCR_DecToBCD(LpRTCR_t_Epoch_Data[2]));
	VsRTCR_h_RX8111CE.register_expected_values[2] = LpRTCR_t_Epoch_Data[2];
	WriteRTCR_RX8111CE_Register();

	SetRTCR_RX8111CE_Register((RX8111CE_REG_Week),
		CalcRTCR_DecToBCD(LpRTCR_t_Epoch_Data[3]));
	VsRTCR_h_RX8111CE.register_expected_values[3] = LpRTCR_t_Epoch_Data[3];
	WriteRTCR_RX8111CE_Register();

	SetRTCR_RX8111CE_Register((RX8111CE_REG_Day),
		CalcRTCR_DecToBCD(LpRTCR_t_Epoch_Data[4]));
	VsRTCR_h_RX8111CE.register_expected_values[4] = LpRTCR_t_Epoch_Data[4];
	WriteRTCR_RX8111CE_Register();

	SetRTCR_RX8111CE_Register((RX8111CE_REG_Month),
		CalcRTCR_DecToBCD(LpRTCR_t_Epoch_Data[5]));
	VsRTCR_h_RX8111CE.register_expected_values[5] = LpRTCR_t_Epoch_Data[5];
	WriteRTCR_RX8111CE_Register();

	SetRTCR_RX8111CE_Register((RX8111CE_REG_Year),
		CalcRTCR_DecToBCD(LpRTCR_t_Epoch_Data[6]));
	VsRTCR_h_RX8111CE.register_expected_values[6] = LpRTCR_t_Epoch_Data[6];
	WriteRTCR_RX8111CE_Register();

	SetRTCR_RX8111CE_Register(RX8111CE_REG_Control, 0x00);
	WriteRTCR_RX8111CE_Register();
}

//Initialize default values
/******************************************************************************
 * Function:    MngRTCR_RX8111CE_Init
 * Arguments:   void
 * Description: Initializes the chip.
 *****************************************************************************/
void MngRTCR_RX8111CE_Init(void)
{
	VsRTCR_h_RX8111CE.i2c_address = KeRTCR_B_RTCRX8111Addr;
	VsRTCR_h_RX8111CE.register_address = RX8111CE_REG_Sec;
	VsRTCR_h_RX8111CE.register_value = 0;
	VsRTCR_h_RX8111CE.register_expected_values[0] = 0;

	//Sets the stop bit to zero, allowing the chip to count.
	SetRTCR_RX8111CE_Register(RX8111CE_REG_Control, 0x00);
	WriteRTCR_RX8111CE_Register();
}


/******************************************************************************
 * Function:    displayChipData
 * Arguments:   void
 * Description: TESTING FUNCTION.
 *****************************************************************************/
void displayChipData(void) {

	std::cout << "--- RX8111CE Chip Status ---" << std::endl;
	std::cout << "I2C Device Address: 0x" << std::hex << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.i2c_address) << std::dec
		<< " (Binary: 0b" << std::bitset<8>(VsRTCR_h_RX8111CE.i2c_address) << ")" << std::endl;

	std::cout << "Current Register Address: 0x" << std::hex << VsRTCR_h_RX8111CE.register_address << std::dec
		<< " (" << VsRTCR_h_RX8111CE.register_address << ")" << std::endl;

	std::cout << "Current Register Value: 0x" << std::hex << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_value) << std::dec
		<< " (" << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_value) << ")" << std::endl;

	std::cout << "Time Register Values:" << std::endl;
	std::cout << "  Seconds:  (0x10): " << std::setw(3) << std::right <<static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[0])
		<< std::left << " (0x" << std::hex << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[0]) << std::dec << ")" << std::endl;
	std::cout << "  Minutes:  (0x11): " << std::setw(3) << std::right << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[1])
		<< std::left << " (0x" << std::hex << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[1]) << std::dec << ")" << std::endl;
	std::cout << "  Hours:    (0x12): " << std::setw(3) << std::right << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[2])
		<< std::left << " (0x" << std::hex << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[2]) << std::dec << ")" << std::endl;
	std::cout << "  Week Day: (0x13): " << std::setw(3) << std::right << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[3])
		<< std::left << " (0x" << std::hex << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[3]) << std::dec << ")" << std::endl;
	std::cout << "  Day:      (0x14): " << std::setw(3) << std::right << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[4])
		<< std::left << " (0x" << std::hex << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[4]) << std::dec << ")" << std::endl;
	std::cout << "  Month:    (0x15): " << std::setw(3) << std::right << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[5])
		<< std::left << " (0x" << std::hex << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[5]) << std::dec << ")" << std::endl;
	std::cout << "  Year:     (0x16): " << std::setw(3) << std::right << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[6])
		<< std::left << " (0x" << std::hex << static_cast<LONGWORD>(VsRTCR_h_RX8111CE.register_expected_values[6]) << std::dec << ")" << std::endl;
	std::cout << "--- End Chip Status ---" << std::endl;
}



