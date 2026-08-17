// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include "../../Employee Management/HR_Management/Payslips/Payslip_Generation.h"

bool Prepare_Payslip_Context(const std::string& username, Pre_Calculation_Payslip& slip);
bool Calculate_Payslip(Pre_Calculation_Payslip& slip);



#endif // CALCULATOR_H
