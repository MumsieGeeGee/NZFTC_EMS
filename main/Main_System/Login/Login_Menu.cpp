// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Login_Menu.h"
#include "../UI/Login_Menu_UI.h"
#include "../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../UI/Shared_UI_Messaging/Info_Messages.h"
#include "Account_Management/Account_Access/Account_Database.h"
#include "Account_Management/Account_Access/Component_Access.h"
#include "Account_Management/Account_Management/Security_Protocols.h"
#include "Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../NZFTC_EMS/Session_Handling/Session_Handling.h"

#include <string>
#include <iostream>
#include <cstdlib>

bool Get_Password(const std::string& password, const std::string& IRD_number, const std::string& username, std::string& authenticated_username);

bool Get_Username(const std::string& username, const std::string& password, const std::string& IRD_number, std::string& authenticated_username) {
    (void)username;
    (void)password;
    (void)IRD_number;
    std::string entered_username;
    std::getline(std::cin, entered_username);
    entered_username = Trim_Copy(entered_username);

    if (entered_username.empty()) {
        std::cout << Display_Error << Username_Empty << std::endl;
        return false;
    }

    if (!Get_Username_From_Employee_Records(entered_username) || !Username_Rules(entered_username)) {
        Log_Unknown_User_Attempt(entered_username);
        std::cout << Display_Error << Username_Invalid << std::endl;
        return false;
    }

    authenticated_username = entered_username;
    return true;
}

bool Get_Password(const std::string& password, const std::string& IRD_number, const std::string& username, std::string& authenticated_username) {
    (void)password;
    if (Is_Account_Locked(username)) {
        Log_Locked_Account_Login_Attempt(username);
        std::cout << Display_Error << Account_Locked << ' ' << Too_Many_Attempts << std::endl;
        return false;
    }

    std::string entered_password;
    Display_Password_Prompt_UI();
    entered_password = Read_Password_Input_From_Console();

    const auto register_failed_attempt = [&username]() -> int {
        const int next_attempt = Get_Failed_Login_Attempts(username) + 1;
        if (next_attempt <= 1) {
            if (!First_Incorrect_Password_Attempt(username)) {
                return 3;
            }
            return 1;
        }

        if (next_attempt == 2) {
            if (!Second_Incorrect_Password_Attempt(username)) {
                return 3;
            }
            return 2;
        }

        if (!Third_Incorrect_Password_Attempt(username)) {
            return 3;
        }
        Log_Too_Many_Incorrect_Password_Attempts(username);
        if (!Lock_Account_After_Three_Failed_Attempts(username)) {
            return 3;
        }
        return 3;
    };

    const auto display_failed_attempt_message =
        [](const std::string& base_message, int attempt_number) {
            if (attempt_number == 1) {
                std::cout << Display_Error << base_message << ' '
                          << First_Incorrect_Password << std::endl;
                return;
            }

            if (attempt_number == 2) {
                std::cout << Display_Error << base_message << ' '
                          << Second_Incorrect_Password << std::endl;
                return;
            }

            std::cout << Display_Error << base_message << ' '
                      << Third_Incorrect_Password << ' '
                      << Account_Locked_Due_to_Failed_Attempts << std::endl;
        };

    if (Trim_Copy(entered_password).empty()) {
        const int attempt_number = register_failed_attempt();
        display_failed_attempt_message(Password_Missing_or_Invalid, attempt_number);
        return false;
    }

    const std::string entered_password_hash = Password_Save(entered_password);

    if (!Check_Account_Exists(username, entered_password_hash, IRD_number)) {
        const int attempt_number = register_failed_attempt();
        display_failed_attempt_message(Incorrect_Password, attempt_number);
        return false;
    }

    if (!Reset_Failed_Login_Attempts(username)) {
        std::cout << Display_Error << Account_Update_Failure << std::endl;
        return false;
    }
    if (!Complete_Required_Password_Reset(username)) {
        return false;
    }
    authenticated_username = username;
    return true;
}

bool Get_Login_Menu_Choice(const std::string& username, const std::string& password, const std::string& IRD_number, std::string& authenticated_username) {
    while (true) {
        int choice = 0;
        if (!Get_Validated_Menu_Choice(1, 2, &choice)) {
            continue;
        }

        if (choice == 1) {
            while (true) {
                Display_Login_Prompt_UI();
                Display_Username_Prompt_UI();

                if (!Get_Username(username, password, IRD_number, authenticated_username)) {
                    continue;
                }

                const std::string entered_username = authenticated_username;
                if (!Get_Password(password, IRD_number, entered_username, authenticated_username)) {
                    continue;
                }

                return true;
            }
        } else if (choice == 2) {
            std::exit(0);
        }
    }
}


void Main_Login_Menu(const std::string& username, const std::string& password, const std::string& IRD_number) {
    while (true) {
        Display_Main_Login_Menu_UI();
        std::string authenticated_username;
        if (Get_Login_Menu_Choice(username, password, IRD_number, authenticated_username)) {
            const std::string account_type = Get_Account_Type_From_Employee_Records(authenticated_username);
            User_Session_Context session_context;
            session_context.username = authenticated_username;
            session_context.account_type = account_type;
            session_context.ird_number = Get_IRD_Number_For_Username(authenticated_username);
            session_context.authenticated = true;
            Set_Current_User_Session_Context(session_context);
            Display_Account_Access_For_Type(account_type, authenticated_username);
            Clear_Current_User_Session_Context();
        }
    }
}

void Return_to_Login_Menu() {
    std::cout << Returning_to_Main_Menu << std::endl;
    const User_Session_Context& session = Get_Current_User_Session_Context();
    if (session.authenticated && !Trim_Copy(session.username).empty()) {
        Log_Session_Ended_Normally(session.username);
    }
    Clear_Current_User_Session_Context();
    std::string username;
    std::string password;
    std::string IRD_number;
    Main_Login_Menu(username, password, IRD_number);
}
