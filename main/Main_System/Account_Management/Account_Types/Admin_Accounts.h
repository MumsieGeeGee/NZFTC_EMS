// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef ADMIN_ACCOUNTS_H
#define ADMIN_ACCOUNTS_H

#include <string>

enum class Admin_Account_Type {
    General, 
    Receptionist,
    Call_Centre_Operator,
    Executive_Assistant,
    Personal_Assistant,
    Human_Resources_Administrator,
    IT_Support_Administrator,
    Manager
};

void Admin_Account_Type_Options(Admin_Account_Type& admin_choice);
void Admin_Username_Input_Rules(std::string& username, Admin_Account_Type& admin_choice);

#endif // ADMIN_ACCOUNTS_H

