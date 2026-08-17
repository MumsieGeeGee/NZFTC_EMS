// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "HR_Management_UI.h"
#include "HR_Management.h"
#include "../../Account_Management/Account_Access/Account_Database.h"
#include "../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../../NZFTC_EMS/Session_Handling/Session_Handling.h"
#include "../../UI/Account_UI/Admin_Dashboard_UI.h"
#include "../../UI/Account_UI/View_Accounts_UI.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Info_Messages.h"
#include "../../Path_Utilities.h"
#include "Payslips/Payslip_Generation_UI.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

namespace {
std::filesystem::path Payslip_Directory() {
	return Path_Utilities::Get_Payslip_Directory();
}

std::string Sanitize_File_Component(std::string value) {
	for (char& ch : value) {
		switch (ch) {
			case '<':
			case '>':
			case ':':
			case '"':
			case '/':
			case '\\':
			case '|':
			case '?':
			case '*':
				ch = '_';
				break;
			default:
				break;
		}
	}
	return value;
}

void Display_Payslip_File(const std::filesystem::path& file_path);
bool Get_Selected_Payslip_Username(std::string& username_out);

std::vector<std::filesystem::path> List_Payslip_Files_For_Username(const std::string& username) {
	std::vector<std::filesystem::path> files;
	const std::filesystem::path directory = Payslip_Directory();
	if (!std::filesystem::exists(directory)) {
		return files;
	}

	const std::string prefix = Sanitize_File_Component(Trim_Copy(username));
	for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		if (!entry.is_regular_file() || entry.path().extension() != ".txt") {
			continue;
		}

		const std::string file_name = entry.path().filename().string();
		if (file_name.rfind(prefix + "_", 0) == 0 || file_name == prefix + ".txt") {
			files.push_back(entry.path());
		}
	}

	std::sort(files.begin(), files.end(), [](const auto& left, const auto& right) {
		return left.filename().string() > right.filename().string();
	});
	return files;
}

void Display_Payslip_List_For_Username(const std::string& username, const std::string& heading) {
	const std::vector<std::filesystem::path> payslips =
		List_Payslip_Files_For_Username(username);
	if (payslips.empty()) {
		std::cout << Display_Info << "No payslips have been generated yet." << std::endl;
		return;
	}

	while (true) {
		std::cout << heading << std::endl;
		for (std::size_t index = 0; index < payslips.size(); ++index) {
			std::cout << (index + 1) << ". "
			          << payslips[index].filename().string() << std::endl;
		}
		std::cout << (payslips.size() + 1) << ". Return to HR Main Menu" << std::endl;
		std::cout << Please_Select_an_Option_Label << " (1-" << (payslips.size() + 1) << "): ";

		int choice = 0;
		if (!Get_Validated_Menu_Choice(1, static_cast<int>(payslips.size() + 1), &choice)) {
			continue;
		}

		if (choice == static_cast<int>(payslips.size()) + 1) {
			return;
		}

		Display_Payslip_File(payslips[static_cast<std::size_t>(choice - 1)]);
	}
}

void Display_Payslip_File(const std::filesystem::path& file_path) {
	std::ifstream input(file_path);
	if (!input.is_open()) {
		std::cout << Display_Error << "Unable to open payslip file." << std::endl;
		return;
	}

	std::cout << "Payslip File: " << file_path.filename().string() << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::string line;
	while (std::getline(input, line)) {
		std::cout << line << std::endl;
	}
}

bool Get_Selected_Payslip_Username(std::string& username_out) {
	const User_Session_Context& session = Get_Current_User_Session_Context();
	if (!session.authenticated || Trim_Copy(session.username).empty()) {
		std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
		return false;
	}

	std::map<std::string, std::string> account_details;
	if (!Get_Account_Record_Details_For_Username(session.username, &account_details)) {
		std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
		return false;
	}

	const std::string job_role = Trim_Copy(account_details["Job Role"]);
	const bool assistant_access =
		job_role == Admin_Executive_Assistant ||
		job_role == Admin_Personal_Assistant;

	if (assistant_access) {
		const std::vector<Role_Assignment_Option> delegated_managers =
			Get_Managers_Assistant_Can_Act_On_Behalf_Of(session.username);
		if (delegated_managers.empty()) {
			std::cout << Display_Error << No_Accounts_Found << std::endl;
			return false;
		}
		if (delegated_managers.size() == 1) {
			username_out = delegated_managers.front().username;
			return true;
		}

		std::vector<std::string> manager_labels;
		for (const Role_Assignment_Option& manager : delegated_managers) {
			manager_labels.push_back(
				manager.display_name + " - " + manager.job_role +
				" (" + manager.business_role + ")");
		}
		Display_Dynamic_Username_List_UI(manager_labels, "Senior Managers:");
		Display_Account_Selection_Input_Prompt_UI(manager_labels.size());

		int manager_choice = 0;
		if (!Get_Validated_Menu_Choice(
				1,
				static_cast<int>(delegated_managers.size()),
				&manager_choice)) {
			return false;
		}
		username_out = delegated_managers[
			static_cast<std::size_t>(manager_choice - 1)].username;
		return true;
	}

	Display_View_Accounts_Menu_UI();

	int business_role_choice = 0;
	if (!Get_Validated_Menu_Choice(1, 4, &business_role_choice)) {
		return false;
	}

	if (business_role_choice == 4) {
		return false;
	}

	std::string username_prefix;
	std::string role_heading;
	if (business_role_choice == 1) {
		username_prefix = "admin_";
		role_heading = Admin_Accounts_Label;
	} else if (business_role_choice == 2) {
		username_prefix = "finance_";
		role_heading = Finance_Accounts_Label;
	} else {
		username_prefix = "trade_";
		role_heading = Trade_Accounts_Label;
	}

	const std::vector<std::string> usernames = Get_Usernames_By_Prefix(username_prefix);
	if (usernames.empty()) {
		std::cout << Display_Error << No_Accounts_Found << std::endl;
		return false;
	}

	Display_Dynamic_Username_List_UI(usernames, role_heading);
	Display_Account_Selection_Input_Prompt_UI(usernames.size());

	int username_choice = 0;
	if (!Get_Validated_Menu_Choice(1, static_cast<int>(usernames.size()), &username_choice)) {
		return false;
	}

	username_out = usernames[static_cast<std::size_t>(username_choice - 1)];
	return true;
}

void Display_Leave_and_Holiday_Entitlements_Body(const Leave_and_Holiday_Entitlements_Details& details) {
	std::cout << Username_Label << ": " << details.username << '\n';
	std::cout << "Business Department : " << details.business_role << '\n';
	std::cout << "Job Role: " << details.job_role << '\n';
	std::cout << Annual_Leave_Label << ": " << details.annual_leave_entitlement << " days total." << '\n';
	std::cout << Sick_Leave_Label << ": " << details.sick_leave_entitlement << " days total." << '\n';
	std::cout << Public_Holidays_Label << ": " << details.public_holidays_entitlement << " days total." << '\n';
	std::cout << Parental_Leave_Label << ": Default 26 weeks + " << details.parental_leave_entitlement << " weeks extra." << '\n';
	std::cout << Special_Leave_Label << ": " << details.special_leave_entitlement << " days total." << '\n';
	std::cout << Public_Holidays_Label << " Dates:" << '\n';
	std::cout << "New Years Day: " << details.new_years_day << '\n';
	std::cout << "Day after New Years Day: " << details.day_after_new_years_day << '\n';
	std::cout << "Waitangi Day: " << details.waitangi_day << '\n';
	std::cout << "Good Friday: " << details.good_friday << '\n';
	std::cout << "Easter Monday: " << details.easter_monday << '\n';
	std::cout << "ANZAC Day: " << details.anzac_day << '\n';
	std::cout << "Queens/Kings Birthday: " << details.queens_kings_birthday << '\n';
	std::cout << "Labour Day: " << details.labour_day << '\n';
	std::cout << "Christmas Day: " << details.christmas_day << '\n';
	std::cout << "Boxing Day: " << details.boxing_day << '\n';
}

void Display_Tax_Information_Record_Body(const Tax_Information_Record_Details& details) {
	std::cout << Username_Label << ": " << details.username << '\n';
	std::cout << IRD_Number_Label << ": " << details.ird_number << '\n';
	std::cout << details.file_contents << '\n';
}
}

void Display_Generate_Payslip_Form_UI(const std::string& employee_name) {
	Payslip_Form_Welcome_Header_UI(employee_name);
	Display_Deductions_ALL_Employee_UI();
}

void Display_Current_User_Payslips_UI() {
	const User_Session_Context& session = Get_Current_User_Session_Context();
	if (!session.authenticated || Trim_Copy(session.username).empty()) {
		std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
		return;
	}

	Display_Payslip_List_For_Username(session.username, "Current Payslips:");
}

void Display_Selected_User_Payslips_UI() {
	std::string username;
	if (!Get_Selected_Payslip_Username(username)) {
		return;
	}

	Display_Payslip_List_For_Username(username, "Selected User Payslips:");
}

void Display_Request_Payslip_Review_UI() {} // tbc

void Personal_Payslip_Review_Request_UI() {} // tbc

void On_Behalf_of_User_Payslip_Review_Request_UI() {} // tbc

void Display_Current_User_Leave_and_Holiday_Entitlements_UI(const Leave_and_Holiday_Entitlements_Details& details) {
	std::cout << "Your Leave and Holiday Entitlements:" << '\n';
	Display_Leave_and_Holiday_Entitlements_Body(details);
}

void Display_Selected_User_Leave_and_Holiday_Entitlements_UI(const Leave_and_Holiday_Entitlements_Details& details) {
	std::cout << "Leave and Holiday Entitlements:" << '\n';
	Display_Leave_and_Holiday_Entitlements_Body(details);
}

void Display_View_Current_User_Open_Leave_Requests_UI() {} // tbc

void Display_View_Selected_User_Open_Leave_Requests_UI() {} // tbc

void Display_View_Current_User_Resolved_Leave_Requests_UI() {} // tbc

void Display_Submit_Leave_Request_UI() {} // tbc

void Personal_Submit_Leave_Request_UI() {} // tbc

void On_Behalf_of_User_Submit_Leave_Request_UI(){} // tbc

void Display_View_Current_User_Open_Grievance_Report_UI() {} // tbc

void Display_View_Selected_User_Open_Grievance_Report_UI() {} // tbc

void Display_View_Current_User_Resolved_Grievance_Report_UI() {} // tbc

void Display_View_Selected_User_Resolved_Grievance_Report_UI() {} // tbc

void Display_Submit_Grievance_Report_UI() {} // tbc

void Personal_Submit_Grievance_Report_UI() {} // tbc

void On_Behalf_of_User_Submit_Grievance_Report_UI() {} // tbc

void Display_View_Current_User_Tax_Information_UI(const Tax_Information_Record_Details& details) {
	std::cout << "Your Tax Information:" << '\n';
	Display_Tax_Information_Record_Body(details);
}

void Display_Selected_User_Tax_Information_UI(const Tax_Information_Record_Details& details) {
	std::cout << "Selected User Tax Information:" << '\n';
	Display_Tax_Information_Record_Body(details);
}

void Display_Edit_Current_User_Tax_Information_UI(const Tax_Information_Record_Details& details) {
	std::cout << "Your Tax Information:" << '\n';
	Display_Tax_Information_Record_Body(details);
}

void Display_Edit_Selected_User_Tax_Information_UI(const Tax_Information_Record_Details& details) {
	std::cout << "Selected User Tax Information:" << '\n';
	Display_Tax_Information_Record_Body(details);
}
