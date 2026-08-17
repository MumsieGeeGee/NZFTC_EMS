// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef INPUT_VALIDATION_HELPERS_H
#define INPUT_VALIDATION_HELPERS_H

#include <string>
#include <vector>

// Input Validation Helper Functions

extern std::string Trim_Copy(const std::string& value);
bool Starts_With(const std::string& text, const std::string& prefix);
bool Ends_With(const std::string& text, const std::string& suffix);

// Numerical Input Validation
bool Is_Numeric_Only(const std::string& input);
bool Is_Numeric_accepts_Symbols(const std::string& input);
// Decimal Input Validation
bool Is_Decimal_Number(const std::string& input);
// Date Input Validation
bool Is_Valid_Year(const std::string& input);
bool Is_Valid_DOB_Year(const std::string & input);
// Date Format Validation
bool Is_Date_Format(const std::string& input);
// Po Box Rules
// BIC Code Validation
bool Is_Valid_BIC_Code(const std::string& input);
// Yes/No Input Validation
bool Yes_or_No(const std::string& input);
std::string Yes_Or_No_Input_to_String(const std::string& input);
bool Is_Yes_Input(const std::string& input);
bool Is_No_Input(const std::string& input);
bool Is_Student_Loan_Active_Input(const std::string& input);
bool Is_Active_Status_Input(const std::string& input);
bool Get_Validated_Menu_Choice(int minimum, int maximum, int* choiceOut);
// IRD Number Validation
bool Is_IRD_Number_Format(const std::string& input);

std::vector<std::string> Parse_CSV_List(const std::string& input);
std::vector<int> Parse_Comma_Separated_Selection(const std::string& input);

#endif // INPUT_VALIDATION_HELPERS_H
