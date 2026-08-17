// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Child_Support.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include "../../Account_Management/Account_Management/Database.h"
#include "../../Account_Management/Account_Management/Save_Functions.h"
#include "../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include <string>
#include <iostream>

void Get_Child_Support_Status(const std::string& IRD_Number) {
    std::string input;
    std::cout << Child_Support_Status_Input_Prompt << ' ';
    std::cout << Child_Support_Status_Label;
    getline(std::cin, input);
    if (input != "active" && input != "inactive") {
        std::cout << Display_Error << Invalid_Child_Support_Status << std::endl;
        return;
    }
    if (!Save_Child_Support_Status(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

void Get_Child_Support_Standard_Deduction(const std::string& IRD_Number) {
    std::string input;
    std::cout << Child_Support_Standard_Deduction_Input_Prompt << ' ';
    std::cout << Child_Support_Standard_Deduction_Label;
    getline(std::cin, input);
    if (!Is_Decimal_Number(input)) {
        std::cout << Display_Error << Invalid_Child_Support_Standard_Deduction << std::endl;
        return;
    }
    if (!Save_Child_Support_Standard_Deduction(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

void Get_Child_Support_Voluntary_Deduction(const std::string& IRD_Number) {
    std::string input;
    std::cout << Child_Support_Voluntary_Deduction_Input_Prompt << ' ';
    std::cout << Child_Support_Voluntary_Deduction_Label;
    getline(std::cin, input);
    if (!Is_Decimal_Number(input)) {
        std::cout << Display_Error << Invalid_Child_Support_Voluntary_Deduction << std::endl;
        return;
    }
    if (!Save_Child_Support_Voluntary_Deduction(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

void Get_Child_Support_Protected_Net_Earnings(const std::string& IRD_Number) {
    std::string input;
    std::cout << Child_Support_Protected_Net_Earnings_Input_Prompt << ' ';
    std::cout << Child_Support_Protected_Net_Earnings_Label;
    getline(std::cin, input);
    if (!Is_Decimal_Number(input)) {
        std::cout << Display_Error << Invalid_Child_Support_Protected_Net_Earnings << std::endl;
        return;
    }
    if (!Save_Child_Support_Protected_Net_Earnings(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

void Get_Child_Support_Shortfall_Calculation(const std::string& IRD_Number) {
    if (!Is_Child_Support_Active(IRD_Number)) {
        return;
    }

    std::cout << Child_Support_Shortfall_Calculation_Input_Prompt << ' ';
    // All IRD/PAYE data must be entered before the shortfall calculation can be performed.
    // This function does not perform any calculations itself, but checks that the calculation has been completed and saved in the database.
    // TBD - once calculation logic allows
}

void Get_Child_Support_IRD_Notice_of_Deduction_Existence(const std::string& IRD_Number) {
    std::string input;
    std::cout << Child_Support_IRD_Notice_of_Deduction_Existence_Input_Prompt << ' ';
    std::cout << Yes_No_Question_Input_Label;
    getline(std::cin, input);
    if (!Yes_or_No(input)) {
        std::cout << Display_Error << Invalid_Child_Support_IRD_Notice_of_Deduction_Existence << std::endl;
        return;
    }
    if (!Save_Child_Support_IRD_Notice_of_Deduction_Existence(IRD_Number, Yes_Or_No_Input_to_String(input))) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

void Get_Child_Support_IRD_Notice_of_Deduction_Amount(const std::string& IRD_Number) {
    std::string input;
    std::cout << Child_Support_IRD_Notice_of_Deduction_Amount_Input_Prompt << ' ';
    std::cout << Child_Support_IRD_Notice_of_Deduction_Amount_Label;
    std::getline(std::cin, input);
    if (!Is_Decimal_Number(input)) {
        std::cout << Display_Error << Invalid_Child_Support_IRD_Notice_of_Deduction_Amount << std::endl;
        return;
    }
    if (!Save_Child_Support_IRD_Notice_of_Deduction_Amount(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

void Get_Child_Support_IRD_Notice_of_Deduction_Effective_Date(const std::string& IRD_Number) {
    std::string input;
    std::cout << Child_Support_IRD_Notice_of_Deduction_Effective_Date_Input_Prompt << ' ';
    std::cout << Child_Support_IRD_Notice_of_Deduction_Effective_Date_Label;
    std::getline(std::cin, input);
    if (!Is_Date_Format(input)) {
        std::cout << Display_Error << Invalid_Child_Support_IRD_Notice_of_Deduction_Effective_Date << std::endl;
        return;
    }
    if (!Is_Valid_Year(input)) {
        std::cout << Display_Error << Invalid_Child_Support_IRD_Notice_of_Deduction_Effective_Date << std::endl;
        return;
    }
    if (!Save_Child_Support_IRD_Notice_of_Deduction_Effective_Date(IRD_Number, input)) {
       std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

void Get_Child_Support_IRD_Notice_of_Deduction_Expiry_Date(const std::string& IRD_Number) {
    std::string input;
    std::cout << Child_Support_IRD_Notice_of_Deduction_Expiry_Date_Input_Prompt << ' ';
    std::cout << Child_Support_IRD_Notice_of_Deduction_Expiry_Date_Label;
    std::getline(std::cin, input);
    if (!Is_Date_Format(input)) {
        std::cout << Display_Error << Invalid_Child_Support_IRD_Notice_of_Deduction_Expiry_Date << std::endl;
        return;
    }
    if (!Is_Valid_Year(input)) {
        std::cout << Display_Error << Invalid_Child_Support_IRD_Notice_of_Deduction_Expiry_Date << std::endl;
        return;
    }
    if (!Save_Child_Support_IRD_Notice_of_Deduction_Expiry_Date(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

void Get_Child_Support_IRD_Notice_of_Deduction_Reference_Number(const std::string& IRD_Number) {
    std::string input;
    std::cout << Child_Support_IRD_Notice_of_Deduction_Reference_Number_Input_Prompt << ' ';
    std::cout << Child_Support_IRD_Notice_of_Deduction_Reference_Number_Label;
    std::getline(std::cin, input);
    if (input.empty()) {
        std::cout << Display_Error << Invalid_Child_Support_IRD_Notice_of_Deduction_Reference_Number << std::endl;
        return;
    }
    if (!Save_Child_Support_IRD_Notice_of_Deduction_Reference_Number(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

