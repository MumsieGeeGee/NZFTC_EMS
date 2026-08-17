// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef PAYSLIP_GENERATION_H
#define PAYSLIP_GENERATION_H

#include <string>
#include "../../../Tax Calculator/Components/IRD_PAYE.h"
#include <vector>

struct Pre_Calculation_Payslip {
    std::string IRD_Number;
    Pay_Period period;
    double hours_worked;
    double sick_leave_days;
    double annual_leave_days;
    double parent_leave_days;
    double unpaid_leave_days;
    double overtime_paid_hours;
    double overtime_unpaid_hours;
    bool public_holidays_unworked;
    bool public_holidays_worked;
    double public_holiday_hours_worked;
    double travel_allowance_total;
    double meal_allowance_total;
    double tool_allowance_total;
    double clothing_allowance_total;
    double back_pay_amount;
    std::string backpay_start_date;
    std::string backpay_end_date;
    double cash_gift_amount;
    std::string commission_manager_name;
    double commission_amount;
    double on_call_shift_amount;
    double pd_amount;
    std::string pd_details;
    double eap_amount;
    std::string eap_details;
    double gross_earnings;
    double taxable_earnings;
    double paye;
    double acc;
    double student_loan;
    double child_support;
    double kiwisaver_employee;
    double kiwisaver_employer;
    double esct;
    double net_pay;
    double sick_leave_remaining;
    double annual_leave_remaining;
    double parental_leave_remaining;
    double pdc_remaining;
    double eap_remaining;
    bool requires_secondary_auth;
};

struct Payslip_Calculated {
    std::string IRD_Number;
    Pay_Period period;
    // Inputs
    double hours_worked;
    double sick_leave_days;
    double annual_leave_days;
    double parent_leave_days;
    double unpaid_leave_days;
    double overtime_paid_hours;
    double overtime_unpaid_hours;
    bool public_holidays_unworked;
    bool public_holidays_worked;
    // Allowances
    double travel_allowance_total;
    double meal_allowance_total;
    double tool_allowance_total;
    double clothing_allowance_total;
    // Lump sums
    double back_pay_amount;
    std::string backpay_start_date;
    std::string backpay_end_date;
    double cash_gift_amount;
    double commission_amount;
    double on_call_shift_amount;
    // PD / EAP
    double pd_amount;
    std::string pd_details;
    double eap_amount;
    std::string eap_details;
    // Calculated earnings
    double gross_earnings;
    double taxable_earnings;
    // Deductions
    double paye;
    double acc;
    double student_loan;
    double child_support;
    double kiwisaver_employee;
    double kiwisaver_employer;
    double esct;
    // Final
    double net_pay;
    double sick_leave_remaining;
    double annual_leave_remaining;
    double parental_leave_remaining;
    double pdc_remaining;
    double eap_remaining;
    bool requires_secondary_auth;
};

void Get_Pay_Period_Question_Value(Pre_Calculation_Payslip& slip);
bool Get_Hours_Worked_Value(double& hoursOut);
bool Get_Contracted_Hours(const std::string& IRD, double& contractedHours);
void Process_Hours_and_Overtime(const std::string& IRD, Pre_Calculation_Payslip& slip);
void Get_Selected_User_Hours_Worked(const std::string& username, Pre_Calculation_Payslip& slip);
bool Get_Employee_Additional_Earnings_Question_Value();
std::vector<std::string> Get_Allowance_Selection();
std::vector<std::string> Get_Lump_Sum_Selection();
void Get_Backpay_Dates(std::string& start, std::string& end);
double Get_Decimal_Value();
int Get_Integer_Value();
void Apply_Allowance_Value(const std::string& type, Pre_Calculation_Payslip& slip);
void Apply_Lump_Sum_Value(const std::string& type, Pre_Calculation_Payslip& slip);
void Get_Additional_Earnings(const std::string& username, Pre_Calculation_Payslip& slip);
void Get_Display_Employee_Additional_Earnings_Travel_Value(Pre_Calculation_Payslip& slip);
void Get_Display_Employee_Additional_Earnings_Meal_Value(Pre_Calculation_Payslip& slip);
void Get_Display_Employee_Additional_Earnings_Tool_Value(Pre_Calculation_Payslip& slip);
void Get_Display_Employee_Additional_Earnings_Clothing_Uniform_Value(Pre_Calculation_Payslip& slip);
void Get_Display_Employee_Additional_Earnings_On_Call_Shift_Value(Pre_Calculation_Payslip& slip);
void Get_Display_Employee_Additional_Earnings_Backpay_Value(Pre_Calculation_Payslip& slip);
void Get_Display_Employee_Additional_Earnings_Cash_Gift_Value(Pre_Calculation_Payslip& slip);
void Get_Display_Employee_Additional_Earnings_Commission_Value(Pre_Calculation_Payslip& slip);
void Handle_Sick_Leave(Pre_Calculation_Payslip& slip);
void Handle_Annual_Leave(Pre_Calculation_Payslip& slip);
void Handle_Unpaid_Leave(Pre_Calculation_Payslip& slip);
void Handle_Public_Holidays(Pre_Calculation_Payslip& slip);
void Handle_PD_Contribution(Pre_Calculation_Payslip& slip);
void Handle_EAP_Contribution(Pre_Calculation_Payslip& slip);
bool Handle_Payslip_Confirmation(const Pre_Calculation_Payslip& slip);
void Generate_Payslip(const std::string& username, Pre_Calculation_Payslip& slip);




#endif // PAYSLIP_GENERATION_Value_H
