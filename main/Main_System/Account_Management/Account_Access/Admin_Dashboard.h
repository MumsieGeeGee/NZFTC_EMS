// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef ADMIN_DASHBOARD_H
#define ADMIN_DASHBOARD_H

#include <string>
#include "Employee Management/Role_Management/Role_and_Salary_Details.h"

struct Pre_Calculation_Payslip;

// Account Management
void Get_Personal_Account_Management_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice);
void Get_Admin_All_Accounts_Management_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice, std::string& first_name);

// HR Management
void Get_Personal_HR_Management_Menu_Choice(Admin_Role admin_role, int& choice);
void Get_Admin_All_Employees_HR_Management_Menu_Choice(const std::string& employee_name, Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice, Pre_Calculation_Payslip& slip);

// Employee Management
void Get_Personal_Employee_Management_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice);
void Get_Admin_Employee_Management_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice);

// Dashboard Routing
void Get_FULL_Admin_Dashboard_Main_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice);
void Get_Partial_Admin_Dashboard_Main_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice);
void Get_Account_Management_Only_Admin_Dashboard_Main_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice);
void Get_Employee_Management_Only_Admin_Dashboard_Main_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice);
void Get_HR_Management_Only_Admin_Dashboard_Main_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice);

#endif // ADMIN_DASHBOARD_H
