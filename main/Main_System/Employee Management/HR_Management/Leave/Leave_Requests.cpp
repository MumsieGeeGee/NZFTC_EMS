// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Leave_Requests.h"
#include "Leave_Requests_UI.h"
#include "../../../Account_Management/Account_Access/Account_Database.h"
#include "../../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../../../NZFTC_EMS/Session_Handling/Session_Handling.h"
#include "../../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../../UI/Shared_UI_Messaging/Info_Messages.h"
#include "../../../UI/Shared_UI_Messaging/Success_Messages.h"
#include "../../Role_Management/Role_and_Salary_Details.h"
#include <algorithm>
#include <chrono>
#include <cctype>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include <vector>
#include <string>
#include <iostream>

namespace {
constexpr const char OpenLeaveRequestsHeader[] =
    "Current Open Leave Requests:";
constexpr const char TempDeniedLeaveRequestsHeader[] =
    "Current Temp Denied Leave Requests:";
constexpr const char ResolvedLeaveRequestsHeader[] =
    "Current Resolved Leave Requests:";

std::string Current_Timestamp() {
    const std::time_t now = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());
    std::tm local_time{};
    localtime_s(&local_time, &now);
    std::ostringstream output;
    output << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
    return output.str();
}

std::string New_Leave_Request_ID() {
    const auto milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    return "LEAVE-" + std::to_string(milliseconds);
}

std::string Leave_Request_Type_To_String(Leave_Request_Type type) {
    switch (type) {
        case Leave_Request_Type::Sick:
            return "Sick Leave";
        case Leave_Request_Type::Annual:
            return "Annual Leave";
        case Leave_Request_Type::Parental:
            return "Parental Leave";
        case Leave_Request_Type::Special:
            return "Special Leave";
    }
    return "";
}

std::string Parental_Leave_Type_To_String(Parental_Leave_Type type) {
    switch (type) {
        case Parental_Leave_Type::Maternity_Leave:
            return "Maternity Leave";
        case Parental_Leave_Type::Paternity_Leave:
            return "Paternity Leave";
        case Parental_Leave_Type::Adoptive_Parent_Leave:
            return "Adoptive Parent Leave";
        case Parental_Leave_Type::Shared_Parental_Leave:
            return "Shared Parental Leave";
        case Parental_Leave_Type::None:
            return "None";
    }
    return "None";
}

bool Leave_Request_Type_From_String(
        const std::string& value,
        Leave_Request_Type* type) {
        if (type == nullptr) {
            return false;
        }
        if (value == "Sick Leave") {
            *type = Leave_Request_Type::Sick;
        } else if (value == "Annual Leave") {
            *type = Leave_Request_Type::Annual;
        } else if (value == "Parental Leave") {
            *type = Leave_Request_Type::Parental;
        } else if (value == "Special Leave") {
            *type = Leave_Request_Type::Special;
        } else {
            return false;
        }
        return true;
    }

void Leave_Request_Status_From_String(
        const std::string& value,
        Leave_Request_Status* status) {
        if (status == nullptr) {
            return;
        }
        if (value == "Received") {
            *status = Leave_Request_Status::Received;
        } else if (value == "Approved" || value == "Accepted") {
            *status = Leave_Request_Status::Approved;
        } else if (value == "Denied") {
            *status = Leave_Request_Status::Denied;
        } else if (value == "Temporarily Denied") {
            *status = Leave_Request_Status::Temp_Denied;
        } else if (value == "Resolved") {
            *status = Leave_Request_Status::Resolved;
        } else {
            *status = Leave_Request_Status::Open;
        }
    }

void Parental_Leave_Type_From_String(
        const std::string& value,
        Parental_Leave_Type* type) {
        if (type == nullptr) {
            return;
        }
        if (value == "Maternity Leave") {
            *type = Parental_Leave_Type::Maternity_Leave;
        } else if (value == "Paternity Leave") {
            *type = Parental_Leave_Type::Paternity_Leave;
        } else if (value == "Adoptive Parent Leave") {
            *type = Parental_Leave_Type::Adoptive_Parent_Leave;
        } else if (value == "Shared Parental Leave") {
            *type = Parental_Leave_Type::Shared_Parental_Leave;
        }
    }

std::vector<Leave_Request> Load_Leave_Requests_From_Section(
        const std::string& username,
        const std::string& section_header) {
        std::vector<Leave_Request> requests;
        const std::filesystem::path record_path =
            Get_Employee_Record_File_Path_For_Username(username);
        std::ifstream input(record_path);
        if (record_path.empty() || !input.is_open()) {
            return requests;
        }

        bool in_section = false;
        bool in_request = false;
        Leave_Request request;
        request.employee_username = username;
        std::string line;
        while (std::getline(input, line)) {
            const std::string trimmed_line = Trim_Copy(line);
            if (trimmed_line == section_header) {
                in_section = true;
                continue;
            }
            if (in_section &&
                (trimmed_line == OpenLeaveRequestsHeader ||
                 trimmed_line == TempDeniedLeaveRequestsHeader ||
                 trimmed_line == ResolvedLeaveRequestsHeader)) {
                break;
            }
            if (!in_section) {
                continue;
            }
            if (trimmed_line == "Leave Request:") {
                request = Leave_Request{};
                request.employee_username = username;
                in_request = true;
                continue;
            }
            if (trimmed_line == "End Leave Request") {
                if (in_request && !request.request_id.empty()) {
                    requests.push_back(request);
                }
                in_request = false;
                continue;
            }
            if (!in_request) {
                continue;
            }

            const std::size_t separator = trimmed_line.find(':');
            if (separator == std::string::npos) {
                continue;
            }
            const std::string label =
                Trim_Copy(trimmed_line.substr(0, separator));
            const std::string value =
                Trim_Copy(trimmed_line.substr(separator + 1));
            if (label == "Request ID") {
                request.request_id = value;
            } else if (label == "Leave Type") {
                Leave_Request_Type_From_String(value, &request.type);
            } else if (label == "Parental Leave Type") {
                Parental_Leave_Type_From_String(
                    value, &request.parental_type);
            } else if (label == "Start Date") {
                request.start_date = value;
            } else if (label == "End Date") {
                request.end_date = value;
            } else if (label == "Employee Reason") {
                request.employee_reason = value;
            } else if (label == "Status" ||
                       label == "Resolution Status") {
                Leave_Request_Status_From_String(value, &request.status);
            } else if (label == "Submitted At") {
                request.date_created = value;
            } else if (label == "Resolved At" ||
                       label == "Resolution Date" ||
                       label == "Updated At") {
                request.date_updated = value;
            } else if (label == "Responded By" ||
                       label == "Resolved By") {
                request.admin_username = value;
            } else if (label == "Decision Reason" ||
                       label == "Admin Reason") {
                request.admin_reason = value;
            } else if (label == "Temporary Denial Reason") {
                request.temp_denial_other_text = value;
            } else if (label == "Status History") {
                request.status_history.push_back(value);
            }
        }
        if (in_request && !request.request_id.empty()) {
            requests.push_back(request);
        }
        return requests;
    }

void View_Current_User_Leave_Request_Section(
        const std::string& section_header) {
        const User_Session_Context& session =
            Get_Current_User_Session_Context();
        if (!session.authenticated || Trim_Copy(session.username).empty()) {
            std::cout << Display_Error << Account_Retrieval_Failure
                      << std::endl;
            return;
        }

        const std::vector<Leave_Request> requests =
            Load_Leave_Requests_From_Section(
                session.username, section_header);
        if (requests.empty()) {
            std::cout << Display_Info << No_Requests_Found_Message
                      << std::endl;
            return;
        }

        while (true) {
            Display_Leave_Request_List_UI(
                requests, "Return to HR Main Menu");
            const int return_option =
                static_cast<int>(requests.size()) + 1;
            std::cout << "Select an option (1-"
                      << return_option << "): ";
            int choice = 0;
            if (!Get_Validated_Menu_Choice(
                    1, return_option, &choice)) {
                continue;
            }
            if (choice == return_option) {
                return;
            }
            Display_Leave_Request_Details_UI(
                requests[static_cast<std::size_t>(choice - 1)]);
        }
    }
bool Parse_Date_Key(
        const std::string& value,
        int* date_key) {
    if (date_key == nullptr || value.size() != 10 ||
        (value[2] != '-' && value[2] != '/' && value[2] != '.') ||
        value[5] != value[2]) {
        return false;
    }
    for (std::size_t index : {0U, 1U, 3U, 4U, 6U, 7U, 8U, 9U}) {
        if (!std::isdigit(static_cast<unsigned char>(value[index]))) {
            return false;
        }
    }

    const int day = std::stoi(value.substr(0, 2));
    const int month = std::stoi(value.substr(3, 2));
    const int year = std::stoi(value.substr(6, 4));
    if (year < 2025 || year > 2100 || month < 1 || month > 12) {
        return false;
    }
    const bool leap_year =
        year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    const int days_per_month[] = {
        31, leap_year ? 29 : 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };
    if (day < 1 || day > days_per_month[month - 1]) {
        return false;
    }
    *date_key = year * 10000 + month * 100 + day;
    return true;
}

bool Save_Open_Leave_Request_To_Employee_Record(
        const Leave_Request& request) {
    const std::filesystem::path record_path =
        Get_Employee_Record_File_Path_For_Username(
            request.employee_username);
    if (record_path.empty()) {
        return false;
    }

    std::vector<std::string> lines;
    std::ifstream input(record_path);
    if (!input.is_open()) {
        return false;
    }
    std::string line;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    input.close();

    const auto open_section = std::find(
        lines.begin(), lines.end(), OpenLeaveRequestsHeader);
    const auto temp_denied_section = std::find(
        lines.begin(), lines.end(), TempDeniedLeaveRequestsHeader);
    if (open_section == lines.end() ||
        temp_denied_section == lines.end() ||
        open_section >= temp_denied_section) {
        return false;
    }

    std::vector<std::string> request_lines = {
        "Leave Request:",
        "Request ID: " + request.request_id,
        "Leave Type: " + Leave_Request_Type_To_String(request.type)
    };
    if (request.type == Leave_Request_Type::Parental) {
        request_lines.push_back(
            "Parental Leave Type: " +
            Parental_Leave_Type_To_String(request.parental_type));
    }
    request_lines.push_back("Start Date: " + request.start_date);
    request_lines.push_back("End Date: " + request.end_date);
    request_lines.push_back("Employee Reason: " + request.employee_reason);
    request_lines.push_back("Status: Open");
    request_lines.push_back("Submitted At: " + request.date_created);
    request_lines.push_back("Status History: " + request.date_created +
                            " | Open | Request submitted");
    request_lines.push_back("End Leave Request");
    request_lines.push_back("");

    const std::size_t insertion_index = static_cast<std::size_t>(
        std::distance(lines.begin(), temp_denied_section));
    lines.insert(
        lines.begin() + static_cast<std::ptrdiff_t>(insertion_index),
        request_lines.begin(),
        request_lines.end());

    std::filesystem::path temporary_path = record_path;
    temporary_path += ".leave-request.tmp";
    std::ofstream output(temporary_path, std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }
    for (const std::string& output_line : lines) {
        output << output_line << '\n';
    }
    output.close();
    if (!output) {
        std::filesystem::remove(temporary_path);
        return false;
    }

    std::error_code replace_error;
    std::filesystem::copy_file(
        temporary_path,
        record_path,
        std::filesystem::copy_options::overwrite_existing,
        replace_error);
    std::filesystem::remove(temporary_path);
    if (replace_error) {
        return false;
    }
    return true;
}

void Display_Leave_Request_Prompts(
        Leave_Request_Type type,
        std::string* start_date,
        std::string* end_date,
        std::string* reason) {
    if (type == Leave_Request_Type::Sick) {
        Display_Sick_Leave_Request_UI();
        Display_Sick_Leave_Request_Start_Date_UI();
    } else if (type == Leave_Request_Type::Annual) {
        Display_Annual_Leave_Request_UI();
        Display_Annual_Leave_Request_Start_Date_UI();
    } else if (type == Leave_Request_Type::Parental) {
        Display_Parental_Leave_Request_UI();
        Display_Parental_Leave_Request_Start_Date_UI();
    } else {
        Display_Special_Leave_Request_UI();
        Display_Special_Leave_Request_Start_Date_UI();
    }
    std::getline(std::cin, *start_date);

    if (type == Leave_Request_Type::Sick) {
        Display_Sick_Leave_Request_End_Date_UI();
    } else if (type == Leave_Request_Type::Annual) {
        Display_Annual_Leave_Request_End_Date_UI();
    } else if (type == Leave_Request_Type::Parental) {
        Display_Parental_Leave_Request_End_Date_UI();
    } else {
        Display_Special_Leave_Request_End_Date_UI();
    }
    std::getline(std::cin, *end_date);

    if (type == Leave_Request_Type::Sick) {
        Display_Sick_Leave_Request_Brief_Reason_UI();
    } else if (type == Leave_Request_Type::Annual) {
        Display_Annual_Leave_Request_Brief_Reason_UI();
    } else if (type == Leave_Request_Type::Parental) {
        Display_Parental_Leave_Request_Brief_Reason_UI();
    } else {
        Display_Special_Leave_Request_Brief_Reason_UI();
    }
    std::getline(std::cin, *reason);
}
}

Leave_Request Create_Leave_Request(
    const std::string& employee_username,
    Leave_Request_Type type,
    const std::string& start_date,
    const std::string& end_date,
    const std::string& reason,
    Parental_Leave_Type parental_type)
{
    Leave_Request req;

    req.employee_username = employee_username;
    req.admin_username = "";

    req.type = type;
    req.parental_type = parental_type;

    req.start_date = start_date;
    req.end_date = end_date;
    req.employee_reason = reason;

    req.status = Leave_Request_Status::Open;

    req.date_created = "";
    req.date_updated = req.date_created;

    req.reopened = false;

    return req;
}

void Receive_Leave_Request(Leave_Request& req, const std::string& admin_username) {
    req.status = Leave_Request_Status::Received;
    req.admin_username = admin_username;
    req.date_updated = "";
}

void Approve_Leave_Request(Leave_Request& req, const std::string& admin_reason) {
    req.status = Leave_Request_Status::Approved;
    req.admin_reason = admin_reason;
    req.date_updated = "";
}

void Deny_Leave_Request(Leave_Request& req, const std::string& admin_reason) {
    req.status = Leave_Request_Status::Denied;
    req.admin_reason = admin_reason;
    req.date_updated = "";
}

void Temp_Deny_Leave_Request(Leave_Request& req, const std::vector<Temp_Denial_Reason>& reasons, const std::string& other_text) {
    req.status = Leave_Request_Status::Temp_Denied;
    req.temp_denial_reasons = reasons;
    req.temp_denial_other_text = other_text;
    req.date_updated = "";
}

void Resolve_Leave_Request(Leave_Request& req) {
    req.status = Leave_Request_Status::Resolved;
    req.date_updated = "";
}

void Reopen_Temp_Denied_Request(Leave_Request& req) {
    req.reopened = true;
    req.status = Leave_Request_Status::Open;
    req.temp_denial_reasons.clear();
    req.temp_denial_other_text.clear();
    req.date_updated = "";
}

void Store_Leave_Request(Leave_Request_Storage& storage, const Leave_Request& req) {
    storage.requests_by_user[req.employee_username].push_back(req);
}

void Start_Current_User_Leave_Request_Workflow() {
    const User_Session_Context& session =
        Get_Current_User_Session_Context();
    if (!session.authenticated || Trim_Copy(session.username).empty()) {
        std::cout << Display_Error << Request_Save_Failure_Message
                  << std::endl;
        return;
    }

    while (true) {
        Display_Leave_Request_Type_Menu_UI();
        int leave_type_choice = 0;
        if (!Get_Validated_Menu_Choice(1, 5, &leave_type_choice)) {
            continue;
        }
        if (leave_type_choice == 5) {
            return;
        }

        const Leave_Request_Type leave_type =
            static_cast<Leave_Request_Type>(leave_type_choice - 1);
        Parental_Leave_Type parental_type = Parental_Leave_Type::None;
        if (leave_type == Leave_Request_Type::Parental) {
            while (true) {
                Display_Parental_Leave_Type_Question_UI();
                int parental_choice = 0;
                if (Get_Validated_Menu_Choice(1, 4, &parental_choice)) {
                    parental_type = static_cast<Parental_Leave_Type>(
                        parental_choice);
                    break;
                }
            }
        }

        std::string start_date;
        std::string end_date;
        std::string reason;
        Display_Leave_Request_Prompts(
            leave_type, &start_date, &end_date, &reason);

        int start_date_key = 0;
        int end_date_key = 0;
        if (!Parse_Date_Key(Trim_Copy(start_date), &start_date_key) ||
            !Parse_Date_Key(Trim_Copy(end_date), &end_date_key) ||
            end_date_key < start_date_key ||
            Trim_Copy(reason).empty()) {
            std::cout << Display_Error << Invalid_Input_Try_Again
                      << std::endl;
            continue;
        }

        Leave_Request request = Create_Leave_Request(
            session.username,
            leave_type,
            Trim_Copy(start_date),
            Trim_Copy(end_date),
            Trim_Copy(reason),
            parental_type);
        request.request_id = New_Leave_Request_ID();
        request.date_created = Current_Timestamp();
        request.date_updated = request.date_created;
        if (!Save_Open_Leave_Request_To_Employee_Record(request)) {
            std::cout << Display_Error << Request_Save_Failure_Message
                      << std::endl;
            return;
        }
        Display_Leave_Request_ID_UI(request.request_id);
        std::cout << Display_Success
                  << Request_Submission_Success_Message << std::endl;
        return;
    }
}

void View_Current_User_Open_Leave_Requests() {
    View_Current_User_Leave_Request_Section(
        OpenLeaveRequestsHeader);
}

void View_Current_User_Temp_Denied_Leave_Requests() {
    View_Current_User_Leave_Request_Section(
        TempDeniedLeaveRequestsHeader);
}

void View_Current_User_Resolved_Leave_Requests() {
    View_Current_User_Leave_Request_Section(
        ResolvedLeaveRequestsHeader);
}

std::vector<Leave_Request*> Get_Open_Requests(Leave_Request_Storage& storage, const std::string& username) {
    std::vector<Leave_Request*> open;
    for (auto& req : storage.requests_by_user[username]) {
        if (req.status == Leave_Request_Status::Open ||
            req.status == Leave_Request_Status::Temp_Denied)
        {
            open.push_back(&req);
        }
    }
    return open;
}
