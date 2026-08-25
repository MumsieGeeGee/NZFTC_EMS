// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef COMPONENT_ACCESS_H
#define COMPONENT_ACCESS_H

#include <string>

struct Account_Access_Profile {
    bool resolved = false;
    std::string username;
    std::string account_type;
    std::string business_role;
    std::string job_role;
    std::string dashboard_mode;
    bool can_manage_all_accounts = false;
    bool can_manage_all_employees = false;
    bool can_manage_all_hr = false;
    bool can_manage_requests = false;
    bool can_use_payroll_features = false;
    bool assistant_delegated_scope_only = false;
    bool can_view_assigned_team = false;
    bool requires_secondary_auth = false;
};

void Display_Account_Access_For_Type(const std::string& account_type);
void Display_Account_Access_For_Type(const std::string& account_type, const std::string& username);
bool Secondary_Authenticators(const std::string& business_role, const std::string& job_role);
bool Resolve_Access_Profile_For_User(const std::string& account_type,
    const std::string& username,
    Account_Access_Profile* profile_out);

#endif // COMPONENT_ACCESS_H
