// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

// Primary Income Tax Codes
// M â€“ Standard
// ME â€“ Standard, no student loan
// Student Loan Tax Codes Handled in Student Loan Component

// Secondary Income Tax Codes
// SB â€“ Secondary Low Bracket (< $14,000)
// S  â€“ Secondary Standard ($14,000â€“$48,000)
// SH â€“ Secondary Higher Bracket ($48,000â€“$70,000)
// ST â€“ Secondary Top Bracket ($70,000â€“$180,000)
// SA â€“ Secondary Additional Top Bracket (> $180,000)

// Special Tax Codes and Exemptions
// STC â€“ Special Tax Code (requires IRD letter)
// CAE â€“ Casual Agricultural Employee
// EDW â€“ Election Day Worker
// NSW â€“ Non-resident Seasonal Worker
// WT â€“ Schedular Payments (Contractors)

// Other Tax Code Specific Functions
// STC Rate (percentage)
// STC Letter Received (bool)

#include "Tax_Codes.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/Success_Messages.h"
#include "../../Account_Management/Account_Management/Database.h"
#include "../../Account_Management/Account_Management/Save_Functions.h"
#include "../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include <string>
#include <iostream>

void Get_Tax_Code_By_Job_Type(const std::string& IRD_Number) {
    std::cout << Tax_Codes_Input_Prompt << ' ';
    std::cout << Tax_Codes_Job_Type_Input_Prompt;

    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << Display_Error << Invalid_Empty_Input << std::endl;
            continue;
        }

        if (input == "Primary" || input == "primary" || input == "PRIMARY") {
            if (!Save_Tax_Code_Job_Type(IRD_Number, "Primary")) {
                std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
                continue;
            }
            Get_Primary_Tax_Code(IRD_Number);
            return;
        }

        if (input == "Secondary" || input == "secondary" || input == "SECONDARY") {
            if (!Save_Tax_Code_Job_Type(IRD_Number, "Secondary")) {
                std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
                continue;
            }
            Get_Secondary_Tax_Code(IRD_Number);
            return;
        }

        std::cout << Display_Error << Invalid_Tax_Codes_Job_Type_Input << std::endl;
    }
}

void Get_Tax_Code_Job_Type(const std::string& IRD_Number) {
    std::cout << Tax_Codes_Input_Prompt << ' ';
    std::cout << Tax_Codes_Job_Type_Input_Prompt;
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << Display_Error << Invalid_Empty_Input << std::endl;
            continue;
        }
        if (input != 
            "Primary" && input != 
            "primary" && input != 
            "PRIMARY" && input != 
            "Secondary" && input != 
            "secondary" && input != 
            "SECONDARY") {
            std::cout << Display_Error << Invalid_Tax_Codes_Job_Type_Input << std::endl;
            continue;
        }
        if (!Save_Tax_Code_Job_Type(IRD_Number, input)) {
            std::cout << Display_Error << IRD_Tax_Information_Save_Failure << std::endl;
            continue;
        }
        std::cout << Display_Success << IRD_Tax_Information_Saved_Success_Message << std::endl;
        break;
    }
}

void Get_Primary_Tax_Code(const std::string& IRD_Number) {
    std::cout << Primary_Tax_Codes_Input_Guide1 << std::endl;
    std::cout << Primary_Tax_Codes_Input_Guide2 << std::endl;
    std::cout << Primary_Tax_Codes_Input_Guide3 << std::endl;
    std::cout << Primary_Tax_Codes_List << std::endl;
    std::string input;
    while (true) {
        std::cout << Primary_Tax_Code_Input_Prompt << ' ';
        std::cout << Primary_Tax_Code_Label;
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << Display_Error << Invalid_Empty_Input << std::endl;
            continue;
        }
        if (input != 
            "M" && input != 
            "M SL" && input != 
            "ME" && input != 
            "SL" && input != 
            "M" && input != 
            "m sl" && input != 
            "me" && input != 
            "sl" && input != 
            "m") {
            std::cout << Display_Error << Invalid_Primary_Tax_Code << std::endl;
            continue;
        }
        if (!Save_Primary_Tax_Code(IRD_Number, input)) {
            std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            continue;
        }
        break;
    }
}

void Get_Secondary_Tax_Code(const std::string& IRD_Number) {
    std::string input;
    while (true) {
        std::cout << Secondary_Tax_Code_Input_Prompt << ' ';
        std::cout << Secondary_Tax_Code_Label;
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << Display_Error << Invalid_Empty_Input << std::endl;
            continue;
        }
        if (input != 
            "SB" && input  != 
            "sb" && input != 
            "S" && input != 
            "s" && input != 
            "SH" && input != 
            "sh" && input != 
            "ST" && input != 
            "st" && input != 
            "SA" && input != 
            "sa") {
            std::cout << Display_Error << Invalid_Secondary_Tax_Code << std::endl;
            continue;
        }
        if (!Save_Secondary_Tax_Code(IRD_Number, input)) {
            std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            continue;
        }
        break;
    }
}

void Get_Special_Tax_Code(const std::string& IRD_Number) {
    std::string input;
    while (true) {
        std::cout << Special_Tax_Code_Input_Prompt << ' ';
        std::cout << Special_Tax_Code_Label;
        std::getline(std::cin, input);
        if (input.empty()) {
            std::cout << Display_Error << Invalid_Empty_Input << std::endl;
            continue;
        }
        if (input != 
            "STC" && input != 
            "stc" && input !=
            "CAE" && input !=
            "cae" && input != 
            "EDW" && input !=
            "edw" && input != 
            "NSW" && input !=
            "nsw" && input != 
            "WT" && input != 
            "wt" && input != 
            "none" && input != 
            "None" && input != 
            "NONE") {
            std::cout << Display_Error << Invalid_Special_Tax_Code << std::endl;
            continue;
        }

        if (input == "none" || input == "NONE") {
            input = "None";
        }

        if (!Save_Special_Tax_Code(IRD_Number, input)) {
            std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            continue;
        }
        break;
    }
}

void Get_STC_Rate(const std::string& IRD_Number) {
    std::cout << STC_Rate_Input_Prompt << ' ';
    std::cout << STC_Rate_Label;

    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) {
        std::cout << Display_Error << Invalid_Empty_Input << std::endl;
        return;
    }
    if (!Is_Numeric_Only(input)) {
        std::cout << Display_Error << Invalid_STC_Rate << std::endl;
        return;
    }
    if (!Save_STC_Rate(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

void Get_STC_Letter_Received(const std::string& IRD_Number) {
    std::cout << STC_Letter_Received_Input_Prompt << ' ';
    std::cout << STC_Letter_Received_Label;

    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) {
        std::cout << Display_Error << Invalid_Empty_Input << std::endl;
        return;
    }
    if (!Yes_or_No(input)) {
        std::cout << Display_Error << Yes_No_Input << std::endl;
        return;
    }
    if (!Save_STC_Letter_Received(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

void Get_STC_Expiry_Date(const std::string& IRD_Number) {
    std::cout << STC_Expiry_Date_Input_Prompt << ' ';
    std::cout << STC_Expiry_Date_Label;

    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) {
        std::cout << Display_Error << Invalid_Empty_Input << std::endl;
        return;
    }
    if (!Is_Date_Format(input)) {
        std::cout << Display_Error << Invalid_Date_Format << std::endl;
        return;
    }
    if (!Save_STC_Expiry_Date(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}


