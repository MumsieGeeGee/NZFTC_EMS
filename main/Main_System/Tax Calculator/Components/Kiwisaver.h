// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef KIWISAVER_H
#define KIWISAVER_H

#include <string>
// Valid KiwiSaver Contribution Rates (as percentages)
bool Valid_Kiwisaver_Contribution_Rates(std::string& input);
// Opt In / Opt Out Status
void Get_KiwiSaver_Opt_In_Out_Status(const std::string& IRD_Number);
// Employee Contribution Rate
void Get_KiwiSaver_Employee_Contribution_Rate(const std::string& IRD_Number);
// Employer Contribution Rate
void Get_KiwiSaver_Employer_Contribution_Rate(const std::string& IRD_Number);
// Opt-Out Date (if applicable)
void Get_KiwiSaver_Opt_Out_Date(const std::string& IRD_Number);

#endif // KIWI_SAVER_H
