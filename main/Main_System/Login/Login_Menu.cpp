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
#include "../NZFTC_EMS/Session_Handling/Session_Handling.h"

#include <string>
#include <iostream>
#include <limits>
#include <cstdlib>

bool Get_Password(const std::string& password, const std::string& IRD_number, const std::string& username, std::string& authenticated_username);

bool Get_Username(const std::string& username, const std::string& password, const std::string& IRD_number, std::string& authenticated_username) {
    (void)username;
    (void)password;
    (void)IRD_number;
    std::string entered_username;
    std::getline(std::cin, entered_username);

    if (!Get_Username_From_Employee_Records(entered_username) || !Username_Rules(entered_username)) {
        std::cout << Display_Error << Username_Invalid << std::endl;
        return false;
    }

    authenticated_username = entered_username;
    return true;
}

bool Get_Password(const std::string& password, const std::string& IRD_number, const std::string& username, std::string& authenticated_username) {
    (void)password;
    std::string entered_password;
    Display_Password_Prompt_UI();
    entered_password = Read_Password_Input_From_Console();

    const std::string entered_password_hash = Password_Save(entered_password);
    
    if (!Get_Password_From_Employee_Records(entered_password_hash)) {
        std::cout << Display_Error << Password_Missing_or_Invalid << std::endl;
        return false;
    }

    if (!Check_Account_Exists(username, entered_password_hash, IRD_number)) {
        std::cout << Display_Error << Invalid_Credentials << std::endl;
        return false;
    }

    authenticated_username = username;
    return true;
}

bool Get_Login_Menu_Choice(const std::string& username, const std::string& password, const std::string& IRD_number, std::string& authenticated_username) {
    int choice = 0;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            continue;
        }
        if (choice == 1) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
            exit(0);
        } else {
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            continue;
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
    Clear_Current_User_Session_Context();
    std::string username;
    std::string password;
    std::string IRD_number;
    Main_Login_Menu(username, password, IRD_number);
}
