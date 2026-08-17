// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Delete_Accounts.h"
#include "Create_Account.h"
#include "../Account_Access/Account_Database.h"
#include "../Account_Management/Security_Protocols.h"
#include "../Account_Management/Input_Validation_Helpers.h"
#include "../../UI/Account_UI/Delete_Accounts_UI.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"

#include "../../UI/Shared_UI_Messaging/Success_Messages.h"

#include <iostream>
#include <string>

namespace {
std::string deletion_username;
std::string deletion_ird_number;
bool deletion_confirmation = false;
bool initial_secondary_authentication = false;
bool final_secondary_authentication = false;
}

bool Delete_Confirmed_and_Authenticated_Account_and_IRD_Records() {
	if (!deletion_confirmation || !initial_secondary_authentication || !final_secondary_authentication) {
		std::cout << Display_Error << Secondary_Auth_Delete_Account_Cannot_Proceed << std::endl;
		return false;
	}

	if (!Delete_Account_And_IRD_Records_For_Username_and_IRD(deletion_username, deletion_ird_number)) {
		std::cout << Display_Error << Account_Deletion_Failure << std::endl;
		return false;
	}

	std::cout << Display_Success << Account_Deletion_Success_Message << std::endl;
	return true;
}

void Get_Username() {
	std::cout << "Username: ";
	std::getline(std::cin, deletion_username);
	deletion_username = Trim_Copy(deletion_username);
	if (!Username_Rules(deletion_username)) {
		deletion_username.clear();
	}
}

void Get_IRD_Number() {
	std::cout << "IRD Number: ";
	std::getline(std::cin, deletion_ird_number);
	deletion_ird_number = Trim_Copy(deletion_ird_number);
	if (!Is_IRD_Number_Format(deletion_ird_number)) {
		std::cout << Display_Error << IRD_Number_Invalid_Format << std::endl;
		deletion_ird_number.clear();
	}
}

void Get_Deletion_Confirmation() {
	deletion_confirmation = false;
	Deletion_Confirmation_Prompt();

	std::string confirmation;
	std::getline(std::cin, confirmation);
	confirmation = Trim_Copy(confirmation);
	if (!Yes_or_No(confirmation)) {
		std::cout << Display_Error << Yes_No_Input << std::endl;
		return;
	}

	deletion_confirmation = (Yes_Or_No_Input_to_String(confirmation) == "Yes");
}

bool Secondary_Authentication() {
	return ::Secondary_Authentication("Final deletion confirmation for: " + deletion_username + " / " + deletion_ird_number);
}

void Get_Delete_Accounts_Menu_Options() {
	while (true) {
		int choice = 0;
		if (!Get_Validated_Menu_Choice(1, 2, &choice)) {
			continue;
		}

		if (choice == 1) {
			initial_secondary_authentication = false;
			final_secondary_authentication = false;
			deletion_confirmation = false;
			deletion_username.clear();
			deletion_ird_number.clear();

			Display_Secondary_Authentication_For_Deletion_Prompt_UI();

			std::string deletionReason;
			if (!Get_Reason(&deletionReason)) {
				continue;
			}

			if (!Secondary_Authentication(deletionReason)) {
				std::cout << Display_Error << Secondary_Auth_Cannot_Proceed << std::endl;
				std::cout << Display_Error << Account_Deletion_Reason_Must_Be_Provided << std::endl;
				continue;
			}
			initial_secondary_authentication = true;

            Display_Delete_Account_Prompt();
            Get_Username();
            if (deletion_username.empty()) {
				continue;
			}

            Get_IRD_Number();
            if (deletion_ird_number.empty()) {
				continue;
			}

            if (!Get_IRD_Username_Match(deletion_ird_number, deletion_username)) {
				std::cout << Display_Error << Invalid_Credentials << std::endl;
				continue;
			}

            Get_Deletion_Confirmation();
            if (!deletion_confirmation) {
				continue;
			}

            if (!Secondary_Authentication()) {
				std::cout << Display_Error << Secondary_Auth_Delete_Account_Cannot_Proceed << std::endl;
				continue;
			}
			final_secondary_authentication = true;

			if (Delete_Confirmed_and_Authenticated_Account_and_IRD_Records()) {
				return;
			}

			continue;
		}

		return;
	}
}
