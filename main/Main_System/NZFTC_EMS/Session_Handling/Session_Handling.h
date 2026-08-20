// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef SESSION_HANDLING_H
#define SESSION_HANDLING_H

#include <string>

struct User_Session_Context {
	std::string username;
	std::string account_type;
	std::string ird_number;
	bool authenticated = false;
	bool requires_secondary_auth = false;
};

const User_Session_Context& Get_Current_User_Session_Context();
void Set_Current_User_Session_Context(const User_Session_Context& sessionContext);
void Clear_Current_User_Session_Context();
void Log_Incorrect_Password_Attempt(const std::string& username);
void Log_Second_Incorrect_Password_Attempt(const std::string& username);
void Log_Third_Incorrect_Password_Attempt(const std::string& username);
void Log_Unknown_User_Attempt(const std::string& username);
void Log_Account_Locked(const std::string& username);
void Log_Too_Many_Incorrect_Password_Attempts(const std::string& username);
void Log_Account_Unlocked(const std::string& username);
void Log_Password_Changed_After_Admin_Unlock(const std::string& username);
void Log_Locked_Account_Login_Attempt(const std::string& username);
void Log_Successful_Logout(const std::string& username);
void Log_Session_Ended_Normally(const std::string& username);

#endif // SESSION_HANDLING_H
