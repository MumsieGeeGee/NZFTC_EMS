// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Leave_and_Holiday.h"
#include "../../UI/Employee_Management_Menu_UI.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "Role_and_Salary_Details.h"
#include <string>
#include <iostream>
#include <array>
#include <fstream>
#include <ctime>


namespace {
    constexpr const char PublicHolidaysFileExtension[] = ".txt";
    constexpr const char PublicHolidaysFilePrefix[] = "Public_Holidays_NZ";
    constexpr const char PublicHolidaysFolderName[] = "Calendar (NZ Public Holidays)\\";
    
    std::string Get_Public_Holidays_File_Path(int year) {
        return std::to_string(year) + "_" + PublicHolidaysFilePrefix + PublicHolidaysFileExtension;
    }

    std::array<std::string, 4> Get_Public_Holidays_File_Path_Candidates(int year) {
        const std::string fileName = Get_Public_Holidays_File_Path(year);
        return {
            fileName,
            std::string(PublicHolidaysFolderName) + fileName,
            std::string("..\\") + PublicHolidaysFolderName + fileName,
            std::string("..\\..\\") + PublicHolidaysFolderName + fileName
        };
    }
    
    int Get_Current_Year() {
        time_t now = time(0);
        tm* localTime = localtime(&now);
        return localTime->tm_year + 1900;
    }

    std::string Trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }
}

// Global variables to store holiday dates
std::string New_Years_Day_Date = "";
std::string Day_After_New_Years_Day_Date = "";
std::string Waitangi_Day_Date = "";
std::string Good_Friday_Date = "";
std::string Easter_Monday_Date = "";
std::string ANZAC_Day_Date = "";
std::string Queens_Kings_Birthday_Date = "";
std::string Labour_Day_Date = "";
std::string Christmas_Day_Date = "";
std::string Boxing_Day_Date = "";

enum class Current_Years_NZ_Public_Holidays {
    New_Years_Day,
    Day_After_New_Years_Day,
    Waitangi_Day,
    Good_Friday,
    Easter_Monday,
    ANZAC_Day,
    Queens_Kings_Birthday,
    Labour_Day,
    Christmas_Day,
    Boxing_Day
};

void Parse_Holiday_Line(const std::string& line) {
    size_t colonPos = line.find(':');
    if (colonPos == std::string::npos) return;
    
    std::string holidayName = Trim(line.substr(0, colonPos));
    std::string holidayDate = Trim(line.substr(colonPos + 1));
    
    if (holidayName == "New Years Day") {
        New_Years_Day_Date = holidayDate;
    } else if (holidayName == "Day after New Years Day") {
        Day_After_New_Years_Day_Date = holidayDate;
    } else if (holidayName == "Waitangi Day") {
        Waitangi_Day_Date = holidayDate;
    } else if (holidayName == "Good Friday") {
        Good_Friday_Date = holidayDate;
    } else if (holidayName == "Easter Monday") {
        Easter_Monday_Date = holidayDate;
    } else if (holidayName == "ANZAC Day") {
        ANZAC_Day_Date = holidayDate;
    } else if (holidayName == "Queens/Kings Birthday") {
        Queens_Kings_Birthday_Date = holidayDate;
    } else if (holidayName == "Labour Day") {
        Labour_Day_Date = holidayDate;
    } else if (holidayName == "Christmas Day") {
        Christmas_Day_Date = holidayDate;
    } else if (holidayName == "Boxing Day") {
        Boxing_Day_Date = holidayDate;
    }
}

void Get_Current_Years_NZ_Public_Holidays() {
    int currentYear = Get_Current_Year();
    std::ifstream file;
    std::string resolvedPath;

    for (const std::string& candidatePath : Get_Public_Holidays_File_Path_Candidates(currentYear)) {
        file.open(candidatePath);
        if (file.is_open()) {
            resolvedPath = candidatePath;
            break;
        }
        file.clear();
    }

    if (!file.is_open()) {
        std::cerr << "Error: Could not open public holidays file in expected locations for year "
                  << currentYear << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Parse_Holiday_Line(line);
        }
    }
    file.close();
}

std::string Get_New_Years_Day() {
    return New_Years_Day_Date;
}

std::string Get_Day_After_New_Years_Day() {
    return Day_After_New_Years_Day_Date;
}

std::string Get_Waitangi_Day() {
    return Waitangi_Day_Date;
}

std::string Get_Good_Friday() {
    return Good_Friday_Date;
}

std::string Get_Easter_Monday() {
    return Easter_Monday_Date;
}

std::string Get_ANZAC_Day() {
    return ANZAC_Day_Date;
}

std::string Get_Queens_Kings_Birthday() {
    return Queens_Kings_Birthday_Date;
}

std::string Get_Labour_Day() {
    return Labour_Day_Date;
}

std::string Get_Christmas_Day() {
    return Christmas_Day_Date;
}

std::string Get_Boxing_Day() {
    return Boxing_Day_Date;
}

int Calculate_Public_Holidays_Count() {
    int count = 0;
    if (!New_Years_Day_Date.empty()) count++;
    if (!Day_After_New_Years_Day_Date.empty()) count++;
    if (!Waitangi_Day_Date.empty()) count++;
    if (!Good_Friday_Date.empty()) count++;
    if (!Easter_Monday_Date.empty()) count++;
    if (!ANZAC_Day_Date.empty()) count++;
    if (!Queens_Kings_Birthday_Date.empty()) count++;
    if (!Labour_Day_Date.empty()) count++;
    if (!Christmas_Day_Date.empty()) count++;
    if (!Boxing_Day_Date.empty()) count++;
    return count;
}

// Global variables to store parental leave entitlements
int Maternity_Leave = 0;
int Paternity_Leave = 0;
int Adoption_Leave = 0;
int Shared_Parental_Leave = 0;

void Maternity_Leave_Entitlement() {
    int Maternity_Leave;
    std::cin >> Maternity_Leave;
    if (Maternity_Leave < 0) {
        std::cerr << "Error: Maternity leave entitlement cannot be negative." << std::endl;
        return;
    }

    if (Maternity_Leave > 26) {
        std::cerr << "Error: Maternity leave entitlement cannot exceed 26 weeks." << std::endl;
        return;
    }
}

void Paternity_Leave_Entitlement() {
    int Paternity_Leave;
    std::cin >> Paternity_Leave;
    if (Paternity_Leave < 0) {
        std::cerr << "Error: Paternity leave entitlement cannot be negative." << std::endl;
        return;
    }
    if (Paternity_Leave > 26) {
        std::cerr << "Error: Paternity leave entitlement cannot exceed 2 weeks." << std::endl;
        return;
    }
}

void Adoption_Leave_Entitlement() {
    int Adoption_Leave;
    std::cin >> Adoption_Leave;
    if (Adoption_Leave < 0) {
        std::cerr << "Error: Adoption leave entitlement cannot be negative." << std::endl;
        return;
    }
    if (Adoption_Leave > 26) {
        std::cerr << "Error: Adoption leave entitlement cannot exceed 26 weeks." << std::endl;
        return;
    }
}

void Shared_Parental_Leave_Entitlement() {
    int Shared_Parental_Leave;
    std::cin >> Shared_Parental_Leave;
    if (Shared_Parental_Leave < 0) {
        std::cerr << "Error: Shared parental leave entitlement cannot be negative." << std::endl;
        return;
    }
    if (Shared_Parental_Leave > 25) {
        std::cerr << "Error: Entitlement cannot exceed 25 weeks. 26 weeks = 'Maternal' or 'Paternal' Leave, not 'Shared Leave'." << std::endl;
        return;
    }
}

void Get_Parent_Type() {
    int choice = 0;
    std::cin >> choice;
    if (choice == 1) {
        Maternity_Leave_Entitlement();
    } else if (choice == 2) {
        Paternity_Leave_Entitlement();
    } else if (choice == 3) {
        Adoption_Leave_Entitlement();
    } else if (choice == 4) {
        Shared_Parental_Leave_Entitlement();
    } else {
        std::cerr << "Invalid choice for parental leave type." << std::endl;
    }
}



