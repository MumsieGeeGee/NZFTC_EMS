// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef ROLE_AND_SALARY_DETAILS_H
#define ROLE_AND_SALARY_DETAILS_H

#include <string>

// Role Types (Admin, Finance, Trade)
enum class Role_Type {
    Admin,
    Finance,
    Trade
};
enum class Admin_Role {
    Junior_General_Administrator, 
    Senior_General_Administrator,
    Junior_Trainee_Administrator,
    Senior_Trainee_Administrator,
    Junior_Casual_Administrator,
    Senior_Casual_Administrator,
    Receptionist,
    Call_Centre_Operator,
    Executive_Assistant,
    Personal_Assistant,
    Human_Resources_Administrator,
    Junior_IT_Support_Administrator,
    Senior_IT_Support_Administrator,
    Junior_Level_Manager,
    Senior_Level_Manager
};
enum class Finance_Role {
    Junior_General_Finance_Staff,
    Senior_General_Finance_Staff,
    Junior_Trainee_Finance_Staff,
    Senior_Trainee_Finance_Staff,
    Junior_Casual_Finance_Staff,
    Senior_Casual_Finance_Staff,
    Payroll_Administrator,
    Accounts_Payable_Administrator,
    Accounts_Receivable_Administrator,
    Junior_Finance_Manager,
    Senior_Finance_Manager
};
enum class Trade_Role {
    Junior_General_Trade_Staff,
    Senior_General_Trade_Staff,
    Junior_Trainee_Trade_Staff,
    Senior_Trainee_Trade_Staff,
    Junior_Casual_Trade_Staff,
    Senior_Casual_Trade_Staff,
    Junior_Apprentice, 
    Senior_Mentor,
    Safety_Officer,
    Sales_Representative,
    Junior_Trade_Manager,
    Senior_Trade_Manager
};

// Role Salary Package
enum class Role_Salary {
    Approximate_Annual_Salary,
    KiwiSaver_Employer_Contribution,
    Annual_Leave_Entitlement,
    Sick_Leave_Entitlement,
    Public_Holidays_Entitlement,
    Parental_Leave_Entitlement,
    Special_Leave_Entitlement,
    Superannuation_Contribution,
    Health_Insurance_Contribution,
    Professional_Development_Contribution,
    Employee_Assistance_Program_Contribution
};

// Parental Leave Type
enum class Parental_Leave_Type {
    None,
    Maternity_Leave,
    Paternity_Leave, 
    Adoptive_Parent_Leave,
    Shared_Parental_Leave
};

// Pay Type (Hourly, Salaried, Casual)
enum class Pay_Type {
    Hourly,
    Salary,
    Casual
};
// Pay Rate (Hourly Rate, Contracted Hourly Rate, Overtime Rate, Annual Salary, Monthly Salary, Fortnightly Salary, Weekly Salary)
enum class Pay_Rate {
    Hourly_Rate,
    Contracted_Hourly_Rate,
    Overtime_Rate,
    Annual_Salary,
    Monthly_Salary,
    Fortnightly_Salary,
    Weekly_Salary
};
// Employment Type (Full-time, Part-time, Fixed-term, Single-Occasion, Casual)
enum class Employment_Type {
    Full_Time,
    Part_Time,
    Fixed_Term,
    Single_Occasion,
    Casual
};
// Pay Frequency (Weekly, Fortnightly, Monthly, Annually)
enum class Pay_Frequency {
    Weekly,
    Fortnightly,
    Monthly,
    Annually
};
// Schedule Type (Fixed, Flexible)
enum class Schedule_Type {
    Fixed,
    Flexible
};

// Function Declarations
void Display_Employee_Health_Insurance_Contribution_UI(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role);
void Display_Employee_Professional_Development_Contribution_UI(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role);
void Display_Employee_Employee_Assistance_Program_Contribution_UI(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role);
void Get_Parental_Leave_Entitlement(Parental_Leave_Type& parental_leave_type);
void Get_Pay_Type(Pay_Type& pay_type);
void Get_Employment_Type(Employment_Type& employment_type);
void Get_Pay_Frequency_Type(Pay_Frequency& pay_frequency_type);
void Get_Contracted_Hours(int& contracted_hours);
void Get_Schedule_Type(Schedule_Type& schedule_type);
void Get_Overtime_Rate(int& overtime_rate);
void Get_Overtime_Maximum_Hours(int& overtime_maximum_hours);

#endif // ROLE_AND_SALARY_DETAILS_H

