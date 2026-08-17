// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Leave_Requests_UI.h"
#include "../../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../../UI/Shared_UI_Messaging/Warning_Messages.h"
#include <string>
#include <iostream>

void Display_Leave_Request_Type_Menu_UI() {
    std::cout << "Apply for Leave:" << std::endl;
    std::cout << "1. Sick Leave" << std::endl;
    std::cout << "2. Annual Leave" << std::endl;
    std::cout << "3. Parental Leave" << std::endl;
    std::cout << "4. Special Leave" << std::endl;
    std::cout << "5. Return to HR Main Menu" << std::endl;
    std::cout << Please_Select_an_Option_Label << " (1-5): ";
}

void Display_Sick_Leave_Request_UI() {
    std::cout << Sick_Leave_Label << " " << Request_Label << std::endl;
}

void Display_Sick_Leave_Request_Start_Date_UI() {
    std::cout << Enter_Start_Date_Label << " " << Sick_Leave_Label << ": ";
}

void Display_Sick_Leave_Request_End_Date_UI() {
    std::cout << Enter_End_Date_Label << " " << Sick_Leave_Label << ": ";
}

void Display_Sick_Leave_Request_Brief_Reason_UI() {
    std::cout << Enter_Brief_Reason_Description_Label << " " << Sick_Leave_Label << ": ";
}

void Display_Annual_Leave_Request_UI() {
    std::cout << Annual_Leave_Label << " " << Request_Label << std::endl;
}

void Display_Annual_Leave_Request_Start_Date_UI() {
    std::cout << Enter_Start_Date_Label << " " << Annual_Leave_Label << ": ";
}

void Display_Annual_Leave_Request_End_Date_UI() {
    std::cout << Enter_End_Date_Label << " " << Annual_Leave_Label << ": ";
}

void Display_Annual_Leave_Request_Brief_Reason_UI() {
    std::cout << Enter_Brief_Reason_Description_Label << " " << Annual_Leave_Label << ": ";
}

void Display_Parental_Leave_Request_UI() {
    std::cout << Parental_Leave_Label << " " << Request_Label << std::endl;
}

void Display_Parental_Leave_Type_Question_UI() {
    std::cout << "Which Parent Leave Type are you applying for?" << std::endl;
    std::cout << "1. " << Maternity_Leave_Label << std::endl;
    std::cout << "2. " << Paternity_Leave_Label << std::endl;
    std::cout << "3. " << Adoptive_Parent_Leave_Label << std::endl;
    std::cout << "4. " << Shared_Parent_Leave_Label << std::endl;
    std::cout << "Enter Here (1, 2, 3 or 4): ";
}

void Display_Parental_Leave_Request_Start_Date_UI() {
    std::cout << Enter_Start_Date_Label << " " << Parental_Leave_Label << ": ";
}

void Display_Parental_Leave_Request_End_Date_UI() {
    std::cout << Enter_End_Date_Label << " " << Parental_Leave_Label << ": ";
}

void Display_Parental_Leave_Request_Brief_Reason_UI() {
    std::cout << Enter_Brief_Reason_Description_Label << " " << Parental_Leave_Label << ": ";
}

void Display_Special_Leave_Request_UI() {
    std::cout << Special_Leave_Label << " " << Request_Label << std::endl;
}

void Display_Special_Leave_Request_Start_Date_UI() {
    std::cout << Enter_Start_Date_Label << " " << Special_Leave_Label << ": ";
}

void Display_Special_Leave_Request_End_Date_UI() {
    std::cout << Enter_End_Date_Label << " " << Special_Leave_Label << ": ";
}

void Display_Special_Leave_Request_Brief_Reason_UI() {
    std::cout << Enter_Brief_Reason_Description_Label << " " << Special_Leave_Label << ": ";
}

void Display_Received_Sick_Leave_Request_UI() {
    std::cout << Sick_Leave_Label << " " << Received_Label << std::endl;
}

void Display_Received_Annual_Leave_Request_UI() {
    std::cout << Annual_Leave_Label << " " << Received_Label << std::endl;
}

void Display_Received_Parental_Leave_Request_UI() {
    std::cout << Parental_Leave_Label << " " << Received_Label << std::endl;
}

void Display_Received_Special_Leave_Request_UI() {
    std::cout << Special_Leave_Label << " " << Received_Label << std::endl;
}

void Display_Received_Leave_Request_Decision_Options_UI() {
    std::cout << "Please select an action:" << std::endl;
    std::cout << "1. " << Approved_Label << std::endl;
    std::cout << "2. " << Denied_Label << std::endl;
    std::cout << "3. " << Temp_Denied_Label << std::endl;
    std::cout << "Enter Here (1, 2 or 3): ";
}

void Display_Temp_Denial_Reasons_UI() {
    std::cout << Temp_Denied_Label << " " << Reason_Label << std::endl;
    std::cout << "Please select all applicable reasons for temporary denial." << std::endl;
    std::cout << "These reasons will be displayed to the employee in the order you enter them." << std::endl;
    std::cout << "Separate multiple selections with a comma." << std::endl;
    std::cout << "1. " << Needs_More_Info_Label << std::endl;
    std::cout << "2. " << Popular_Leave_Time_Label << std::endl;
    std::cout << "3. " << Shift_Conflict_Label << std::endl;
    std::cout << "4. " << No_Leave_Balance_Remaining_Label << std::endl;
    std::cout << "5. " << Scheduling_Requirements_Currently_Unmet_Label << std::endl;
    std::cout << "6. " << Set_Rules_for_Leave_Request_Label << std::endl;
    std::cout << "7. " << Waiting_for_Other_Info_Label << std::endl;
    std::cout << "8. " << Does_Not_Met_Other_Leave_Criteria_Label << std::endl;
    std::cout << "Enter Here (e.g., 1,7 or 2,3,5,7,8): ";
}

void Display_Deny_Request_Reasons_UI() {
    std::cout << Denied_Label << " " << Reason_Label << std::endl;
    std::cout << "Please select all applicable reasons for temporary denial." << std::endl;
    std::cout << "1. " << Shift_Conflict_Label << std::endl;
    std::cout << "2. " << No_Leave_Balance_Remaining_Label << std::endl;
    std::cout << "3. " << Does_Not_Met_Other_Leave_Criteria_Label << std::endl;
    std::cout << "Enter Here (1, 2 or 3): ";
}

void Display_Approved_Requests_UI() {
    std::cout << Approved_Label << " " << Request_Label << "s: " << std::endl;
}

void Display_Denied_Requests_UI() {
    std::cout << Denied_Label << " " << Request_Label << "s: " << std::endl;
}

void Display_Temp_Denied_Requests_UI() {
    std::cout << Temp_Denied_Label << " " << Request_Label << "s: " << std::endl;
}

void Display_Open_Sick_Leave_Requests_UI() {
    std::cout << Open_Label << " " << Sick_Leave_Label << " " << Request_Label << "s: " << std::endl;
}

void Display_Open_Annual_Leave_Requests_UI() {
    std::cout << Open_Label << " " << Annual_Leave_Label << " " << Request_Label << "s: " << std::endl;
}

void Display_Open_Parental_Leave_Requests_UI() {
    std::cout << Open_Label << " " << Parental_Leave_Label << " " << Request_Label << "s: " << std::endl;
}

void Display_Open_Special_Leave_Requests_UI() {
    std::cout << Open_Label << " " << Special_Leave_Label << " " << Request_Label << "s: " << std::endl;
}

void Display_Resolved_Sick_Leave_Requests_UI() {
    std::cout << Resolved_Label << " " << Sick_Leave_Label << " " << Request_Label << "s: " << std::endl;
}

void Display_Resolved_Annual_Leave_Requests_UI() {
    std::cout << Resolved_Label << " " << Annual_Leave_Label << " " << Request_Label << "s: " << std::endl;
}

void Display_Resolved_Parental_Leave_Requests_UI() {
    std::cout << Resolved_Label << " " << Parental_Leave_Label << " " << Request_Label << "s: " << std::endl;
}

void Display_Resolved_Special_Leave_Requests_UI() {
    std::cout << Resolved_Label << " " << Special_Leave_Label << " " << Request_Label << "s: " << std::endl;
}

void Display_Reopen_Temp_Denied_Requests_UI() {
    std::cout << Reopen_Label << " " << Temp_Denied_Label << " " << Request_Label << "s" << std::endl;
    std::cout << "1. " << Reopen_Label << " " << For_Approval_Label << std::endl;
    std::cout << "2. " << Reopen_Label << " " << For_Full_Denial_Label << std::endl;
    std::cout << "Enter Here (1 or 2): ";
}

void Display_Temp_Denied_Requests() { // placeholder for a dynamic numbered list that displays the "temp denied" requests (open requests area, NOT resolved)

}

void Display_Reopen_for_Approval_UI() {
    Display_Temp_Denied_Requests(); // placeholder for a dynamic numbered list that displays the "temp denied" requests (open requests area, NOT resolved)
    std::cout << "" << std::endl;
}

void Display_Reopen_for_Full_Denial_UI() {
    Display_Temp_Denied_Requests(); // placeholder for a dynamic numbered list that displays the "temp denied" requests (open requests area, NOT resolved)
    std::cout << "" << std::endl;
}

void Display_Leave_Request_ID_UI(
        const std::string& request_id) {
    std::cout << "Request ID: " << request_id << std::endl;
}

namespace {
std::string Leave_Type_Label(Leave_Request_Type type) {
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

std::string Leave_Status_Label(Leave_Request_Status status) {
    switch (status) {
        case Leave_Request_Status::Open:
            return "Open";
        case Leave_Request_Status::Received:
            return "Received";
        case Leave_Request_Status::Approved:
            return "Approved";
        case Leave_Request_Status::Denied:
            return "Denied";
        case Leave_Request_Status::Temp_Denied:
            return "Temporarily Denied";
        case Leave_Request_Status::Resolved:
            return "Resolved";
    }
    return "";
}
}

void Display_Leave_Request_List_UI(
        const std::vector<Leave_Request>& requests,
        const std::string& return_option_label) {
    for (std::size_t index = 0; index < requests.size(); ++index) {
        const Leave_Request& request = requests[index];
        std::cout << (index + 1) << ". "
                  << request.request_id << " - "
                  << Leave_Type_Label(request.type) << " - "
                  << request.start_date << " to " << request.end_date
                  << " - " << Leave_Status_Label(request.status)
                  << std::endl;
    }
    std::cout << (requests.size() + 1) << ". "
              << return_option_label << std::endl;
}

void Display_Leave_Request_Details_UI(
        const Leave_Request& request) {
    std::cout << "Leave Request Details:" << std::endl;
    std::cout << "Request ID: " << request.request_id << std::endl;
    std::cout << "Leave Type: " << Leave_Type_Label(request.type)
              << std::endl;
    std::cout << "Start Date: " << request.start_date << std::endl;
    std::cout << "End Date: " << request.end_date << std::endl;
    std::cout << "Employee Reason: " << request.employee_reason
              << std::endl;
    std::cout << "Status: " << Leave_Status_Label(request.status)
              << std::endl;
    std::cout << "Submitted At: " << request.date_created << std::endl;
    if (!request.admin_username.empty()) {
        std::cout << "Responded By: " << request.admin_username
                  << std::endl;
    }
    if (!request.admin_reason.empty()) {
        std::cout << "Decision Reason: " << request.admin_reason
                  << std::endl;
    }
    if (!request.date_updated.empty() &&
        request.date_updated != request.date_created) {
        std::cout << "Resolved/Updated At: " << request.date_updated
                  << std::endl;
    }
    if (!request.status_history.empty()) {
        std::cout << "Status History:" << std::endl;
        for (const std::string& history_entry :
                request.status_history) {
            std::cout << "- " << history_entry << std::endl;
        }
    }
}
