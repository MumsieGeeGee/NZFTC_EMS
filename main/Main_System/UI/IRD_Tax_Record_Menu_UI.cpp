// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "IRD_Tax_Record_Menu_UI.h"
#include "Shared_UI_Messaging/Form_Labels_UI.h"
#include <string>
#include <iostream>

void Display_View_Update_IRD_Menu_UI(const std::string& IRD_Number) {
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "IRD Number:" << IRD_Number << " Record Menu" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "1. View File Contents" << std::endl;
    std::cout << "2. Edit Tax Information Record" << std::endl;
    std::cout << "3. Back to HR Management Main Menu" << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-3): ";
}

void Display_Edit_Tax_Record_Fields_Menu_UI(
	const std::string& IRD_Number,
	bool showStudentLoanOptions,
	bool specialTaxCodeActive,
	bool studentLoanActive,
	bool childSupportActive,
	bool kiwiSaverActive,
	bool kiwiSaverOptedOut,
	bool esctActive) {
    std::cout << "----------------------------------------------" << std::endl;
	std::cout << "IRD Number: " << IRD_Number << std::endl;
	std::cout << "----------------------------------------------" << std::endl;
	int nextChoice = 1;
	std::cout << nextChoice++ << ". PAYE Frequency" << std::endl;
	std::cout << nextChoice++ << ". PAYE Table Version" << std::endl;
	std::cout << nextChoice++ << ". Extra PAYE Voluntary Deductions" << std::endl;
	std::cout << nextChoice++ << ". Tax Code (Primary or Secondary)" << std::endl;
	std::cout << nextChoice++ << ". Special Tax Code" << std::endl;
	if (showStudentLoanOptions) {
		std::cout << nextChoice++ << ". Student Loan Existence" << std::endl;
	}
	std::cout << nextChoice++ << ". Child Support Status" << std::endl;
	std::cout << nextChoice++ << ". KiwiSaver Opt In/Out Status" << std::endl;

	if (specialTaxCodeActive) {
		std::cout << "----------------------------------------------" << std::endl;
		std::cout << nextChoice++ << ". STC Rate" << std::endl;
		std::cout << nextChoice++ << ". STC Letter Received" << std::endl;
		std::cout << nextChoice++ << ". STC Expiry Date" << std::endl;
	}
	if (showStudentLoanOptions && studentLoanActive) {
		std::cout << "----------------------------------------------" << std::endl;
		std::cout << nextChoice++ << ". Student Loan SDR IRD Letter Existence" << std::endl;
		std::cout << nextChoice++ << ". Student Loan Special Deduction Rate" << std::endl;
		std::cout << nextChoice++ << ". Student Loan Repayment Exemption Existence" << std::endl;
		std::cout << nextChoice++ << ". Student Loan Repayment Exemption Reason" << std::endl;
		std::cout << nextChoice++ << ". Student Loan Repayment Exemption Expiry Date" << std::endl;
		std::cout << nextChoice++ << ". Student Loan Repayment Threshold" << std::endl;
		std::cout << nextChoice++ << ". Student Loan Repayment Rate" << std::endl;
	}
	if (childSupportActive) {
		std::cout << "----------------------------------------------" << std::endl;
		std::cout << nextChoice++ << ". Child Support Standard Deduction" << std::endl;
		std::cout << nextChoice++ << ". Child Support Voluntary Deduction" << std::endl;
		std::cout << nextChoice++ << ". Child Support Protected Net Earnings" << std::endl;
		std::cout << nextChoice++ << ". Child Support Shortfall Calculation" << std::endl;
		std::cout << nextChoice++ << ". Child Support IRD Notice of Deduction Existence" << std::endl;
		std::cout << nextChoice++ << ". Child Support IRD Notice of Deduction Amount" << std::endl;
		std::cout << nextChoice++ << ". Child Support IRD Notice of Deduction Effective Date" << std::endl;
		std::cout << nextChoice++ << ". Child Support IRD Notice of Deduction Expiry Date" << std::endl;
		std::cout << nextChoice++ << ". Child Support IRD Notice of Deduction Reference Number" << std::endl;
	}
	if (kiwiSaverActive) {
		std::cout << nextChoice++ << ". KiwiSaver Employee Contribution Rate" << std::endl;
		if (kiwiSaverOptedOut) {
			std::cout << nextChoice++ << ". KiwiSaver Opt Out Date" << std::endl;
		}
	}

	std::cout << nextChoice++ << ". Employee Extras" << std::endl;

	if (esctActive) {
		std::cout << "----------------------------------------------" << std::endl;
		std::cout << nextChoice++ << ". Employee Extra Rate" << std::endl;
		std::cout << nextChoice++ << ". Employee Extra Type" << std::endl;
	}
	std::cout << "----------------------------------------------" << std::endl;
	std::cout << "0. Back to IRD Number:" << IRD_Number << " Record Menu" << std::endl;
	std::cout << Please_Select_an_Option_Label << " (0-" << (nextChoice - 1) << "): ";
}

void Display_HAS_STC_Edit_Tax_Record_Fields_Menu_UI(const std::string& IRD_Number) {
	std::cout << ". STC Rate" << std::endl;
	std::cout << ". STC Letter Received" << std::endl;
	std::cout << ". STC Expiry Date" << std::endl;
}

void Display_HAS_Student_Loan_Edit_Tax_Record_Fields_Menu_UI(const std::string& IRD_Number) {
	std::cout << ". Student Loan Special Deduction Rate Letter Received" << std::endl;
	std::cout << ". Student Loan Special Deduction Rate" << std::endl;
	std::cout << ". Student Loan Repayment Exemption Existence" << std::endl;
	std::cout << ". Student Loan Repayment Exemption Reason" << std::endl;
	std::cout << ". Student Loan Repayment Exemption Expiry Date" << std::endl;
	std::cout << ". Student Loan Repayment Threshold" << std::endl;
}

void Display_HAS_Child_Support_Edit_Tax_Record_Fields_Menu_UI(const std::string& IRD_Number) {
	std::cout << ". Child Support Standard Deduction" << std::endl;
	std::cout << ". Child Support Voluntary Deduction" << std::endl;
	std::cout << ". Child Support Protected Net Earnings" << std::endl;
	std::cout << ". Child Support Shortfall Calculation" << std::endl;
}

void Display_HAS_KiwiSaver_Edit_Tax_Record_Fields_Menu_UI(const std::string& IRD_Number) {
	std::cout << ". KiwiSaver Employee Contribution Rate" << std::endl;
	std::cout << ". KiwiSaver Employer Contribution Rate" << std::endl;
}

void Display_HAS_ESCT_Edit_Tax_Record_Fields_Menu_UI(const std::string& IRD_Number) {
	std::cout << ". ESCT Tax Rate" << std::endl;
	std::cout << ". ESCT Income Band" << std::endl;
}

void Display_Salary_Employee_Menu_UI() {
	std::cout << ". Contracted Weekly Hours" << std::endl;
	std::cout << ". Contracted Hourly Rate" << std::endl;
	std::cout << ". Annual Salary" << std::endl;
	std::cout << ". Monthly Salary" << std::endl;
	std::cout << ". Fortnightly Salary" << std::endl;
	std::cout << ". Weekly Salary" << std::endl;
}

