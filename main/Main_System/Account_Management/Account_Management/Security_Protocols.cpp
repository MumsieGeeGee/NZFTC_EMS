// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Security_Protocols.h"
#include "Save_Account_Details.h"
#include "../Account_Access/Account_Database.h"
#include "../Account_Access/Component_Access.h"
#include "../../Employee Management/Role_Management/Role_and_Salary_Details.h"
#include "../../NZFTC_EMS/Session_Handling/Session_Handling.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Info_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include "../../UI/Account_UI/Admin_Dashboard_UI.h"
#include "../../UI/Account_UI/Employee_Dashboard_UI.h"
#include "Input_Validation_Helpers.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <conio.h>
#include <io.h>
#include <iostream>
#include <map>

const std::string allowed_symbols = "!@#$%^&*()_+-=`~{}[]:\";'<>,.?/|\\";

namespace {
bool password_input_masking_enabled = true;
}

void Set_Password_Input_Masking_Enabled(bool enabled) {
	password_input_masking_enabled = enabled;
}

std::string Read_Password_Input_From_Console() {
	std::string password;

	if (_isatty(_fileno(stdin)) == 0) {
		std::getline(std::cin, password);
		return password;
	}

	while (true) {
		const int key = _getch();

		if (key == '\r' || key == '\n') {
			std::cout << std::endl;
			break;
		}

		if (key == 8) {
			if (!password.empty()) {
				password.pop_back();
				if (password_input_masking_enabled) {
					std::cout << "\b \b";
				}
			}
			continue;
		}

		if (key == 0 || key == 224) {
			(void)_getch();
			continue;
		}

		if (!std::isprint(static_cast<unsigned char>(key))) {
			continue;
		}

		password.push_back(static_cast<char>(key));
		if (password_input_masking_enabled) {
			std::cout << '*';
		} else {
			std::cout << static_cast<char>(key);
		}
	}

	return password;
}

bool Confirm_Unconfirmed_IRD_For_Tax_Record(const std::string& IRD_Number) {
	std::string input;

	while (true) {
		std::cout << IRD_Number_Label << IRD_Number << std::endl;
		std::cout << IRD_Confirmation_Status_Unconfirmed << std::endl;
		std::cout << Secondary_Auth_for_IRD_Number_Prompt << ' ';
		std::cout << Yes_No_Question_Input_Label;
		std::getline(std::cin, input);

        if (!Yes_or_No(input)) {
			std::cout << Display_Error << Yes_No_Input << std::endl;
            continue;
		}

		if (Yes_Or_No_Input_to_String(input) == "Yes") {
			return true;
		}

		std::cout << Display_Error << Unconfirmed_IRD_Number_Block << std::endl;
		return false;
	}
}

bool Username_Rules(const std::string& username) {
    if (username.empty()) {
        std::cout << Display_Error << Username_Empty << std::endl;
        return false;
    }

    if (username.length() > 50) {
        std::cout << Display_Error << Username_Too_Long << std::endl;
        return false;
    }

    // Accept letters, digits, and all symbols you listed
    const std::string allowed_symbols = "!@#$%^&*()_+-=`~{}[]:\";'<>,.?/|\\";
    
    if (!std::all_of(username.begin(), username.end(),
        [&](char c) {
            return std::isalnum(static_cast<unsigned char>(c)) ||
                   allowed_symbols.find(c) != std::string::npos;
        }))
    {
        std::cout << Display_Error << Username_Invalid_Characters << std::endl;
        return false;
    }

    return true;
}

namespace {
std::string Normalize_Name_Token(const std::string& raw_value, bool allow_hash_fallback) {
	std::string token = Trim_Copy(raw_value);
	std::transform(token.begin(), token.end(), token.begin(), [](unsigned char c) {
		return static_cast<char>(std::tolower(c));
	});

	std::string normalized;
	normalized.reserve(token.size());
	for (char c : token) {
		if (std::isalnum(static_cast<unsigned char>(c))) {
			normalized.push_back(c);
		} else if (std::isspace(static_cast<unsigned char>(c))) {
			normalized.push_back('_');
		}
	}

	if (normalized.empty() && allow_hash_fallback) {
		return "#";
	}
	return normalized;
}
}

std::string Generate_Role_Based_Username(int role_type_selection,
	int role_account_selection,
	const std::string& first_name,
	const std::string& middle_name,
	const std::string& last_name) {
	std::string username;

	if (role_type_selection == 1) {
		username = "admin_";
		switch (role_account_selection) {
			case 1: username += "jg"; break;
			case 2: username += "sg"; break;
			case 3: username += "jt"; break;
			case 4: username += "st"; break;
			case 5: username += "jc"; break;
			case 6: username += "sc"; break;
			case 7: username += "r"; break;
			case 8: username += "cc"; break;
			case 9: username += "ea"; break;
			case 10: username += "pa"; break;
			case 11: username += "hr"; break;
			case 12: username += "jit"; break;
			case 13: username += "sit"; break;
			case 14: username += "jlm"; break;
			case 15: username += "slm"; break;
			default: return "";
		}
	} else if (role_type_selection == 2) {
		username = "finance_";
		switch (role_account_selection) {
			case 1: username += "jg"; break;
			case 2: username += "sg"; break;
			case 3: username += "jt"; break;
			case 4: username += "st"; break;
			case 5: username += "jc"; break;
			case 6: username += "sc"; break;
			case 7: username += "pr"; break;
			case 8: username += "ap"; break;
			case 9: username += "ar"; break;
			case 10: username += "jfm"; break;
			case 11: username += "sfm"; break;
			default: return "";
		}
	} else if (role_type_selection == 3) {
		username = "trade_";
		switch (role_account_selection) {
			case 1: username += "jg"; break;
			case 2: username += "sg"; break;
			case 3: username += "jt"; break;
			case 4: username += "st"; break;
			case 5: username += "jc"; break;
			case 6: username += "sc"; break;
			case 7: username += "app"; break;
			case 8: username += "men"; break;
			case 9: username += "saf"; break;
			case 10: username += "sal"; break;
			case 11: username += "jtm"; break;
			case 12: username += "stm"; break;
			default: return "";
		}
	} else {
		return "";
	}

	const std::string firstToken = Normalize_Name_Token(first_name, false);
	const std::string middleToken = Normalize_Name_Token(middle_name, true);
	const std::string lastToken = Normalize_Name_Token(last_name, false);
	if (firstToken.empty() || lastToken.empty()) {
		return "";
	}

	username += "_" + firstToken + "_" + middleToken + "_" + lastToken;

	std::string unique_username = username;
	int suffix = 2;
	while (Username_Exists_In_Index(unique_username)) {
		unique_username = username + "_" + std::to_string(suffix);
		++suffix;
	}

	return unique_username;
}

void Password_Making_Rules(std::string& password,
	const std::string& username,
	const std::string& first_name,
	const std::string& last_name,
	const std::string& date_of_birth,
	const std::string& email_address,
	const std::string& phone_number,
	const std::string& address,
	const std::string& IRD_number) {
	if (password.find(username) != std::string::npos) {
		return;
	}
	if (password.find(first_name) != std::string::npos || password.find(last_name) != std::string::npos) {
		return;
	}
	if (password.find(date_of_birth) != std::string::npos) {
		return;
	}
	if (password.find(email_address) != std::string::npos) {
		return;
	}
	if (password.find(phone_number) != std::string::npos) {
		return;
	}
	if (password.find(address) != std::string::npos) {
		return;
	}
	if (password.find(IRD_number) != std::string::npos) {
		return;
	}
}

bool Password_Rules(std::string& password) {
	if (password.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos) {
		return false;
	}
	if (password.find_first_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos) {
		return false;
	}
	if (password.find_first_of("0123456789") == std::string::npos) {
		return false;
	}
	if (password.find_first_of("!@#$%^&*") == std::string::npos) {
		return false;
	}
	if (password.find(' ') != std::string::npos) {
		return false;
	}
	if (password.empty()) {
		return false;
	}
	if (password.length() > 50) {
		return false;
	}
	if (password.length() < 8) {
		return false;
	}

	return true;
}


std::string Password_Hashing(const std::string& password) {
	constexpr std::uint64_t FnvOffsetBasis = 1469598103934665603ULL;
	constexpr std::uint64_t FnvPrime = 1099511628211ULL;
	constexpr char Salt[] = "NZFTC_EMS_PASSWORD_SALT_v1";

	auto Fnv1a_Update = [FnvPrime](std::uint64_t hash, const std::string& input) {
		for (unsigned char c : input) {
			hash ^= c;
			hash *= FnvPrime;
		}
		return hash;
	};

	std::uint64_t hash = FnvOffsetBasis;
	hash = Fnv1a_Update(hash, Salt);
	hash = Fnv1a_Update(hash, password);

	static const char HexDigits[] = "0123456789abcdef";
	std::string output(16, '0');
	for (int i = 15; i >= 0; --i) {
		output[i] = HexDigits[hash & 0x0F];
		hash >>= 4;
	}

	return output;
}

std::string Password_Save(const std::string& password) {
	return Password_Hashing(password);
}

std::string Mask_Password_Index_Entry(const std::string& password_hash) {
	if (password_hash.empty()) {
		return "";
	}
	return std::string(password_hash.size(), '*');
}

bool Get_Reason(std::string* reasonOut) {
	if (reasonOut == nullptr) {
		return false;
	}

	std::getline(std::cin, *reasonOut);
	*reasonOut = Trim_Copy(*reasonOut);
	if (reasonOut->empty()) {
		std::cout << Display_Error << Invalid_Empty_Input << std::endl;
		return false;
	}

	return true;
}

void Prompt_Secondary_Authentication_Username(std::string* usernameOut) {
	if (usernameOut == nullptr) {
		return;
	}

	std::cout << "Secondary Admin Username: ";
	std::getline(std::cin, *usernameOut);
	*usernameOut = Trim_Copy(*usernameOut);
}

void Prompt_Secondary_Authentication_Password(std::string* passwordOut) {
	if (passwordOut == nullptr) {
		return;
	}

	std::cout << "Secondary Admin Password: ";
	*passwordOut = Read_Password_Input_From_Console();
}

bool Secondary_Authentication(const std::string& reason) {
	return Secondary_Authentication(reason, nullptr);
}

bool Secondary_Authentication(const std::string& reason, std::string* secondaryUsernameOut) {
	const std::string normalizedReason = Trim_Copy(reason);
	if (normalizedReason.empty()) {
		std::cout << Display_Error << Invalid_Empty_Input << std::endl;
		return false;
	}

	std::string secondaryUsername;
	Prompt_Secondary_Authentication_Username(&secondaryUsername);
	if (!Username_Rules(secondaryUsername)) {
		return false;
	}

	const User_Session_Context& currentSession = Get_Current_User_Session_Context();
	if (currentSession.authenticated && currentSession.username == secondaryUsername) {
		std::cout << Display_Error << Secondary_Auth_Username_Different << std::endl;
		return false;
	}

	std::map<std::string, std::string> secondaryDetails;
	if (!Get_Account_Record_Details_For_Username(secondaryUsername, &secondaryDetails)) {
		std::cout << Display_Error << Account_Retrieval_Failure << Secondary_Auth_Cannot_Proceed << std::endl;
		return false;
	}

	if (secondaryDetails["Account Type"] != "Admin") {
		std::cout << Display_Error << Role_Type_Access_Failed << std::endl;
		return false;
	}

	const std::string businessRole = secondaryDetails["Business Role"];
	const std::string jobRole = secondaryDetails["Job Role"];
	if (!Secondary_Authenticators(businessRole, jobRole)) {
		std::cout << Display_Error << Role_Type_Access_Failed << std::endl;
		return false;
	}

	std::string providedPassword;
	Prompt_Secondary_Authentication_Password(&providedPassword);
	if (Trim_Copy(providedPassword).empty()) {
		std::cout << Display_Error << Password_Empty << std::endl;
		return false;
	}

	const std::string expectedPasswordHash = secondaryDetails["Password"];
	if (expectedPasswordHash.empty()) {
		std::cout << Display_Error << Password_Missing_or_Invalid << std::endl;
		return false;
	}

	const std::string providedPasswordHash = Password_Save(providedPassword);
	if (providedPasswordHash != expectedPasswordHash) {
		std::cout << Display_Error << Password_Incorrect << std::endl;
		return false;
	}

	if (secondaryUsernameOut != nullptr) {
		*secondaryUsernameOut = secondaryUsername;
	}

	// Reason is validated and retained at call-site for later session/record logging.
	(void)normalizedReason;
	return true;
}

void Return_To_Menu_For_Role(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role) {
    if (admin_role == Admin_Role::Senior_Level_Manager ||
        admin_role == Admin_Role::Senior_IT_Support_Administrator) {
        Display_FULL_Admin_Dashboard_Main_Menu_UI();
        return;
    }

    if (admin_role == Admin_Role::Human_Resources_Administrator ||
		finance_role == Finance_Role::Payroll_Administrator ||
		finance_role == Finance_Role::Senior_Finance_Manager ||
		trade_role == Trade_Role::Senior_Trade_Manager) {
        Display_Partial_Admin_Dashboard_Main_Menu_UI();
        Display_Personal_Main_Management_Menu_UI2();
        return;
    }

    if (admin_role == Admin_Role::Executive_Assistant ||
        admin_role == Admin_Role::Personal_Assistant ||
		finance_role == Finance_Role::Junior_Finance_Manager) {
        Display_HR_Management_Only_Admin_Dashboard_Main_Menu_UI();
        Display_Personal_Main_Management_Menu_UI1();
        return;
    }

    if (admin_role == Admin_Role::Junior_IT_Support_Administrator) {
        Display_Account_Management_Only_Admin_Dashboard_Main_Menu_UI();
        Display_Personal_Main_Management_Menu_UI1();
        return;
    }

    if (admin_role == Admin_Role::Junior_Level_Manager ||
		trade_role == Trade_Role::Senior_Mentor ||
		trade_role == Trade_Role::Junior_Trade_Manager) {
        Display_Employee_Management_Only_Admin_Dashboard_Main_Menu_UI();
        Display_Personal_Main_Management_Menu_UI1();
        return;
    }
}
