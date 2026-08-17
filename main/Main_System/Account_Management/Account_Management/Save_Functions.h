// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef SAVE_FUNCTIONS_H
#define SAVE_FUNCTIONS_H

#include <string>

enum class ACC_Levy_Types;

bool Save_IRD_Number(const std::string& IRD_Number);
bool Create_IRD_Record_File(const std::string& IRD_Number);
bool Save_IRD_Record_Field(const std::string& IRD_Number, const std::string& fieldLabel, const std::string& value);
bool Save_IRD_Confirmation_Status(const std::string& IRD_Number, const std::string& value);
bool Save_Tax_Code_Job_Type(const std::string& IRD_Number, const std::string& value);

bool Save_PAYE_Frequency(const std::string& IRD_Number, const std::string& value);
bool Save_PAYE_Table_Version(const std::string& IRD_Number, const std::string& value);
bool Save_Extra_PAYE_Voluntary_Deduction(const std::string& IRD_Number, const std::string& value);

bool Save_Primary_Tax_Code(const std::string& IRD_Number, const std::string& value);
bool Save_Secondary_Tax_Code(const std::string& IRD_Number, const std::string& value);
bool Save_Special_Tax_Code(const std::string& IRD_Number, const std::string& value);
bool Save_STC_Rate(const std::string& IRD_Number, const std::string& value);
bool Save_STC_Letter_Received(const std::string& IRD_Number, const std::string& value);
bool Save_STC_Expiry_Date(const std::string& IRD_Number, const std::string& value);
bool Save_ACC_Levy_Types(const std::string& IRD_Number, const std::string& value);
bool Save_ACC_Levy_Rate(const std::string& IRD_Number, const std::string& value, ACC_Levy_Types levyType);
bool Save_ACC_Levy_Maximum(const std::string& IRD_Number, const std::string& value);

bool Save_Student_Loan_Existence(const std::string& IRD_Number, const std::string& value);
bool Save_Student_Loan_Tax_Code(const std::string& IRD_Number, const std::string& value);
bool Save_Student_Loan_SDR_IRD_Letter_Existence(const std::string& IRD_Number, const std::string& value);
bool Save_Student_Loan_Special_Deduction_Rate(const std::string& IRD_Number, const std::string& value);
bool Save_Student_Loan_Repayment_Exemption_Existence(const std::string& IRD_Number, const std::string& value);
bool Save_Student_Loan_Repayment_Exemption_Reason(const std::string& IRD_Number, const std::string& value);
bool Save_Student_Loan_Repayment_Exemption_Expiry_Date(const std::string& IRD_Number, const std::string& value);
bool Save_Student_Loan_Repayment_Threshold(const std::string& IRD_Number, const std::string& value);

bool Save_Child_Support_Status(const std::string& IRD_Number, const std::string& value);
bool Save_Child_Support_Standard_Deduction(const std::string& IRD_Number, const std::string& value);
bool Save_Child_Support_Voluntary_Deduction(const std::string& IRD_Number, const std::string& value);
bool Save_Child_Support_Protected_Net_Earnings(const std::string& IRD_Number, const std::string& value);
bool Save_Child_Support_Shortfall_Calculation(const std::string& IRD_Number, const std::string& value);
bool Save_Child_Support_IRD_Notice_of_Deduction_Existence(const std::string& IRD_Number, const std::string& value);
bool Save_Child_Support_IRD_Notice_of_Deduction_Amount(const std::string& IRD_Number, const std::string& value);
bool Save_Child_Support_IRD_Notice_of_Deduction_Effective_Date(const std::string& IRD_Number, const std::string& value);
bool Save_Child_Support_IRD_Notice_of_Deduction_Expiry_Date(const std::string& IRD_Number, const std::string& value);
bool Save_Child_Support_IRD_Notice_of_Deduction_Reference_Number(const std::string& IRD_Number, const std::string& value);

bool Save_KiwiSaver_Opt_In_Out_Status(const std::string& IRD_Number, const std::string& value);
bool Save_KiwiSaver_Employee_Contribution_Rate(const std::string& IRD_Number, const std::string& value);
bool Save_KiwiSaver_Employer_Contribution_Rate(const std::string& IRD_Number, const std::string& value);
bool Save_KiwiSaver_Opt_Out_Date(const std::string& IRD_Number, const std::string& value);

bool Save_ESCT_Existence(const std::string& IRD_Number, const std::string& value);
bool Save_ESCT_Tax_Rate(const std::string& IRD_Number, const std::string& value);
bool Save_ESCT_Income_Band(const std::string& IRD_Number, const std::string& value);

bool Save_Pay_Type(const std::string& IRD_Number, const std::string& value);
bool Save_Employment_Type(const std::string& IRD_Number, const std::string& value);
bool Save_Contracted_Weekly_Hours(const std::string& IRD_Number, const std::string& value);
bool Save_Contracted_Hourly_Rate(const std::string& IRD_Number, const std::string& value);
bool Save_Annual_Salary(const std::string& IRD_Number, const std::string& value);
bool Save_Monthly_Salary(const std::string& IRD_Number, const std::string& value);
bool Save_Fortnightly_Salary(const std::string& IRD_Number, const std::string& value);
bool Save_Weekly_Salary(const std::string& IRD_Number, const std::string& value);


#endif // SAVE_FUNCTIONS_H
