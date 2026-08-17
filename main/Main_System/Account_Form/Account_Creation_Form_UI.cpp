// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.


#include "Account_Creation_Form_UI.h"
#include "Account_Creation_Form_Labels.h"
#include "../UI/Shared_UI_Messaging/Error_Messages.h"
#include <string>
#include <iostream>


void Display_Account_Access_Type_Options_UI() {
    std::cout << "Select the account type:" << std::endl;
    std::cout << "1. " << Admin_Account << Admin_Uses << std::endl;
    std::cout << "2. " << User_Account << User_Uses << std::endl;
    std::cout << Please_Select_an_Option << " (1-2): ";
}

void Display_Account_Username_Details_Required_UI() {
    std::cout << "      " << Username_Requires << std::endl;
    std::cout << "      " << Account_Type << std::endl;
    std::cout << "      " << Business_Department << std::endl;
    std::cout << "      " << Job_Position << std::endl;
}

void Display_Address_User_Details_Required_UI() {
    std::cout << "      " << Address_Requires << std::endl;
    std::cout << "      " << House_Number << std::endl;
    std::cout << "      " << Street_Name << std::endl;
    std::cout << "      " << Suburb << std::endl;
    std::cout << "      " << City << std::endl;
    std::cout << "      " << Postcode << std::endl;
    std::cout << "      " << Po_Box << " (if applicable)" << std::endl;
}

void Password_Rules_Full_UI() {
    std::cout << "      Password Requires:" << std::endl;
    std::cout << "      " << Password_Too_Long << std::endl;
    std::cout << "      " << Password_Too_Short << std::endl;
    std::cout << "      " << Password_Must_Contain_1_Lower << std::endl;
    std::cout << "      " << Password_Must_Contain_1_Upper << std::endl;
    std::cout << "      " << Password_Must_Contain_1_Number << std::endl;
    std::cout << "      " << Password_Must_Contain_1_Symbol << std::endl;
    std::cout << "      " << Password_Empty << std::endl;
    std::cout << "      " << Password_Cannot_Contain_Space << std::endl;
    std::cout << "      " << Password_Cannot_Contain_Username << std::endl;
    std::cout << "      " << Password_Cannot_Contain_Account_User_Name << std::endl;
    std::cout << "      " << Password_Cannot_Contain_Account_User_DOB << std::endl;
    std::cout << "      " << Password_Cannot_Contain_Account_User_Email << std::endl;
    std::cout << "      " << Password_Cannot_Contain_Account_User_Phone << std::endl;
    std::cout << "      " << Password_Cannot_Contain_Account_User_Address << std::endl;
    std::cout << "      " << Password_Cannot_Contain_Account_User_IRD << std::endl;
}


void Display_Account_User_Details_Required_UI() {
    std::cout << "......................................................................." << std::endl;
    std::cout << "Once you begin, you must finish to exit the account creation programme." << std::endl;
    std::cout << "......................................................................." << std::endl;
    std::cout << "      The following will be required to create an account:" << std::endl;
    std::cout << "                                                          " << std::endl;
    std::cout << "      " << First_Name << std::endl;
    std::cout << "      " << Middle_Name << std::endl;
    std::cout << "      " << Last_Name << std::endl;
    std::cout << "      " << Date_of_Birth << std::endl;
    std::cout << "      " << Address << std::endl;
    std::cout << "                                                          " << std::endl;
    Display_Address_User_Details_Required_UI();
    std::cout << "      " << Email_Address << std::endl;
    std::cout << "      " << Phone_Number << std::endl;
    std::cout << "      " << IRD_Number << " (requires confirmation status 'Confirmed')" << std::endl;
    std::cout << "      " << Username << std::endl;
    std::cout << "      " << Password << std::endl;
    std::cout << "                                                          " << std::endl;
    Display_Account_Username_Details_Required_UI();
    Password_Rules_Full_UI();
    std::cout << "......................................................................." << std::endl;
}

void Display_Continue_Account_Creation_Question_UI() {
    std::cout << "Do you wish to continue with the account creation process?" << std::endl;
    std::cout << "1. Yes" << std::endl;
    std::cout << "2. No" << std::endl;
    std::cout << Please_Select_an_Option << " (1-2): ";
}

void First_Name_Input_Prompt() {
    std::cout << Enter_First_Name << ' ';
}

void Middle_Name_Input_Prompt() {
    std::cout << "If no middle name exists, please enter 'N/A'." << std::endl;
    std::cout << Enter_Middle_Name << ' ';
}

void Last_Name_Input_Prompt() {
    std::cout << Enter_Surname << ' ';
}

void Date_of_Birth_Input_Prompt() {
    std::cout << Enter_DOB << " " << Date_Format << ' ';
}

void Address_Input_Prompt() {
    std::cout << Enter_Address << ' ';
}

void Address_House_Number_Input_Prompt() {
    std::cout << Enter_House_Number << ' ';
}

void Address_Street_Name_Input_Prompt() {
    std::cout << Enter_Street_Name << ' ';
}

void Address_Suburb_Input_Prompt() {
    std::cout << Enter_Suburb << ' ';
}

void Address_City_Input_Prompt() {
    std::cout << Enter_City << ' ';
}

void Address_Postcode_Input_Prompt() {
    std::cout << Enter_Postcode << ' ';
}

void Address_Post_Box_Input_Prompt() {
    std::cout << Enter_Po_Box << ' ';
}

void Email_Address_Input_Prompt() {
    std::cout << Enter_Email_Address << ' ';
}

void Phone_Number_Input_Prompt() {
    std::cout << Enter_Phone_Number << ' ';
}

void IRD_Number_Form_Input_Prompt() {
    std::cout << Enter_IRD_Number << ' ';
}

void Display_Role_Assignment_Options_UI(
        const std::string& question,
        const std::vector<std::string>& role_assignment_options) {
    std::cout << question << std::endl;
    for (std::size_t i = 0; i < role_assignment_options.size(); ++i) {
        std::cout << (i + 1) << ". " << role_assignment_options[i] << std::endl;
    }
    std::cout << Please_Select_an_Option << " (1-"
              << role_assignment_options.size() << "): ";
}
