// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Kiwisaver.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include "../../Account_Management/Account_Management/Save_Functions.h"
#include "../../Account_Management/Account_Management/Database.h"
#include "../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include <string>
#include <iostream>
#include <fstream>

// Helper function to read a specific field from the shared employer details file.
bool Read_Employer_Details_Value(const std::string& fieldLabel, std::string& outValue) {
    std::ifstream file(Employer_Details_File_Path);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind(fieldLabel, 0) == 0) {
            outValue = Trim_Copy(line.substr(fieldLabel.length()));
            return true;
        }
    }

    return false;
}
// Helper function to get the KiwiSaver employer contribution rate from the shared employer details file.
bool Get_KiwiSaver_Employer_Contribution_Rate_From_Employer_Details(std::string& outRate) {
    return Read_Employer_Details_Value("KiwiSaver Employer Contribution Rate: ", outRate)
        && !Trim_Copy(outRate).empty();
}
// Normalize "(default)" marker in the employer contribution rate
std::string Normalize_Default_Marker(const std::string& rawValue) {
    std::string value = Trim_Copy(rawValue);

    const std::size_t lowerMarkerPos = value.find("(default)");
    if (lowerMarkerPos != std::string::npos) {
        value.erase(lowerMarkerPos, std::string("(default)").length());
    }

    return Trim_Copy(value);
}
// Opt In / Opt Out Status
void Get_KiwiSaver_Opt_In_Out_Status(const std::string& IRD_Number) {
    std::string input;
    std::cout << KiwiSaver_Opt_In_Out_Status_Input_Prompt << ' ';
    std::cout << KiwiSaver_Opt_In_Out_Status_Label;
    getline(std::cin, input);
    if (input != "opt-in" && input != "Opt-In" && input != "OPT-IN" && input != "opt-out" && input != "Opt-Out" && input != "OPT-OUT") {
        std::cout << Display_Error << Invalid_KiwiSaver_Opt_In_Out_Status << std::endl;
        return;
    }
    if (!Save_KiwiSaver_Opt_In_Out_Status(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }

    if (input == "opt-in" || input == "Opt-In" || input == "OPT-IN") {
        Get_KiwiSaver_Employer_Contribution_Rate(IRD_Number);
    }
}
// Valid KiwiSaver Contribution Rates (as percentages)
bool Valid_Kiwisaver_Contribution_Rates(std::string& input) {
    if (input == "3%" || input == "4%" || input == "5%" || input == "6%" || input == "7%" || input == "8%" || input == "9%" || input == "10%") {
        return true;
    }

    return false;
}
// Normalize KiwiSaver Contribution Rate Input
bool Normalize_KiwiSaver_Contribution_Rate_Input(std::string& input) {
    input = Trim_Copy(input);
    if (input.empty()) {
        return false;
    }

    if (!input.empty() && input.back() == '%') {
        input.pop_back();
        input = Trim_Copy(input);
    }

    if (!Is_Decimal_Number(input)) {
        return false;
    }

    if (Valid_Kiwisaver_Contribution_Rates(std::string(input + "%"))) {
        return false;
    }

    input += "%";
    return true;
}

// Employee Contribution Rate
void Get_KiwiSaver_Employee_Contribution_Rate(const std::string& IRD_Number) {
    std::string input;
    std::cout << KiwiSaver_Employee_Contribution_Rate_Input_Prompt << ' ';
    std::cout << KiwiSaver_Employee_Contribution_Rate_Label;
    getline(std::cin, input);

    if (!Normalize_KiwiSaver_Contribution_Rate_Input(input)) {
        std::cout << Display_Error << Invalid_KiwiSaver_Employee_Contribution_Rate << std::endl;
        return;
    }

    if (!Save_KiwiSaver_Employee_Contribution_Rate(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}
// Employer Contribution Rate
void Get_KiwiSaver_Employer_Contribution_Rate(const std::string& IRD_Number) {
    std::string employerRate;
    if (!Get_KiwiSaver_Employer_Contribution_Rate_From_Employer_Details(employerRate)) {
        std::cout << Display_Error << Employer_Financial_Details_Not_Found << std::endl;
        return;
    }

    employerRate = Normalize_Default_Marker(employerRate);
    if (employerRate.empty()) {
        std::cout << Display_Error << Employer_Financial_Details_Not_Found << std::endl;
        return;
    }

    if (!Save_KiwiSaver_Employer_Contribution_Rate(IRD_Number, employerRate)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}
// Opt-Out Date (if applicable)
void Get_KiwiSaver_Opt_Out_Date(const std::string& IRD_Number) {
    std::string input;
    std::cout << KiwiSaver_Opt_Out_Date_Input_Prompt << ' ';
    std::cout << KiwiSaver_Opt_Out_Date_Label;
    getline(std::cin, input);
    if (!Is_Date_Format(input)) {
        std::cout << Display_Error << Invalid_KiwiSaver_Opt_Out_Date << std::endl;
        return;
    }
    if (!Save_KiwiSaver_Opt_Out_Date(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

// tbd KiwiSaver Employer Contribution Rate Validation - shared employer details file database
