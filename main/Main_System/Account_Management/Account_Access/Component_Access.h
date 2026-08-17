// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef COMPONENT_ACCESS_H
#define COMPONENT_ACCESS_H

#include <string>

void Display_Account_Access_For_Type(const std::string& account_type);
void Display_Account_Access_For_Type(const std::string& account_type, const std::string& username);
bool Secondary_Authenticators(const std::string& business_role, const std::string& job_role);

#endif // COMPONENT_ACCESS_H

