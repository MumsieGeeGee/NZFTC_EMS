// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef IRD_PAYE_UI_H
#define IRD_PAYE_UI_H

#include "Form_Labels_UI.h"
#include <string>

extern const std::string Secondary_Auth_for_IRD_Number_Prompt;
extern const std::string IRD_Number_Input_Prompt;
extern const std::string IRD_Number_Search_Prompt;
extern const std::string Pay_Frequency_Input_Prompt;
extern const std::string PAYE_Table_Version_Input_Prompt;
extern const std::string Extra_PAYE_Voluntary_Deduction_Input_Prompt;

void Display_Role_Type_as_Label(const std::string& role_type);

void Display_HR_PAYE_Main_Menu_UI();

extern const std::string ACC_Levy_Rate_Input_Prompt;
extern const std::string ACC_Levy_Maximum_Input_Prompt;
extern const std::string ACC_Annual_Maximum_Liable_Earnings_Threshold_Input_Prompt;
extern const std::string Child_Support_Status_Input_Prompt;
extern const std::string Child_Support_Standard_Deduction_Input_Prompt;
extern const std::string Child_Support_Voluntary_Deduction_Input_Prompt;
extern const std::string Child_Support_Protected_Net_Earnings_Input_Prompt;
extern const std::string Child_Support_Shortfall_Calculation_Input_Prompt;
extern const std::string Child_Support_IRD_Notice_of_Deduction_Existence_Input_Prompt;
extern const std::string Child_Support_IRD_Notice_of_Deduction_Amount_Input_Prompt;
extern const std::string Child_Support_IRD_Notice_of_Deduction_Effective_Date_Input_Prompt;
extern const std::string Child_Support_IRD_Notice_of_Deduction_Expiry_Date_Input_Prompt;
extern const std::string Child_Support_IRD_Notice_of_Deduction_Reference_Number_Input_Prompt;
extern const std::string ESCT_Existence_Input_Prompt;
extern const std::string ESCT_Tax_Rate_Input_Prompt;
extern const std::string ESCT_Income_Band_Input_Prompt;
extern const std::string KiwiSaver_Opt_In_Out_Status_Input_Prompt;
extern const std::string KiwiSaver_Employee_Contribution_Rate_Input_Prompt;
extern const std::string KiwiSaver_Employer_Contribution_Rate_Input_Prompt;
extern const std::string KiwiSaver_Planned_Opt_Out_Date_Not_Applicable;
extern const std::string KiwiSaver_Opt_Out_Date_Input_Prompt;
extern const std::string Student_Loan_Existence_Input_Prompt;
extern const std::string Student_Loan_SDR_IRD_Letter_Existence_Input_Question;
extern const std::string Student_Loan_SDR_IRD_Letter_Existence_Input_Prompt;
extern const std::string Student_Loan_Special_Deduction_Rate_Input_Prompt;
extern const std::string Student_Loan_Repayment_Exemption_Existence_Input_Question;
extern const std::string Student_Loan_Repayment_Exemption_Reason_Input_Prompt;
extern const std::string Student_Loan_Repayment_Exemption_Reason_Full_Time_Study_Low_Income_Option;
extern const std::string Student_Loan_Repayment_Exemption_Reason_Serious_Hardship_Option;
extern const std::string Student_Loan_Repayment_Exemption_Reason_Multiple_Jobs_Uneven_Deductions_Option;
extern const std::string Student_Loan_Repayment_Exemption_Reason_Adverse_Emergency_Event_Option;
extern const std::string Student_Loan_Repayment_Exemption_Expiry_Date_Input_Prompt;
extern const std::string Tax_Codes_Input_Prompt;
extern const std::string Tax_Codes_Job_Type_Input_Prompt;
extern const std::string Primary_Tax_Codes_Input_Guide1;
extern const std::string Primary_Tax_Codes_Input_Guide2;
extern const std::string Primary_Tax_Codes_Input_Guide3;
extern const std::string Primary_Tax_Codes_List;
extern const std::string Secondary_Tax_Codes_List;
extern const std::string Special_Tax_Codes_List;
extern const std::string Full_Tax_Code_List;
extern const std::string Primary_Tax_Code_Input_Prompt;
extern const std::string Secondary_Tax_Code_Input_Prompt;
extern const std::string Special_Tax_Code_Input_Prompt;
extern const std::string STC_Rate_Input_Prompt;
extern const std::string STC_Letter_Received_Input_Prompt;
extern const std::string STC_Expiry_Date_Input_Prompt;

#endif // IRD_PAYE_UI_H
