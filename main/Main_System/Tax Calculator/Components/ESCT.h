// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef ESCT_H
#define ESCT_H

#include <string>
// Employer Superannuation Contribution Tax (ESCT)
void Get_ESCT_Existence(const std::string& IRD_Number);
// ESCT Rate (10.5%, 17.5%, 30%, 33%, 39%)
void Get_ESCT_Tax_Rate(const std::string& IRD_Number);
// ESCT Income Band
void Get_ESCT_Income_Band(const std::string& IRD_Number);

#endif // ESCT_H