// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Save_Functions.h"
#include "Database.h"
#include "Input_Validation_Helpers.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include "../../Tax Calculator/Components/IRD_PAYE.h"
#include "../../Tax Calculator/Components/Tax_Codes.h"
#include "../../Tax Calculator/Components/ACC.h"
#include "../../Tax Calculator/Components/Student_Loan.h"
#include "../../Tax Calculator/Components/Child_Support.h"
#include "../../Tax Calculator/Components/Kiwisaver.h"
#include "../../Tax Calculator/Components/ESCT.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

namespace {
std::string Apply_Student_Loan_Suffix(const std::string& primaryTaxCode) {
	const std::string trimmed = Trim_Copy(primaryTaxCode);
	if (trimmed.empty()) {
		return "SL";
	}

	if (trimmed == "SL") {
		return "SL";
	}
	if (trimmed.size() >= 3 && trimmed.substr(trimmed.size() - 3) == " SL") {
		return trimmed;
	}

	return trimmed + " SL";
}

bool Upsert_IRD_Record_Field_In_Order(const std::string& IRD_Number, const std::string& fieldLabel, const std::string& fieldValue) {
	std::string filename = Get_IRD_Record_File_Path(IRD_Number);
	std::ifstream inFile(filename);
	if (!inFile) {
		std::cout << Display_Error << IRD_Tax_Information_Not_Found << std::endl;
		return false;
	}

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(inFile, line)) {
		lines.push_back(line);
	}

	const std::string newLine = fieldLabel + fieldValue;
	bool replaced = false;
	for (std::string& existingLine : lines) {
		if (Starts_With(existingLine, fieldLabel)) {
			existingLine = newLine;
			replaced = true;
			break;
		}
	}

	if (!replaced) {
		const std::array<std::string, 44> orderedFieldLabels = {
			PAYE_Frequency_Label,
			PAYE_Table_Version_Label,
			PAYE_Extra_Voluntary_Deductions_Label,
			Tax_Codes_Job_Type_Label,
			Primary_Tax_Code_Label,
			Secondary_Tax_Code_Label,
			Special_Tax_Code_Label,
			STC_Rate_Label,
			STC_Letter_Received_Label,
			STC_Expiry_Date_Label,
			ACC_Levy_Type_Label,
			ACC_Earners_Levy_Label,
			ACC_Earners_Levy_Rate_Label,
			ACC_Workplace_Levy_Label,
			ACC_Workplace_Levy_Rate_Label,
			ACC_Levy_Maximum_Earnings_Threshold_Label,
			Student_Loan_Existence_Label,
			Student_Loan_Tax_Code_Label,
			Student_Loan_SDR_IRD_Letter_Existence_Label,
			Student_Loan_Special_Deduction_Rate_Label,
			Student_Loan_Repayment_Exemption_Existence_Label,
			Student_Loan_Repayment_Exemption_Reason_Label,
			Student_Loan_Repayment_Exemption_Expiry_Date_Label,
			Student_Loan_Repayment_Threshold_Label,
			Child_Support_Status_Label,
			Child_Support_Standard_Deduction_Label,
			Child_Support_Voluntary_Deduction_Label,
			Child_Support_Protected_Net_Earnings_Label,
			Child_Support_Shortfall_Calculation_Label,
			KiwiSaver_Opt_In_Out_Status_Label,
			KiwiSaver_Employee_Contribution_Rate_Label,
			KiwiSaver_Employer_Contribution_Rate_Label,
            KiwiSaver_Opt_Out_Date_Label,
            ESCT_Existence_Label,
			ESCT_Tax_Rate_Label,
			ESCT_Income_Band_Label,
            Pay_Type_Label, 
            Employment_Type_Label,
            Contracted_Weekly_Hours_Label,
            Contracted_Hourly_Rate_Label,
            Annual_Salary_Label,
            Monthly_Salary_Label,
            Fortnightly_Salary_Label,
            Weekly_Salary_Label
		};

		std::size_t insertPos = lines.size();
		std::size_t currentIndex = orderedFieldLabels.size();
		for (std::size_t i = 0; i < orderedFieldLabels.size(); ++i) {
			if (orderedFieldLabels[i] == fieldLabel) {
				currentIndex = i;
				break;
			}
		}

		if (currentIndex < orderedFieldLabels.size()) {
			for (std::size_t i = currentIndex + 1; i < orderedFieldLabels.size(); ++i) {
				for (std::size_t lineIndex = 0; lineIndex < lines.size(); ++lineIndex) {
					if (Starts_With(lines[lineIndex], orderedFieldLabels[i])) {
						insertPos = lineIndex;
						i = orderedFieldLabels.size();
						break;
					}
				}
			}
		}

		lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(insertPos), newLine);
	}

	std::ofstream outFile(filename);
	if (!outFile) {
		std::cout << Display_Error << IRD_Write_to_File_Failure << std::endl;
		return false;
	}

	for (const auto& outLine : lines) {
		outFile << outLine << "\n";
	}

	return true;
}

bool Remove_IRD_Record_Field(const std::string& IRD_Number, const std::string& fieldLabel) {
	const std::string filename = Get_IRD_Record_File_Path(IRD_Number);
	std::ifstream inFile(filename);
	if (!inFile) {
		std::cout << Display_Error << IRD_Tax_Information_Not_Found << std::endl;
		return false;
	}

	std::vector<std::string> lines;
	std::string line;
	bool removed = false;
	while (std::getline(inFile, line)) {
		if (Starts_With(line, fieldLabel)) {
			removed = true;
			continue;
		}
		lines.push_back(line);
	}

	if (!removed) {
		return true;
	}

	std::ofstream outFile(filename);
	if (!outFile) {
		std::cout << Display_Error << IRD_Write_to_File_Failure << std::endl;
		return false;
	}

	for (const auto& outLine : lines) {
		outFile << outLine << "\n";
	}

	return true;
}

void Update_Tax_Code_When_Student_Loan_Applies(const std::string& IRD_Number, const std::string& studentLoanExistenceValue) {
	if (!Is_Student_Loan_Active_Input(studentLoanExistenceValue)) {
		return;
	}

	std::string currentPrimaryTaxCode;
	if (!Read_Record_Field_Value(IRD_Number, Primary_Tax_Code_Label, currentPrimaryTaxCode)) {
		Upsert_IRD_Record_Field_In_Order(IRD_Number, Primary_Tax_Code_Label, "SL");
		return;
	}

	const std::string updatedPrimaryTaxCode = Apply_Student_Loan_Suffix(currentPrimaryTaxCode);
	Upsert_IRD_Record_Field_In_Order(IRD_Number, Primary_Tax_Code_Label, updatedPrimaryTaxCode);
}
}

bool Save_IRD_Number(const std::string& IRD_Number) {
	if (IRD_Record_Exists_In_Master_Folder(IRD_Number)) {
		std::cout << IRD_Number_Already_Exists << std::endl;
		return false;
	}
	return true;
}

bool Create_IRD_Record_File(const std::string& IRD_Number) {
	std::string filename = Get_IRD_Record_File_Path(IRD_Number);
	std::ofstream file(filename);
	if (!file) {
		std::cout << Display_Error << IRD_Tax_Information_Save_Failure << std::endl;
		return false;
	}
	file << "Employee Tax Information Record" << std::endl;
	file << "IRD Number: " << IRD_Number << std::endl;
	file << IRD_Confirmation_Status_Label << Confirmation_Status_to_String(confirmation_Status::Unconfirmed) << std::endl;
	return true;
}

bool Save_IRD_Record_Field(const std::string& IRD_Number, const std::string& fieldLabel, const std::string& value) {
	const std::string normalizedIRD = Trim_Copy(IRD_Number);
	if (normalizedIRD.empty()) {
		return false;
	}

	if (!IRD_Record_Exists_In_Master_Folder(normalizedIRD)) {
		if (!Create_IRD_Record_File(normalizedIRD)) {
			return false;
		}
	}

	return Upsert_IRD_Record_Field_In_Order(normalizedIRD, fieldLabel + ": ", value);
}

bool Save_IRD_Confirmation_Status(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, IRD_Confirmation_Status_Label, value);
}

bool Save_Tax_Code_Job_Type(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Tax_Codes_Job_Type_Label, value);
}

bool Save_PAYE_Frequency(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, PAYE_Frequency_Label, value);
}

bool Save_PAYE_Table_Version(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, PAYE_Table_Version_Label, value);
}

bool Save_Extra_PAYE_Voluntary_Deduction(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, PAYE_Extra_Voluntary_Deductions_Label, value);
}

bool Save_Primary_Tax_Code(const std::string& IRD_Number, const std::string& value) {
	std::string primaryTaxCodeValue = Trim_Copy(value);
	if (Is_Student_Loan_Active(IRD_Number)) {
		primaryTaxCodeValue = Apply_Student_Loan_Suffix(primaryTaxCodeValue);
	}
	if (!Upsert_IRD_Record_Field_In_Order(IRD_Number, Primary_Tax_Code_Label, primaryTaxCodeValue)) {
		return false;
	}
	if (!Upsert_IRD_Record_Field_In_Order(IRD_Number, Tax_Codes_Job_Type_Label, "Primary")) {
		return false;
	}
	return Remove_IRD_Record_Field(IRD_Number, Secondary_Tax_Code_Label);
}

bool Save_Secondary_Tax_Code(const std::string& IRD_Number, const std::string& value) {
	if (!Upsert_IRD_Record_Field_In_Order(IRD_Number, Secondary_Tax_Code_Label, value)) {
		return false;
	}
	if (!Upsert_IRD_Record_Field_In_Order(IRD_Number, Tax_Codes_Job_Type_Label, "Secondary")) {
		return false;
	}
	return Remove_IRD_Record_Field(IRD_Number, Primary_Tax_Code_Label);
}

bool Save_Special_Tax_Code(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Special_Tax_Code_Label, value);
}

bool Save_STC_Rate(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, STC_Rate_Label, value);
}

bool Save_STC_Letter_Received(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, STC_Letter_Received_Label, value);
}

bool Save_STC_Expiry_Date(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, STC_Expiry_Date_Label, value);
}

bool Save_ACC_Levy_Types(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, ACC_Levy_Type_Label, value);
}

bool Save_ACC_Levy_Rate(const std::string& IRD_Number, const std::string& value, ACC_Levy_Types levyType) {

	if (levyType == ACC_Levy_Types::Work_Levy_Rate) {
		return Upsert_IRD_Record_Field_In_Order(IRD_Number, ACC_Workplace_Levy_Rate_Label, value);
	} else if (levyType == ACC_Levy_Types::Earners_Levy_Rate) {
		return Upsert_IRD_Record_Field_In_Order(IRD_Number, ACC_Earners_Levy_Rate_Label, value);
	}
	return false;
}

bool Save_ACC_Levy_Maximum(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, ACC_Levy_Maximum_Earnings_Threshold_Label, value);
}

bool Save_Student_Loan_Existence(const std::string& IRD_Number, const std::string& value) {
	if (!Upsert_IRD_Record_Field_In_Order(IRD_Number, Student_Loan_Existence_Label, value)) {
		return false;
	}
	Update_Tax_Code_When_Student_Loan_Applies(IRD_Number, value);
	return true;
}

bool Save_Student_Loan_Tax_Code(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Student_Loan_Tax_Code_Label, value);
}

bool Save_Student_Loan_SDR_IRD_Letter_Existence(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Student_Loan_SDR_IRD_Letter_Existence_Label, value);
}

bool Save_Student_Loan_Special_Deduction_Rate(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Student_Loan_Special_Deduction_Rate_Label, value);
}

bool Save_Student_Loan_Repayment_Exemption_Existence(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Student_Loan_Repayment_Exemption_Existence_Label, value);
}

bool Save_Student_Loan_Repayment_Exemption_Reason(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Student_Loan_Repayment_Exemption_Reason_Label, value);
}

bool Save_Student_Loan_Repayment_Exemption_Expiry_Date(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Student_Loan_Repayment_Exemption_Expiry_Date_Label, value);
}

bool Save_Student_Loan_Repayment_Threshold(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Student_Loan_Repayment_Threshold_Label, value);
}

bool Save_Child_Support_Status(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Child_Support_Status_Label, value);
}

bool Save_Child_Support_Standard_Deduction(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Child_Support_Standard_Deduction_Label, value);
}

bool Save_Child_Support_Voluntary_Deduction(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Child_Support_Voluntary_Deduction_Label, value);
}

bool Save_Child_Support_Protected_Net_Earnings(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Child_Support_Protected_Net_Earnings_Label, value);
}

bool Save_Child_Support_Shortfall_Calculation(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Child_Support_Shortfall_Calculation_Label, value);
}

bool Save_Child_Support_IRD_Notice_of_Deduction_Existence(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Child_Support_IRD_Notice_of_Deduction_Existence_Label, value);
}

bool Save_Child_Support_IRD_Notice_of_Deduction_Amount(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Child_Support_IRD_Notice_of_Deduction_Amount_Label, value);
}

bool Save_Child_Support_IRD_Notice_of_Deduction_Effective_Date(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Child_Support_IRD_Notice_of_Deduction_Effective_Date_Label, value);
}

bool Save_Child_Support_IRD_Notice_of_Deduction_Expiry_Date(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Child_Support_IRD_Notice_of_Deduction_Expiry_Date_Label, value);
}

bool Save_Child_Support_IRD_Notice_of_Deduction_Reference_Number(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, Child_Support_IRD_Notice_of_Deduction_Reference_Number_Label, value);
}

bool Save_KiwiSaver_Opt_In_Out_Status(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, KiwiSaver_Opt_In_Out_Status_Label, value);
}

bool Save_KiwiSaver_Employee_Contribution_Rate(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, KiwiSaver_Employee_Contribution_Rate_Label, value);
}

bool Save_KiwiSaver_Employer_Contribution_Rate(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, KiwiSaver_Employer_Contribution_Rate_Label, value);
}

bool Save_KiwiSaver_Opt_Out_Date(const std::string& IRD_Number, const std::string& value) {
    return Upsert_IRD_Record_Field_In_Order(IRD_Number, KiwiSaver_Opt_Out_Date_Label, value);
}

bool Save_ESCT_Existence(const std::string& IRD_Number, const std::string& value) {
    return Upsert_IRD_Record_Field_In_Order(IRD_Number, ESCT_Existence_Label, value);
}

bool Save_ESCT_Tax_Rate(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, ESCT_Tax_Rate_Label, value);
}

bool Save_ESCT_Income_Band(const std::string& IRD_Number, const std::string& value) {
	return Upsert_IRD_Record_Field_In_Order(IRD_Number, ESCT_Income_Band_Label, value);
}

bool Save_Pay_Type(const std::string& IRD_Number, const std::string& value) {
    return Upsert_IRD_Record_Field_In_Order(IRD_Number, Pay_Type_Label, value);
}

bool Save_Employment_Type(const std::string& IRD_Number, const std::string& value) {
    return Upsert_IRD_Record_Field_In_Order(IRD_Number, Employment_Type_Label, value);
}

bool Save_Contracted_Weekly_Hours(const std::string& IRD_Number, const std::string& value) {
    return Upsert_IRD_Record_Field_In_Order(IRD_Number, Contracted_Weekly_Hours_Label, value);
}

bool Save_Contracted_Hourly_Rate(const std::string& IRD_Number, const std::string& value) {
    return Upsert_IRD_Record_Field_In_Order(IRD_Number, Contracted_Hourly_Rate_Label, value);
}

bool Save_Annual_Salary(const std::string& IRD_Number, const std::string& value) {
    return Upsert_IRD_Record_Field_In_Order(IRD_Number, Annual_Salary_Label, value);
}

bool Save_Monthly_Salary(const std::string& IRD_Number, const std::string& value) {
    return Upsert_IRD_Record_Field_In_Order(IRD_Number, Monthly_Salary_Label, value);
}

bool Save_Fortnightly_Salary(const std::string& IRD_Number, const std::string& value) {
    return Upsert_IRD_Record_Field_In_Order(IRD_Number, Fortnightly_Salary_Label, value);
}

bool Save_Weekly_Salary(const std::string& IRD_Number, const std::string& value) {
    return Upsert_IRD_Record_Field_In_Order(IRD_Number, Weekly_Salary_Label, value);
}
