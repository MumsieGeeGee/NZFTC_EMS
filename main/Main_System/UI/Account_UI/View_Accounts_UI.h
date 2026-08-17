// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef VIEW_ACCOUNTS_UI_H
#define VIEW_ACCOUNTS_UI_H

#include <map>
#include <string>
#include <vector>

void Display_Dynamic_Username_List_UI(const std::vector<std::string>& usernames, const std::string& business_role_type_heading);
void Display_Details_Menu_Options_UI();
void Display_Current_User_Account_UI(const std::string& username);
void Display_Selected_User_Account_UI(const std::string& username);
void Display_Account_Selection_Input_Prompt_UI(std::size_t number_of_accounts);
void Display_Current_Users_Employee_Details_UI(const std::string& username,
											   const std::map<std::string, std::string>& employee_details);
void Display_Selected_Users_Employee_Details_UI(const std::string& username,
												const std::map<std::string, std::string>& employee_details);

#endif // VIEW_ACCOUNTS_UI_H
