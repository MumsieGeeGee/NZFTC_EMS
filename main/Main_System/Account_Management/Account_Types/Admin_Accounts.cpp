// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Admin_Accounts.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../Account_Creation/Create_Account.h"
#include "../../Account_Form/Account_Creation_Form_UI.h"
#include <string>
#include <iostream>

static int General = 0;
static int Receptionist = 0;
static int Call_Centre_Operator = 0;
static int Executive_Assistant = 0;
static int Personal_Assistant = 0;
static int Human_Resources_Administrator = 0;
static int IT_Support_Administrator = 0;
static int Manager = 0;

void Admin_Account_Type_Options(Admin_Account_Type& admin_choice) {
    if (admin_choice == Admin_Account_Type::General) {
        std::cout << "General Admin account type selected." << std::endl;
    } else if (admin_choice == Admin_Account_Type::Receptionist) {
        std::cout << "Receptionist Admin account type selected." << std::endl;
    } else if (admin_choice == Admin_Account_Type::Call_Centre_Operator) {
        std::cout << "Call Centre Operator Admin account type selected." << std::endl;
    } else if (admin_choice == Admin_Account_Type::Executive_Assistant) {
        std::cout << "Executive Assistant Admin account type selected." << std::endl;
    } else if (admin_choice == Admin_Account_Type::Personal_Assistant) {
        std::cout << "Personal Assistant Admin account type selected." << std::endl;
    } else if (admin_choice == Admin_Account_Type::Human_Resources_Administrator) {
        std::cout << "Human Resources Admin account type selected." << std::endl;
    } else if (admin_choice == Admin_Account_Type::IT_Support_Administrator) {
        std::cout << "IT Support Admin account type selected." << std::endl;
    } else if (admin_choice == Admin_Account_Type::Manager) {
        std::cout << "Manager Admin account type selected." << std::endl;
    } else {
        std::cout << Display_Error << Invalid_Admin_Role << std::endl;
    }
    Display_Account_User_Details_Required_UI();
    Display_Continue_Account_Creation_Question_UI();
    Get_Continue_Account_Creation_Question_Options();
}

void Admin_Username_Input_Rules(std::string& username, Admin_Account_Type& admin_choice) {
    username = "admin_" + username;
    if (admin_choice == Admin_Account_Type::General) {
        username += "_junior_general";
    } else if (admin_choice == Admin_Account_Type::Receptionist) {
        username += "_r";
    } else if (admin_choice == Admin_Account_Type::Call_Centre_Operator) {
        username += "_cco";
    } else if (admin_choice == Admin_Account_Type::Executive_Assistant) {
        username += "_ea";
    } else if (admin_choice == Admin_Account_Type::Personal_Assistant) {
        username += "_pa";
    } else if (admin_choice == Admin_Account_Type::Human_Resources_Administrator) {
        username += "_hr";
    } else if (admin_choice == Admin_Account_Type::IT_Support_Administrator) {
        username += "_it";
    } else if (admin_choice == Admin_Account_Type::Manager) {
        username += "_manager";
    }
}

