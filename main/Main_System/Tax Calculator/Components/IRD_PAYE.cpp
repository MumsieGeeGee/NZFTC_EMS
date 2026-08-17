// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.
//
// Note:
// IRD Number - eg 123-456-789
// PAYE (Pay As You Earn)
// Pay Frequency (Weekly, Fortnightly, Monthly)
// PAYE Table Version (yearly IRD update) - eg 2024, 2025, 2026
// Extra PAYE Voluntary Deduction (optional) - eg Advanced PAYE Repayments, Voluntary Deductions, etc. (numeric value or 0 if none) 
// IRD Number = Database Identifier/Container "Employee Tax Information"

#include "IRD_PAYE.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/Info_Messages.h"
#include "../../UI/Shared_UI_Messaging/Success_Messages.h"
#include "../../UI/Shared_UI_Messaging/Warning_Messages.h"
#include "../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../../Account_Management/Account_Management/Save_Functions.h"
#include "../../Account_Management/Account_Management/Database.h"
#include <string>
#include <iostream>

std::string Confirmation_Status_to_String(confirmation_Status status) {
    switch (status) {
        case confirmation_Status::Unconfirmed:
            return "Unconfirmed";
        case confirmation_Status::Confirmed:
            return "Confirmed";
        default:
            return "Unknown";
    }
}

std::string Pay_Frequency_to_String(pay_Frequency frequency) {
    switch (frequency) {
        case pay_Frequency::Weekly:
            return "Weekly";
        case pay_Frequency::Fortnightly:
            return "Fortnightly";
        case pay_Frequency::Monthly:
            return "Monthly";
        default:
            return "Unknown";
    }
}

void Get_New_IRD_Number(std::string& IRD_Number) {
    while (true) {
        std::string input;
        std::cout << IRD_Number_Input_Prompt << ": ";
        std::cout << IRD_Number_Label;
        std::getline(std::cin, input);

        if (!Is_IRD_Number_Format(input)) {
            std::cout << Invalid_IRD_Number << std::endl;
            continue;
        }

        if (IRD_Record_Exists_In_Master_Folder(input)) {
            std::cout << IRD_Number_Already_Exists << std::endl;
            continue;
        }

        IRD_Number = input;
        return;
    }
}

void Get_Existing_IRD_Number(std::string& IRD_Number) {
    while (true) {
        std::string input;
        std::cout << IRD_Number_Input_Prompt << ": ";
        std::cout << IRD_Number_Label;
        std::getline(std::cin, input);

        if (!Is_IRD_Number_Format(input)) {
            std::cout << IRD_Number_Invalid_Format << std::endl;
            continue;
        }

        if (!IRD_Record_Exists_In_Master_Folder(input)) {
            std::cout << IRD_Tax_Information_Not_Found << std::endl;
            continue;
        }

        IRD_Number = input;
        return;
    }
}

void Register_IRD_Number() {
    std::string input;
    Get_New_IRD_Number(input);

    if (!Save_IRD_Number(input)) {
        std::cout << IRD_Tax_Information_Save_Failure << std::endl;
        return;
    }
    if (!Create_IRD_Record_File(input)) {
        std::cout << IRD_Tax_Information_Save_Failure << std::endl;
        return;
    }
    std::cout << Display_Success << IRD_Tax_Information_Saved_Success_Message << std::endl;
    std::cout << Display_Warning << Begin_Account_Creation_Warning << std::endl;
    std::cout << Unconfirmed_IRD_Number_Requires_Secondary_Auth << std::endl;
}

void Get_Pay_Frequency(const std::string& IRD_Number) {
    while (true) {
        std::string input;
        std::cout << Pay_Frequency_Input_Prompt << ' ';
        std::cout << PAYE_Frequency_Label;
        std::getline(std::cin, input);

        pay_Frequency frequency;
        if (input == "Weekly") {
            frequency = pay_Frequency::Weekly;
        } else if (input == "Fortnightly") {
            frequency = pay_Frequency::Fortnightly;
        } else if (input == "Monthly") {
            frequency = pay_Frequency::Monthly;
        } else {
            std::cout << Display_Error << Invalid_Pay_Frequency << std::endl;
            continue;
        }

        if (!Save_PAYE_Frequency(IRD_Number, Pay_Frequency_to_String(frequency))) {
            return;
        }

        return;
    }
}

void Get_PAYE_Table_Version(const std::string& IRD_Number) {
    while (true) {
        std::string input;
        std::cout << PAYE_Table_Version_Input_Prompt << ' ';
        std::cout << PAYE_Table_Version_Label;
        std::getline(std::cin, input);

        if (input.empty()) {
            std::cout << Display_Error << Invalid_Empty_Input << " " << Invalid_PAYE_Table_Version << std::endl;
            continue;
        }
        if (!Is_Numeric_Only(input)) {
            std::cout << Display_Error << Invalid_Numerical_Input << " " << Invalid_PAYE_Table_Version << std::endl;
            continue;
        }
        if (input.length() != 4) {
            std::cout << Display_Error << Invalid_PAYE_Table_Version << std::endl;
            continue;
        }
        if (!Is_Valid_Year(input)) {
            std::cout << Display_Error << Invalid_PAYE_Table_Version << std::endl;
            continue;
        }
        if (!Save_PAYE_Table_Version(IRD_Number, input)) {
            return;
        }

        return;
    }
}

void Get_Extra_PAYE_Voluntary_Deduction(const std::string& IRD_Number) {
    while (true) {
        std::string input;
        std::cout << Extra_PAYE_Voluntary_Deduction_Input_Prompt << ' ';
        std::cout << PAYE_Extra_Voluntary_Deductions_Label;
        std::getline(std::cin, input);

        if (input.empty()) {
            std::cout << Display_Error << Invalid_Empty_Input << " " << Invalid_Extra_PAYE_Voluntary_Deduction << std::endl;
            continue;
        }

        if (input == "none" || input == "None" || input == "NONE") {
            input = "0";
        }

        if (!Is_Numeric_Only(input)) {
            std::cout << Display_Error << Invalid_Numerical_Input << " " << Invalid_Extra_PAYE_Voluntary_Deduction << std::endl;
            continue;
        }

        if (!Save_Extra_PAYE_Voluntary_Deduction(IRD_Number, input)) {
            return;
        }

        return;
    }
}

bool Valid_Pay_Cycle(const std::string& start, const std::string& end) {
    auto parse = [](const std::string& date) {
        int day = std::stoi(date.substr(0, 2));
        int month = std::stoi(date.substr(3, 2));
        int year = std::stoi(date.substr(6, 4));
        return std::tuple<int,int,int>(day, month, year);
    };

    auto [d1, m1, y1] = parse(start);
    auto [d2, m2, y2] = parse(end);
    // Convert to rough day count (no leap-year precision needed)
    auto toDays = [](int d, int m, int y) {
        return y * 365 + m * 30 + d;
    };

    int diff = toDays(d2, m2, y2) - toDays(d1, m1, y1);
    // Weekly minimum = 7 days
    if (diff < 7) {
        std::cout << Display_Error << Invalid_Pay_Period_Too_Short << std::endl;
        return false;
    }
    // Monthly maximum = 31 days
    if (diff > 31) {
        std::cout << Display_Error << Invalid_Pay_Period_Too_Long << std::endl;
        return false;
    }

    return true;
}

bool Valid_Pay_Period_for_Payslips(const std::string& input) {
    if (!Is_Numeric_accepts_Symbols(input)) {
        std::cout << Display_Error << Invalid_Pay_Period_Invalid_Characters << std::endl;
        return false;
    }
    // Must contain "to"
    if (input.find("to") == std::string::npos) {
        std::cout << Display_Error << Invalid_Pay_Period_Invalid_Format << std::endl;
        return false;
    }
    std::size_t pos = input.find("to");
    std::string start = Trim_Copy(input.substr(0, pos));
    std::string end   = Trim_Copy(input.substr(pos + 2));

    if (!Is_Date_Format(start) || !Is_Date_Format(end)) {
        std::cout << Display_Error << Invalid_Date_Format << std::endl;
        return false;
    }
    // Validate year >= 2025
    if (!Is_Valid_Year(start.substr(6)) || !Is_Valid_Year(end.substr(6))) {
        std::cout << Display_Error << Invalid_Pay_Period_Invalid_Dates << std::endl;
        return false;
    }
    // Validate pay cycle length
    if (!Valid_Pay_Cycle(start, end)) {
        return false;
    }

    return true;
}
