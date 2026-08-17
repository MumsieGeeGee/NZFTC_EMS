// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef CHILD_SUPPORT_H
#define CHILD_SUPPORT_H

#include <string>

// Get Child Support Status
void Get_Child_Support_Status(const std::string& IRD_Number);
// Standard Deduction (fixed amount set by IRD)
void Get_Child_Support_Standard_Deduction(const std::string& IRD_Number);
// Voluntary Deduction (employee-chosen)
void Get_Child_Support_Voluntary_Deduction(const std::string& IRD_Number);
// Protected Net Earnings (PNE)
void Get_Child_Support_Protected_Net_Earnings(const std::string& IRD_Number);
// Shortfall Calculation and Reporting
void Get_Child_Support_Shortfall_Calculation(const std::string& IRD_Number);
// IRD Notice of Deduction (NOD)
void Get_Child_Support_IRD_Notice_of_Deduction_Existence(const std::string& IRD_Number);
// NOD Amount
void Get_Child_Support_IRD_Notice_of_Deduction_Amount(const std::string& IRD_Number);
// NOD Effective Date
void Get_Child_Support_IRD_Notice_of_Deduction_Effective_Date(const std::string& IRD_Number);
// NOD Expiry Date
void Get_Child_Support_IRD_Notice_of_Deduction_Expiry_Date(const std::string& IRD_Number);
// NOD Reference Number
void Get_Child_Support_IRD_Notice_of_Deduction_Reference_Number(const std::string& IRD_Number);

#endif // CHILD_SUPPORT_H