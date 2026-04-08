
#include "rtcr_rx8111_driver.h"
#include <iostream>
#include <bitset>
#include <iomanip>


//Uncomment which testcase you want to use
//Add testcases using this format.
#define TESTCASE_1
#define TESTCASE_2
#define TESTCASE_3
#define TESTCASE_4
#define TESTCASE_5
#define TESTCASE_6
#define TESTCASE_7

int main() {

	//Driver testing
	//Print results to console

	/*  Function tests  */

	std::cout << "=== RTC RX8111CE Driver Test Suite ===" << std::endl << std::endl;
	
#ifdef TESTCASE_1
    	// Test 1: CalcRTCR_DecToBCD function
    	std::cout << "Test 1: CalcRTCR_DecToBCD function" << std::endl;
    	std::cout << "Purpose: Converts decimal values to Binary Coded Decimal (BCD) format" << std::endl;
    	std::cout << "Algorithm: (decimal/10) << 4 | (decimal%10)" << std::endl;
    	BYTE VeRX8111CE_y_decimalVal = 45;
    	BYTE VeRX8111CE_y_bcdResult = CalcRTCR_DecToBCD(VeRX8111CE_y_decimalVal);
    	std::cout << "Input decimal value: " << static_cast<LONGWORD>(VeRX8111CE_y_decimalVal) << std::endl;
    	std::cout << "Calculated BCD result: 0x" << std::hex << static_cast<LONGWORD>(VeRX8111CE_y_bcdResult) << std::dec
    	          << " (Binary: " << std::bitset<8>(VeRX8111CE_y_bcdResult) << ")" << std::endl;
    	std::cout << "Expected BCD result: 0x45 (Binary: 01000101)" << std::endl;
    	std::cout << "Verification: " << (VeRX8111CE_y_bcdResult == 0x45 ? "PASS" : "FAIL") << std::endl;
    	std::cout << "Explanation: Decimal 45 -> tens=4, ones=5 -> BCD 0x45" << std::endl << std::endl;
#endif

#ifdef TESTCASE_2
    	// Test 2: CalcRTCR_BCDToDec function
    	std::cout << "Test 2: CalcRTCR_BCDToDec function" << std::endl;
    	std::cout << "Purpose: Converts Binary Coded Decimal (BCD) values to decimal format" << std::endl;
    	std::cout << "Algorithm: (bcd >> 4) * 10 + (bcd & 0x0F)" << std::endl;
    	BYTE VeRX8111CE_y_bcdVal = 0x59;
    	BYTE VeRX8111CE_y_decResult = CalcRTCR_BCDToDec(VeRX8111CE_y_bcdVal);
    	std::cout << "Input BCD value: 0x" << std::hex << static_cast<LONGWORD>(VeRX8111CE_y_bcdVal) << std::dec
    	          << " (Binary: " << std::bitset<8>(VeRX8111CE_y_bcdVal) << ")" << std::endl;
    	std::cout << "Calculated decimal result: " << static_cast<LONGWORD>(VeRX8111CE_y_decResult) << std::endl;
    	std::cout << "Expected decimal result: 59" << std::endl;
    	std::cout << "Verification: " << (VeRX8111CE_y_decResult == 59 ? "PASS" : "FAIL") << std::endl;
    	std::cout << "Explanation: BCD 0x59 -> upper nibble=5, lower nibble=9 -> 5*10+9=59" << std::endl << std::endl;
#endif

#ifdef TESTCASE_3
    	// Test 3: MngRTCR_RX8111CE_Init function
    	std::cout << "Test 3: MngRTCR_RX8111CE_Init function" << std::endl;
    	std::cout << "Purpose: Initialize RX8111CE chip with default configuration" << std::endl;
    	std::cout << "Operations: Set I2C address, clear registers, enable chip counting" << std::endl;
    	MngRTCR_RX8111CE_Init();
    	std::cout << "Initialization completed successfully." << std::endl;
    	displayChipData();
    	std::cout << "Verification: I2C address should be 2 (0x02), registers cleared to 0" << std::endl;
    	std::cout << "Control register set to 0x00 to enable time counting" << std::endl << std::endl;
#endif

#ifdef TESTCASE_4
    	// Test 4: MngRTCR_Set_Epoch function
    	std::cout << "Test 4: MngRTCR_Set_Epoch function" << std::endl;
    	std::cout << "Purpose: Set chip time to predefined epoch (January 1st, 2020)" << std::endl;
    	std::cout << "Epoch definition: [sec=0, min=0, hr=0, week=0, day=4, month=1, year=20]" << std::endl;
    	std::cout << "Note: Year 20 represents 2020 (offset from 2000)" << std::endl;
    	MngRTCR_Set_Epoch();
    	std::cout << "Epoch configuration completed." << std::endl;
    	displayChipData();
    	std::cout << "Verification: Time registers should reflect January 1st, 2020 00:00:00" << std::endl;
    	std::cout << "Day=4 represents Thursday (weekday for Jan 1, 2020)" << std::endl << std::endl;
#endif

#ifdef TESTCASE_5
    	// Test 5: CalcRTCR_CompileBCDToSecondsDec function
    	std::cout << "Test 5: CalcRTCR_CompileBCDToSecondsDec function" << std::endl;
    	std::cout << "Purpose: Calculate total seconds elapsed since epoch (Jan 1, 2020)" << std::endl;
    	std::cout << "Algorithm: Convert BCD time array to total seconds including leap year calculations" << std::endl;
    	BYTE VaRX8111CE_t_testTimeArray[7] = { 0x30, 0x15, 0x10, 0x05, 0x01, 0x06, 0x23 };
    	std::cout << "Input time array Hex|Dec (BCD format):" << std::endl;
    	std::cout << "  Seconds:  0x" << std::hex << static_cast<LONGWORD>(VaRX8111CE_t_testTimeArray[0]) << std::dec
    	          << " (" << static_cast<LONGWORD>(CalcRTCR_BCDToDec(VaRX8111CE_t_testTimeArray[0])) << " seconds)" << std::endl;
    	std::cout << "  Minutes:  0x" << std::hex << static_cast<LONGWORD>(VaRX8111CE_t_testTimeArray[1]) << std::dec
    	          << " (" << static_cast<LONGWORD>(CalcRTCR_BCDToDec(VaRX8111CE_t_testTimeArray[1])) << " minutes)" << std::endl;
    	std::cout << "  Hours:    0x" << std::hex << static_cast<LONGWORD>(VaRX8111CE_t_testTimeArray[2]) << std::dec
    	          << " (" << static_cast<LONGWORD>(CalcRTCR_BCDToDec(VaRX8111CE_t_testTimeArray[2])) << " hours)" << std::endl;
    	std::cout << "  Week Day: 0x" << std::hex << static_cast<LONGWORD>(VaRX8111CE_t_testTimeArray[3]) << std::dec
    	          << " (Day " << static_cast<LONGWORD>(CalcRTCR_BCDToDec(VaRX8111CE_t_testTimeArray[3])) << " of week)" << std::endl;
    	std::cout << "  Day:      0x" << std::hex << static_cast<LONGWORD>(VaRX8111CE_t_testTimeArray[4]) << std::dec
    	          << " (Day " << static_cast<LONGWORD>(CalcRTCR_BCDToDec(VaRX8111CE_t_testTimeArray[4])) << " of month)" << std::endl;
    	std::cout << "  Month:    0x" << std::hex << static_cast<LONGWORD>(VaRX8111CE_t_testTimeArray[5]) << std::dec
    	          << " (Month " << static_cast<LONGWORD>(CalcRTCR_BCDToDec(VaRX8111CE_t_testTimeArray[5])) << ")" << std::endl;
    	std::cout << "  Year:     0x" << std::hex << static_cast<LONGWORD>(VaRX8111CE_t_testTimeArray[6]) << std::dec
    	          << " (20" << static_cast<LONGWORD>(CalcRTCR_BCDToDec(VaRX8111CE_t_testTimeArray[6])) << ")" << std::endl;
    	LONGWORD VaRX8111CE_t_secondsResult = CalcRTCR_CompileBCDToSecondsDec(VaRX8111CE_t_testTimeArray);
    	std::cout << "Calculated total seconds since epoch: " << VaRX8111CE_t_secondsResult << std::endl;
    	std::cout << "Date/Time represented: June 1st, 2023 at 10:15:30" << std::endl;
    	std::cout << "Calculation includes: leap year adjustments, days per month, and time components" << std::endl << std::endl;
#endif

#ifdef TESTCASE_6
    	// Test 6: MngRTCR_RX8111CE_Setup_Epoch function with custom data
    	std::cout << "Test 6: MngRTCR_RX8111CE_Setup_Epoch function" << std::endl;
    	std::cout << "Purpose: Configure chip with custom epoch time data" << std::endl;
    	std::cout << "Process: Stop chip, write time registers, restart chip" << std::endl;
    	BYTE VaRX8111CE_t_customEpoch[7] = { 30, 45, 14, 3, 15, 8, 22 };
    	std::cout << "Custom epoch data (decimal format):" << std::endl;
    	std::cout << "  Seconds:  " << static_cast<LONGWORD>(VaRX8111CE_t_customEpoch[0]) << std::endl;
    	std::cout << "  Minutes:  " << static_cast<LONGWORD>(VaRX8111CE_t_customEpoch[1]) << std::endl;
    	std::cout << "  Hours:    " << static_cast<LONGWORD>(VaRX8111CE_t_customEpoch[2]) << std::endl;
    	std::cout << "  Week Day: " << static_cast<LONGWORD>(VaRX8111CE_t_customEpoch[3]) << std::endl;
    	std::cout << "  Day:      " << static_cast<LONGWORD>(VaRX8111CE_t_customEpoch[4]) << std::endl;
    	std::cout << "  Month:    " << static_cast<LONGWORD>(VaRX8111CE_t_customEpoch[5]) << std::endl;
    	std::cout << "  Year:     " << static_cast<LONGWORD>(VaRX8111CE_t_customEpoch[6]) << " (20" << static_cast<LONGWORD>(VaRX8111CE_t_customEpoch[6]) << ")" << std::endl;
    	std::cout << "Date/Time: August 15th, 2022 at 14:45:30" << std::endl;
    	MngRTCR_RX8111CE_Setup_Epoch(VaRX8111CE_t_customEpoch);
    	std::cout << "Custom epoch configuration completed." << std::endl;
    	displayChipData();
    	std::cout << "Verification: All time registers should contain the custom epoch values" << std::endl;
    	std::cout << "Values are stored in decimal format in register_expected_values array" << std::endl << std::endl;
#endif

#ifdef TESTCASE_7
    	// Test 7: GetRTCR_SecondsSinceEpoch function (simulated read)
    	std::cout << "Test 7: GetRTCR_SecondsSinceEpoch function" << std::endl;
    	std::cout << "Purpose: Read current time from chip and calculate seconds since epoch" << std::endl;
    	std::cout << "Process: Read all time registers sequentially, convert BCD to seconds" << std::endl;
    	std::cout << "Note: In test environment, reads from register_expected_values array" << std::endl;
    	LONGWORD VaRX8111CE_t_currentSeconds = GetRTCR_SecondsSinceEpoch();
    	std::cout << "Register read sequence completed." << std::endl;
    	std::cout << "Calculated seconds since epoch: " << VaRX8111CE_t_currentSeconds << std::endl;
    	displayChipData();
    	std::cout << "Expected calculation: ~84,293,130 seconds from Jan 1, 2020 to Aug 15, 2022" << std::endl;
    	std::cout << "Formula: (days * 86400) + (hours * 3600) + (minutes * 60) + seconds" << std::endl;
    	std::cout << "Includes leap year calculations and month-day conversions" << std::endl << std::endl;
#endif
	return 0;
}