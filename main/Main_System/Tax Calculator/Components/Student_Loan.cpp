// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

// Student Loan
// SL Suffix â€“ Student Loan Required - Set  Via Tax_Code functions
// Default Rate (IRD)
// Special Deduction Rate (IRD letter)
// Repayment Exemption (rare)

#include "Student_Loan.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Info_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include "../../Account_Management/Account_Management/Database.h"
#include "../../Account_Management/Account_Management/Save_Functions.h"
#include "../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>

namespace {
bool Try_Read_Employer_Details_Value(const std::string& fieldLabel, std::string& outValue) {
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

std::string Extract_Digits(const std::string& rawValue) {
    std::string digitsOnly;
    for (char c : rawValue) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            digitsOnly.push_back(c);
        }
    }
    return digitsOnly;
}
}

// Get Student Loan Existence
bool Get_Student_Loan_Existence(const std::string& IRD_Number) {
    std::string input;
    std::cout << Student_Loan_Existence_Input_Prompt << ' ';
    std::cout << Yes_No_Question_Input_Label;
    std::getline(std::cin, input);
    if (!Yes_or_No(input)) {
        std::cout << Display_Error << Yes_No_Input << std::endl;
        return false;
    }
    if (Yes_Or_No_Input_to_String(input) == "No") {
        if (!Save_Student_Loan_Existence(IRD_Number, "No")) {
            std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            return false;
        }
        std::cout << Display_Info << Student_Loan_Existence_Skip_Message << std::endl;
        return false;
    }
    if (Yes_Or_No_Input_to_String(input) == "Yes") {
        if (!Save_Student_Loan_Existence(IRD_Number, "Yes")) {
            std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            return false;
        }
        std::cout << Display_Info << Student_Loan_Existence_Continue_to_Questions << std::endl;
        return true;
    }
    // when "no/No/NO/n/N" is selected, skip all remaining Student Loan Repayment questions
    return false;
}
// Get Student Loan SDR IRD Letter Existence
bool Get_Student_Loan_SDR_IRD_Letter_Existence(const std::string& IRD_Number) {
    std::string input;
    std::cout << Student_Loan_SDR_IRD_Letter_Existence_Input_Prompt << ' ';
    std::cout << Yes_No_Question_Input_Label;
    std::getline(std::cin, input);
    if (!Yes_or_No(input)) {
        std::cout << Display_Error << Yes_No_Input << std::endl;
        return false;
    }
    if (Yes_Or_No_Input_to_String(input) == "No") {
        std::cout << Display_Info << Student_Loan_SRD_IRD_Letter_Existence_None_Message << std::endl;
        if (!Save_Student_Loan_SDR_IRD_Letter_Existence(IRD_Number, "No")) {
            std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            return false;
        }
        return false;
    }
    if (Yes_Or_No_Input_to_String(input) == "Yes") {
        std::cout << Display_Info << Student_Loan_SRD_IRD_Letter_Yes_Continue_to_Questions << std::endl;
        if (!Save_Student_Loan_SDR_IRD_Letter_Existence(IRD_Number, "Yes")) {
            std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            return false;
        }
        return true;
    }
    // when "no/No/NO/n/N" is selected, skip all remaining Student Loan Repayment questions
    return false;
}
// Get Student Loan Special Deduction Rate
bool Get_Student_Loan_Special_Deduction_Rate(const std::string& IRD_Number) {
    std::string input;
    std::cout << Student_Loan_Special_Deduction_Rate_Input_Prompt << ' ';
    std::getline(std::cin, input);
    if (!Is_Decimal_Number(input)) {
        std::cout << Display_Error << Invalid_Decimal_Input << std::endl;
        return false;
    }
    if (!Save_Student_Loan_Special_Deduction_Rate(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return false;
    }
    return true;
}
// Get Student Loan Repayment Exemption Existence
bool Get_Student_Loan_Repayment_Exemption_Existence(const std::string& IRD_Number) {
    std::string input;
    std::cout << Student_Loan_Repayment_Exemption_Existence_Input_Question << std::endl;
    std::cout << Yes_No_Question_Input_Label;
    std::getline(std::cin, input);
    if (!Yes_or_No(input)) {
        std::cout << Display_Error << Yes_No_Input << std::endl;
        return false;
    }
    if (Yes_Or_No_Input_to_String(input) == "No") {
        std::cout << Display_Info << Student_Loan_Repayment_Exemption_Existence_Skip_Message << std::endl;
        if (!Save_Student_Loan_Repayment_Exemption_Existence(IRD_Number, "No")) {
            std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            return false;
        }
        return false;
    }
    if (Yes_Or_No_Input_to_String(input) == "Yes") {
        std::cout << Display_Info << Student_Loan_Existence_Continue_to_Questions << std::endl;
        if (!Save_Student_Loan_Repayment_Exemption_Existence(IRD_Number, "Yes")) {
            std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
            return false;
        }
        return true;
    }
    // when "no/No/NO/n/N" is selected, skip all remaining Student Loan Repayment questions
    return false;
}
// Get Student Loan Repayment Exemption Reason
bool Get_Student_Loan_Repayment_Exemption_Reason(const std::string& IRD_Number) {
    std::string input;
    std::cout << Student_Loan_Repayment_Exemption_Reason_Input_Prompt << ' ';
    std::cout << "Options: " << std::endl;
    std::cout << "1. " << Student_Loan_Repayment_Exemption_Reason_Full_Time_Study_Low_Income_Option << std::endl;
    std::cout << "2. " << Student_Loan_Repayment_Exemption_Reason_Serious_Hardship_Option << std::endl;
    std::cout << "3. " << Student_Loan_Repayment_Exemption_Reason_Multiple_Jobs_Uneven_Deductions_Option << std::endl;
    std::cout << "4. " << Student_Loan_Repayment_Exemption_Reason_Adverse_Emergency_Event_Option << std::endl;
    std::getline(std::cin, input);
    switch (input[0]) {
        case '1':
            input = Student_Loan_Repayment_Exemption_Reason_Full_Time_Study_Low_Income_Option;
            Save_Student_Loan_Repayment_Exemption_Reason(IRD_Number, input);
            break;
        case '2':
            input = Student_Loan_Repayment_Exemption_Reason_Serious_Hardship_Option;
            Save_Student_Loan_Repayment_Exemption_Reason(IRD_Number, input);
            break;
        case '3':
            input = Student_Loan_Repayment_Exemption_Reason_Multiple_Jobs_Uneven_Deductions_Option;
            Save_Student_Loan_Repayment_Exemption_Reason(IRD_Number, input);
            break;
        case '4':
            input = Student_Loan_Repayment_Exemption_Reason_Adverse_Emergency_Event_Option;
            Save_Student_Loan_Repayment_Exemption_Reason(IRD_Number, input);
            break;
        default:
            std::cout << Display_Error << Invalid_Student_Loan_Repayment_Exemption_Reason << std::endl;
            return false;
    }

    return true;

}
// Get Student Loan Repayment Exemption Expiry Date
bool Get_Student_Loan_Repayment_Exemption_Expiry_Date(const std::string& IRD_Number) {
    std::string input;
    std::cout << Student_Loan_Repayment_Exemption_Expiry_Date_Input_Prompt << ' ';
    std::cout << Student_Loan_Repayment_Exemption_Expiry_Date_Label;
    std::getline(std::cin, input);
    if (!Is_Valid_Year(input)) {
        std::cout << Display_Error << Invalid_Year << std::endl;
        return false;
    }
    if (!Is_Date_Format(input)) {
        std::cout << Display_Error << Invalid_Date_Format << std::endl;
        return false;
    }
    if (!Save_Student_Loan_Repayment_Exemption_Expiry_Date(IRD_Number, input)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return false;
    }

    return true;
}
// Get Student Loan Repayment Threshold
bool Get_Student_Loan_Repayment_Threshold(const std::string& IRD_Number) {
    std::cout << Student_Loan_Repayment_Threshold_Message << std::endl;
    if (!Is_Student_Loan_Active(IRD_Number)) {
        return false;
    }

    std::string thresholdValueRaw;
    if (!Try_Read_Employer_Details_Value(Student_Loan_Repayment_Threshold_Label, thresholdValueRaw)) {
        std::cout << Display_Error << Employer_Financial_Details_Not_Found << std::endl;
        return false;
    }

    const std::string thresholdValue = Extract_Digits(thresholdValueRaw);
    if (!Is_Numeric_Only(thresholdValue)) {
        std::cout << Display_Error << Invalid_Numerical_Input << std::endl;
        return false;
    }

    if (!Save_Student_Loan_Repayment_Threshold(IRD_Number, thresholdValue)) {
        std::cout << Display_Error << Employee_Tax_Information_Save_Failure << std::endl;
        return false;
    }

    return true;
}
// Get Student Loan Repayment Rate
bool Get_Student_Loan_Repayment_Rate(const std::string& IRD_Number) {
    if (!Is_Student_Loan_Active(IRD_Number)) {
        return false;
    }

    std::string input;
    std::cout << Student_Loan_Repayment_Rate_Message << std::endl;
    std::cout << Student_Loan_Repayment_Rate_Label;
    std::getline(std::cin, input);
    if (!Is_Numeric_Only(input)) {
        std::cout << Display_Error << Invalid_Student_Loan_Repayment_Rate << std::endl;
        return false;
    }

    return true;
}

