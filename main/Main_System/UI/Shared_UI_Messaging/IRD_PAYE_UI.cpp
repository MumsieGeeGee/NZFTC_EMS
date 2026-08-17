// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "IRD_PAYE_UI.h"
#include "Form_Labels_UI.h"
#include <string>
#include <iostream>

const std::string Secondary_Auth_for_IRD_Number_Prompt = "Has the IRD Number been confirmed through official IRD documentation and sighted by another admin?";
const std::string IRD_Number_Input_Prompt = "Please enter the IRD Number";
const std::string IRD_Number_Search_Prompt = "Please enter the IRD Number to search";
const std::string Pay_Frequency_Input_Prompt = "Please select the pay frequency (Weekly, Fortnightly, Monthly)";
const std::string PAYE_Table_Version_Input_Prompt = "Please enter the PAYE table version (e.g., 2024)";
const std::string Extra_PAYE_Voluntary_Deduction_Input_Prompt = "Please enter any extra PAYE voluntary deduction amount (or 0 if none): ";

void Display_Role_Type_as_Label(const std::string& role_type) {
	if (role_type.compare("Admin") == 0) {
		std::cout << Role_Type_Label_Admin << std::endl;
	} else if (role_type.compare("Finance") == 0) {
		std::cout << Role_Type_Label_Finance << std::endl;
	} else if (role_type.compare("Trade") == 0) {
		std::cout << Role_Type_Label_Trade << std::endl;
	} else {
		std::cout << "Unknown role type." << std::endl;
	}
}

void Display_HR_PAYE_Main_Menu_UI() {
    std::cout << "Employee PAYE Menu:" << std::endl;
    std::cout << "1. View an Employees Tax Information Record" << std::endl;
    std::cout << "2. Edit an Employees Tax Information Record" << std::endl;
    std::cout << "3. View an Employees Payslip" << std::endl;
    std::cout << "4. Create New Payslip" << std::endl;
    std::cout << "5. " << Return_to_HR_Management_Main_Menu_Label << std::endl;
    std::cout << Please_Select_an_Option_Label << " (1-5):";
}

const std::string ACC_Levy_Rate_Input_Prompt = "Please enter the ACC levy rate (as a percentage, e.g., 1.75): ";
const std::string ACC_Levy_Maximum_Input_Prompt = "Please enter the ACC levy maximum earnings threshold (e.g., 130000): ";
const std::string ACC_Annual_Maximum_Liable_Earnings_Threshold_Input_Prompt = "Enter the Maximum Liable Earnings Threshold (as a dollar amount)";
const std::string Child_Support_Status_Input_Prompt = "Please enter the child support status (active/inactive).";
const std::string Child_Support_Standard_Deduction_Input_Prompt = "Please enter the child support standard deduction amount (e.g., 136.50).";
const std::string Child_Support_Voluntary_Deduction_Input_Prompt = "Please enter any child support voluntary deduction amount (or 0 if none).";
const std::string Child_Support_Protected_Net_Earnings_Input_Prompt = "Please enter the child support protected net earnings (PNE) amount (e.g., 50000).";
const std::string Child_Support_Shortfall_Calculation_Input_Prompt = "All Data for the IRD Number must be entered before the child support shortfall calculation can be performed. Please ensure all required fields are completed.";
const std::string Child_Support_IRD_Notice_of_Deduction_Existence_Input_Prompt = "Please enter whether the child support IRD Notice of Deduction (NOD) exists (yes/no).";
const std::string Child_Support_IRD_Notice_of_Deduction_Amount_Input_Prompt = "Please enter the child support IRD Notice of Deduction (NOD) amount (e.g., 150).";
const std::string Child_Support_IRD_Notice_of_Deduction_Effective_Date_Input_Prompt = "Please enter the child support IRD Notice of Deduction (NOD) effective date.";
const std::string Child_Support_IRD_Notice_of_Deduction_Expiry_Date_Input_Prompt = "Please enter the child support IRD Notice of Deduction (NOD) expiry date.";
const std::string Child_Support_IRD_Notice_of_Deduction_Reference_Number_Input_Prompt = "Please enter the child support IRD Notice of Deduction (NOD) reference number.";
const std::string ESCT_Existence_Input_Prompt = "Does this record include employee extras (benefits/deductions)? (yes/no)";
const std::string ESCT_Tax_Rate_Input_Prompt = "Please enter the employee extra rate (as a percentage, e.g., 3.0)";
const std::string ESCT_Income_Band_Input_Prompt = "Select employee extra type: 1) Secondary Superannuation Scheme, 2) Union Rates, 3) Other";
const std::string KiwiSaver_Opt_In_Out_Status_Input_Prompt = "Is the employee opted in or out of KiwiSaver? (opt-in/opt-out)";
const std::string KiwiSaver_Employee_Contribution_Rate_Input_Prompt = "Please enter the employee contribution rate for KiwiSaver (as a percentage, e.g., 3%)";
const std::string KiwiSaver_Employer_Contribution_Rate_Input_Prompt = "Please enter the employer contribution rate for KiwiSaver (as a percentage, e.g., 3%)";
const std::string KiwiSaver_Planned_Opt_Out_Date_Not_Applicable = "If not applicable please enter 'N/A'.";
const std::string KiwiSaver_Opt_Out_Date_Input_Prompt = "Please enter the opt-out date (e.g., DD/MM/YYYY).";
const std::string Student_Loan_Existence_Input_Prompt = "Does this IRD Number have a student loan obligation?";
const std::string Student_Loan_SDR_IRD_Letter_Existence_Input_Question = "Does this IRD Number have a Special Deduction Rate (SDR) IRD Letter seen by Admin?";
const std::string Student_Loan_SDR_IRD_Letter_Existence_Input_Prompt = "Has an Admin seen the IRD Letter for the Student Loan Special Deduction Rate (SDR)?";
const std::string Student_Loan_Special_Deduction_Rate_Input_Prompt = "Please enter the Student Loan Special Deduction Rate (SDR) percentage.";
const std::string Student_Loan_Repayment_Exemption_Existence_Input_Question = "Does this IRD Number have a Student Loan Repayment Exemption?";
const std::string Student_Loan_Repayment_Exemption_Reason_Input_Prompt = "Please enter the Student Loan Repayment Exemption Reason";
const std::string Student_Loan_Repayment_Exemption_Reason_Full_Time_Study_Low_Income_Option = "Full-time study with low income.";
const std::string Student_Loan_Repayment_Exemption_Reason_Serious_Hardship_Option = "Serious hardship.";
const std::string Student_Loan_Repayment_Exemption_Reason_Multiple_Jobs_Uneven_Deductions_Option = "Multiple jobs/uneven deductions.";
const std::string Student_Loan_Repayment_Exemption_Reason_Adverse_Emergency_Event_Option = "Adverse/Emergency event.";
const std::string Student_Loan_Repayment_Exemption_Expiry_Date_Input_Prompt = "Please enter the Student Loan Repayment Exemption Expiry Date (DD/MM/YYYY)";
const std::string Tax_Codes_Input_Prompt = "Is this the primary/main job/employment for the IRD number, or secondary job/employment?";
const std::string Tax_Codes_Job_Type_Input_Prompt = "Primary or Secondary?: ";
const std::string Primary_Tax_Codes_Input_Guide1 = "'M SL' = Main Job, Student Loan obligations apply.";
const std::string Primary_Tax_Codes_Input_Guide2 = "'ME' = Main Job, Student Loan obligations do not apply.";
const std::string Primary_Tax_Codes_Input_Guide3 =  "'SL' = Student Loan Only, rarely used and usually only for special cases.";
const std::string Primary_Tax_Codes_List = 
    "Primary Tax Codes:\n" +
    M_Standard_Label + "\n" +
    ME_Standard_No_Student_Loan_Label + "\n" +
    SL_Student_Loan_Label + "\n";
const std::string Secondary_Tax_Codes_List =
    "Secondary Tax Codes:\n" +
    SB_Secondary_Low_Bracket_Label + "\n" +
    S_Secondary_Standard_Label + "\n" +
    SH_Secondary_Higher_Bracket_Label + "\n" +
    ST_Secondary_Top_Bracket_Label + "\n" +
    SA_Secondary_Additional_Top_Bracket_Label + "\n";
const std::string Special_Tax_Codes_List =
    "Special Tax Codes:\n" +
    STC_Special_Tax_Code_Label + "\n" +
    CAE_Casual_Agricultural_Employee_Label + "\n" +
    EDW_Election_Day_Worker_Label + "\n" +
    NSW_Non_Resident_Seasonal_Worker_Label + "\n" +
    WT_Schedular_Payments_Contractors_Label + "\n";
const std::string Full_Tax_Code_List =
    "Full Tax Code List\n" +
    Primary_Tax_Code_Label + "\n" +
    M_Standard_Label + "\n" +
    ME_Standard_No_Student_Loan_Label + "\n" +
    SL_Student_Loan_Label + "\n" +
    Secondary_Tax_Code_Label + "\n" +
    SB_Secondary_Low_Bracket_Label + "\n" +
    S_Secondary_Standard_Label + "\n" +
    SH_Secondary_Higher_Bracket_Label + "\n" +
    ST_Secondary_Top_Bracket_Label + "\n" +
    SA_Secondary_Additional_Top_Bracket_Label + "\n" +
    Special_Tax_Code_Label + "\n" +
    STC_Special_Tax_Code_Label + "\n" +
    STC_Rate_Label + "\n" +
    STC_Letter_Received_Label + "\n" +
    STC_Expiry_Date_Label + "\n" +
    CAE_Casual_Agricultural_Employee_Label + "\n" +
    EDW_Election_Day_Worker_Label + "\n" +
    NSW_Non_Resident_Seasonal_Worker_Label + "\n" +
    WT_Schedular_Payments_Contractors_Label + "\n";
const std::string Primary_Tax_Code_Input_Prompt = "Please select the primary tax code for the IRD number.";
const std::string Secondary_Tax_Code_Input_Prompt = "Please select the secondary tax code or input 'none' if none: ";
const std::string Special_Tax_Code_Input_Prompt = "Please select the special tax code, or 'None' if there isn't one.";
const std::string STC_Rate_Input_Prompt = "Please enter the STC rate (%).";
const std::string STC_Letter_Received_Input_Prompt = "Please enter whether the STC letter has been received (yes/no).";
const std::string STC_Expiry_Date_Input_Prompt = "Please enter the STC expiry date (dd-mm-yyyy).";
