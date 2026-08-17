// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

// Accident Compensation Corporation (ACC)
// ACC Earners’ Levy Rate
// Maximum Liable Earnings Threshold

#ifndef ACC_H
#define ACC_H

#include <string>

enum class ACC_Levy_Types {
    Earners_Levy_Rate,
    Work_Levy_Rate,
};

std::string ACC_Levy_Types_to_String(ACC_Levy_Types levyType);
std::string ACC_Levy_Default_Rate_to_String(ACC_Levy_Types levyType);

void Get_ACC_Levy_Types(const std::string& IRD_Number);
void Get_ACC_Levy_Rate(const std::string& IRD_Number);
void Get_ACC_Levy_Maximum(const std::string& IRD_Number);
bool Auto_Apply_ACC_Levy_From_Employer_Details(const std::string& IRD_Number);

#endif // ACC_H