// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "HR_Management.h"
#include "HR_Management_UI.h"
#include "../../Account_Management/Account_Access/Account_Database.h"
#include "../../Account_Management/Account_Management/Database.h"
#include "../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../../Account_Management/Account_Management/Save_Functions.h"
#include "../../Account_Management/Account_Management/Security_Protocols.h"
#include "../Role_Management/Department_Roles/Admin/Admin_Salary_Packages.h"
#include "../Role_Management/Department_Roles/Finance/Finance_Salary_Packages.h"
#include "../Role_Management/Department_Roles/Trade/Trade_Salary_Packages.h"
#include "../Role_Management/Leave_and_Holiday.h"
#include "../../Tax Calculator/Calculator/Other_Menus.h"
#include "../../Tax Calculator/Components/ACC.h"
#include "../../Tax Calculator/Components/IRD_PAYE.h"
#include "../../NZFTC_EMS/Session_Handling/Session_Handling.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include <iostream>
#include <map>

int Business_Role_To_Selection(const std::string& business_role);
int Job_Role_To_Selection(int business_role_selection, const std::string& job_role);

namespace {
bool Load_Leave_and_Holiday_Entitlements_For_Role(const std::string& business_role,
		const std::string& job_role,
		Leave_and_Holiday_Entitlements_Details* details_out) {
	if (details_out == nullptr) {
		return false;
	}

	const int business_role_selection = Business_Role_To_Selection(business_role);
	if (business_role_selection == 0) {
		return false;
	}

	const int job_role_selection = Job_Role_To_Selection(business_role_selection, job_role);
	if (job_role_selection == 0) {
		return false;
	}

	if (business_role_selection == 1) {
		Get_Admin_Salary_Package(static_cast<Admin_Role>(job_role_selection - 1));
	} else if (business_role_selection == 2) {
		Get_Finance_Salary_Package(static_cast<Finance_Role>(job_role_selection - 1));
	} else if (business_role_selection == 3) {
		Get_Trade_Salary_Package(static_cast<Trade_Role>(job_role_selection - 1));
	} else {
		return false;
	}

	details_out->annual_leave_entitlement = Annual_Leave_Entitlement;
	details_out->sick_leave_entitlement = Sick_Leave_Entitlement;
	details_out->public_holidays_entitlement = Public_Holidays_Entitlement;
	details_out->parental_leave_entitlement = Parental_Leave_Entitlement;
	details_out->special_leave_entitlement = Special_Leave_Entitlement;

	Get_Current_Years_NZ_Public_Holidays();
	details_out->new_years_day = Get_New_Years_Day();
	details_out->day_after_new_years_day = Get_Day_After_New_Years_Day();
	details_out->waitangi_day = Get_Waitangi_Day();
	details_out->good_friday = Get_Good_Friday();
	details_out->easter_monday = Get_Easter_Monday();
	details_out->anzac_day = Get_ANZAC_Day();
	details_out->queens_kings_birthday = Get_Queens_Kings_Birthday();
	details_out->labour_day = Get_Labour_Day();
	details_out->christmas_day = Get_Christmas_Day();
	details_out->boxing_day = Get_Boxing_Day();
	return true;
}

bool Get_Leave_and_Holiday_Entitlements_For_Username(const std::string& username,
		Leave_and_Holiday_Entitlements_Details* details_out) {
	if (details_out == nullptr) {
		return false;
	}

	std::map<std::string, std::string> account_details;
	if (!Get_Account_Record_Details_For_Username(username, &account_details)) {
		std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
		return false;
	}

	details_out->username = username;
	details_out->business_role = Trim_Copy(account_details["Business Role"]);
	details_out->job_role = Trim_Copy(account_details["Job Role"]);
	if (details_out->business_role.empty() || details_out->job_role.empty()) {
		std::cout << Display_Error << Role_Type_Access_Failed << std::endl;
		return false;
	}

	if (!Load_Leave_and_Holiday_Entitlements_For_Role(details_out->business_role,
			details_out->job_role,
			details_out)) {
		std::cout << Display_Error << Role_Type_Access_Failed << std::endl;
		return false;
	}

	return true;
}

bool Load_Tax_Information_Record_For_Username(const std::string& username,
		Tax_Information_Record_Details* details_out,
		bool* is_confirmed_out) {
	if (details_out == nullptr || is_confirmed_out == nullptr) {
		return false;
	}

	const std::string normalized_username = Trim_Copy(username);
	if (normalized_username.empty()) {
		std::cout << Display_Error << Username_Empty << std::endl;
		return false;
	}

	const std::string ird_number = Get_IRD_Number_For_Username(normalized_username);
	if (Trim_Copy(ird_number).empty() || !IRD_Record_Exists_In_Master_Folder(ird_number)) {
		std::cout << Display_Error << Employee_Tax_Information_Not_Found << std::endl;
		return false;
	}

	std::string confirmation_status;
	if (!Get_IRD_Record_Confirmation_Status(ird_number, confirmation_status)) {
		std::cout << Display_Error << IRD_Confirmation_Required_For_View << std::endl;
		return false;
	}

	details_out->username = normalized_username;
	details_out->ird_number = ird_number;
	*is_confirmed_out = (confirmation_status == Confirmation_Status_to_String(confirmation_Status::Confirmed));

	if (!Get_IRD_Record_File_Contents(ird_number, details_out->file_contents)) {
		std::cout << Display_Error << Employee_Tax_Information_Not_Found << std::endl;
		return false;
	}

	return true;
}

bool Confirm_Selected_User_IRD_Record_For_Edit(Tax_Information_Record_Details* details_out) {
	if (details_out == nullptr) {
		return false;
	}

	if (!Confirm_Unconfirmed_IRD_For_Tax_Record(details_out->ird_number)) {
		std::cout << Display_Error << IRD_Confirmation_Required_For_View << std::endl;
		return false;
	}

	const User_Session_Context& current_session = Get_Current_User_Session_Context();
	std::string secondary_username;
	if (!Secondary_Authentication("Confirm IRD status", &secondary_username)) {
		std::cout << Display_Error << IRD_Tax_Information_Update_Failure << std::endl;
		return false;
	}

	const std::string audit_reason = "Confirmed IRD status, documentation sighted by: "
		+ current_session.username + " + " + secondary_username;
	(void)audit_reason;

	if (!Save_IRD_Confirmation_Status(details_out->ird_number, Confirmation_Status_to_String(confirmation_Status::Confirmed))) {
		std::cout << Display_Error << IRD_Tax_Information_Update_Failure << std::endl;
		return false;
	}

	if (!Auto_Apply_ACC_Levy_From_Employer_Details(details_out->ird_number)) {
		std::cout << Display_Error << Unable_to_Find_ACC_Levy_Type_in_Employer_Details << std::endl;
		return false;
	}

	if (!Get_IRD_Record_File_Contents(details_out->ird_number, details_out->file_contents)) {
		std::cout << Display_Error << Employee_Tax_Information_Not_Found << std::endl;
		return false;
	}

	return true;
}
}

void Get_Current_User_Leave_and_Holiday_Entitlements() {
	const User_Session_Context& session = Get_Current_User_Session_Context();
	if (!session.authenticated || Trim_Copy(session.username).empty()) {
		std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
		return;
	}

	Leave_and_Holiday_Entitlements_Details details;
	if (!Get_Leave_and_Holiday_Entitlements_For_Username(session.username, &details)) {
		std::cout << Display_Error << Employee_Record_Not_Found << std::endl;
		return;
	}

	Display_Current_User_Leave_and_Holiday_Entitlements_UI(details);
}

void Get_Selected_User_Leave_and_Holiday_Entitlements(const std::string& username) {
	const std::string selected_username = Trim_Copy(username);
	if (selected_username.empty()) {
		std::cout << Display_Error << Username_Empty << std::endl;
		return;
	}

	Leave_and_Holiday_Entitlements_Details details;
	if (!Get_Leave_and_Holiday_Entitlements_For_Username(selected_username, &details)) {
		return;
	}

	Display_Selected_User_Leave_and_Holiday_Entitlements_UI(details);
}

void Get_View_Current_User_Tax_Information() {
	const User_Session_Context& session = Get_Current_User_Session_Context();
	if (!session.authenticated || Trim_Copy(session.username).empty()) {
		std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
		return;
	}

	Tax_Information_Record_Details details;
	bool is_confirmed = false;
	if (!Load_Tax_Information_Record_For_Username(session.username, &details, &is_confirmed)) {
		return;
	}

	if (!is_confirmed) {
		std::cout << Display_Error << Payroll_Tax_Information_Not_Confirmed << std::endl;
		return;
	}

	Display_View_Current_User_Tax_Information_UI(details);
}

void Get_View_Selected_User_Tax_Information(const std::string& username) {
	Tax_Information_Record_Details details;
	bool is_confirmed = false;
	if (!Load_Tax_Information_Record_For_Username(username, &details, &is_confirmed)) {
		return;
	}

	if (!is_confirmed) {
		std::cout << Display_Error << Payroll_Tax_Information_Not_Confirmed << std::endl;
		return;
	}

	Display_Selected_User_Tax_Information_UI(details);
}

void Get_Edit_Current_User_Tax_Information() {
	const User_Session_Context& session = Get_Current_User_Session_Context();
	if (!session.authenticated || Trim_Copy(session.username).empty()) {
		std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
		return;
	}

	Tax_Information_Record_Details details;
	bool is_confirmed = false;
	if (!Load_Tax_Information_Record_For_Username(session.username, &details, &is_confirmed)) {
		return;
	}

	if (!is_confirmed) {
		std::cout << Display_Error << Payroll_Tax_Information_Not_Confirmed << std::endl;
		return;
	}

	Display_Edit_Current_User_Tax_Information_UI(details);
}

void Get_Edit_Selected_User_Tax_Information(const std::string& username) {
	Tax_Information_Record_Details details;
	bool is_confirmed = false;
	if (!Load_Tax_Information_Record_For_Username(username, &details, &is_confirmed)) {
		return;
	}

	if (!is_confirmed && !Confirm_Selected_User_IRD_Record_For_Edit(&details)) {
		return;
	}

	Display_Edit_Selected_User_Tax_Information_UI(details);
	Edit_Tax_Record_Fields_Menu_Choices(details.ird_number);
}
