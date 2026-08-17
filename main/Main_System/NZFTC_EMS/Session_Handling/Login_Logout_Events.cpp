// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Login_Logout_Events.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include <string>

// Login event messages
std::string Unknown_User_Login_Attempt_Event = "an " + Unknown_Capitalized_Label + Username_Label + " attempted to log in";
std::string Incorrect_Password_Login_Attempt_Event = "an " + Incorrect_Label + Password_Label + " was used in a login attempt";
std::string Secondary_Incorrect_Password_Login_Attempt_Event = Incorrect_Label + Password_Label + " was used in a secondary login attempt";
std::string Third_Incorrect_Password_Login_Attempt_Event = Incorrect_Label + Password_Label + " was used in a third login attempt";
std::string Too_Many_Incorrect_Password_Login_Attempts_Event = "too many " + Incorrect_Label + Password_Label + " attempts";
std::string Account_Locked_Due_to_Failed_Login_Attempts_Event = "account locked due to failed login attempts";
std::string Successful_Logout_Event = "successful logout";
std::string Session_Ended_Normally_Event = "session ended normally";

// Logout event messages
std::string Unsuccessful_Logout_Event = "unsuccessful logout";
std::string Session_Ended_Due_to_Timeout_Event = "session ended due to timeout";