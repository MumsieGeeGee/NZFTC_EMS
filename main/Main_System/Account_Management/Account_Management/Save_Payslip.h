// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef SAVE_PAYSLIP_H
#define SAVE_PAYSLIP_H

#include <string>
#include "../../Employee Management/HR_Management/Payslips/Payslip_Generation.h"

bool Save_Payslip(const std::string& username, const Pre_Calculation_Payslip& slip);

#endif // SAVE_PAYSLIP_H