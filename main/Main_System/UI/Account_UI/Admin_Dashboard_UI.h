// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef ADMIN_DASHBOARD_UI_H
#define ADMIN_DASHBOARD_UI_H

#include <string>
#include <vector>

void Display_FULL_Admin_Dashboard_Main_Menu_UI();
void Display_Admin_Account_Management_Question_UI();
void Display_Admin_Employee_Management_Question_UI();
void Display_Admin_HR_Management_Question_UI();
void Display_Personal_Account_Management_Menu_UI();
void Display_Admin_All_Accounts_Management_Menu_UI();
void Display_View_Accounts_Menu_UI();
void Display_View_Accounts_UI();
void Display_Personal_HR_Management_Menu_UI();
void Display_Admin_All_Employees_HR_Management_Menu_UI(bool payroll_access);
void Display_Assistant_Delegated_HR_Management_Menu_UI(
	const std::vector<std::string>& manager_descriptions);
void Display_Assigned_Team_Members_UI(
	const std::string& heading,
	const std::vector<std::string>& team_members);
void Display_Personal_Employee_Management_Menu_UI();
void Display_Admin_Employee_Management_Menu_UI();
void Display_Partial_Admin_Dashboard_Main_Menu_UI();
void Display_Personal_Main_Management_Menu_UI2();
void Display_Personal_Main_Management_Menu_UI1();
void Display_Account_Management_Only_Admin_Dashboard_Main_Menu_UI();
void Display_Employee_Management_Only_Admin_Dashboard_Main_Menu_UI();
void Display_HR_Management_Only_Admin_Dashboard_Main_Menu_UI();

#endif // ADMIN_DASHBOARD_UI_H
