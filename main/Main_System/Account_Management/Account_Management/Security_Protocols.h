// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef SECURITY_PROTOCOLS_H
#define SECURITY_PROTOCOLS_H

#include "Employee Management/Role_Management/Role_and_Salary_Details.h"
#include <string>

extern const std::string allowed_symbols;
// Returns true when a user confirms the IRD has been sighted as confirmed.
bool Confirm_Unconfirmed_IRD_For_Tax_Record(const std::string& IRD_Number);

// Username format checks used by login/account creation.
bool Username_Rules(const std::string& username);

// Generates a final role-based username for account creation.
std::string Generate_Role_Based_Username(int role_type_selection,
	int role_account_selection,
	const std::string& first_name,
	const std::string& middle_name,
	const std::string& last_name);

// Password length and complexity checks used by account creation.
void Password_Making_Rules(std::string& password,
	const std::string& username,
	const std::string& first_name,
	const std::string& last_name,
	const std::string& date_of_birth,
	const std::string& email_address,
	const std::string& phone_number,
	const std::string& address,
	const std::string& IRD_number
);
bool Password_Rules(std::string& password);
std::string Password_Save(const std::string& password);
std::string Password_Hashing(const std::string& password);
std::string Mask_Password_Index_Entry(const std::string& password_hash);
bool Is_Account_Locked(const std::string& username);
int Get_Failed_Login_Attempts(const std::string& username);
bool Account_Requires_Password_Reset(const std::string& username);
bool Reset_Failed_Login_Attempts(const std::string& username);
bool First_Incorrect_Password_Attempt(const std::string& username);
bool Second_Incorrect_Password_Attempt(const std::string& username);
bool Third_Incorrect_Password_Attempt(const std::string& username);
bool Lock_Account_After_Three_Failed_Attempts(const std::string& username);
bool Unlock_Account_from_Admin_Dashboard(const std::string& username, const std::string& temporary_password);
bool Complete_Required_Password_Reset(const std::string& username);

// Console password input handling (masked by default; toggleable for future UI settings).
void Set_Password_Input_Masking_Enabled(bool enabled);
std::string Read_Password_Input_From_Console();

// Secondary authentication for high-risk account actions.
bool Get_Reason(std::string* reasonOut);
bool Secondary_Authentication(const std::string& reason);
bool Secondary_Authentication(const std::string& reason, std::string* secondaryUsernameOut);
void Return_To_Menu_For_Role(Admin_Role admin_role,  Finance_Role finance_role, Trade_Role trade_role);



#endif // SECURITY_PROTOCOLS_H
