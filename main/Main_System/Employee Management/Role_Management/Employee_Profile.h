// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef EMPLOYEE_PROFILE_H
#define EMPLOYEE_PROFILE_H

#include <string>

struct Employee_Profile {
    std::string first_name;
    std::string middle_name;
    std::string last_name;
    std::string employee_name;
    std::string username;
    std::string IRD_Number;
    std::string business_role;
    std::string job_role;
    std::string pay_type;
    std::string employment_type;
    std::string pay_frequency;
    double contracted_weekly_hours = 0.0;
    double contracted_hourly_rate = 0.0;
    double annual_salary = 0.0;
    double monthly_salary = 0.0;
    double fortnightly_salary = 0.0;
    double weekly_salary = 0.0;
    double overtime_rate_time_and_half = 0.0;
    double overtime_rate_double_time = 0.0;
    double overtime_max_hours = 0.0;
    double sick_leave_hours_remaining = 0.0;
    double annual_leave_hours_remaining = 0.0;
    double alternative_holidays_remaining = 0.0;
    double parental_leave_weeks_remaining = 0.0;
    double special_leave_days_remaining = 0.0;
    double sick_leave_hours_lawful = 0.0;
    double sick_leave_hours_package_extra = 0.0;
    double annual_leave_hours_lawful = 0.0;
    double annual_leave_hours_package_extra = 0.0;
    double alternative_holidays_lawful = 0.0;
    double alternative_holidays_package_extra = 0.0;
    double parental_leave_weeks_lawful = 0.0;
    double parental_leave_weeks_package_extra = 0.0;
    double special_leave_days_lawful = 0.0;
    double special_leave_days_package_extra = 0.0;
    double pd_balance_remaining = 0.0;
    double eap_balance_remaining = 0.0;
    std::string salary_package_name;
    std::string salary_package_display_name;
    double kiwisaver_employer_rate = 0.0;
    double superannuation_employer = 0.0;
    double health_insurance_employer = 0.0;
    double pd_contribution_yearly = 0.0;
    double eap_contribution_yearly = 0.0;
    double kiwisaver_employee_rate = 0.0;
    double health_insurance_employee = 0.0;
    double superannuation_employee = 0.0;
    std::string primary_tax_code;
    std::string secondary_tax_code;
    std::string special_tax_code;
    double paye_rate = 0.0;
    double acc_levy_rate = 0.0;
    double acc_maximum_threshold = 0.0;
    bool student_loan_active = false;
    double student_loan_rate = 0.0;
    double student_loan_sdr_rate = 0.0;
    bool student_loan_excemption = false;
    bool child_support_active = false;
    double child_support_standard = 0.0;
    double child_support_voluntary = 0.0;
    bool kiwisaver_opted_out = false;
    std::string kiwisaver_opt_out_date;
    bool esct_active = false;
    double esct_rate = 0.0;
    std::string esct_income_band;
    bool is_trade_employee = false;
    bool requires_secondary_auth = false;
};

#endif // EMPLOYEE_PROFILE_H
