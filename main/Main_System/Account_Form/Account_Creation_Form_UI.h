// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef ACCOUNT_CREATION_FORM_UI_H
#define ACCOUNT_CREATION_FORM_UI_H

#include <string>
#include <iostream>
#include <vector>

void Display_Account_Access_Type_Options_UI();
void Display_Account_Username_Details_Required_UI();
void Display_Address_User_Details_Required_UI();
void Password_Rules_Full_UI();
void Display_Account_User_Details_Required_UI();
void Display_Continue_Account_Creation_Question_UI();
void First_Name_Input_Prompt();
void Middle_Name_Input_Prompt();
void Last_Name_Input_Prompt();
void Date_of_Birth_Input_Prompt();
void Address_Input_Prompt();
void Address_House_Number_Input_Prompt();
void Address_Street_Name_Input_Prompt();
void Address_Suburb_Input_Prompt();
void Address_City_Input_Prompt();
void Address_Postcode_Input_Prompt();
void Address_Post_Box_Input_Prompt();
void Email_Address_Input_Prompt();
void Phone_Number_Input_Prompt();
void IRD_Number_Form_Input_Prompt();
void Display_Role_Assignment_Options_UI(
	const std::string& question,
	const std::vector<std::string>& role_assignment_options);


#endif // ACCOUNT_CREATION_FORM_UI_H
