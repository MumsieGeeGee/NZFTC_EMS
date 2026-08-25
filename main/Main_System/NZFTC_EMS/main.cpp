// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "../Login/Login_Menu.h"
#include "../NZFTC_EMS/Session_Handling/Session_Handling.h"

#include <iostream>
#include <string>

namespace {
std::string Escape_Json_String(const std::string& value) {
    std::string escaped;
    escaped.reserve(value.size());
    for (char character : value) {
        switch (character) {
            case '\\':
                escaped += "\\\\";
                break;
            case '"':
                escaped += "\\\"";
                break;
            case '\n':
                escaped += "\\n";
                break;
            case '\r':
                escaped += "\\r";
                break;
            case '\t':
                escaped += "\\t";
                break;
            default:
                escaped.push_back(character);
                break;
        }
    }
    return escaped;
}

std::string To_Json_Bool(const bool value) {
    return value ? "true" : "false";
}

void Print_UI_Login_Result(const UI_Login_Result& result) {
    const User_Session_Context& session = Get_Current_User_Session_Context();
    std::cout
        << "{"
        << "\"success\":" << To_Json_Bool(result.success) << ","
        << "\"passwordResetRequired\":" << To_Json_Bool(result.password_reset_required) << ","
        << "\"message\":\"" << Escape_Json_String(result.message) << "\","
        << "\"username\":\"" << Escape_Json_String(session.username) << "\","
        << "\"accountType\":\"" << Escape_Json_String(session.account_type) << "\","
        << "\"irdNumber\":\"" << Escape_Json_String(session.ird_number) << "\","
        << "\"accessProfile\":{"
            << "\"resolved\":" << To_Json_Bool(result.access_profile.resolved) << ","
            << "\"username\":\"" << Escape_Json_String(result.access_profile.username) << "\","
            << "\"accountType\":\"" << Escape_Json_String(result.access_profile.account_type) << "\","
            << "\"businessRole\":\"" << Escape_Json_String(result.access_profile.business_role) << "\","
            << "\"jobRole\":\"" << Escape_Json_String(result.access_profile.job_role) << "\","
            << "\"dashboardMode\":\"" << Escape_Json_String(result.access_profile.dashboard_mode) << "\","
            << "\"canManageAllAccounts\":" << To_Json_Bool(result.access_profile.can_manage_all_accounts) << ","
            << "\"canManageAllEmployees\":" << To_Json_Bool(result.access_profile.can_manage_all_employees) << ","
            << "\"canManageAllHr\":" << To_Json_Bool(result.access_profile.can_manage_all_hr) << ","
            << "\"canManageRequests\":" << To_Json_Bool(result.access_profile.can_manage_requests) << ","
            << "\"canUsePayrollFeatures\":" << To_Json_Bool(result.access_profile.can_use_payroll_features) << ","
            << "\"assistantDelegatedScopeOnly\":" << To_Json_Bool(result.access_profile.assistant_delegated_scope_only) << ","
            << "\"canViewAssignedTeam\":" << To_Json_Bool(result.access_profile.can_view_assigned_team) << ","
            << "\"requiresSecondaryAuth\":" << To_Json_Bool(result.access_profile.requires_secondary_auth)
        << "}"
        << "}";
}

void Print_UI_Action_Result(const UI_Action_Result& result) {
    std::cout
        << "{"
        << "\"success\":" << To_Json_Bool(result.success) << ","
        << "\"message\":\"" << Escape_Json_String(result.message) << "\""
        << "}";
}
}

int main(int argc, char* argv[]) {
    if (argc >= 3 && std::string(argv[1]) == "--ui-bridge") {
        const std::string command = argv[2];
        if (command == "login") {
            std::string username;
            std::string password;
            std::string ird_number;
            std::getline(std::cin, username);
            std::getline(std::cin, password);
            std::getline(std::cin, ird_number);

            const UI_Login_Result result =
                Authenticate_And_Build_UI_Context(username, password, ird_number);
            Print_UI_Login_Result(result);
            return result.success ? 0 : 1;
        }

        if (command == "action") {
            if (argc < 4) {
                std::cout << "{\"success\":false,\"message\":\"Missing action id.\"}";
                return 1;
            }

            const std::string action_username = argc >= 5 ? argv[4] : "";
            const UI_Action_Result result = Execute_UI_Action(argv[3], action_username);
            Print_UI_Action_Result(result);
            return result.success ? 0 : 1;
        }

        std::cout << "{\"success\":false,\"message\":\"Unknown ui-bridge command.\"}";
        return 1;
    }

    Main_Login_Menu("", "", "");
    return 0;
}
