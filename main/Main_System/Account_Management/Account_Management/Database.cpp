// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Database.h"
#include "Save_Functions.h"
#include "Input_Validation_Helpers.h"
#include "Security_Protocols.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include "../../Tax Calculator/Calculator/Other_Menus.h"
#include "../../Tax Calculator/Components/IRD_PAYE.h"
#include "../../Tax Calculator/Components/Tax_Codes.h"
#include "../../Tax Calculator/Components/ACC.h"
#include "../../Tax Calculator/Components/Student_Loan.h"
#include "../../Tax Calculator/Components/Child_Support.h"
#include "../../Tax Calculator/Components/Kiwisaver.h"
#include "../../Tax Calculator/Components/ESCT.h"
#include "../../Employee Management/HR_Management/Leave/Leave_Requests.h"
#include "../../Path_Utilities.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <string>
#include <iostream>

namespace {
constexpr const char RecordFileExtension[] = ".txt";
std::filesystem::path Get_Employee_Records_Directory_Path() {
    return Path_Utilities::Get_Employee_Records_Directory();
}

}

const std::filesystem::path Employer_Details_File_Path =
    Path_Utilities::Get_Employer_Details_File();

std::string Get_IRD_Record_File_Path(const std::string& IRD_Number) {
    return (Get_Employee_Records_Directory_Path() / "IRD" / (IRD_Number + RecordFileExtension)).string();
}

bool Read_Record_Field_Value(const std::string& IRD_Number, const std::string& fieldLabel, std::string& outValue) {
    std::ifstream inFile(Get_IRD_Record_File_Path(IRD_Number));
    if (!inFile) {
        return false;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (Starts_With(line, fieldLabel)) {
            outValue = Trim_Copy(line.substr(fieldLabel.length()));
            return true;
        }
    }

    return false;
}

bool IRD_Record_Exists_In_Master_Folder(const std::string& IRD_Number) {
    std::ifstream recordFile(Get_IRD_Record_File_Path(IRD_Number));
    return static_cast<bool>(recordFile);
}

bool Get_IRD_Record_Confirmation_Status(const std::string& IRD_Number, std::string& statusOut) {
    if (!Read_Record_Field_Value(IRD_Number, IRD_Confirmation_Status_Label, statusOut)) {
        return false;
    }

    statusOut = Trim_Copy(statusOut);
    return !statusOut.empty();
}

bool Get_IRD_Record_File_Contents(const std::string& IRD_Number, std::string& fileContentsOut) {
    std::ifstream record(Get_IRD_Record_File_Path(IRD_Number));
    if (!record) {
        return false;
    }

    fileContentsOut.clear();
    std::string line;
    bool firstLine = true;
    while (std::getline(record, line)) {
        if (!firstLine) {
            fileContentsOut += '\n';
        }
        fileContentsOut += line;
        firstLine = false;
    }

    return true;
}

bool Is_Student_Loan_Active(const std::string& IRD_Number) {
    std::string studentLoanStatus;
    if (!Read_Record_Field_Value(IRD_Number, Student_Loan_Existence_Label, studentLoanStatus)) {
        return false;
    }

    return Is_Student_Loan_Active_Input(studentLoanStatus);
}

bool Is_Primary_Tax_Code_ME(const std::string& IRD_Number) {
    std::string primaryTaxCode;
    if (!Read_Record_Field_Value(IRD_Number, Primary_Tax_Code_Label, primaryTaxCode)) {
        return false;
    }

    const std::string trimmed = Trim_Copy(primaryTaxCode);
    if (trimmed == "ME" || trimmed == "me") {
        return true;
    }

    if (Starts_With(trimmed, "ME ") || Starts_With(trimmed, "me ")) {
        return true;
    }

    return false;
}

bool Is_Special_Tax_Code_Active(const std::string& IRD_Number) {
    std::string specialTaxCode;
    if (!Read_Record_Field_Value(IRD_Number, Special_Tax_Code_Label, specialTaxCode)) {
        return false;
    }

    const std::string value = Trim_Copy(specialTaxCode);
    if (value.empty()) {
        return false;
    }

    if (value == "none" || value == "None" || value == "NONE") {
        return false;
    }

    return true;
}

bool Is_Child_Support_Active(const std::string& IRD_Number) {
    std::string childSupportStatus;
    if (!Read_Record_Field_Value(IRD_Number, Child_Support_Status_Label, childSupportStatus)) {
        return false;
    }

    return Is_Active_Status_Input(childSupportStatus);
}

bool Is_KiwiSaver_Active(const std::string& IRD_Number) {
    std::string kiwiSaverStatus;
    if (!Read_Record_Field_Value(IRD_Number, KiwiSaver_Opt_In_Out_Status_Label, kiwiSaverStatus)) {
        return false;
    }

    return !Trim_Copy(kiwiSaverStatus).empty();
}

bool Is_KiwiSaver_Opted_Out(const std::string& IRD_Number) {
    std::string kiwiSaverStatus;
    if (!Read_Record_Field_Value(IRD_Number, KiwiSaver_Opt_In_Out_Status_Label, kiwiSaverStatus)) {
        return false;
    }

    const std::string value = Trim_Copy(kiwiSaverStatus);
    return value == "opt-out" || value == "Opt-Out" || value == "OPT-OUT";
}

bool Is_ESCT_Active(const std::string& IRD_Number) {
    std::string esctExistence;
    if (!Read_Record_Field_Value(IRD_Number, ESCT_Existence_Label, esctExistence)) {
        return false;
    }

    return Is_Active_Status_Input(esctExistence);
}

bool Is_Salary_Employee(const std::string& IRD_Number) {
    std::string payTypeValue;
    if (!Read_Record_Field_Value(IRD_Number, Pay_Type_Label, payTypeValue)) {
        return false;
    }

    return Trim_Copy(payTypeValue) == Salary_Label;
}

bool Is_Hourly_Employee(const std::string& IRD_Number) {
    std::string payTypeValue;
    if (!Read_Record_Field_Value(IRD_Number, Pay_Type_Label, payTypeValue)) {
        return false;
    }

    return Trim_Copy(payTypeValue) == Hourly_Label;
}

bool Is_Casual_Employee(const std::string& IRD_Number) {
    std::string payTypeValue;
    if (!Read_Record_Field_Value(IRD_Number, Pay_Type_Label, payTypeValue)) {
        return false;
    }

    return Trim_Copy(payTypeValue) == Casual_Label;
}

bool Open_Employer_Details_File(const std::string& EmployerDetailsPath) {
    std::ifstream file(EmployerDetailsPath);
    if (!file) {
        std::cout << Display_Error << Employer_Financial_Details_Not_Found << std::endl;
        return false;
    }
    return true;
}

bool search_IRD_Master_Folder_for_Record(const std::string& IRD_to_Search) {
    if (!IRD_Record_Exists_In_Master_Folder(IRD_to_Search)) {
        std::cout << Display_Error << Employee_Record_Not_Found << std::endl;
        return false;
    }
    std::string filename = Get_IRD_Record_File_Path(IRD_to_Search);
    std::ifstream record(filename);

    if (!record) {
        std::cout << Display_Error << Employee_Tax_Information_Not_Found << std::endl;
        return false;
    }
    record.close();
    return true;

}
