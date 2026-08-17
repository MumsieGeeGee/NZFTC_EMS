// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef EDIT_ACCOUNTS_UI_H
#define EDIT_ACCOUNTS_UI_H

#include <map>
#include <string>

void Display_Edit_Accounts_Menu_UI();
void Display_Personal_Edit_Options_Menu_UI();
void Display_Selected_User_Edit_Options_Menu_UI();
void Display_Selected_User_Edit_Options_Input_Prompt_UI();
void Display_Basic_Employee_Details_Edit_Menu_UI();
void Display_Full_Employee_Details_Edit_Menu_UI();
void Display_Employee_Details_Edit_Field_Menu_UI();
void Display_Employee_Detail_Input_Prompt_UI(const std::string& detail_label);
void Display_Edit_Current_User_Account_Details_UI(const std::map<std::string, std::string>& account_details);
void Display_Edit_Selected_User_Account_Details_UI(const std::map<std::string, std::string>& account_details);

#endif // EDIT_ACCOUNTS_UI_H
