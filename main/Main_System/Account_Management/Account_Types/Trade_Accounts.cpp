// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Trade_Accounts.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../Account_Creation/Create_Account.h"
#include "../../Account_Form/Account_Creation_Form_UI.h"
#include <string>
#include <iostream>

static int General = 0;
static int Apprentice = 0;
static int Mentor = 0;
static int Safety_Officer = 0;
static int Sales_Representative = 0;
static int Manager = 0;

void Trade_Account_Type_Options(Trade_Account_Type& trade_choice) {
    if (trade_choice == Trade_Account_Type::General) {
        std::cout << "General Trade Staff account type selected." << std::endl;
    } else if (trade_choice == Trade_Account_Type::Apprentice) {
        std::cout << "Junior Apprentice account type selected." << std::endl;
    } else if (trade_choice == Trade_Account_Type::Mentor) {
        std::cout << "Senior Mentor account type selected." << std::endl;
    } else if (trade_choice == Trade_Account_Type::Safety_Officer) {
        std::cout << "Safety Officer account type selected." << std::endl;
    } else if (trade_choice == Trade_Account_Type::Sales_Representative) {
        std::cout << "Sales Representative account type selected." << std::endl;
    } else if (trade_choice == Trade_Account_Type::Manager) {
        std::cout << "Manager Trade account type selected." << std::endl;
    } else {
        std::cout << Display_Error << Invalid_Trade_Role << std::endl;
    }
    Display_Account_User_Details_Required_UI();
    Display_Continue_Account_Creation_Question_UI();
    Get_Continue_Account_Creation_Question_Options();
}

void Trade_Username_Input_Rules(std::string& username, Trade_Account_Type& trade_choice) {
    username = "trade_" + username;
    if (trade_choice == Trade_Account_Type::General) {
        username += "_junior_general";
    } else if (trade_choice == Trade_Account_Type::Apprentice) {
        username += "_apprentice";
    } else if (trade_choice == Trade_Account_Type::Mentor) {
        username += "_mentor";
    } else if (trade_choice == Trade_Account_Type::Safety_Officer) {
        username += "_safety";
    } else if (trade_choice == Trade_Account_Type::Sales_Representative) {
        username += "_sales";
    } else if (trade_choice == Trade_Account_Type::Manager) {
        username += "_manager";
    }
}
