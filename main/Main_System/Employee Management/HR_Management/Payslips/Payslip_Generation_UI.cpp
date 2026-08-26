// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Payslip_Generation_UI.h"
#include "Payslip_Generation.h"
#include "../../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../../UI/Shared_UI_Messaging/Warning_Messages.h"
#include "../../../UI/Shared_UI_Messaging/Success_Messages.h"
#include <iostream>

// admin...

void Display_Total_Decimal_Amount_Prompt_UI() {
    std::cout << "Enter the total amount in decimals:" << std::endl;
    std::cout << Total_Decimal_Amount_$_Label;
}

void Payslip_Form_Welcome_Header_UI(const std::string& employee_name) {
    std::cout << "...................................................................." << std::endl;
    std::cout << "                   Payslip Generation Form" << std::endl;
    std::cout << "...................................................................." << std::endl;
    std::cout << "           " << Employee_Name_Label << ": " << employee_name << std::endl;
}

void Display_Deductions_ALL_Employee_UI() {
    std::cout << "...................................................................." << std::endl;
    std::cout << "                          System Reminder:" << std::endl;
    std::cout << "...................................................................." << std::endl;
    std::cout << "   Child Support, Student Loan details, KiwiSaver Contributions" << std::endl;
    std::cout << "    and other salary package tax deductions are set inside of" << std::endl;
    std::cout << "     Employee Management and will be applied automatically." << std::endl;
    std::cout << "...................................................................." << std::endl;
}

void Display_Pay_Period_Question_UI() { // backend complete
    std::cout << "Enter the pay period in the following format:" << std::endl;
    std::cout << "01-01-2020, to 08-01-2020 (if weekly)" << std::endl;
    std::cout << "01-01-2020, to 01-02-2020 (if monthly) and so on. " << std::endl;
    std::cout << Pay_Period_Label;
}

void Display_Selected_User_Overtime_Hours_All_Employee_UI() {
    std::cout << "If the employee has exceeded their contracted overtime rate and maximum hours..." << std::endl;
}

void Display_Exceeded_Hours_All_Employee_UI(){
    std::cout << "Exceeded Hours will be captured and printed in the payslip after successful generation." << std::endl;
}

void Display_Hours_Worked_Input_Prompt_UI() {
    std::cout << "Enter the total hours this Employee worked for this pay period." << std::endl;
    std::cout << Total_Hours_Worked_Label;
}

void Display_Selected_User_Contracted_Hours_Salaried_Employee_UI() { // only show when detects salaried employee
    std::cout << "If the contracted hours for this Employee exceeds the hours worked..." << std::endl;
    std::cout << "The overtime rate and maximum hours set for the employee will be applied automatically." << std::endl;
    Display_Selected_User_Overtime_Hours_All_Employee_UI();
    Display_Exceeded_Hours_All_Employee_UI();
    Display_Hours_Worked_Input_Prompt_UI();
}

void Display_Selected_User_Overtime_Hours_Hourly_Employee_UI() { // only show when detects pay-by-hour (hourly) employee
    Display_Selected_User_Overtime_Hours_All_Employee_UI();
    Display_Exceeded_Hours_All_Employee_UI();
    Display_Hours_Worked_Input_Prompt_UI();
}

void Display_Employee_Additional_Earnings_Allowance_Types_UI() {
    std::cout << "Allowance Types:" << std::endl;
    std::cout << Travel_Allowance_Label << std::endl;
    std::cout << Meal_Allowance_Label << std::endl;
    std::cout << Tool_Allowance_Label << std::endl;
    std::cout << Clothing_Uniform_Allowance_Label << std::endl;
}

void Display_Employee_Additional_Earnings_Lump_Sum_Types_UI() {
    std::cout << "Lump Sum Payment Types:" << std::endl;
    std::cout << Backpay_Lump_Sum_Label << std::endl;
    std::cout << Cash_Gift_Lump_Sum_Label << std::endl;
    std::cout << Commission_Lump_Sum_Label << std::endl;
    std::cout << On_Call_Shift_Lump_Sum_Label << std::endl;
}

void Display_Employee_Additional_Earnings_Question_UI() {
    Display_Employee_Additional_Earnings_Allowance_Types_UI();
    Display_Employee_Additional_Earnings_Lump_Sum_Types_UI();
    std::cout << "Will the employee be receiving any of these in their pay?" << std::endl;
    std::cout << Yes_No_Question_Input_Label;
}

void Allowance_Selection_Menu_UI() {
    std::cout << "Please Enter which Allowances will apply in this payslip." << std::endl;
    Display_Employee_Additional_Earnings_Allowance_Types_UI();
    std::cout << "If more than one of these applies please use a comma to separate eg: Travel, Tool" << std::endl;
    std::cout << "If Allowances dont apply and only Lump Sum Payments do, please enter 'Skip'" << std::endl;
    std::cout << "Enter Here: ";
}

void Lump_Sum_Selection_Menu_UI() {
    std::cout << "Please Enter which Lump Sum Payment Types will apply in this payslip." << std::endl;
    Display_Employee_Additional_Earnings_Lump_Sum_Types_UI();
    std::cout << "If more than one of these applies please use a comma to separate eg: Backpay, On Call/Shift" << std::endl;
    std::cout << "Enter Here: ";
}

void Display_Employee_Additional_Earnings_Travel_UI() {
    std::cout << "Travel Allowances are decided prior to Travel." << std::endl;
    std::cout << "Multiple Travel Allowances in a pay period must be added to payslip as a total amount." << std::endl;
    std::cout << "How much (in total) is the employee entitled to for this pay period?" << std::endl;
    Display_Total_Decimal_Amount_Prompt_UI();
}

void Display_Employee_Additional_Earnings_Meal_UI() {
    std::cout << "Meal Allowances are given to those who have had to travel long distances away from the office." << std::endl;
    std::cout << "Meal Allowances are calculated based on $20 a meal, 3 meals a day." << std::endl;
    std::cout << "Most Trade Employees are exempt as their work is almost never at the office." << std::endl;
    std::cout << "If the user spent a half day away, it is counted as one day, any less and Meal Allowance is not given." << std::endl;
    std::cout << "How many days did this employee spend away from the office to complete work tasks?" << std::endl;
    std::cout << Total_Days_Label;
}

void Display_Employee_Additional_Earnings_Tool_UI() {
    std::cout << "Trade Employees are given a Tool allowance by default every payslip automatically." << std::endl;
    std::cout << "Only Special Circumstances will require extra, and will require a secondary authenticator to do when over $500." << std::endl;
    std::cout << "Tool Allowances are usually Office based like Chairs, Sit-Stand Desks, Neurodivergent Tools, Laptops etc." << std::endl;
    Display_Total_Decimal_Amount_Prompt_UI();
}

void Display_Employee_Additional_Earnings_Clothing_Uniform_UI() {
    std::cout << "Clothing and Uniform Allowances must not exceed $300 at a time" << std::endl;
    Display_Total_Decimal_Amount_Prompt_UI();
}

void Display_Employee_Additional_Earnings_Backpay_UI() {
    std::cout << "Backpay must be entered in the format: (Start-Date) + (End-Date)" << std::endl;
    std::cout << "If Backpay is for a singular day just enter the same date twice. " << std::endl;
    std::cout << "eg: 01-01-2020 + 01-01-2020, or 01-01-2020 + 07-01-2020" << std::endl;
    std::cout << Backpay_Dates_Label;
}

void Display_Employee_Additional_Earnings_Cash_Gift_UI() {
    std::cout << "Cash Gifts are not to be confused for/with gift cards." << std::endl;
    std::cout << "Cash Gifts are a taxable earning, gift cards are a gift." << std::endl;
    std::cout << "Cash Gifts requires a secondary authentication to give out." << std::endl;
    std::cout << "How much will the employee be receiving?" << std::endl;
    Display_Total_Decimal_Amount_Prompt_UI();
}

void Display_Employee_Additional_Earnings_Commission_UI() {
    std::cout << "Commission is calculated by the Sales Team Manager for each commissioner." << std::endl;
    std::cout << "You will need to provide the Name of the Team Manager who gave you the documentation." << std::endl;
    std::cout << "How much is the total commission amount?" << std::endl;
    Display_Total_Decimal_Amount_Prompt_UI();
}

void Display_Employee_Additional_Earnings_On_Call_Shift_UI() {
    std::cout << "On Call/Shift Allowances are decided prior to On-Call/Shift time." << std::endl;
    Display_Total_Decimal_Amount_Prompt_UI();
}

void Display_Sick_Leave_Prompt_UI(double sick_leave_remaining_weeks) {
    std::cout << "If the amount of Sick Leave days the employee took exceeds the availability..." << std::endl;
    std::cout << "The system will ask if the user was approved to take annual leave days for the remainder days." << std::endl;
    std::cout << "As Leave is accrued in hours, use '8' hours as one day. eg. 4 = Half Day, 8 = One Day" << std::endl;
    std::cout << "How many days does the employee wish to take? eg. 4" << std::endl;
    std::cout << Total_Sick_Days_Label << ": " << sick_leave_remaining_weeks << std::endl;
    std::cout << Total_Days_In_Hour_Format_Label;
}

void Display_Sick_Leave_Annual_Leave_Question_UI() {
    std::cout << "The Employee appears to have run out of sick days..." << std::endl;
    std::cout << "Were they approved to use annual leave days as a replacement?" << std::endl;
    std::cout << Yes_No_Question_Input_Label;
}

void Display_Unpaid_Leave_Prompt_Question_UI() {
    std::cout << "Did the user have any UNPAID days off during this pay period?" << std::endl;
    std::cout << "Days that they didn't have approved prior, or because they have run out of Annual Leave." << std::endl;
    std::cout << Yes_No_Question_Input_Label;
}

void Display_Sick_Leave_Annual_Leave_Replacement_No_Salary_Contracted_Hours_Employee_Answer_UI() { // only show when detects salaried employee
    std::cout << "Employee is Salaried with Contracted Hours. " << std::endl;
    std::cout << "Total amount of hours worked have now been edited." << std::endl;
    std::cout << "Proceeding to next menu..." << std::endl; 
    Display_Unpaid_Leave_Prompt_Question_UI();
}

void Display_Sick_Leave_Annual_Leave_Replacement_No_Hourly_Employee_Answer_UI() {
    std::cout << "Employee is paid by the hours they work, no edits required." << std::endl;
    std::cout << "Proceeding to next menu..." << std::endl; 
    Display_Unpaid_Leave_Prompt_Question_UI();
}

void Display_Annual_Leave_Prompt_Question_UI() {
    std::cout << "Did the Employee take any (Approved) Annual Leave time during this pay-period?" << std::endl;
    std::cout << Yes_No_Question_Input_Label;
}

void Display_Sick_Leave_Annual_Leave_Replacement_Yes_Answer_UI() {
    std::cout << "Annual Leave availability for the Employee updated accordingly." << std::endl;
    Display_Annual_Leave_Prompt_Question_UI();
}

void Display_Annual_Leave_Prompt_UI() {
    std::cout << "If the amount of Annual Leave days taken exceed the availability..." << std::endl;
    std::cout << "It will fall under 'Unpaid Leave' and calculations will be applied automatically." << std::endl;
    std::cout << "As Leave is accrued in hours, use '8' hours as one day. eg. 4 = Half Day, 8 = One Day" << std::endl;
    std::cout << "How many approved Annual Leave Days did the employee take during this pay-period? eg. 4 " << std::endl;
    std::cout << Total_Days_In_Hour_Format_Label;
}

void Display_Parent_Leave_Question_UI() {
    std::cout << "Did the Employee take any (Approved) Parental Leave time during this pay-period?" << std::endl;
    std::cout << Yes_No_Question_Input_Label;
}

void Display_Parent_Leave_Prompt_UI(double parental_leave_remaining_weeks) {
    std::cout << "If the amount of Parent Leave days taken exceed the availability..." << std::endl;
    std::cout << "It will fall under 'Unpaid Leave' and calculations will be applied automatically." << std::endl;
    std::cout << "As Leave is accrued in hours, use '8' hours as one day. eg. 4 = Half Day, 8 = One Day" << std::endl;
    std::cout << "How many approved Parent Leave Days did the employee take during this pay-period? eg. 4 " << std::endl;
    std::cout << Total_Parent_Leave_Days_Label << ": " << parental_leave_remaining_weeks << std::endl;
    std::cout << Total_Days_In_Hour_Format_Label;
}

void Display_Parental_Leave_Exceeds_Entitlement_UI(const std::string& warning_Message) {
    std::cout << Display_Warning << Parent_Leave_Requested_Exceeds_Entitlement << std::endl;
    std::cout << "Excess days will be treated as unpaid leave unless employer policy states otherwise." << std::endl;
}

void Display_Public_Holidays_Unworked_Question_Prompt_UI() {
    std::cout << "Was there any Public Holidays during this pay-period?" << std::endl;
    std::cout << Yes_No_Question_Input_Label;
}

void Display_Public_Holidays_Worked_Question_Prompt_UI() {
    std::cout << "Unworked Public Holidays are paid and calculated automatically." << std::endl;
    std::cout << "Did the user WORK any public holidays requiring a separate pay calculation?" << std::endl;
    std::cout << Yes_No_Question_Input_Label;
}

void Display_Professional_Development_Contribution_Prompt_UI() {
    std::cout << "Professional Development Contribution differs per role/salary packet." << std::endl;
    std::cout << "If contribution amount is more than the employees available sum..." << std::endl;
    std::cout << "a secondary authenticator will be required." << std::endl;
    std::cout << "Firstly, did the Employee use any of their Professional Development Contribution?" << std::endl;
    std::cout << Yes_No_Question_Input_Label;
}

void Display_Professional_Development_Contribution_Details_Question_UI() {
    std::cout << "Please Enter the Details of the Professional Development" << std::endl;
    std::cout << "Programme/Course Name, Start/Finish Dates and any other relevant information." << std::endl;
    std::cout << "Professional Development Course/Programme Details: ";
}

void Display_Professional_Development_Contribution_Amount_Question_UI() {
    std::cout << "Please enter the Total Cost of the (Pre-Approved) Course/Programme only, this does not include Meal Allowances" << std::endl;
    Display_Total_Decimal_Amount_Prompt_UI();
}

void Display_Employee_Assistance_Programme_Contribution_Prompt_UI() {
    std::cout << "Employee Assistance Programme contributions differ per role/salary packet." << std::endl;
    std::cout << "If contribution amount is more than the employees available sum..." << std::endl;
    std::cout << "a secondary authenticator will be required." << std::endl;
    std::cout << "Firstly, did the Employee use any of their EAP Services that require a contribution?" << std::endl;
    std::cout << Yes_No_Question_Input_Label;
}

void Display_Employee_Assistance_Programme_Contribution_Details_Question_UI() {
    std::cout << "Please Enter the Details of the Employee Assistance Programme" << std::endl;
    std::cout << "Programme/Course Name, Start/Finish Dates and any other relevant information." << std::endl;
    std::cout << "Employee Assistance Programme Details: ";
}

void Display_Employee_Assistance_Programme_Contribution_Amount_Question_UI() {
    std::cout << "Please Enter the Total Cost of the (Pre-Approved) Employee Assistance Programme." << std::endl;
    Display_Total_Decimal_Amount_Prompt_UI();
}

void Display_Employer_Extras_and_Benefits_Secondary_Auth_Prompt_UI() {
    std::cout << "It appears the entered amount is more than the employee has available..." << std::endl;
    std::cout << "Please get a colleague with secondary authenticator status " << std::endl;
    std::cout << "to sight the Approval Form and enter their Username and Password." << std::endl;
    // Run Secondary Auth 
}

void Display_Payslip_Confirmation_before_Generation_Options_with_no_values_UI() {
    std::cout << "1. Edit Sick Leave" << std::endl;
    std::cout << "2. Edit Annual Leave" << std::endl;
    std::cout << "3. Edit Parental Leave" << std::endl;
    std::cout << "4. Edit Unpaid Leave" << std::endl;
    std::cout << "5. Edit Public Holidays" << std::endl;
    std::cout << "6. Edit Professional Development Contribution" << std::endl;
    std::cout << "7. Edit Employee Assistance Programme Contribution" << std::endl;
    std::cout << "8. Edit Additional Earnings" << std::endl;
    std::cout << "9. Cancel Payslip Generation" << std::endl;
}

void Display_Payslip_Confirmation_before_Generation_Options_System_Reminder_UI() {
    std::cout << "KiwiSaver, ACC, Student Loan, Child Support, " << std::endl;
    std::cout << "Health Insurance/Superannuation (Employee Contribution) are all set inside of Employee Management." << std::endl;
    std::cout << "If you wish to change these you will need to exit Payslip Generation and change those values first." << std::endl;
    std::cout << "Employer Contribution Rates can only be changed by Senior Level IT as Salary Packets are Hard Coded" << std::endl;
    std::cout << "to this system to ensure fair pay for all employees no matter the gender. It is unlikely these need changing." << std::endl;
}

void Display_Payslip_Confirmation_before_Generation_UI(const Pre_Calculation_Payslip& slip) {
    std::cout << Employee_IRD_Number_Label << ": " << slip.IRD_Number << std::endl;

    // If Pay_Period has printable fields, adjust this accordingly:
    std::cout << Pay_Period_Label << ": "
              << slip.period.period_start_date << " - " << slip.period.period_end_date << std::endl;

    std::cout << Total_Hours_Worked_Label << ": " << slip.hours_worked << std::endl;
    std::cout << Total_Sick_Days_Label << ": " << slip.sick_leave_days << std::endl;
    std::cout << Total_Annual_Leave_Days_Label << ": " << slip.annual_leave_days << std::endl;
    std::cout << Total_Unpaid_Leave_Days_Label << ": " << slip.unpaid_leave_days << std::endl;

    std::cout << Total_Paid_Overtime_Hours_Label << ": " << slip.overtime_paid_hours << std::endl;
    std::cout << Total_Unpaid_Overtime_Hours_Label << ": " << slip.overtime_unpaid_hours << std::endl;

    std::cout << Total_Allowance_Payment_s_Label << ": "
              << (slip.travel_allowance_total
                  + slip.meal_allowance_total
                  + slip.tool_allowance_total
                  + slip.clothing_allowance_total)
              << std::endl;

    if (slip.travel_allowance_total > 0)
        std::cout << Travel_Allowance_Label << ": " << slip.travel_allowance_total << std::endl;
    if (slip.meal_allowance_total > 0)
        std::cout << Meal_Allowance_Label << ": " << slip.meal_allowance_total << std::endl;
    if (slip.tool_allowance_total > 0)
        std::cout << Tool_Allowance_Label << ": " << slip.tool_allowance_total << std::endl;
    if (slip.clothing_allowance_total > 0)
        std::cout << Clothing_Uniform_Allowance_Label << ": " << slip.clothing_allowance_total << std::endl;

    double total_lump_sums =
        slip.back_pay_amount +
        slip.cash_gift_amount +
        slip.commission_amount +
        slip.on_call_shift_amount;

    std::cout << Total_Lump_Sum_Payment_s_Label << ": " << total_lump_sums << std::endl;

    if (slip.back_pay_amount > 0)
        std::cout << Backpay_Lump_Sum_Label << ": " << slip.back_pay_amount
                  << " (" << slip.backpay_start_date << " - " << slip.backpay_end_date << ")" << std::endl;
    if (slip.cash_gift_amount > 0)
        std::cout << Cash_Gift_Lump_Sum_Label << ": " << slip.cash_gift_amount << std::endl;
    if (slip.commission_amount > 0)
        std::cout << Commission_Lump_Sum_Label << ": " << slip.commission_amount
                  << " (Manager: " << slip.commission_manager_name << ")" << std::endl;
    if (slip.on_call_shift_amount > 0)
        std::cout << On_Call_Shift_Lump_Sum_Label << ": " << slip.on_call_shift_amount << std::endl;

    std::cout << KiwiSaver_Employer_Contribution_Label << ": " << slip.kiwisaver_employer << std::endl;
    std::cout << KiwiSaver_Employee_Contribution_Label << ": " << slip.kiwisaver_employee << std::endl;

    std::cout << ACC_Earners_Levy_Rate_Label << ": " << slip.acc << std::endl;
    std::cout << Student_Loan_Repayment_Label << ": " << slip.student_loan << std::endl;
    std::cout << Child_Support_Deduction_Label << ": " << slip.child_support << std::endl;

    std::cout << PAYE_Tax_Label << ": " << slip.paye << std::endl;
    std::cout << Total_Taxable_Deductions_Label << ": " << slip.taxable_earnings << std::endl;

    std::cout << Gross_Earnings_Label << ": " << slip.gross_earnings << std::endl;
    std::cout << NET_Pay_Label << ": " << slip.net_pay << std::endl;

    std::cout << Remainder_Leave_Balances_Label << ": "
              << "Sick: " << slip.sick_leave_remaining
              << ", Annual: " << slip.annual_leave_remaining << std::endl;
    std::cout << Remainder_PDC_Balances_Label << ": " << slip.pdc_remaining << std::endl;
    std::cout << Remainder_EAP_Balances_Label << ": " << slip.eap_remaining << std::endl;

    std::cout << "..........................................................................." << std::endl;
    Display_Payslip_Confirmation_before_Generation_Options_System_Reminder_UI();
    std::cout << "Please Confirm (to the best of your ability) that the information you have provided is true and correct." << std::endl;
    std::cout << "Do you wish to proceed with payslip generation?" << std::endl;
    std::cout << Yes_No_Question_Input_Label;
}

void Display_Options_for_No_to_Payslip_Generation_Confirmation_UI() {
    std::cout << "Please select the section which you wish to edit or select exit to cancel payslip generation entirely." << std::endl;
    Display_Payslip_Confirmation_before_Generation_Options_with_no_values_UI();
}

void Display_Generation_Progress_Info_UI() {
    std::cout << "Please wait...calculating payslip" << std::endl;
}

void Display_Selected_User_Payslip_Generated_Return_to_Main_HR_Menu_UI(const std::string Payslip_Generated_Message) {
    std::cout << Display_Success << Payslip_Generated_Message << std::endl;
    std::cout << "Returning you to the main HR Management Menu" << std::endl;
}
