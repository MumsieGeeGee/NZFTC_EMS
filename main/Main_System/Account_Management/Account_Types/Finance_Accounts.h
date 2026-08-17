// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef FINANCE_ACCOUNTS_H
#define FINANCE_ACCOUNTS_H

#include <string>

enum class Finance_Account_Type {
    General, 
    Payroll_Administrator,
    Accounts_Payable_Administrator,
    Accounts_Receivable_Administrator,
    Manager
};

void Finance_Account_Type_Options(Finance_Account_Type& finance_choice);
void Finance_Username_Input_Rules(std::string& username, Finance_Account_Type& finance_choice);

#endif // FINANCE_ACCOUNTS_H

