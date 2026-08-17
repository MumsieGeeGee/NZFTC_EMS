// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Finance_Accounts.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../Account_Creation/Create_Account.h"
#include "../../Account_Form/Account_Creation_Form_UI.h"
#include <string>
#include <iostream>

static int General = 0;
static int Payroll_Administrator = 0;
static int Accounts_Payable_Administrator = 0;
static int Accounts_Receivable_Administrator = 0;
static int Manager = 0;

void Finance_Account_Type_Options(Finance_Account_Type& finance_choice) {
    if (finance_choice == Finance_Account_Type::General) {
        std::cout << "General Finance Staff account type selected." << std::endl;
    } else if (finance_choice == Finance_Account_Type::Payroll_Administrator) {
        std::cout << "Payroll Administrator account type selected." << std::endl;
    } else if (finance_choice == Finance_Account_Type::Accounts_Payable_Administrator) {
        std::cout << "Accounts Payable Administrator account type selected." << std::endl;
    } else if (finance_choice == Finance_Account_Type::Accounts_Receivable_Administrator) {
        std::cout << "Accounts Receivable Administrator account type selected." << std::endl;
    } else if (finance_choice == Finance_Account_Type::Manager) {
        std::cout << "Manager Finance account type selected." << std::endl;
    } else {
        std::cout << Display_Error << Invalid_Finance_Role << std::endl;
    }
    Display_Account_User_Details_Required_UI();
    Display_Continue_Account_Creation_Question_UI();
    Get_Continue_Account_Creation_Question_Options();
}

void Finance_Username_Input_Rules(std::string& username, Finance_Account_Type& finance_choice) {
    username = "finance_" + username;
    if (finance_choice == Finance_Account_Type::General) {
        username += "_junior_general";
    } else if (finance_choice == Finance_Account_Type::Payroll_Administrator) {
        username += "_payroll";
    } else if (finance_choice == Finance_Account_Type::Accounts_Payable_Administrator) {
        username += "_ap";
    } else if (finance_choice == Finance_Account_Type::Accounts_Receivable_Administrator) {
        username += "_ar";
    } else if (finance_choice == Finance_Account_Type::Manager) {
        username += "_manager";
    }
}
