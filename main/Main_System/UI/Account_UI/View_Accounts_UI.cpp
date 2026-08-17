// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "View_Accounts_UI.h"
#include "../Shared_UI_Messaging/Form_Labels_UI.h"

#include <iostream>

namespace {
std::string Get_Employee_Detail_Value(const std::map<std::string, std::string>& employee_details,
                                      const std::string& label) {
	const auto it = employee_details.find(label);
	if (it == employee_details.end() || it->second.empty()) {
		return "Not set";
	}
	return it->second;
}

void Display_Employee_Details_Block_UI(const std::map<std::string, std::string>& employee_details) {
	std::cout << Emergency_Contact_Label << ": "
		<< Get_Employee_Detail_Value(employee_details, Emergency_Contact_Label) << '\n';
	std::cout << Diagnosed_Health_Conditions_Label << ": "
		<< Get_Employee_Detail_Value(employee_details, Diagnosed_Health_Conditions_Label) << '\n';
	std::cout << Allergies_Label << ": "
		<< Get_Employee_Detail_Value(employee_details, Allergies_Label) << '\n';
	std::cout << Approved_Emergency_Steps_and_Medicines_Label << ": "
		<< Get_Employee_Detail_Value(employee_details, Approved_Emergency_Steps_and_Medicines_Label) << '\n';
	std::cout << Prior_Training_and_Qualifications_Label << ": "
		<< Get_Employee_Detail_Value(employee_details, Prior_Training_and_Qualifications_Label) << '\n';
	std::cout << First_Aid_Certification_Label << ": "
		<< Get_Employee_Detail_Value(employee_details, First_Aid_Certification_Label) << '\n';
}
}

void Display_Dynamic_Username_List_UI(const std::vector<std::string>& usernames, const std::string& business_role_type_heading) {
	if (!business_role_type_heading.empty()) {
		std::cout << business_role_type_heading << '\n';
	}

	for (std::size_t i = 0; i < usernames.size(); ++i) {
		std::cout << (i + 1) << ". " << usernames[i] << '\n';
	}
}

void Display_Details_Menu_Options_UI() {
	std::cout << "Selected Account Actions:" << std::endl;
	std::cout << "1. Edit Account Details" << std::endl;
	std::cout << "2. Delete Account" << std::endl;
	std::cout << "3. " << Return_to_Account_Management_Main_Menu_Label << std::endl;
	std::cout << "Please select an action (1-3): ";
}

void Display_Current_User_Account_UI(const std::string& username) {
	std::cout << Current_User_Account_Label << '\n';
	std::cout << Username_Label << ": " << username << '\n';
}

void Display_Selected_User_Account_UI(const std::string& username) {
	std::cout << Selected_User_Account_Label << '\n';
	std::cout << Username_Label << ": " << username << '\n';
}

void Display_Account_Selection_Input_Prompt_UI(std::size_t number_of_accounts) {
	std::cout << Please_Select_an_Option_Label << " (1-" << number_of_accounts << "): ";
}

void Display_Current_Users_Employee_Details_UI(const std::string& username,
		const std::map<std::string, std::string>& employee_details) {
	std::cout << "Current User Employee Details:" << '\n';
	std::cout << Username_Label << ": " << username << '\n';
	Display_Employee_Details_Block_UI(employee_details);
}

void Display_Selected_Users_Employee_Details_UI(const std::string& username, const std::map<std::string, std::string>& employee_details) {
	std::cout << "Selected User Employee Details:" << '\n';
	std::cout << Username_Label << ": " << username << '\n';
	Display_Employee_Details_Block_UI(employee_details);
}
