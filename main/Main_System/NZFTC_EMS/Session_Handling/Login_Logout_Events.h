// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef LOGIN_LOGOUT_EVENTS_MESSAGES_H
#define LOGIN_LOGOUT_EVENTS_MESSAGES_H

#include <string>

std::string Unknown_User_Login_Attempt();
std::string Incorrect_Password_Login_Attempt();
std::string Secondary_Incorrect_Password_Login_Attempt();
std::string Third_Incorrect_Password_Login_Attempt();
std::string Too_Many_Incorrect_Password_Login_Attempts();
std::string Account_Locked_Due_to_Failed_Login_Attempts();
std::string Locked_Account_Login_Attempt();
std::string Account_Unlocked_From_Admin_Dashboard();
std::string Password_Changed_After_Admin_Unlock();
std::string Successful_Logout();
std::string Session_Ended_Normally();
std::string Unsuccessful_Logout();
std::string Session_Ended_Due_to_Timeout();
std::string Session_Ended_Due_to_Forced_Logout();

#endif // LOGIN_LOGOUT_EVENTS_MESSAGES_H
