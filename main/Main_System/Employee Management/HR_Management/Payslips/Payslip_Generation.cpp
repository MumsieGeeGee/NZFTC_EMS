// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Payslip_Generation.h"
#include "Payslip_Generation_UI.h"
#include "../../../Tax Calculator/Calculator/Calculator.h"
#include "../../../Account_Management/Account_Management/Save_Payslip.h"
#include "../../../Tax Calculator/Components/IRD_PAYE.h"
#include "../../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../../../Account_Management/Account_Management/Security_Protocols.h"
#include "../../../Account_Management/Account_Management/Database.h"
#include "../../../Account_Management/Account_Access/Account_Database.h"
#include "../../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../../UI/Shared_UI_Messaging/Info_Messages.h"
#include "../../../UI/Shared_UI_Messaging/Success_Messages.h"
#include "../../../UI/Shared_UI_Messaging/Warning_Messages.h"
#include "../Leave/Leave_Requests_UI.h"
#include <string>
#include <iostream>

void Get_Pay_Period_Question_Value(Pre_Calculation_Payslip& slip) {
    std::string input;

    while (true) {
        std::getline(std::cin, input);
        input = Trim_Copy(input);

        if (input.empty()) {
            std::cout << Display_Error << Pay_Period_Empty << std::endl;
            continue;
        }

        if (!Valid_Pay_Period_for_Payslips(input)) {
            // Error already displayed by validation
            continue;
        }

        // Extract start and end dates
        std::size_t pos = input.find("to");
        slip.period.period_start_date = Trim_Copy(input.substr(0, pos));
        slip.period.period_end_date   = Trim_Copy(input.substr(pos + 2));

        break;
    }
}

bool Get_Hours_Worked_Value(double& hoursOut) {
    std::string input;

    while (true) {
        std::getline(std::cin, input);
        input = Trim_Copy(input);

        if (input.empty()) {
            std::cout << Display_Error << Hours_Worked_Empty << std::endl;
            continue;
        }

        if (!Is_Decimal_Number(input)) {
            std::cout << Display_Error << Invalid_Hours_Format << std::endl;
            continue;
        }

        double hours = std::stod(input);

        if (hours < 0) {
            std::cout << Display_Error << Hours_Negative << std::endl;
            continue;
        }

        hoursOut = hours;
        return true;
    }
}
bool Get_Contracted_Hours(const std::string& IRD, double& contractedHours) {
    std::string value;
    if (!Read_Record_Field_Value(IRD, Contracted_Weekly_Hours_Label, value))
        return false;

    contractedHours = std::stod(Trim_Copy(value));
    return true;
}

void Process_Hours_and_Overtime(const std::string& IRD, Pre_Calculation_Payslip& slip) {
    double contractedHours = 0.0;
    Get_Contracted_Hours(IRD, contractedHours);

    double hoursWorked = slip.hours_worked;

    slip.overtime_paid_hours = 0.0;
    slip.overtime_unpaid_hours = 0.0;

    if (Is_Salary_Employee(IRD)) {
        // Salaried logic
        if (hoursWorked > contractedHours) {
            slip.overtime_paid_hours = hoursWorked - contractedHours;
        }
        return;
    }

    if (Is_Hourly_Employee(IRD)) {
        // Hourly logic
        double maxHours = contractedHours * 1.25; // example rule

        if (hoursWorked > contractedHours) {
            slip.overtime_paid_hours = hoursWorked - contractedHours;
        }

        if (hoursWorked > maxHours) {
            slip.overtime_unpaid_hours = hoursWorked - maxHours;
        }

        return;
    }
}

void Get_Selected_User_Hours_Worked(const std::string& username, Pre_Calculation_Payslip& slip) {
    const std::string IRD = Get_IRD_Number_For_Username(username);

    if (Is_Salary_Employee(IRD)) {
        Display_Selected_User_Contracted_Hours_Salaried_Employee_UI();
    }
    if (Is_Hourly_Employee(IRD)) {
        Display_Selected_User_Overtime_Hours_Hourly_Employee_UI();
    }

    double hoursWorked = 0.0;
    if (!Get_Hours_Worked_Value(hoursWorked)) {
        return;
    }

    slip.hours_worked = hoursWorked;

    Process_Hours_and_Overtime(IRD, slip);
}

bool Get_Employee_Additional_Earnings_Question_Value() {
    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    if (!Yes_or_No(input)) {
        std::cout << Display_Error << Yes_No_Input << std::endl;
        return false;
    }

    return Is_Yes_Input(input);
}

std::vector<std::string> Get_Allowance_Selection() {
    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    if (input == "Skip" || input == "skip") {
        return {};
    }

    return Parse_CSV_List(input);
}

std::vector<std::string> Get_Lump_Sum_Selection() {
    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    return Parse_CSV_List(input);
}

void Get_Backpay_Dates(std::string& start, std::string& end) {
    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    // Expected format: "01-01-2020 + 07-01-2020"
    std::size_t pos = input.find('+');
    if (pos == std::string::npos) {
        std::cout << Display_Error << Invalid_Pay_Dates_Format << std::endl;
        return;
    }

    start = Trim_Copy(input.substr(0, pos));
    end   = Trim_Copy(input.substr(pos + 1));

    if (!Is_Date_Format(start) || !Is_Date_Format(end)) {
        std::cout << Display_Error << Invalid_Date_Format << std::endl;
    }
}

void Apply_Allowance_Value(const std::string& type, Pre_Calculation_Payslip& slip) {
    if (type == "Travel") {
        Get_Display_Employee_Additional_Earnings_Travel_Value(slip);
    }
    else if (type == "Meal") {
        Get_Display_Employee_Additional_Earnings_Meal_Value(slip);
    }
    else if (type == "Tool") {
        Get_Display_Employee_Additional_Earnings_Tool_Value(slip);
    }
    else if (type == "Clothing" || type == "Uniform") {
        Get_Display_Employee_Additional_Earnings_Clothing_Uniform_Value(slip);
    }
}

void Apply_Lump_Sum_Value(const std::string& type, Pre_Calculation_Payslip& slip) {
    if (type == "Backpay") {
        Get_Display_Employee_Additional_Earnings_Backpay_Value(slip);
    }
    else if (type == "Cash Gift") {
        Get_Display_Employee_Additional_Earnings_Cash_Gift_Value(slip);
    }
    else if (type == "Commission") {
        Get_Display_Employee_Additional_Earnings_Commission_Value(slip);
    }
    else if (type == "On Call" || type == "On Call/Shift") {
        Get_Display_Employee_Additional_Earnings_On_Call_Shift_Value(slip);
    }
}

void Get_Additional_Earnings(const std::string& username, Pre_Calculation_Payslip& slip) {
    Display_Employee_Additional_Earnings_Question_UI();

    if (!Get_Employee_Additional_Earnings_Question_Value()) {
        return; // No additional earnings
    }
    // Allowances
    Allowance_Selection_Menu_UI();
    auto allowances = Get_Allowance_Selection();

    for (const auto& a : allowances) {
        Apply_Allowance_Value(a, slip);
    }
    // Lump sums
    Lump_Sum_Selection_Menu_UI();
    auto lumps = Get_Lump_Sum_Selection();

    for (const auto& l : lumps) {
        Apply_Lump_Sum_Value(l, slip);
    }
}

void Get_Display_Employee_Additional_Earnings_Travel_Value(Pre_Calculation_Payslip& slip) {
    Display_Employee_Additional_Earnings_Travel_UI();
    // Get decimal input (validated)
    double value = Get_Decimal_Value();
    // Store in slip
    slip.travel_allowance_total = value;
}

void Get_Display_Employee_Additional_Earnings_Meal_Value(Pre_Calculation_Payslip& slip) {
    Display_Employee_Additional_Earnings_Meal_UI();
    // Get number of days (validated integer)
    int days = Get_Integer_Value();
    // Convert days → total meal allowance
    // $20 per meal × 3 meals per day
    slip.meal_allowance_total = static_cast<double>(days) * 20.0 * 3.0;
}

void Get_Display_Employee_Additional_Earnings_Tool_Value(Pre_Calculation_Payslip& slip) {
    Display_Employee_Additional_Earnings_Tool_UI();
    // Get decimal input (validated)
    double value = Get_Decimal_Value();
    // Secondary authentication trigger for > $500
    if (value > 500.0) {
        Secondary_Authentication("Tool Allowance exceeds $500");
    }
    // Store in slip
    slip.tool_allowance_total = value;
}

void Get_Display_Employee_Additional_Earnings_Clothing_Uniform_Value(Pre_Calculation_Payslip& slip) {
    Display_Employee_Additional_Earnings_Clothing_Uniform_UI();
    // Get decimal input (validated)
    double value = Get_Decimal_Value();
    // Secondary authentication trigger for > $300
    if (value > 300.0) {
        Secondary_Authentication("Clothing/Uniform Allowance exceeds $300");
    }
    // Store in slip
    slip.clothing_allowance_total = value;
}

void Get_Display_Employee_Additional_Earnings_Backpay_Value(Pre_Calculation_Payslip& slip) {
    Display_Employee_Additional_Earnings_Backpay_UI();
    // Read the date range input
    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);
    // Expected format: "01-01-2020 + 07-01-2020"
    std::size_t pos = input.find('+');
    if (pos == std::string::npos) {
        std::cout << Display_Error << Invalid_Pay_Dates_Format << std::endl;
        return;
    }
    // Extract start and end dates
    std::string start = Trim_Copy(input.substr(0, pos));
    std::string end   = Trim_Copy(input.substr(pos + 1));
    // Validate date formats
    if (!Is_Date_Format(start) || !Is_Date_Format(end)) {
        std::cout << Display_Error << Invalid_Date_Format << std::endl;
        return;
    }
    // Store dates
    slip.backpay_start_date = start;
    slip.backpay_end_date   = end;
    // Now ask for the backpay amount
    Display_Total_Decimal_Amount_Prompt_UI();
    double amount = Get_Decimal_Value();

    slip.back_pay_amount = amount;
}

void Get_Display_Employee_Additional_Earnings_Cash_Gift_Value(Pre_Calculation_Payslip& slip) {
    Display_Employee_Additional_Earnings_Cash_Gift_UI();
    // Get decimal input (validated)
    double value = Get_Decimal_Value();
    // Cash Gifts ALWAYS require secondary authentication
    Secondary_Authentication("Cash Gift requires secondary authentication");
    // Store in slip
    slip.cash_gift_amount = value;
}

void Get_Display_Employee_Additional_Earnings_Commission_Value(Pre_Calculation_Payslip& slip) {
    Display_Employee_Additional_Earnings_Commission_UI();
    // --- Manager Name Input ---
    std::string manager;
    while (true) {
        std::getline(std::cin, manager);
        manager = Trim_Copy(manager);

        if (manager.empty()) {
            std::cout << Display_Error << Invalid_Empty_Input << std::endl;
            continue;
        }

        break;
    }

    slip.commission_manager_name = manager;
    // --- Commission Amount Input ---
    double amount = Get_Decimal_Value();
    slip.commission_amount = amount;
}

void Get_Display_Employee_Additional_Earnings_On_Call_Shift_Value(Pre_Calculation_Payslip& slip) {
    Display_Employee_Additional_Earnings_On_Call_Shift_UI();
    // Get decimal input (validated)
    double value = Get_Decimal_Value();
    // Store in slip
    slip.on_call_shift_amount = value;
}

void Handle_Sick_Leave(Pre_Calculation_Payslip& slip) {
    Display_Sick_Leave_Prompt_UI(slip.sick_leave_remaining);
    // Get sick leave HOURS
    int sick_hours = Get_Integer_Value();
    double sick_days = static_cast<double>(sick_hours) / 8.0;
    // Store sick leave days taken
    slip.sick_leave_days = sick_days;
    // If within availability → nothing else to do
    if (sick_days <= slip.sick_leave_remaining)
        return;
    // Excess sick leave → ask about annual leave replacement
    Display_Sick_Leave_Annual_Leave_Question_UI();

    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    bool use_annual_leave = Is_Yes_Input(input);

    double excess_days = sick_days - slip.sick_leave_remaining;

    if (use_annual_leave)
    {
        Display_Sick_Leave_Annual_Leave_Replacement_Yes_Answer_UI();
        // Ask how many annual leave HOURS to use
        Display_Annual_Leave_Prompt_UI();

        int annual_hours = Get_Integer_Value();
        double annual_days = static_cast<double>(annual_hours) / 8.0;

        slip.annual_leave_days += annual_days;
        // If annual leave still not enough → remainder becomes unpaid leave
        if (annual_days < excess_days)
        {
            double unpaid_days = excess_days - annual_days;
            slip.unpaid_leave_days += unpaid_days;
        }

        return;
    }
    // If NOT approved to use annual leave
    const std::string& IRD = slip.IRD_Number;

    if (Is_Salary_Employee(IRD))
    {
        Display_Sick_Leave_Annual_Leave_Replacement_No_Salary_Contracted_Hours_Employee_Answer_UI();
        // Salaried employees lose hours
        slip.hours_worked -= (excess_days * 8.0);
    }
    else if (Is_Hourly_Employee(IRD))
    {
        Display_Sick_Leave_Annual_Leave_Replacement_No_Hourly_Employee_Answer_UI();
        // Hourly employees: no edits required
    }
    // Remainder becomes unpaid leave
    slip.unpaid_leave_days += excess_days;
}

void Handle_Annual_Leave(Pre_Calculation_Payslip& slip) {
    Display_Annual_Leave_Prompt_Question_UI();
    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    bool took_annual_leave = Is_Yes_Input(input);

    if (!took_annual_leave)
        return; // No annual leave taken, nothing else to do
    // Show the annual leave input UI
    Display_Annual_Leave_Prompt_UI();
    // User enters HOURS (e.g., 8 = 1 day, 4 = half day)
    int annual_hours = Get_Integer_Value();
    double annual_days = static_cast<double>(annual_hours) / 8.0;
    // Store the annual leave days taken
    slip.annual_leave_days = annual_days;
    // If within availability → done
    if (annual_days <= slip.annual_leave_remaining)
        return;
    // If annual leave exceeds availability → convert excess to unpaid leave
    double excess_days = annual_days - slip.annual_leave_remaining;

    slip.unpaid_leave_days += excess_days;
}

void Handle_Parental_Leave(Pre_Calculation_Payslip& slip)
{
    // 1. Ask if any approved parental leave was taken this period
    Display_Parent_Leave_Question_UI();

    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    bool took_parental_leave = Is_Yes_Input(input);

    if (!took_parental_leave)
        return; // Nothing to do

    // 2. Ask how many HOURS of parental leave were taken
    Display_Parent_Leave_Prompt_UI(slip.parental_leave_remaining);

    int parent_leave_hours = Get_Integer_Value();
    double parent_leave_days = static_cast<double>(parent_leave_hours) / 8.0;

    // Store parental leave days taken
    slip.parent_leave_days = parent_leave_days;

    // 3. If within entitlement → nothing else to do
    if (parent_leave_days <= slip.parental_leave_remaining)
        return;

    // 4. Excess parental leave → becomes unpaid leave
    double excess_days = parent_leave_days - slip.parental_leave_remaining;

    std::cout << Display_Warning << "Parent Leave exceeds entitlement." << std::endl;

    const std::string& IRD = slip.IRD_Number;

    // Salary employees lose hours (same logic as Sick Leave)
    if (Is_Salary_Employee(IRD))
    {
        slip.hours_worked -= (excess_days * 8.0);
    }
    else if (Is_Hourly_Employee(IRD))
    {
        // Hourly employees: no hours adjustment required
    }

    // Remainder becomes unpaid leave
    slip.unpaid_leave_days += excess_days;
}

void Handle_Unpaid_Leave(Pre_Calculation_Payslip& slip) {
    Display_Unpaid_Leave_Prompt_Question_UI();
    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    bool took_unpaid_leave = Is_Yes_Input(input);

    if (!took_unpaid_leave)
        return; // No unpaid leave taken
    // Ask for unpaid leave HOURS
    std::cout << "Enter the total number of UNPAID leave hours taken:" << std::endl;
    std::cout << Total_Days_In_Hour_Format_Label;

    int unpaid_hours = Get_Integer_Value();
    double unpaid_days = static_cast<double>(unpaid_hours) / 8.0;

    // Add to existing unpaid leave days (Sick/Annual may have added some)
    slip.unpaid_leave_days += unpaid_days;
}

void Handle_Public_Holidays(Pre_Calculation_Payslip& slip) {
    Display_Public_Holidays_Unworked_Question_Prompt_UI();

    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    bool had_public_holiday = Is_Yes_Input(input);

    if (!had_public_holiday)
    {
        slip.public_holidays_unworked = false;
        slip.public_holidays_worked = false;
        slip.public_holiday_hours_worked = 0.0;
        return;
    }

    // There WAS at least one public holiday
    slip.public_holidays_unworked = true;
    // Ask if any public holidays were WORKED
    Display_Public_Holidays_Worked_Question_Prompt_UI();
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    bool worked_public_holiday = Is_Yes_Input(input);

    if (!worked_public_holiday)
    {
        slip.public_holidays_worked = false;
        slip.public_holiday_hours_worked = 0.0;
        return;
    }
    // Public holiday WAS worked
    slip.public_holidays_worked = true;
    // Ask for hours worked
    std::cout << "Enter the number of hours worked on the Public Holiday:" << std::endl;
    std::cout << Total_Days_In_Hour_Format_Label;

    int hours = Get_Integer_Value();
    slip.public_holiday_hours_worked = static_cast<double>(hours);
}

void Handle_PD_Contribution(Pre_Calculation_Payslip& slip) {
    Display_Professional_Development_Contribution_Prompt_UI();
    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    bool used_pd = Is_Yes_Input(input);

    if (!used_pd)
    {
        slip.pd_amount = 0.0;
        slip.pd_details = "";
        return;
    }
    // Ask for PD details
    Display_Professional_Development_Contribution_Details_Question_UI();
    std::string details;
    std::getline(std::cin, details);
    details = Trim_Copy(details);

    slip.pd_details = details;
    // Ask for PD amount
    Display_Professional_Development_Contribution_Amount_Question_UI();
    double amount = Get_Decimal_Value();
    slip.pd_amount = amount;
    if (amount > slip.pdc_remaining)
    {
        bool approved = Secondary_Authentication("Professional Development Contribution Exceeds Remaining Balance");

        if (!approved)
        {
            // Revert PD entry
            slip.pd_amount = 0.0;
            slip.pd_details = "";
            return;
        }
    }
    // Deduct from remaining balance (can go negative if approved)
    slip.pdc_remaining -= amount;
}

void Handle_EAP_Contribution(Pre_Calculation_Payslip& slip) {
    Display_Employee_Assistance_Programme_Contribution_Prompt_UI();
    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    bool used_eap = Is_Yes_Input(input);

    if (!used_eap)
    {
        slip.eap_amount = 0.0;
        slip.eap_details = "";
        return;
    }
    // Ask for EAP details
    Display_Employee_Assistance_Programme_Contribution_Details_Question_UI();
    std::string details;
    std::getline(std::cin, details);
    details = Trim_Copy(details);

    slip.eap_details = details;
    // Ask for EAP amount
    Display_Employee_Assistance_Programme_Contribution_Amount_Question_UI();
    double amount = Get_Decimal_Value();
    slip.eap_amount = amount;
    if (amount > slip.eap_remaining)
    {
        bool approved = Secondary_Authentication("EAP Contribution Exceeds Remaining Balance");

        if (!approved)
        {
            // Revert EAP entry
            slip.eap_amount = 0.0;
            slip.eap_details = "";
            return;
        }
    }
    // Deduct from remaining balance (can go negative if approved)
    slip.eap_remaining -= amount;
}

bool Handle_Payslip_Confirmation(const Pre_Calculation_Payslip& slip) {
    Display_Payslip_Confirmation_before_Generation_UI(slip);

    std::string input;
    std::getline(std::cin, input);
    input = Trim_Copy(input);

    bool confirm = Is_Yes_Input(input);

    if (!confirm)
    {
        Display_Options_for_No_to_Payslip_Generation_Confirmation_UI();
        return false;
    }

    Display_Generation_Progress_Info_UI();
    return true;
}


void Generate_Payslip(const std::string& username, Pre_Calculation_Payslip& slip) {
    if (!Calculate_Payslip(slip)) {
        return;
    }

    if (!Handle_Payslip_Confirmation(slip)) {
        return;
    }

    if (!Save_Payslip(username, slip)) {
        std::cout << Display_Error << Payslip_Save_Failure << std::endl;
        return;
    }

    Display_Selected_User_Payslip_Generated_Return_to_Main_HR_Menu_UI(
        Payslip_Generated_Message);
}
