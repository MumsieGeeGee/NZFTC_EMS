// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

// Payroll calculator orchestration.
// Deductions are applied in this order:
// 1. PAYE, ACC, Student Loan
// 2. Child Support
// 3. KiwiSaver
// 4. Other optional deductions

#include "Calculator.h"

#include "../../Account_Management/Account_Access/Account_Database.h"
#include "../../Account_Management/Account_Management/Database.h"
#include "../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../../Employee Management/Role_Management/Department_Roles/Admin/Admin_Salary_Packages.h"
#include "../../Employee Management/Role_Management/Department_Roles/Finance/Finance_Salary_Packages.h"
#include "../../Employee Management/Role_Management/Department_Roles/Trade/Trade_Salary_Packages.h"
#include "../../Employee Management/Role_Management/Role_and_Salary_Details.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/Info_Messages.h"
#include "../../UI/Shared_UI_Messaging/Warning_Messages.h"

#include <algorithm>
#include <array>
#include <exception>
#include <cmath>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <iostream>
#include <utility>

namespace {
struct Role_Entitlements {
    double annual_leave = 0.0;
    double sick_leave = 0.0;
    double public_holidays = 0.0;
    double parental_leave = 0.0;
    double special_leave = 0.0;
    double pdc = 0.0;
    double eap = 0.0;
};

double Clamp_Non_Negative(double value) {
    return value < 0.0 ? 0.0 : value;
}

double Round_To_Cents(double value) {
    return std::round(value * 100.0) / 100.0;
}

std::string Normalize_Number_Text(std::string value) {
    value = Trim_Copy(value);
    value.erase(std::remove(value.begin(), value.end(), ','), value.end());
    value.erase(std::remove(value.begin(), value.end(), '$'), value.end());
    value.erase(std::remove(value.begin(), value.end(), '%'), value.end());
    return Trim_Copy(value);
}

bool Try_Parse_Double(const std::string& raw, double* out) {
    if (out == nullptr) {
        return false;
    }

    const std::string normalized = Normalize_Number_Text(raw);
    if (normalized.empty()) {
        return false;
    }

    std::size_t consumed = 0;
    try {
        const double value = std::stod(normalized, &consumed);
        if (consumed != normalized.length()) {
            return false;
        }
        *out = value;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool Try_Read_Double_Field(
    const std::string& ird_number,
    const std::string& label,
    double* out,
    double default_value,
    bool required)
{
    if (out == nullptr) {
        return false;
    }

    std::string raw;
    if (!Read_Record_Field_Value(ird_number, label, raw)) {
        if (required) {
            return false;
        }
        *out = default_value;
        return true;
    }

    if (!Try_Parse_Double(raw, out)) {
        if (required) {
            return false;
        }
        *out = default_value;
        return true;
    }

    return true;
}

bool Try_Read_Percentage_Field(
    const std::string& ird_number,
    const std::string& label,
    double* out,
    double default_value,
    bool required)
{
    if (!Try_Read_Double_Field(ird_number, label, out, default_value, required)) {
        return false;
    }

    if (*out > 1.0) {
        *out /= 100.0;
    }
    return true;
}

double Periods_Per_Year(const std::string& pay_frequency) {
    const std::string value = Trim_Copy(pay_frequency);
    if (value == "Weekly") {
        return 52.0;
    }
    if (value == "Fortnightly") {
        return 26.0;
    }
    if (value == "Monthly") {
        return 12.0;
    }
    return 52.0;
}

double Base_Hourly_Rate_For_Payslip(const std::string& ird_number, const std::string& pay_type) {
    double rate = 0.0;
    if (Try_Read_Double_Field(ird_number, Contracted_Hourly_Rate_Label, &rate, 0.0, false) && rate > 0.0) {
        return rate;
    }

    double contracted_hours = 0.0;
    if (!Get_Contracted_Hours(ird_number, contracted_hours) || contracted_hours <= 0.0) {
        contracted_hours = 40.0;
    }

    double annual_salary = 0.0;
    if (Try_Read_Double_Field(ird_number, Annual_Salary_Label, &annual_salary, 0.0, false) && annual_salary > 0.0) {
        return annual_salary / 52.0 / contracted_hours;
    }

    double monthly_salary = 0.0;
    if (Try_Read_Double_Field(ird_number, Monthly_Salary_Label, &monthly_salary, 0.0, false) && monthly_salary > 0.0) {
        return monthly_salary * 12.0 / 52.0 / contracted_hours;
    }

    double fortnightly_salary = 0.0;
    if (Try_Read_Double_Field(ird_number, Fortnightly_Salary_Label, &fortnightly_salary, 0.0, false) && fortnightly_salary > 0.0) {
        return fortnightly_salary / contracted_hours;
    }

    double weekly_salary = 0.0;
    if (Try_Read_Double_Field(ird_number, Weekly_Salary_Label, &weekly_salary, 0.0, false) && weekly_salary > 0.0) {
        return weekly_salary / contracted_hours;
    }

    if (Trim_Copy(pay_type) == Salary_Label) {
        return 0.0;
    }

    return rate;
}

double Period_Base_Pay(const std::string& ird_number, const std::string& pay_frequency) {
    double base_pay = 0.0;
    double annual_salary = 0.0;
    double weekly_salary = 0.0;
    double fortnightly_salary = 0.0;
    double monthly_salary = 0.0;

    const double periods = Periods_Per_Year(pay_frequency);

    if (Try_Read_Double_Field(ird_number, Annual_Salary_Label, &annual_salary, 0.0, false) && annual_salary > 0.0) {
        return annual_salary / periods;
    }
    if (Try_Read_Double_Field(ird_number, Weekly_Salary_Label, &weekly_salary, 0.0, false) && weekly_salary > 0.0) {
        return weekly_salary;
    }
    if (Try_Read_Double_Field(ird_number, Fortnightly_Salary_Label, &fortnightly_salary, 0.0, false) && fortnightly_salary > 0.0) {
        return periods == 52.0 ? fortnightly_salary / 2.0 : fortnightly_salary;
    }
    if (Try_Read_Double_Field(ird_number, Monthly_Salary_Label, &monthly_salary, 0.0, false) && monthly_salary > 0.0) {
        return periods == 12.0 ? monthly_salary : monthly_salary * periods / 12.0;
    }
    if (Try_Read_Double_Field(ird_number, Contracted_Hourly_Rate_Label, &base_pay, 0.0, false) && base_pay > 0.0) {
        double contracted_hours = 0.0;
        if (!Get_Contracted_Hours(ird_number, contracted_hours) || contracted_hours <= 0.0) {
            contracted_hours = 40.0;
        }
        return base_pay * contracted_hours;
    }

    return 0.0;
}

bool Resolve_Admin_Role(const std::string& job_role, Admin_Role* out_role) {
    if (out_role == nullptr) {
        return false;
    }

    static const std::pair<const char*, Admin_Role> roles[] = {
        {Junior_General_Administrator_Role_Label.c_str(), Admin_Role::Junior_General_Administrator},
        {Senior_General_Administrator_Role_Label.c_str(), Admin_Role::Senior_General_Administrator},
        {Junior_Trainee_Administrator_Role_Label.c_str(), Admin_Role::Junior_Trainee_Administrator},
        {Senior_Trainee_Administrator_Role_Label.c_str(), Admin_Role::Senior_Trainee_Administrator},
        {Junior_Casual_Administrator_Role_Label.c_str(), Admin_Role::Junior_Casual_Administrator},
        {Senior_Casual_Administrator_Role_Label.c_str(), Admin_Role::Senior_Casual_Administrator},
        {Admin_Receptionist.c_str(), Admin_Role::Receptionist},
        {Admin_Call_Centre_Operator.c_str(), Admin_Role::Call_Centre_Operator},
        {Admin_Executive_Assistant.c_str(), Admin_Role::Executive_Assistant},
        {Admin_Personal_Assistant.c_str(), Admin_Role::Personal_Assistant},
        {Admin_Human_Resources.c_str(), Admin_Role::Human_Resources_Administrator},
        {Junior_IT_Support_Administrator_Role_Label.c_str(), Admin_Role::Junior_IT_Support_Administrator},
        {Senior_IT_Support_Administrator_Role_Label.c_str(), Admin_Role::Senior_IT_Support_Administrator},
        {Admin_Junior_Manager.c_str(), Admin_Role::Junior_Level_Manager},
        {Admin_Senior_Manager.c_str(), Admin_Role::Senior_Level_Manager},
    };

    const std::string trimmed_role = Trim_Copy(job_role);
    for (const auto& role : roles) {
        if (trimmed_role == role.first) {
            *out_role = role.second;
            return true;
        }
    }
    return false;
}

bool Resolve_Finance_Role(const std::string& job_role, Finance_Role* out_role) {
    if (out_role == nullptr) {
        return false;
    }

    static const std::pair<const char*, Finance_Role> roles[] = {
        {Junior_General_Finance_Staff_Member.c_str(), Finance_Role::Junior_General_Finance_Staff},
        {Senior_General_Finance_Staff_Member.c_str(), Finance_Role::Senior_General_Finance_Staff},
        {Junior_Trainee_Finance_Staff_Member.c_str(), Finance_Role::Junior_Trainee_Finance_Staff},
        {Senior_Trainee_Finance_Staff_Member.c_str(), Finance_Role::Senior_Trainee_Finance_Staff},
        {Junior_Casual_Finance_Staff_Member.c_str(), Finance_Role::Junior_Casual_Finance_Staff},
        {Senior_Casual_Finance_Staff_Member.c_str(), Finance_Role::Senior_Casual_Finance_Staff},
        {Payroll_Administrator.c_str(), Finance_Role::Payroll_Administrator},
        {Accounts_Payable_Administrator.c_str(), Finance_Role::Accounts_Payable_Administrator},
        {Accounts_Receivable_Administrator.c_str(), Finance_Role::Accounts_Receivable_Administrator},
        {Junior_Finance_Manager.c_str(), Finance_Role::Junior_Finance_Manager},
        {Senior_Finance_Manager.c_str(), Finance_Role::Senior_Finance_Manager},
    };

    const std::string trimmed_role = Trim_Copy(job_role);
    for (const auto& role : roles) {
        if (trimmed_role == role.first) {
            *out_role = role.second;
            return true;
        }
    }
    return false;
}

bool Resolve_Trade_Role(const std::string& job_role, Trade_Role* out_role) {
    if (out_role == nullptr) {
        return false;
    }

    static const std::pair<const char*, Trade_Role> roles[] = {
        {Junior_General_Trade_Staff_Member.c_str(), Trade_Role::Junior_General_Trade_Staff},
        {Senior_General_Trade_Staff_Member.c_str(), Trade_Role::Senior_General_Trade_Staff},
        {Junior_Trainee_Trade_Staff_Member.c_str(), Trade_Role::Junior_Trainee_Trade_Staff},
        {Senior_Trainee_Trade_Staff_Member.c_str(), Trade_Role::Senior_Trainee_Trade_Staff},
        {Junior_Casual_Trade_Staff_Member.c_str(), Trade_Role::Junior_Casual_Trade_Staff},
        {Senior_Casual_Trade_Staff_Member.c_str(), Trade_Role::Senior_Casual_Trade_Staff},
        {Junior_Apprentice.c_str(), Trade_Role::Junior_Apprentice},
        {Senior_Mentor.c_str(), Trade_Role::Senior_Mentor},
        {Safety_Officer.c_str(), Trade_Role::Safety_Officer},
        {Sales_Representative.c_str(), Trade_Role::Sales_Representative},
        {Junior_Trade_Manager.c_str(), Trade_Role::Junior_Trade_Manager},
        {Senior_Trade_Manager.c_str(), Trade_Role::Senior_Trade_Manager},
    };

    const std::string trimmed_role = Trim_Copy(job_role);
    for (const auto& role : roles) {
        if (trimmed_role == role.first) {
            *out_role = role.second;
            return true;
        }
    }
    return false;
}

bool Load_Role_Entitlements(
    const std::string& username,
    Role_Entitlements* entitlements_out)
{
    if (entitlements_out == nullptr) {
        return false;
    }

    std::map<std::string, std::string> account_details;
    if (!Get_Account_Record_Details_For_Username(username, &account_details)) {
        return false;
    }

    const std::string business_role = Trim_Copy(account_details["Business Role"]);
    const std::string job_role = Trim_Copy(account_details["Job Role"]);
    if (business_role.empty() || job_role.empty()) {
        return false;
    }

    if (business_role == Role_Type_Label_Admin) {
        Admin_Role role{};
        if (!Resolve_Admin_Role(job_role, &role)) {
            return false;
        }
        Get_Admin_Salary_Package(role);
    } else if (business_role == Role_Type_Label_Finance) {
        Finance_Role role{};
        if (!Resolve_Finance_Role(job_role, &role)) {
            return false;
        }
        Get_Finance_Salary_Package(role);
    } else if (business_role == Role_Type_Label_Trade) {
        Trade_Role role{};
        if (!Resolve_Trade_Role(job_role, &role)) {
            return false;
        }
        Get_Trade_Salary_Package(role);
    } else {
        return false;
    }

    entitlements_out->annual_leave = static_cast<double>(Annual_Leave_Entitlement);
    entitlements_out->sick_leave = static_cast<double>(Sick_Leave_Entitlement);
    entitlements_out->public_holidays = static_cast<double>(Public_Holidays_Entitlement);
    entitlements_out->parental_leave = static_cast<double>(Parental_Leave_Entitlement);
    entitlements_out->special_leave = static_cast<double>(Special_Leave_Entitlement);
    entitlements_out->pdc = static_cast<double>(Professional_Development_Contribution);
    entitlements_out->eap = static_cast<double>(Employee_Assistance_Program_Contribution);
    return true;
}

double Parse_Taxable_Annual_Amount(double taxable_earnings, const std::string& pay_frequency) {
    const double periods = Periods_Per_Year(pay_frequency);
    return taxable_earnings * periods;
}

double Calculate_Annual_Progressive_Tax(double annual_income) {
    struct Bracket {
        double ceiling;
        double rate;
    };

    static const std::array<Bracket, 5> brackets = {{
        {15600.0, 0.105},
        {53500.0, 0.175},
        {78100.0, 0.30},
        {180000.0, 0.33},
        {std::numeric_limits<double>::infinity(), 0.39},
    }};

    const double floors[] = {0.0, 15600.0, 53500.0, 78100.0, 180000.0};
    double remaining = annual_income;
    double total_tax = 0.0;

    for (std::size_t index = 0; index < brackets.size(); ++index) {
        const double bracket_floor = floors[index];
        const double bracket_ceiling = brackets[index].ceiling;
        const double taxable_portion = std::max(0.0, std::min(annual_income, bracket_ceiling) - bracket_floor);
        total_tax += taxable_portion * brackets[index].rate;
    }

    (void)remaining;
    return Round_To_Cents(total_tax);
}

double Calculate_PAYE(
    const std::string& ird_number,
    const std::string& primary_tax_code,
    const std::string& secondary_tax_code,
    const std::string& special_tax_code,
    double taxable_earnings,
    const std::string& pay_frequency)
{
    const std::string special = Trim_Copy(special_tax_code);
    if (!special.empty() && special != "None" && special != "none" && special != "NONE") {
        double special_rate = 0.0;
        if (Try_Read_Percentage_Field(ird_number, STC_Rate_Label, &special_rate, 0.0, true)) {
            return Round_To_Cents(taxable_earnings * special_rate);
        }
        return 0.0;
    }

    const std::string secondary = Trim_Copy(secondary_tax_code);
    if (!secondary.empty()) {
        double flat_rate = 0.175;
        if (secondary == "SB" || secondary == "sb") {
            flat_rate = 0.105;
        } else if (secondary == "S" || secondary == "s") {
            flat_rate = 0.175;
        } else if (secondary == "SH" || secondary == "sh") {
            flat_rate = 0.30;
        } else if (secondary == "ST" || secondary == "st") {
            flat_rate = 0.33;
        } else if (secondary == "SA" || secondary == "sa") {
            flat_rate = 0.39;
        }
        return Round_To_Cents(taxable_earnings * flat_rate);
    }

    const double annual_income = Parse_Taxable_Annual_Amount(taxable_earnings, pay_frequency);
    const double annual_tax = Calculate_Annual_Progressive_Tax(annual_income);
    return Round_To_Cents(annual_tax / Periods_Per_Year(pay_frequency));
}

double Calculate_ACC(const std::string& ird_number, double gross_earnings) {
    double rate = 0.0167;
    Try_Read_Percentage_Field(ird_number, ACC_Earners_Levy_Rate_Label, &rate, 0.0167, false);

    double threshold = 0.0;
    Try_Read_Double_Field(ird_number, ACC_Levy_Maximum_Earnings_Threshold_Label, &threshold, 0.0, false);
    if (threshold > 0.0) {
        return Round_To_Cents(std::min(gross_earnings, threshold) * rate);
    }
    return Round_To_Cents(gross_earnings * rate);
}

double Calculate_Student_Loan(
    const std::string& ird_number,
    const std::string& primary_tax_code,
    double taxable_earnings)
{
    if (!Is_Student_Loan_Active(ird_number)) {
        return 0.0;
    }

    const std::string primary = Trim_Copy(primary_tax_code);
    if (primary == "ME" || primary == "me") {
        return 0.0;
    }

    std::string exemption_status;
    if (Read_Record_Field_Value(ird_number, Student_Loan_Repayment_Exemption_Existence_Label, exemption_status)) {
        const std::string trimmed = Trim_Copy(exemption_status);
        if (trimmed == "Yes" || trimmed == "yes" || trimmed == "YES") {
            return 0.0;
        }
    }

    double threshold = 0.0;
    Try_Read_Double_Field(ird_number, Student_Loan_Repayment_Threshold_Label, &threshold, 0.0, false);

    double rate = 0.12;
    Try_Read_Percentage_Field(ird_number, Student_Loan_Repayment_Rate_Label, &rate, 0.12, false);

    if (taxable_earnings <= threshold) {
        return 0.0;
    }
    return Round_To_Cents((taxable_earnings - threshold) * rate);
}

double Calculate_Child_Support(const std::string& ird_number) {
    if (!Is_Child_Support_Active(ird_number)) {
        return 0.0;
    }

    double nod_amount = 0.0;
    std::string nod_exists;
    if (Read_Record_Field_Value(ird_number, Child_Support_IRD_Notice_of_Deduction_Existence_Label, nod_exists)) {
        const std::string trimmed = Trim_Copy(nod_exists);
        if (trimmed == "Yes" || trimmed == "yes" || trimmed == "YES") {
            if (Try_Read_Double_Field(ird_number, Child_Support_IRD_Notice_of_Deduction_Amount_Label, &nod_amount, 0.0, false) && nod_amount > 0.0) {
                return Round_To_Cents(nod_amount);
            }
        }
    }

    double standard = 0.0;
    double voluntary = 0.0;
    Try_Read_Double_Field(ird_number, Child_Support_Standard_Deduction_Label, &standard, 0.0, false);
    Try_Read_Double_Field(ird_number, Child_Support_Voluntary_Deduction_Label, &voluntary, 0.0, false);
    return Round_To_Cents(standard + voluntary);
}

double Calculate_KiwiSaver_Employee(const std::string& ird_number, double gross_earnings) {
    if (!Is_KiwiSaver_Active(ird_number) || Is_KiwiSaver_Opted_Out(ird_number)) {
        return 0.0;
    }

    double rate = 0.03;
    Try_Read_Percentage_Field(ird_number, KiwiSaver_Employee_Contribution_Rate_Label, &rate, 0.03, false);
    return Round_To_Cents(gross_earnings * rate);
}

double Calculate_KiwiSaver_Employer(const std::string& ird_number, double gross_earnings) {
    if (!Is_KiwiSaver_Active(ird_number) || Is_KiwiSaver_Opted_Out(ird_number)) {
        return 0.0;
    }

    double rate = 0.03;
    Try_Read_Percentage_Field(ird_number, KiwiSaver_Employer_Contribution_Rate_Label, &rate, 0.03, false);
    return Round_To_Cents(gross_earnings * rate);
}

double Calculate_ESCT(const std::string& ird_number, double kiwisaver_employer) {
    if (!Is_ESCT_Active(ird_number) || kiwisaver_employer <= 0.0) {
        return 0.0;
    }

    double rate = 0.0;
    Try_Read_Percentage_Field(ird_number, ESCT_Tax_Rate_Label, &rate, 0.0, false);
    return Round_To_Cents(kiwisaver_employer * rate);
}

double Calculate_Optional_Deductions(const std::string& ird_number) {
    double voluntary_paye = 0.0;
    Try_Read_Double_Field(ird_number, PAYE_Extra_Voluntary_Deductions_Label, &voluntary_paye, 0.0, false);
    return Round_To_Cents(voluntary_paye);
}

void Apply_Remaining_Leave_Balances(Pre_Calculation_Payslip& slip) {
    slip.sick_leave_remaining = Clamp_Non_Negative(slip.sick_leave_remaining - slip.sick_leave_days);
    slip.annual_leave_remaining = Clamp_Non_Negative(slip.annual_leave_remaining - slip.annual_leave_days);
    slip.parental_leave_remaining = Clamp_Non_Negative(slip.parental_leave_remaining - slip.parent_leave_days);
}
}

bool Prepare_Payslip_Context(const std::string& username, Pre_Calculation_Payslip& slip) {
    const std::string normalized_username = Trim_Copy(username);
    if (normalized_username.empty()) {
        std::cout << Display_Error << Username_Empty << std::endl;
        return false;
    }

    const std::string ird_number = Get_IRD_Number_For_Username(normalized_username);
    if (Trim_Copy(ird_number).empty()) {
        std::cout << Display_Error << Employee_Tax_Information_Not_Found << std::endl;
        return false;
    }

    Role_Entitlements entitlements;
    if (!Load_Role_Entitlements(normalized_username, &entitlements)) {
        std::cout << Display_Error << Role_Type_Access_Failed << std::endl;
        return false;
    }

    slip.IRD_Number = ird_number;
    slip.sick_leave_remaining = entitlements.sick_leave;
    slip.annual_leave_remaining = entitlements.annual_leave;
    slip.parental_leave_remaining = entitlements.parental_leave;
    slip.pdc_remaining = entitlements.pdc;
    slip.eap_remaining = entitlements.eap;
    return true;
}

bool Calculate_Payslip(Pre_Calculation_Payslip& slip) {
    const std::string ird_number = Trim_Copy(slip.IRD_Number);
    if (ird_number.empty()) {
        std::cout << Display_Error << Employee_Tax_Information_Not_Found << std::endl;
        return false;
    }

    std::string pay_type;
    std::string pay_frequency;
    std::string primary_tax_code;
    std::string secondary_tax_code;
    std::string special_tax_code;

    if (!Read_Record_Field_Value(ird_number, Pay_Type_Label, pay_type) ||
        !Read_Record_Field_Value(ird_number, Pay_Frequency_Label, pay_frequency) ||
        !Read_Record_Field_Value(ird_number, Primary_Tax_Code_Label, primary_tax_code)) {
        std::cout << Display_Error << Employee_Tax_Information_Not_Found << std::endl;
        return false;
    }

    Read_Record_Field_Value(ird_number, Secondary_Tax_Code_Label, secondary_tax_code);
    Read_Record_Field_Value(ird_number, Special_Tax_Code_Label, special_tax_code);

    const std::string trimmed_special_tax_code = Trim_Copy(special_tax_code);
    if (!trimmed_special_tax_code.empty() &&
        trimmed_special_tax_code != "None" &&
        trimmed_special_tax_code != "none" &&
        trimmed_special_tax_code != "NONE") {
        std::string stc_rate;
        double validated_stc_rate = 0.0;
        if (!Read_Record_Field_Value(ird_number, STC_Rate_Label, stc_rate) ||
            !Try_Parse_Double(stc_rate, &validated_stc_rate) ||
            validated_stc_rate < 0.0) {
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            return false;
        }
    }

    const std::string trimmed_pay_type = Trim_Copy(pay_type);
    const std::string trimmed_pay_frequency = Trim_Copy(pay_frequency);
    if (trimmed_pay_type.empty() || trimmed_pay_frequency.empty()) {
        std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
        return false;
    }

    const bool salary_employee = trimmed_pay_type == Salary_Label;
    const double base_hourly_rate = Base_Hourly_Rate_For_Payslip(ird_number, trimmed_pay_type);
    const double period_base_pay = salary_employee ? Period_Base_Pay(ird_number, trimmed_pay_frequency) : 0.0;

    if (salary_employee) {
        if (period_base_pay <= 0.0) {
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            return false;
        }
    } else if (base_hourly_rate <= 0.0) {
        std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
        return false;
    }

    double gross_earnings = salary_employee ? period_base_pay : slip.hours_worked * base_hourly_rate;

    if (!salary_employee) {
        gross_earnings += (slip.sick_leave_days + slip.annual_leave_days + slip.parent_leave_days) * 8.0 * base_hourly_rate;
        if (slip.public_holidays_unworked) {
            gross_earnings += 8.0 * base_hourly_rate;
        }
    }

    if (salary_employee) {
        gross_earnings += slip.overtime_paid_hours * base_hourly_rate * 1.5;
        if (slip.public_holidays_worked) {
            gross_earnings += slip.public_holiday_hours_worked * base_hourly_rate * 0.5;
        }
        gross_earnings -= slip.unpaid_leave_days * 8.0 * base_hourly_rate;
    } else {
        gross_earnings += slip.overtime_paid_hours * base_hourly_rate * 0.5;
        if (slip.public_holidays_worked) {
            gross_earnings += slip.public_holiday_hours_worked * base_hourly_rate * 1.5;
        }
        gross_earnings -= slip.unpaid_leave_days * 8.0 * base_hourly_rate;
    }

    gross_earnings += slip.travel_allowance_total;
    gross_earnings += slip.meal_allowance_total;
    gross_earnings += slip.tool_allowance_total;
    gross_earnings += slip.clothing_allowance_total;
    gross_earnings += slip.back_pay_amount;
    gross_earnings += slip.cash_gift_amount;
    gross_earnings += slip.commission_amount;
    gross_earnings += slip.on_call_shift_amount;

    if (gross_earnings < 0.0) {
        std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
        return false;
    }

    gross_earnings = Round_To_Cents(gross_earnings);
    slip.gross_earnings = gross_earnings;
    slip.taxable_earnings = gross_earnings;

    slip.paye = Calculate_PAYE(
        ird_number,
        primary_tax_code,
        secondary_tax_code,
        special_tax_code,
        slip.taxable_earnings,
        trimmed_pay_frequency);
    slip.acc = Calculate_ACC(ird_number, slip.gross_earnings);
    slip.student_loan = Calculate_Student_Loan(ird_number, primary_tax_code, slip.taxable_earnings);
    slip.child_support = Calculate_Child_Support(ird_number);
    slip.kiwisaver_employee = Calculate_KiwiSaver_Employee(ird_number, slip.gross_earnings);
    slip.kiwisaver_employer = Calculate_KiwiSaver_Employer(ird_number, slip.gross_earnings);
    slip.esct = Calculate_ESCT(ird_number, slip.kiwisaver_employer);

    const double optional_deductions = Calculate_Optional_Deductions(ird_number);
    const double total_employee_deductions =
        slip.paye +
        slip.acc +
        slip.student_loan +
        slip.child_support +
        slip.kiwisaver_employee +
        optional_deductions +
        slip.pd_amount +
        slip.eap_amount;

    slip.net_pay = Round_To_Cents(gross_earnings - total_employee_deductions);
    if (slip.net_pay < 0.0) {
        slip.net_pay = 0.0;
    }

    Apply_Remaining_Leave_Balances(slip);
    slip.requires_secondary_auth = false;
    return true;
}
