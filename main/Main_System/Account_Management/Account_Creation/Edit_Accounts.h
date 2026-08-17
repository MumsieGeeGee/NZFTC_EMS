// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef EDIT_ACCOUNTS_H
#define EDIT_ACCOUNTS_H

#include <string>

void Get_Edit_Accounts_Menu_Options();
bool Edit_Current_User_Account_Details(const std::string& username);
bool Edit_Selected_User_Account_Details(std::string& username);


#endif // EDIT_ACCOUNTS_H

