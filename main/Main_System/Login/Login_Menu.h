// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef LOGIN_MENU_H
#define LOGIN_MENU_H

#include <string>
#include "../NZFTC_EMS/Session_Handling/Session_Handling.h"
#include "../Account_Management/Account_Access/Component_Access.h"

struct UI_Login_Result {
    bool success = false;
    bool password_reset_required = false;
    std::string message;
    Account_Access_Profile access_profile;
};

struct UI_Action_Result {
    bool success = false;
    std::string message;
};

void Main_Login_Menu(const std::string& username, const std::string& password, const std::string& IRD_number);
void Return_to_Login_Menu();
UI_Login_Result Authenticate_And_Build_UI_Context(
    const std::string& username,
    const std::string& password,
    const std::string& IRD_number);
UI_Action_Result Execute_UI_Action(
    const std::string& action_id,
    const std::string& username = "");

#endif // LOGIN_MENU_H
