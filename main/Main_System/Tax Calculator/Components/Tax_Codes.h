// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

// Primary Income Tax Codes
// M – Standard
// ME – Standard, no student loan
// M SL – Standard with student loan
// ME SL – No student loan but SL obligations

// Secondary Income Tax Codes
// SB – Secondary Low Bracket (< $14,000)
// S  – Secondary Standard ($14,000–$48,000)
// SH – Secondary Higher Bracket ($48,000–$70,000)
// ST – Secondary Top Bracket ($70,000–$180,000)
// SA – Secondary Additional Top Bracket (> $180,000)

// Special Tax Codes and Exemptions
// STC – Special Tax Code (requires IRD letter)
// STC Rate (percentage)
// STC Letter Received (bool)
// Expiry Date (nullable)
// CAE – Casual Agricultural Employee
// EDW – Election Day Worker
// NSW – Non-resident Seasonal Worker
// WT – Schedular Payments (Contractors)

#ifndef TAX_CODES_H
#define TAX_CODES_H

#include <string>

void Get_Tax_Code_Job_Type(const std::string& IRD_Number);
void Get_Tax_Code_By_Job_Type(const std::string& IRD_Number);
void Get_Primary_Tax_Code(const std::string& IRD_Number);
void Get_Secondary_Tax_Code(const std::string& IRD_Number);
void Get_Special_Tax_Code(const std::string& IRD_Number);
void Get_STC_Rate(const std::string& IRD_Number);
void Get_STC_Letter_Received(const std::string& IRD_Number);
void Get_STC_Expiry_Date(const std::string& IRD_Number);

#endif // TAX_CODES_H