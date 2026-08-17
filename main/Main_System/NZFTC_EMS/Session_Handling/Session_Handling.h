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

#endif // SESSION_HANDLING_H

