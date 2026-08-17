// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

// Accident Compensation Corporation (ACC)
// ACC Earners Levy Rate
// Maximum Liable Earnings Threshold

#include "ACC.h"
#include "../../Account_Management/Account_Management/Database.h"
#include "../../Account_Management/Account_Management/Save_Functions.h"
#include "../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/Success_Messages.h"
#include <string>
#include <iostream>
#include <fstream>

void Get_ACC_Levy_Maximum(const std::string& IRD_Number);

namespace {
    std::string Format_ACC_Levy_Type_Display(const std::string& levyTypeName, const std::string& rateText) {
        return levyTypeName + " (" + rateText + ")";
    }

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

    bool Get_ACC_Levy_Type_From_Employer_Details(std::string& outLevyType) {
        const char* candidateLabels[] = {
            "ACC Levy Type: "
        };

        for (const char* label : candidateLabels) {
            if (Read_Employer_Details_Value(label, outLevyType) && !Trim_Copy(outLevyType).empty()) {
                return true;
            }
        }

        return false;
    }

    bool Get_ACC_Levy_Rate_From_Employer_Details(std::string& outLevyRate) {
        const char* candidateLabels[] = {
            "ACC Levy Rate: "
        };

        for (const char* label : candidateLabels) {
            if (Read_Employer_Details_Value(label, outLevyRate) && !Trim_Copy(outLevyRate).empty()) {
                return true;
            }
        }

        return false;
    }

    bool Get_ACC_Levy_Maximum_From_Employer_Details(std::string& outLevyMaximum) {
        const char* candidateLabels[] = {
            "ACC Levy Maximum: "
        };

        for (const char* label : candidateLabels) {
            if (Read_Employer_Details_Value(label, outLevyMaximum) && !Trim_Copy(outLevyMaximum).empty()) {
                return true;
            }
        }

        return false;
    }

    bool Parse_ACC_Levy_Type(const std::string& levyTypeText, ACC_Levy_Types& outLevyType) {
        const std::string trimmedLevyType = Trim_Copy(levyTypeText);
        if (trimmedLevyType == "Earners Levy Rate" || trimmedLevyType == "Earners' Levy Rate") {
            outLevyType = ACC_Levy_Types::Earners_Levy_Rate;
            return true;
        }

        if (trimmedLevyType == "Work Levy Rate" || trimmedLevyType == "Workplace Levy Rate") {
            outLevyType = ACC_Levy_Types::Work_Levy_Rate;
            return true;
        }

        return false;
    }
}
std::string ACC_Levy_Default_Rate_to_String(ACC_Levy_Types levyType) {
    switch (levyType) {
        case ACC_Levy_Types::Earners_Levy_Rate:
            return "default rate 1.75%";
        case ACC_Levy_Types::Work_Levy_Rate:
            return "custom rate required";
        default:
            return "rate unknown";
    }
}

std::string ACC_Levy_Types_to_String(ACC_Levy_Types levyType) {
    switch (levyType) {
        case ACC_Levy_Types::Earners_Levy_Rate:
            return Format_ACC_Levy_Type_Display("Earners' Levy Rate", ACC_Levy_Default_Rate_to_String(levyType));
        case ACC_Levy_Types::Work_Levy_Rate:
            return Format_ACC_Levy_Type_Display("Work Levy Rate", ACC_Levy_Default_Rate_to_String(levyType));
        default:
            return "Unknown ACC Levy Type";
    }
}

void Get_ACC_Levy_Types(const std::string& IRD_Number) {
    if (!Auto_Apply_ACC_Levy_From_Employer_Details(IRD_Number)) {
        std::cout << Display_Error << IRD_Tax_Information_Save_Failure << std::endl;
        return;
    }

    std::cout << IRD_Tax_Information_Saved_Success_Message << std::endl;
}

void Get_ACC_Levy_Rate(const std::string& IRD_Number) {
    if (!Auto_Apply_ACC_Levy_From_Employer_Details(IRD_Number)) {
        std::cout << Display_Error << IRD_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

void Get_ACC_Levy_Maximum_Earnings_Threshold(const std::string& IRD_Number) {
    if (!Auto_Apply_ACC_Levy_From_Employer_Details(IRD_Number)) {
        std::cout << Display_Error << IRD_Tax_Information_Save_Failure << std::endl;
        return;
    }
}

void Get_ACC_Levy_Maximum(const std::string& IRD_Number) {
    Get_ACC_Levy_Maximum_Earnings_Threshold(IRD_Number);
}

bool Auto_Apply_ACC_Levy_From_Employer_Details(const std::string& IRD_Number) {
    std::string levyType;
    if (!Get_ACC_Levy_Type_From_Employer_Details(levyType)) {
        std::cout << Display_Error << Unable_to_Find_ACC_Levy_Type_in_Employer_Details << std::endl;
        return false;
    }

    std::string levyRate;
    if (!Get_ACC_Levy_Rate_From_Employer_Details(levyRate)) {
        std::cout << Display_Error << Unable_to_Find_ACC_Levy_Rate_in_Employer_Details << std::endl;
        return false;
    }

    std::string levyMaximum;
    if (!Get_ACC_Levy_Maximum_From_Employer_Details(levyMaximum)) {
        std::cout << Display_Error << Unable_to_Find_ACC_Levy_Maximum_in_Employer_Details << std::endl;
        return false;
    }

    ACC_Levy_Types parsedLevyType;
    if (!Parse_ACC_Levy_Type(levyType, parsedLevyType)) {
        std::cout << Display_Error << "Invalid ACC Levy Type in the employer details file." << std::endl;
        return false;
    }

    if (!Save_ACC_Levy_Types(IRD_Number, levyType)) {
        return false;
    }
    if (!Save_ACC_Levy_Rate(IRD_Number, levyRate, parsedLevyType)) {
        return false;
    }
    if (!Save_ACC_Levy_Maximum(IRD_Number, levyMaximum)) {
        return false;
    }

    return true;
}

