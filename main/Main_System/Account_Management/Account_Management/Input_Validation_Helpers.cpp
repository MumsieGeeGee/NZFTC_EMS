// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Input_Validation_Helpers.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../Tax Calculator/Components/Kiwisaver.h"
#include <cctype>
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <sstream>

std::string Trim_Copy(const std::string& value) {
    const std::size_t first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

bool Starts_With(const std::string& text, const std::string& prefix) {
    return text.rfind(prefix, 0) == 0;
}

bool Ends_With(const std::string& text, const std::string& suffix) {
    return text.size() >= suffix.size() && text.compare(text.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// Input Validation Helper Functions

// Numerical Input Validation
bool Is_Numeric_Only(const std::string& input) {
    if (input.empty()) {
        std::cout << Display_Error << Invalid_Empty_Input << ". " << Invalid_Numerical_Input << std::endl;
        return false;
    }
    if (input.size() == 1 && !std::isdigit(input[0])) {
        std::cout << Display_Error << Invalid_Numerical_Input << std::endl;
        return false;
    }
    for (char c : input) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            // If any character is not a digit, it's not numeric - reject symbols
            std::cout << Display_Error << Invalid_Numerical_Input << std::endl;
            return false;
        }
    }
    return true;
}

bool Is_Numeric_accepts_Symbols(const std::string& input) {
    if (input.empty()) {
        std::cout << Display_Error << Invalid_Empty_Input << ". " << Invalid_Numerical_Input << std::endl;
        return false;
    }
    if (input.size() == 1 && !std::isdigit(input[0])) {
        std::cout << Display_Error << Invalid_Numerical_Input << std::endl;
        return false;
    }
    return true;
}

// Decimal Input Validation
bool Is_Decimal_Number(const std::string& input) {
    if (input.empty()) {
        std::cout << Display_Error << Invalid_Empty_Input << ". " << Invalid_Numerical_Input << std::endl;
        return false;
    }

    std::size_t firstDot = input.find('.');
    if (firstDot == std::string::npos) {
        return Is_Numeric_Only(input);
    }

    if (input.find('.', firstDot + 1) != std::string::npos) {
        std::cout << Display_Error << Invalid_Numerical_Input << std::endl;
        return false;
    }

    std::string wholePart = input.substr(0, firstDot);
    std::string fractionalPart = input.substr(firstDot + 1);

    if (wholePart.empty() || fractionalPart.empty()) {
        std::cout << Display_Error << Invalid_Numerical_Input << std::endl;
        return false;
    }

    if (!Is_Numeric_Only(wholePart)) {
        return false;
    }

    if (!Is_Numeric_Only(fractionalPart)) {
        return false;
    }

    return true;
}
// Percentage Input Validation
bool Is_Percentage(const std::string& input) {
    if (!Is_Decimal_Number(input)) {
        std::cout << Display_Error << Invalid_Percentage_Input << std::endl;
        return false;
    }
    double value = std::stod(input);
    if (value < 0.0 || value > 100.0) {
        std::cout << Display_Error << Invalid_Percentage_Input << std::endl;
        return false;
    }
    if (input == "0" || input == "0.0" || input == "0.00") {
        std::cout << Display_Error << Invalid_Percentage_Input << std::endl;
        return false;
    }
    if (input == "100" || input == "100.0" || input == "100.00") {
        std::cout << Display_Error << Invalid_Percentage_Input << std::endl;
        return false;
    }
    if (Valid_Kiwisaver_Contribution_Rates(std::string(input + "%"))) {
        return true;
    }
    return true;
}
// Date Input Validation
bool Is_Valid_Year(const std::string& input) {
    const std::string value = Trim_Copy(input);
    if (!Is_Numeric_Only(value)) {
        return false;
    }
    int year = std::stoi(value);
    if (year < 2025 || year > 2100) {
        std::cout << Display_Error << Invalid_Date << " " << Invalid_Year << std::endl;
        return false;
    }
    return true;
}

bool Is_Valid_DOB_Year(const std::string & input) {
    const std::string value = Trim_Copy(input);
    if (!Is_Numeric_Only(value)) {
        return false;
    }
    int year = std::stoi(value);
    if (year < 1920 || year > 2020) { // update years as needed
        std::cout << Display_Error << Invalid_Year_DOB << std::endl;
        return false;
    }
    return true;
}

// Date Format Validation - dd-mm-yyyy or dd/mm/yyyy or dd.mm.yyyy
bool Is_Date_Format(const std::string& input) {
    const std::string value = Trim_Copy(input);
    if (value.length() != 10) {
        std::cout << Display_Error << Invalid_Date_Format << std::endl;
        return false;
    }

    const char separator = value[2];
    if (separator != '/' && separator != '-' && separator != '.') {
        std::cout << Display_Error << Invalid_Date_Format << std::endl;
        return false;
    }

    if (value[5] != separator) {
        std::cout << Display_Error << Invalid_Date_Format << std::endl;
        return false;
    }

    const std::string day = value.substr(0, 2);
    const std::string month = value.substr(3, 2);
    const std::string year = value.substr(6, 4);
    if (!Is_Numeric_Only(day) || !Is_Numeric_Only(month) || !Is_Numeric_Only(year)) {
        std::cout << Display_Error << Invalid_Date_Format << std::endl;
        return false;
    }

    const int day_value = std::stoi(day);
    const int month_value = std::stoi(month);
    const int year_value = std::stoi(year);
    if (day_value < 1 || day_value > 31 || month_value < 1 || month_value > 12 || year_value < 1900 || year_value > 2100) {
        std::cout << Display_Error << Invalid_Date << std::endl;
        return false;
    }
    return true;
}

// BIC Code Validation - Business Identifier Code - non specific, pattern based, not a lookup of existing codes.
bool Is_Valid_BIC_Code(const std::string& input)
{
    if (input.empty())
    return false;

    // Must start with uppercase letter
    if (!std::isalpha(input[0]) || !std::isupper(input[0]))
        return false;

    int i = 1;
    int n = input.length();
    // Next characters must be uppercase letters or digits
    while (i < n && (std::isupper(input[i]) || std::isdigit(input[i]))) {
        i++;
    }

    return i == n;
}
// Yes/No Input Validation
bool Yes_or_No (const std::string& input) {
    if (Is_Yes_Input(input)) {
        return true;
    }
    if (Is_No_Input(input)) {
        return true;
    }
    std::cout << Display_Error << Yes_No_Input << std::endl;
	return false;
}

std::string Yes_Or_No_Input_to_String(const std::string& input) {
    if (Is_Yes_Input(input)) {
        return "Yes";
    }
    if (Is_No_Input(input)) {
        return "No";
    }
    return "";
}

bool Is_Yes_Input(const std::string& input) {
    const std::string value = Trim_Copy(input);
    return value == "yes" || value == "y" || value == "Yes" || value == "Y" || value == "YES";
}

bool Is_No_Input(const std::string& input) {
    const std::string value = Trim_Copy(input);
    return value == "no" || value == "n" || value == "No" || value == "N" || value == "NO";
}
bool Is_Student_Loan_Active_Input(const std::string& input) {
    const std::string value = Trim_Copy(input);
    return Is_Yes_Input(value) || value == "True" || value == "true" || value == "TRUE" || value == "1" || value == "SL";
}

bool Is_Active_Status_Input(const std::string& input) {
    const std::string value = Trim_Copy(input);
    return value == "active" || value == "Active" || value == "ACTIVE";
}

bool Get_Validated_Menu_Choice(int minimum, int maximum, int* choiceOut) {
    if (choiceOut == nullptr || minimum > maximum) {
        std::cout << Display_Error << Menu_Option_Input << std::endl;
        return false;
    }

    int choice = 0;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
        return false;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (choice < minimum || choice > maximum) {
        std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
        return false;
    }

    *choiceOut = choice;
    return true;
}

bool Is_IRD_Number_Format(const std::string& input) {
    std::string digits_Only;
    for (char c : input) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            digits_Only.push_back(c);
        }
        else if (c == '-') {
            continue;
        }
        else {
            std::cout << Display_Error << IRD_Number_Invalid_Characters << std::endl;
            return false;
        }
    }
    if (digits_Only.length() == 9) {
        return true;
    }
    // If old 8 Number format, tell user to add 0. 
    if (digits_Only.length() == 8) {
        std::cout << Display_Error << IRD_Number_Input_8 << std::endl;
        return false;
    }
    std::cout << Display_Error << Invalid_IRD_Number << " " << Valid_IRD_Number << std::endl;
    return false;
}

std::vector<std::string> Parse_CSV_List(const std::string& input) {
    std::vector<std::string> items;
    std::string token;
    std::stringstream ss(input);

    while (std::getline(ss, token, ',')) {
        token = Trim_Copy(token);
        if (!token.empty()) items.push_back(token);
    }

    return items;
}

double Get_Decimal_Value() {
    std::string input;

    while (true) {
        std::getline(std::cin, input);
        input = Trim_Copy(input);

        if (!Is_Decimal_Number(input)) {
            std::cout << Display_Error << Invalid_Decimal_Input << std::endl;
            continue;
        }

        return std::stod(input);
    }
}

int Get_Integer_Value() {
    std::string input;

    while (true) {
        std::getline(std::cin, input);
        input = Trim_Copy(input);

        if (!Is_Numeric_Only(input)) {
            std::cout << Display_Error << Invalid_Numerical_Input << " " << Try_Again << std::endl;
            continue;
        }

        return std::stoi(input);
    }
}

std::vector<int> Parse_Comma_Separated_Selection(const std::string& input) {
    std::vector<int> selections;
    std::stringstream ss(input);
    std::string item;

    while (std::getline(ss, item, ',')) {
        item = Trim_Copy(item);
        if (item.empty()) {
            continue;
        }

        if (!Is_Numeric_Only(item)) {
            std::cout << Display_Error << Invalid_Numerical_Input << std::endl;
            return {};
        }

        selections.push_back(std::stoi(item));
    }

    return selections;
}


