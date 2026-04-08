//Author: Gideon Davi

#include "rtcr_rx8111_driver.h"
#include <iostream>

//Uncomment which testcase you want to use
//Add testcases using this format.
//#define TESTCASE_1
//#define TESTCASE_2
//#define TESTCASE_3
//#define TESTCASE_4
//#define TESTCASE_5
//#define TESTCASE_6
//#define TESTCASE_7

int main() {

	//Driver testing
	//Print results to console

	/*  Function tests  */

	std::cout << "=== RTC RX8111CE Driver Test Suite ===" << std::endl << std::endl;
	
#ifdef TESTCASE_1
	// Test 1: CalcRTCR_DecToBCD function
	// Function converts decimal value to BCD format by:
	//     shifting the decimal value right one
	//     shifting the binary value left four
	//     returning the bitwise OR masked result modulo ten
	std::cout << "Test 1: CalcRTCR_DecToBCD function" << std::endl;
	BYTE VeRX8111CE_y_decimalVal = 45;
	BYTE VeRX8111CE_y_bcdResult = CalcRTCR_DecToBCD(VeRX8111CE_y_decimalVal);
	std::cout << "Input decimal: " << static_cast<int>(VeRX8111CE_y_decimalVal) << std::endl;
	std::cout << "Output BCD: 0x" << std::hex << static_cast<int>(VeRX8111CE_y_bcdResult) << std::dec << std::endl;
	std::cout << "Expected: 0x45 (decimal 45 -> BCD 0x45)" << std::endl << std::endl;
#endif

#ifdef TESTCASE_2
	// Test 2: CalcRTCR_BCDToDec function
	// Function converts BCD to decimal format by:
	//     shifting the binary value right four
	//     shifting the decimal value left one
	//     returning the added result bitwise AND 0x0F 
	std::cout << "Test 2: CalcRTCR_BCDToDec function" << std::endl;
	BYTE VeRX8111CE_y_bcdVal = 0x59;
	BYTE VeRX8111CE_y_decResult = CalcRTCR_BCDToDec(VeRX8111CE_y_bcdVal);
	std::cout << "Input BCD: 0x" << std::hex << static_cast<int>(VeRX8111CE_y_bcdVal) << std::dec << std::endl;
	std::cout << "Output decimal: " << static_cast<int>(VeRX8111CE_y_decResult) << std::endl;
	std::cout << "Expected: 59 (BCD 0x59 -> decimal 59)" << std::endl << std::endl;
#endif

#ifdef TESTCASE_3
	// Test 3: MngRTCR_RX8111CE_Init function
	std::cout << "Test 3: MngRTCR_RX8111CE_Init function" << std::endl;
	MngRTCR_RX8111CE_Init();
	std::cout << "Chip initialized. Current chip data:" << std::endl;
	displayChipData();
	std::cout << "Expected: i2c_address=2, register_address=16, register_value=0, all expected_values=0" << std::endl << std::endl;
#endif

#ifdef TESTCASE_4
	// Test 4: MngRTCR_Set_Epoch function
	std::cout << "Test 4: MngRTCR_Set_Epoch function" << std::endl;
	MngRTCR_Set_Epoch();
	std::cout << "Epoch set to predefined values. Current chip data:" << std::endl;
	displayChipData();
	std::cout << "Expected: Chip data updated with epoch values [0,0,0,0,4,1,20] in expected_values array" << std::endl << std::endl;
#endif

#ifdef TESTCASE_5
	// Test 5: CalcRTCR_CompileBCDToSecondsDec function
	// Function calculates the total seconds elapsed in the time data array
	//     total seconds = (sec) + (60*min) + (60*60*hour) + (60*60*24*total_days_since_epoch)
	std::cout << "Test 5: CalcRTCR_CompileBCDToSecondsDec function" << std::endl;
	BYTE VaRX8111CE_t_testTimeArray[7] = { 0x30, 0x15, 0x10, 0x05, 0x01, 0x06, 0x23 }; // 30sec, 15min, 10hr, day 5, day 1, month 6, year 23 (2023)
	LONGWORD VaRX8111CE_t_secondsResult = CalcRTCR_CompileBCDToSecondsDec(VaRX8111CE_t_testTimeArray);
	std::cout << "Input time array (BCD): [30sec, 15min, 10hr, day 5, day 1, month 6, year 23]" << std::endl;
	std::cout << "Total seconds since epoch: " << VaRX8111CE_t_secondsResult << std::endl;
	std::cout << "Expected: Large number representing seconds from Jan 1 2020 to June 5 2023 10:15:30" << std::endl << std::endl;
#endif

#ifdef TESTCASE_6
	// Test 6: MngRTCR_RX8111CE_Setup_Epoch function with custom data
	std::cout << "Test 7: MngRTCR_RX8111CE_Setup_Epoch function" << std::endl;
	BYTE VaRX8111CE_t_customEpoch[7] = { 30, 45, 14, 3, 15, 8, 22 }; // 30sec, 45min, 14hr, day 3, day 15, month 8, year 22
	MngRTCR_RX8111CE_Setup_Epoch(VaRX8111CE_t_customEpoch);
	std::cout << "Custom epoch set: [30sec, 45min, 14hr, day 3, day 15, month 8, year 22]" << std::endl;
	std::cout << "Current chip data:" << std::endl;
	displayChipData();
	std::cout << "Expected: expected_values array contains [30,45,14,3,15,8,22]" << std::endl << std::endl;
#endif

#ifdef TESTCASE_7
	// Test 7: GetRTCR_SecondsSinceEpoch function (simulated read)
	std::cout << "Test 8: GetRTCR_SecondsSinceEpoch function" << std::endl;
	std::cout << "Note: This function reads from I2C, simulating current time read..." << std::endl;
	LONGWORD VaRX8111CE_t_currentSeconds = GetRTCR_SecondsSinceEpoch();
	std::cout << "Seconds since epoch: " << VaRX8111CE_t_currentSeconds << std::endl;
	std::cout << "Current chip data after read operations:" << std::endl;
	displayChipData();
	std::cout << "Expected: 1,599,057,930 seconds from 01/01/20 to 08/15/22." << std::endl;
	std::cout << "Returns calculated seconds and updates expected_values with read data" << std::endl << std::endl;
#endif

	std::cout << "=== All Tests Completed ===" << std::endl;

	return 0;
}
