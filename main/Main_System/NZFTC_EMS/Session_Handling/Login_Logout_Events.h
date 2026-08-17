// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef LOGIN_LOGOUT_EVENTS_MESSAGES_H
#define LOGIN_LOGOUT_EVENTS_MESSAGES_H

#include <string>

// Login event messages
extern std::string Unknown_User_Login_Attempt_Event;
extern std::string Incorrect_Password_Login_Attempt_Event;
extern std::string Secondary_Incorrect_Password_Login_Attempt_Event;
extern std::string Third_Incorrect_Password_Login_Attempt_Event;
extern std::string Too_Many_Incorrect_Password_Login_Attempts_Event;
extern std::string Account_Locked_Due_to_Failed_Login_Attempts_Event;
extern std::string Successful_Logout_Event;
extern std::string Session_Ended_Normally_Event;
// Logout event messages
extern std::string Unsuccessful_Logout_Event;
extern std::string Session_Ended_Due_to_Timeout_Event;

#endif // LOGIN_LOGOUT_EVENTS_MESSAGES_H
