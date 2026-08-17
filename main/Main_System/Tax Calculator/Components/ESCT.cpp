// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "ESCT.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include "../../Account_Management/Account_Management/Database.h"
#include "../../Account_Management/Account_Management/Save_Functions.h"
#include "../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include <string>
#include <iostream>

namespace {
std::string Prompt_Employee_Extra_Type() {
    while (true) {
        std::string selection;
        std::cout << ESCT_Income_Band_Input_Prompt << ' ';
        std::cout << ESCT_Income_Band_Label;
        std::getline(std::cin, selection);

        // implement a mapping of selection to extra type - tbc

        std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
    }
}

std::string Prompt_Employee_Extra_Classification() {
    while (true) {
        std::string classification;
        std::cout << "Is this a benefit or a deduction? (benefit/deduction)" << std::endl;
        std::getline(std::cin, classification);

        if (classification == "benefit" || classification == "Benefit" || classification == "BENEFIT") {
            return "Benefit";
        }
        if (classification == "deduction" || classification == "Deduction" || classification == "DEDUCTION") {
            return "Deduction";
        }

        std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
    }
}
}

// Employer Superannuation Contribution Tax (ESCT)
void Get_ESCT_Existence(const std::string& IRD_Number) {
    while (true) {
        std::string input;
        std::cout << ESCT_Existence_Input_Prompt << ' ';
        std::cout << ESCT_Existence_Label;
        std::getline(std::cin, input);

        std::string normalized;
        if (Is_Yes_Input(input) || input == "active" || input == "Active" || input == "ACTIVE") {
            normalized = "active";
        } else if (Is_No_Input(input) || input == "inactive" || input == "Inactive" || input == "INACTIVE") {
            normalized = "inactive";
        } else {
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            continue;
        }

        if (!Save_ESCT_Existence(IRD_Number, normalized)) {
            std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            return;
        }

        if (normalized == "inactive") {
            return;
        }

        const std::string extraType = Prompt_Employee_Extra_Type();
        const std::string extraClassification = Prompt_Employee_Extra_Classification();
        if (!Save_ESCT_Income_Band(IRD_Number, extraType + " | " + extraClassification)) {
            std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            return;
        }

        while (true) {
            std::string rateInput;
            std::cout << ESCT_Tax_Rate_Input_Prompt << ' ';
            std::cout << ESCT_Tax_Rate_Label;
            std::getline(std::cin, rateInput);

            if (!Is_Decimal_Number(rateInput)) {
                std::cout << Display_Error << Invalid_ESCT_Tax_Rate << std::endl;
                continue;
            }

            if (!Save_ESCT_Tax_Rate(IRD_Number, rateInput)) {
                std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            }

            break;
        }

        return;
    }
}
// ESCT Rate (10.5%, 17.5%, 30%, 33%, 39%)
void Get_ESCT_Tax_Rate(const std::string& IRD_Number) {
    while (true) {
        std::string input;
        std::cout << ESCT_Tax_Rate_Input_Prompt << ' ';
        std::cout << ESCT_Tax_Rate_Label;
        std::getline(std::cin, input);
        if (!Is_Decimal_Number(input)) {
            std::cout << Display_Error << Invalid_ESCT_Tax_Rate << std::endl;
            continue;
        }
        if (!Save_ESCT_Tax_Rate(IRD_Number, input)) {
            std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            return;
        }
        return;
    }
}
// ESCT Income Band
void Get_ESCT_Income_Band(const std::string& IRD_Number) {
    const std::string extraType = Prompt_Employee_Extra_Type();
    const std::string extraClassification = Prompt_Employee_Extra_Classification();
    if (!Save_ESCT_Income_Band(IRD_Number, extraType + " | " + extraClassification)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

// tbd ESCT Tax Rate Validation - shared employer details file database
// tbd ESCT Income Band Validation - shared employer details file database
