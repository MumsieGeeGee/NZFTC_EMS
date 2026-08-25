// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Admin_Dashboard_UI.h"
#include "../Shared_UI_Messaging/Form_Labels_UI.h"

#include <iostream>

void Display_FULL_Admin_Dashboard_Main_Menu_UI() {
	std::cout << "Admin Dashboard Main Menu:" << std::endl;
	std::cout << "1. Account Management" << std::endl;
	std::cout << "2. Employee Management" << std::endl;
	std::cout << "3. HR Management" << std::endl;
	std::cout << "4. Manage Requests" << std::endl;
	std::cout << "5. Logout" << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-5): ";
}

void Display_Admin_Account_Management_Question_UI() {
	std::cout << "Do you wish to manage your own account or manage other accounts?" << std::endl;
	std::cout << "1. Manage My Account" << std::endl;
	std::cout << "2. Manage Other Accounts" << std::endl;
	std::cout << "3. " << Return_to_Admin_Dashboard_Main_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-3): ";
}

void Display_Admin_Employee_Management_Question_UI() {
	std::cout << "Do you wish to manage your own employee information or manage other employees information?" << std::endl;
	std::cout << "1. Manage My Employee Information" << std::endl;
	std::cout << "2. Manage Other Employees Information" << std::endl;
	std::cout << "3. " << Return_to_Admin_Dashboard_Main_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-3): ";
}

void Display_Admin_HR_Management_Question_UI() {
	std::cout << "Do you wish to manage your own HR information or manage other employees HR information?" << std::endl;
	std::cout << "1. Manage My HR Information" << std::endl;
	std::cout << "2. Manage Other Employees HR Information" << std::endl;
	std::cout << "3. " << Return_to_Admin_Dashboard_Main_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-3): ";
}

void Display_Admin_All_Accounts_Management_Menu_UI() {
	std::cout << "Admin Other Accounts Management Menu:" << std::endl;
	std::cout << "1. View Accounts" << std::endl;
	std::cout << "2. Create New Account" << std::endl;
	std::cout << "3. Edit Existing Account" << std::endl;
	std::cout << "4. Delete Account" << std::endl;
	std::cout << "5. " << Return_to_Admin_Dashboard_Main_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-5): ";
}

void Display_Admin_All_Employees_HR_Management_Menu_UI(bool payroll_access) {
	std::cout << "HR Management Menu:" << std::endl;
	int option = 1;
	if (payroll_access) {
		std::cout << option++ << ". Generate Payslip for User" << std::endl;
		std::cout << option++ << ". View Users Payslips" << std::endl;
		std::cout << option++ << ". Request a Payslip Review on behalf of User" << std::endl;
	}
	std::cout << option++ << ". View Users Leave and Holiday entitlements" << std::endl;
	std::cout << option++ << ". View Users Open Leave Requests" << std::endl;
	std::cout << option++ << ". View Users Resolved Leave Requests" << std::endl;
	std::cout << option++ << ". Request Leave on behalf of User" << std::endl;
	std::cout << option++ << ". View Users Open Grievance Reports" << std::endl;
	std::cout << option++ << ". View Users Resolved Grievance Reports" << std::endl;
	std::cout << option++ << ". Submit a Grievance Report on behalf of User" << std::endl;
	std::cout << option++ << ". View Users Tax Information" << std::endl;
	std::cout << option++ << ". Edit Users Tax Information" << std::endl;
	std::cout << option << ". " << Return_to_Admin_Dashboard_Main_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-" << option << "): ";
}

void Display_Assistant_Delegated_HR_Management_Menu_UI(
		const std::vector<std::string>& manager_descriptions) {
	std::cout << "Manager HR Support Menu:" << std::endl;
	std::cout << "Managers You Can Support:" << std::endl;
	for (const std::string& manager_description : manager_descriptions) {
		std::cout << "- " << manager_description << std::endl;
	}
	std::cout << "1. View Manager Leave and Holiday entitlements" << std::endl;
	std::cout << "2. View Manager Open Leave Requests" << std::endl;
	std::cout << "3. View Manager Resolved Leave Requests" << std::endl;
	std::cout << "4. Request Leave on behalf of Manager" << std::endl;
	std::cout << "5. " << Return_to_Admin_Dashboard_Main_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-5): ";
}

void Display_Assigned_Team_Members_UI(
		const std::string& heading,
		const std::vector<std::string>& team_members) {
	std::cout << heading << std::endl;
	for (std::size_t i = 0; i < team_members.size(); ++i) {
		std::cout << (i + 1) << ". " << team_members[i] << std::endl;
	}
}

void Display_Admin_Employee_Management_Menu_UI() {
	std::cout << "Employee Management Menu:" << std::endl;
	std::cout << "1. View an Employees Details" << std::endl;
	std::cout << "2. Edit an Existing Employees Details" << std::endl;
	std::cout << "3. " << Return_to_Admin_Dashboard_Main_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-3): ";
}

void Display_Partial_Admin_Dashboard_Main_Menu_UI() {
	std::cout << "Management Menu:" << std::endl;
	std::cout << "1. Employee Management" << std::endl;
	std::cout << "2. HR Management" << std::endl;
}

void Display_Personal_Account_Management_Menu_UI() {
	std::cout << "Personal Account Management Menu:" << std::endl;
	std::cout << "1. View My Account Details" << std::endl;
	std::cout << "2. Edit My Account Details" << std::endl;
	std::cout << "3. " << Return_to_Dashboard_Main_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-3): ";
}

void Display_Personal_HR_Management_Menu_UI() {
	std::cout << "Personal HR Management Menu:" << std::endl;
	std::cout << "1. View My Payslips" << std::endl;
    std::cout << "2. Request a Payslip Review" << std::endl;
	std::cout << "3. View My Leave and Holiday entitlements" << std::endl;
    std::cout << "4. View My Open Leave Requests" << std::endl;
    std::cout << "5. View My Temporarily Denied Leave Requests" << std::endl;
    std::cout << "6. View My Resolved Leave Requests" << std::endl;
    std::cout << "7. Request Leave" << std::endl;
    std::cout << "8. View My Open Grievance Reports" << std::endl;
    std::cout << "9. View My Resolved Grievance Reports" << std::endl;
    std::cout << "10. Submit a Grievance Report" << std::endl;
    std::cout << "11. View My Tax Information" << std::endl;
    std::cout << "12. Edit My Tax Information" << std::endl;
	std::cout << "13. " << Return_to_Dashboard_Main_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-13): ";
}

void Display_Personal_Employee_Management_Menu_UI() {
	std::cout << "Personal Employment Menu:" << std::endl;
	std::cout << "1. View My Employee Record" << std::endl;
	std::cout << "2. Edit My Employee Information" << std::endl;
	std::cout << "3. " << Return_to_Dashboard_Main_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-3): ";
}

void Display_Personal_Main_Management_Menu_UI2() {
	std::cout << "Personal Menu:" << std::endl;
	std::cout << "3. Manage my Account Details" << std::endl;
	std::cout << "4. Manage my Employee Information" << std::endl;
	std::cout << "5. Manage my HR Information" << std::endl;
	std::cout << "6. Manage Requests" << std::endl;
	std::cout << "7. Logout" << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-7): ";
}

void Display_Personal_Main_Management_Menu_UI1() {
	std::cout << "Personal Menu:" << std::endl;
	std::cout << "2. Manage my Account Details" << std::endl;
	std::cout << "3. Manage my Employee Information" << std::endl;
	std::cout << "4. Manage my HR Information" << std::endl;
	std::cout << "5. Manage Requests" << std::endl;
	std::cout << "6. Logout" << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-6): ";
}

void Display_Account_Management_Only_Admin_Dashboard_Main_Menu_UI() {
	std::cout << "Management Menu:" << std::endl;
	std::cout << "1. Account Management" << std::endl;
}

void Display_Employee_Management_Only_Admin_Dashboard_Main_Menu_UI() {
	std::cout << "Management Menu:" << std::endl;
	std::cout << "1. Employee Management" << std::endl;
}

void Display_HR_Management_Only_Admin_Dashboard_Main_Menu_UI() {
	std::cout << "Management Menu:" << std::endl;
	std::cout << "1. HR Management" << std::endl;
}

void Display_View_Accounts_Menu_UI() {
	std::cout << "View All Accounts:" << std::endl;
	std::cout << "1. View Admin Employee Accounts" << std::endl;
	std::cout << "2. View Finance Employee Accounts" << std::endl;
	std::cout << "3. View Trade Employee Accounts" << std::endl;
	std::cout << "4. " << Return_to_Previous_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-4): ";
}

void Display_View_Accounts_UI() {
	std::cout << "Employee Accounts:" << std::endl;
}
