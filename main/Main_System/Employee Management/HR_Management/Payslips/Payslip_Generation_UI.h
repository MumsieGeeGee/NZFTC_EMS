// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef PAYSLIP_GENERATION_UI_H
#define PAYSLIP_GENERATION_UI_H

// admin
#include <string>
#include "Payslip_Generation.h"
void Display_Total_Decimal_Amount_Prompt_UI();
void Payslip_Form_Welcome_Header_UI(const std::string& employee_name);
void Display_Deductions_ALL_Employee_UI();
void Display_Pay_Period_Question_UI();

void Display_Selected_User_Overtime_Hours_All_Employee_UI();
void Display_Exceeded_Hours_All_Employee_UI();
void Display_Hours_Worked_Input_Prompt_UI();
void Display_Selected_User_Contracted_Hours_Salaried_Employee_UI();
void Display_Selected_User_Overtime_Hours_Hourly_Employee_UI();

void Display_Employee_Additional_Earnings_Allowance_Types_UI();
void Display_Employee_Additional_Earnings_Lump_Sum_Types_UI();
void Display_Employee_Additional_Earnings_Question_UI();
void Allowance_Selection_Menu_UI();
void Lump_Sum_Selection_Menu_UI();
void Display_Employee_Additional_Earnings_Travel_UI();
void Display_Employee_Additional_Earnings_Meal_UI();
void Display_Employee_Additional_Earnings_Tool_UI();
void Display_Employee_Additional_Earnings_Clothing_Uniform_UI();
void Display_Employee_Additional_Earnings_On_Call_Shift_UI();
void Display_Employee_Additional_Earnings_Backpay_UI();
void Display_Employee_Additional_Earnings_Cash_Gift_UI();
void Display_Employee_Additional_Earnings_Commission_UI();
void Display_Sick_Leave_Prompt_UI(double available_days);

void Display_Sick_Leave_Annual_Leave_Question_UI();
void Display_Unpaid_Leave_Prompt_Question_UI();
void Display_Sick_Leave_Annual_Leave_Replacement_No_Salary_Contracted_Hours_Employee_Answer_UI();
void Display_Sick_Leave_Annual_Leave_Replacement_No_Hourly_Employee_Answer_UI();
void Display_Annual_Leave_Prompt_Question_UI();
void Display_Sick_Leave_Annual_Leave_Replacement_Yes_Answer_UI();
void Display_Annual_Leave_Prompt_UI();
void Display_Parent_Leave_Question_UI();
void Display_Parent_Leave_Prompt_UI(double parental_leave_remaining_weeks);
void Display_Parental_Leave_Exceeds_Entitlement_UI(const std::string& warning_Message);

void Display_Public_Holidays_Unworked_Question_Prompt_UI();
void Display_Public_Holidays_Worked_Question_Prompt_UI();
void Display_Professional_Development_Contribution_Prompt_UI();
void Display_Professional_Development_Contribution_Details_Question_UI();
void Display_Professional_Development_Contribution_Amount_Question_UI();
void Display_Employee_Assistance_Programme_Contribution_Prompt_UI();
void Display_Employee_Assistance_Programme_Contribution_Details_Question_UI();
void Display_Employee_Assistance_Programme_Contribution_Amount_Question_UI();
void Display_Employer_Extras_and_Benefits_Secondary_Auth_Prompt_UI();

void Display_Payslip_Confirmation_before_Generation_Options_with_no_values_UI();
void Display_Payslip_Confirmation_before_Generation_UI(const Pre_Calculation_Payslip& slip);
void Display_Options_for_No_to_Payslip_Generation_Confirmation_UI();
void Display_Generation_Progress_Info_UI();
void Display_Selected_User_Payslip_Generated_Return_to_Main_HR_Menu_UI(const std::string Payslip_Generated_Message);



#endif // PAYSLIP_GENERATION_UI_H
