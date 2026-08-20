// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Edit_Accounts_UI.h"
#include "../Shared_UI_Messaging/Form_Labels_UI.h"

#include <iostream>

namespace {
std::string Get_Account_Detail_Value(const std::map<std::string, std::string>& account_details,
                                     const std::string& label) {
    const auto it = account_details.find(label);
    if (it == account_details.end() || it->second.empty()) {
        return "Not set";
    }
    return it->second;
}

void Display_Account_Details_Block(const std::map<std::string, std::string>& account_details) {
    std::cout << Account_Type_Label << ": " << Get_Account_Detail_Value(account_details, "Account Type") << std::endl;
    std::cout << "Business Role: " << Get_Account_Detail_Value(account_details, "Business Role") << std::endl;
    std::cout << Job_Role_Label << ": " << Get_Account_Detail_Value(account_details, "Job Role") << std::endl;
    std::cout << First_Name_Label << ": " << Get_Account_Detail_Value(account_details, "First Name") << std::endl;
    std::cout << Middle_Name_Label << ": " << Get_Account_Detail_Value(account_details, "Middle Name") << std::endl;
    std::cout << Last_Name_Label << ": " << Get_Account_Detail_Value(account_details, "Last Name") << std::endl;
    std::cout << Date_of_Birth_Label << ": " << Get_Account_Detail_Value(account_details, "Date of Birth") << std::endl;
    std::cout << Address_Label << ": " << Get_Account_Detail_Value(account_details, "Address") << std::endl;
    std::cout << Email_Address_Label << ": " << Get_Account_Detail_Value(account_details, "Email Address") << std::endl;
    std::cout << Phone_Number_Label << ": " << Get_Account_Detail_Value(account_details, "Phone Number") << std::endl;
    std::cout << "Account Status: " << Get_Account_Detail_Value(account_details, "Account Status") << std::endl;
    std::cout << "Failed Login Attempts: " << Get_Account_Detail_Value(account_details, "Failed Login Attempts") << std::endl;
    std::cout << "Password Reset Required: " << Get_Account_Detail_Value(account_details, "Password Reset Required") << std::endl;
}
}

void Display_Edit_Accounts_Menu_UI() {
    std::cout << "Edit Existing Account:" << std::endl;
    std::cout << "1. Edit My Account" << std::endl;
    std::cout << "2. Edit Other Accounts" << std::endl;
    std::cout << "3. " << Return_to_Account_Management_Menu_Label << std::endl;
    std::cout << Please_Select_an_Option_Label << " (1-3): ";
}

void Display_Personal_Edit_Options_Menu_UI() {
	std::cout << "Edit Options:" << std::endl;
	std::cout << "1. Edit " << Address_Label << std::endl;
	std::cout << "2. Edit " << Email_Address_Label << std::endl;
    std::cout << "3. Edit " << Phone_Number_Label << std::endl;
    std::cout << "4. Edit " << Password_Label << std::endl;
    std::cout << "Please select an edit option (1-4): ";
}

void Display_Selected_User_Edit_Options_Menu_UI() {
	std::cout << "Edit Options:" << std::endl;
    std::cout << "1. Edit " << Account_Type_Label << std::endl;
    std::cout << "2. Edit " << First_Name_Label << std::endl;
    std::cout << "3. Edit " << Middle_Name_Label << std::endl;
    std::cout << "4. Edit " << Last_Name_Label << std::endl;
    std::cout << "5. Edit " << Date_of_Birth_Label << std::endl;
    std::cout << "6. Edit " << Address_Label << std::endl;
    std::cout << "7. Edit " << Email_Address_Label << std::endl;
    std::cout << "8. Edit " << Phone_Number_Label << std::endl;
	std::cout << "9. Edit " << Business_Department_Label << std::endl;
    std::cout << "10. Edit " << Job_Role_Label << std::endl;
    std::cout << "11. Edit " << Password_Label << std::endl;
    std::cout << "12. Unlock Locked Account" << std::endl;
    std::cout << "13. " << Return_to_Account_Management_Main_Menu_Label << std::endl;
}

void Display_Selected_User_Edit_Options_Input_Prompt_UI() {
	std::cout << "Please select an edit option (1-13): ";
}

void Display_Basic_Employee_Details_Edit_Menu_UI() {
	std::cout << "Employee Record Edit Options:" << std::endl;
	std::cout << "1. Update All Employee Details" << std::endl;
	std::cout << "2. Edit One Employee Detail" << std::endl;
	std::cout << "3. " << Return_to_Previous_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-3): ";
}

void Display_Full_Employee_Details_Edit_Menu_UI() {
	std::cout << "Employee Record Edit Options:" << std::endl;
	std::cout << "1. Update All Employee Details" << std::endl;
	std::cout << "2. Edit One Employee Detail" << std::endl;
	std::cout << "3. Edit Employee Tax Information" << std::endl;
	std::cout << "4. " << Return_to_Previous_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-4): ";
}

void Display_Employee_Details_Edit_Field_Menu_UI() {
	std::cout << "Select employee detail field to edit:" << std::endl;
	std::cout << "1. " << Emergency_Contact_Label << std::endl;
	std::cout << "2. " << Diagnosed_Health_Conditions_Label << std::endl;
	std::cout << "3. " << Allergies_Label << std::endl;
	std::cout << "4. " << Approved_Emergency_Steps_and_Medicines_Label << std::endl;
	std::cout << "5. " << Prior_Training_and_Qualifications_Label << std::endl;
	std::cout << "6. " << First_Aid_Certification_Label << std::endl;
	std::cout << "7. " << Return_to_Employee_Record_Edit_Options_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-7): ";
}

void Display_Employee_Detail_Input_Prompt_UI(const std::string& detail_label) {
	std::cout << "Please enter " << detail_label << ": ";
}

void Display_Edit_Current_User_Account_Details_UI(const std::map<std::string, std::string>& account_details) {
    std::cout << "Edit " << Current_User_Account_Label << std::endl;
    Display_Account_Details_Block(account_details);
    Display_Personal_Edit_Options_Menu_UI();
}

void Display_Edit_Selected_User_Account_Details_UI(const std::map<std::string, std::string>& account_details) {
    std::cout << "Edit " << Selected_User_Account_Label << std::endl;
    Display_Account_Details_Block(account_details);
    Display_Selected_User_Edit_Options_Menu_UI();
}
