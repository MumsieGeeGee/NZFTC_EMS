// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef TRADE_ACCOUNTS_H
#define TRADE_ACCOUNTS_H

#include <string>

enum class Trade_Account_Type {
    General, 
    Apprentice,
    Mentor, 
    Safety_Officer,
    Sales_Representative,
    Manager
};

void Trade_Account_Type_Options(Trade_Account_Type& trade_choice);
void Trade_Username_Input_Rules(std::string& username, Trade_Account_Type& trade_choice);

#endif // TRADE_ACCOUNTS_H

