// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Edit_Accounts.h"
#include "Create_Account.h"
#include "View_Accounts.h"
#include "../Account_Access/Admin_Dashboard.h"
#include "../Account_Access/Account_Database.h"
#include "../Account_Management/Input_Validation_Helpers.h"
#include "../../NZFTC_EMS/Session_Handling/Session_Handling.h"
#include "../../UI/Account_UI/Admin_Dashboard_UI.h"
#include "../../UI/Account_UI/Edit_Accounts_UI.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/Info_Messages.h"
#include "../../UI/Shared_UI_Messaging/Success_Messages.h"
#include <iostream>
#include <map>
#include <string>

namespace {
void Display_Account_Update_Result_Error(Account_Update_Result update_result) {
    if (update_result == Account_Update_Result::User_Not_Found) {
        std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
	    std::cout << Display_Error << Username_Invalid << std::endl;
        return;
    }
    if (update_result == Account_Update_Result::Invalid_Input) {
        std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
        return;
    }
    if (update_result == Account_Update_Result::Missing_IRD_Mapping) {
        std::cout << Display_Error << IRD_Tax_Information_Not_Found << std::endl;
        return;
    }
    if (update_result == Account_Update_Result::Write_Failure ||
        update_result == Account_Update_Result::Index_Update_Failure) {
        std::cout << Display_Error << Account_Update_Failure << std::endl;
        return;
    }

    std::cout << Display_Error << Account_Update_Failure << std::endl;
}

bool Apply_Field_Update(const std::string& username,
						const std::string& field_label,
						const std::string& field_value) {
	Account_Update_Result update_result = Account_Update_Result::Success;
	if (!Update_Account_Record_Field_For_Username(username, field_label, field_value, &update_result)) {
		Display_Account_Update_Result_Error(update_result);
		return false;
	}

	std::cout << Display_Success << Account_Update_Success_Message << std::endl;
	return true;
}

bool Apply_Password_Update(const std::string& username) {
	std::string password;
	Get_Password(password);
	Account_Update_Result update_result = Account_Update_Result::Success;
	if (!Update_Account_Password_For_Username(username, password, &update_result)) {
		Display_Account_Update_Result_Error(update_result);
		return false;
	}

	std::cout << Display_Success << Account_Update_Success_Message << std::endl;
	return true;
}

bool Apply_Role_And_Username_Update(std::string& username,
									const std::string& business_role,
									const std::string& job_role) {
	std::string assignment_field;
	std::string assigned_username;
	if (!Get_Required_Role_Assignment(
			business_role,
			job_role,
			assignment_field,
			assigned_username,
			username)) {
		return false;
	}

	std::string updated_username;
	Account_Update_Result update_result = Account_Update_Result::Success;
	if (!Update_Account_Role_For_Username(username, business_role, job_role, &updated_username, &update_result)) {
		Display_Account_Update_Result_Error(update_result);
		return false;
	}

	if (updated_username != username) {
		User_Session_Context session_context = Get_Current_User_Session_Context();
		if (session_context.authenticated && session_context.username == username) {
			session_context.username = updated_username;
			Set_Current_User_Session_Context(session_context);
		}

		username = updated_username;
	}

	for (const std::string& relationship_field :
			{"Main Manager", "Assigned Manager", "Assigned Mentor"}) {
		const std::string relationship_value =
			relationship_field == assignment_field ? assigned_username : "";
		Account_Update_Result relationship_update_result =
			Account_Update_Result::Success;
		if (!Update_Account_Record_Field_For_Username(
				username,
				relationship_field,
				relationship_value,
				&relationship_update_result)) {
			Display_Account_Update_Result_Error(relationship_update_result);
			return false;
		}
	}

	std::cout << Display_Success << Account_Update_Success_Message << std::endl;
	return true;
}

bool Prompt_Business_And_Job_Role(std::string& business_role, std::string& job_role) {
	int business_role_selection = 0;
	if (!Get_Business_Role_Selection(business_role_selection)) {
		return false;
	}

	int job_role_selection = 0;
	if (!Get_Job_Role_Selection(business_role_selection, job_role_selection)) {
		return false;
	}

	const std::string selected_business = Business_Role_To_String(business_role_selection);
	const std::string selected_job = Job_Role_To_String(business_role_selection, job_role_selection);
	if (selected_business.empty() || selected_job.empty()) {
		return false;
	}

	business_role = selected_business;
	job_role = selected_job;
	return true;
}

bool Prompt_Job_Role_For_Current_Business(
		const std::string& username,
		std::string& business_role,
		std::string& job_role) {
	std::map<std::string, std::string> account_details;
	if (!Get_Account_Record_Details_For_Username(username, &account_details)) {
		return false;
	}

	business_role = Trim_Copy(account_details["Business Role"]);
	int business_role_selection = 0;
	if (business_role == "Admin") {
		business_role_selection = 1;
	} else if (business_role == "Finance") {
		business_role_selection = 2;
	} else if (business_role == "Trade") {
		business_role_selection = 3;
	} else {
		return false;
	}

	int job_role_selection = 0;
	if (!Get_Job_Role_Selection(business_role_selection, job_role_selection)) {
		return false;
	}

	const std::string selected_job = Job_Role_To_String(business_role_selection, job_role_selection);
	if (selected_job.empty()) {
		return false;
	}

	job_role = selected_job;
	return true;
}
}

void Get_Edit_Accounts_Menu_Options() {
	while (true) {
		int choice = 0;
		if (!Get_Validated_Menu_Choice(1, 3, &choice)) {
			continue;
		}

		if (choice == 1) {
			const User_Session_Context& session = Get_Current_User_Session_Context();
			if (!session.authenticated || Trim_Copy(session.username).empty()) {
				std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
				std::cout << Display_Error << Username_Invalid << std::endl;
				continue;
			}

			Edit_Current_User_Account_Details(session.username);
			continue;
		}

		if (choice == 2) {
			Display_View_Accounts_Menu_UI();
			Get_View_All_Accounts_Menu_Options();
			continue;
		}

		return;
	}
}

bool Edit_Current_User_Account_Details(const std::string& username) {
	std::map<std::string, std::string> account_details;
	if (!Get_Account_Record_Details_For_Username(username, &account_details)) {
		std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
		std::cout << Display_Error << Username_Invalid << std::endl;
		return false;
	}

	Display_Edit_Current_User_Account_Details_UI(account_details);

	int choice = 0;
	if (!Get_Validated_Menu_Choice(1, 4, &choice)) {
		return false;
	}

	if (choice == 1) {
		std::string address;
		Get_Full_Address(address);
		return Apply_Field_Update(username, "Address", address);
	}
	if (choice == 2) {
		std::string email_address;
		Get_Email_Address(email_address);
		return Apply_Field_Update(username, "Email Address", email_address);
	}
	if (choice == 3) {
		std::string phone_number;
		Get_Phone_Number(phone_number);
		return Apply_Field_Update(username, "Phone Number", phone_number);
	}

	return Apply_Password_Update(username);
}

bool Edit_Selected_User_Account_Details(std::string& username) {
	std::map<std::string, std::string> account_details;
	if (!Get_Account_Record_Details_For_Username(username, &account_details)) {
		std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
		std::cout << Display_Error << Username_Invalid << std::endl;
		return false;
	}

	Display_Edit_Selected_User_Account_Details_UI(account_details);
	std::cout << Display_Info << Auto_Update_Username_Message << std::endl;
	Display_Selected_User_Edit_Options_Input_Prompt_UI();

	bool repeat_input_prompt = false;
	while (true) {
		if (repeat_input_prompt) {
			Display_Selected_User_Edit_Options_Input_Prompt_UI();
		}
		repeat_input_prompt = true;

		int choice = 0;
		if (!Get_Validated_Menu_Choice(1, 12, &choice)) {
			continue;
		}

		if (choice == 12) {
			return true;
		}

		if (choice == 1) {
			Account_Type account_type = Account_Type::User;
			Get_Account_Type(account_type);
			Apply_Field_Update(username, "Account Type", Account_Type_To_String(account_type));
			continue;
		}
		if (choice == 2) {
			std::string first_name;
			Get_First_Name(first_name);
			Apply_Field_Update(username, "First Name", first_name);
			continue;
		}
		if (choice == 3) {
			std::string middle_name;
			Get_Middle_Name(middle_name);
			Apply_Field_Update(username, "Middle Name", middle_name);
			continue;
		}
		if (choice == 4) {
			std::string last_name;
			Get_Surname(last_name);
			Apply_Field_Update(username, "Last Name", last_name);
			continue;
		}
		if (choice == 5) {
			std::string date_of_birth;
			Get_Date_of_Birth(date_of_birth);
			Apply_Field_Update(username, "Date of Birth", date_of_birth);
			continue;
		}
		if (choice == 6) {
			std::string address;
			Get_Full_Address(address);
			Apply_Field_Update(username, "Address", address);
			continue;
		}
		if (choice == 7) {
			std::string email_address;
			Get_Email_Address(email_address);
			Apply_Field_Update(username, "Email Address", email_address);
			continue;
		}
		if (choice == 8) {
			std::string phone_number;
			Get_Phone_Number(phone_number);
			Apply_Field_Update(username, "Phone Number", phone_number);
			continue;
		}
		if (choice == 9) {
			std::string business_role;
			std::string job_role;
			if (!Prompt_Business_And_Job_Role(business_role, job_role)) {
				std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
				continue;
			}
			Apply_Role_And_Username_Update(username, business_role, job_role);
			continue;
		}
		if (choice == 10) {
			std::string business_role;
			std::string job_role;

			if (!Prompt_Job_Role_For_Current_Business(
					username, business_role, job_role)) {
				std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
				continue;
			}

			Apply_Role_And_Username_Update(username, business_role, job_role);
			continue;
		}
		if (choice == 11) {
			Apply_Password_Update(username);
		}
	}
}
