// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef REQUEST_TYPES_H
#define REQUEST_TYPES_H

#include "../Employee Management/HR_Management/Grievances/Grievance_Form/Grievance_Form_UI.h"
#include "../Employee Management/Role_Management/Role_and_Salary_Details.h"
#include <map>
#include <string>
#include <vector>

enum class Request_Status {
    Open,
    Approved,
    Temporarily_Denied,
    Denied
};

struct Request_Status_Update {
    std::string updated_at;
    Request_Status status = Request_Status::Open;
    std::string updated_by;
    std::string reason;
};

struct Stored_Request {
    std::string request_type;
    std::string request_id;
    Request_Status status = Request_Status::Open;
    std::string submitted_by;
    std::string submitted_for;
    std::string business_role;
    std::string job_role;
    std::string created_at;
    std::string updated_at;
    std::map<std::string, std::string> fields;
    std::vector<Request_Status_Update> status_history;
};

struct Grievance_Request {
    Stored_Request request;
    Grievance_Details grievance_details;
};

enum class Leave_Request_Type {
    Sick,
    Annual,
    Parental,
    Special
};

enum class Leave_Request_Status {
    Open,
    Received,
    Approved,
    Denied,
    Temp_Denied,
    Resolved
};

enum class Temp_Denial_Reason {
    Needs_More_Info,
    Popular_Leave_Time,
    Shift_Conflict,
    No_Leave_Balance_Remaining,
    Scheduling_Requirements_Unmet,
    Set_Rules_For_Leave,
    Waiting_For_Other_Info,
    Other
};

struct Leave_Request {
    std::string request_id;
    std::string employee_username;
    std::string admin_username;
    Leave_Request_Type type = Leave_Request_Type::Sick;
    Parental_Leave_Type parental_type = Parental_Leave_Type::None;
    std::string start_date;
    std::string end_date;
    std::string employee_reason;
    Leave_Request_Status status = Leave_Request_Status::Open;
    std::vector<Temp_Denial_Reason> temp_denial_reasons;
    std::string temp_denial_other_text;
    std::string admin_reason;
    std::string date_created;
    std::string date_updated;
    std::vector<std::string> status_history;
    bool reopened = false;
};

struct Leave_Request_Storage {
    std::map<std::string, std::vector<Leave_Request>> requests_by_user;
};

#endif
