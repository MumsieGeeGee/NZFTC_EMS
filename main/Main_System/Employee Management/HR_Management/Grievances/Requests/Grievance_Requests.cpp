// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Grievance_Requests.h"
#include "../Grievance_Form/Grievance_Service.h"
#include "../../../../Requests_Management/Requests_Management.h"
#include "../../../../Requests_Management/Requests_Management_UI.h"
#include "../../../../Account_Management/Account_Access/Account_Database.h"
#include "../../../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../../../../NZFTC_EMS/Session_Handling/Session_Handling.h"
#include "../../../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../../../UI/Shared_UI_Messaging/Info_Messages.h"
#include "../../../../UI/Shared_UI_Messaging/Success_Messages.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace {
constexpr const char Grievance_Request_Type[] = "Grievance";

std::string Get_Detail(
        const std::map<std::string, std::string>& details,
        const std::string& label) {
    const auto detail = details.find(label);
    return detail == details.end() ? "" : Trim_Copy(detail->second);
}

std::string Get_Employee_Name(const std::string& username) {
    std::map<std::string, std::string> details;
    if (!Get_Account_Record_Details_For_Username(username, &details)) {
        return username;
    }
    return Trim_Copy(
        Get_Detail(details, "First Name") + " " +
        Get_Detail(details, "Middle Name") + " " +
        Get_Detail(details, "Last Name"));
}

Grievance_Request To_Grievance_Request(const Stored_Request& request) {
    Grievance_Request grievance_request;
    grievance_request.request = request;
    grievance_request.grievance_details.Employee_Attending_Supervisor =
        Get_Detail(request.fields, "Attending Supervisor");
    grievance_request.grievance_details.Persons_Involved =
        Get_Detail(request.fields, "Persons Involved");
    grievance_request.grievance_details.Event_Date =
        Get_Detail(request.fields, "Event Date");
    grievance_request.grievance_details.Event_Details =
        Get_Detail(request.fields, "Event Details");
    grievance_request.grievance_details.Affect_Details =
        Get_Detail(request.fields, "Affect Details");
    grievance_request.grievance_details.Suggestions =
        Get_Detail(request.fields, "Suggestions");
    grievance_request.grievance_details.Additional =
        Get_Detail(request.fields, "Additional Comments");
    grievance_request.grievance_details.Declaration.I_Do =
        Get_Detail(request.fields, "Declaration");
    grievance_request.grievance_details.Declaration.agreed =
        grievance_request.grievance_details.Declaration.I_Do == "I do";
    grievance_request.grievance_details.Declaration.Signed_Date =
        Get_Detail(request.fields, "Signed Date");
    return grievance_request;
}

bool Is_Resolved(Request_Status status) {
    return status == Request_Status::Approved ||
           status == Request_Status::Denied;
}

bool Username_Is_In_Options(
        const std::string& username,
        const std::vector<Role_Assignment_Option>& options) {
    return std::any_of(
        options.begin(),
        options.end(),
        [&username](const Role_Assignment_Option& option) {
            return option.username == username;
        });
}

bool Current_User_Can_View_Request(
        const Stored_Request& request,
        bool* can_respond) {
    if (can_respond != nullptr) {
        *can_respond = false;
    }

    const User_Session_Context& session = Get_Current_User_Session_Context();
    std::map<std::string, std::string> current_user_details;
    if (!session.authenticated ||
        !Get_Account_Record_Details_For_Username(
            session.username, &current_user_details)) {
        return false;
    }

    const std::string job_role =
        Get_Detail(current_user_details, "Job Role");
    const std::string business_role =
        Get_Detail(current_user_details, "Business Role");
    if (job_role == "Human Resources Administrator") {
        if (can_respond != nullptr) {
            *can_respond = true;
        }
        return true;
    }

    const bool senior_manager =
        job_role == "Senior Level Manager" ||
        job_role == "Senior Finance Manager" ||
        job_role == "Senior Trade Manager";
    if (senior_manager && request.business_role == business_role) {
        if (can_respond != nullptr) {
            *can_respond = true;
        }
        return true;
    }

    if (job_role == "Senior Mentor" &&
        Username_Is_In_Options(
            request.submitted_for,
            Get_Assigned_Team_Members(session.username))) {
        if (can_respond != nullptr) {
            *can_respond = true;
        }
        return true;
    }

    if ((job_role == "Executive Assistant" ||
         job_role == "Personal Assistant") &&
        Username_Is_In_Options(
            request.submitted_for,
            Get_Managers_Assistant_Can_Act_On_Behalf_Of(
                session.username))) {
        return true;
    }

    return request.submitted_by == session.username ||
           request.submitted_for == session.username;
}

std::vector<Stored_Request> Get_Current_User_Requests(bool resolved) {
    const User_Session_Context& session = Get_Current_User_Session_Context();
    std::vector<Stored_Request> requests;
    for (const Stored_Request& request :
            Load_Requests_By_Type(Grievance_Request_Type)) {
        if (Is_Resolved(request.status) != resolved) {
            continue;
        }
        if (request.submitted_by == session.username ||
            request.submitted_for == session.username) {
            requests.push_back(request);
        }
    }
    return requests;
}

std::vector<Stored_Request> Get_Manageable_Requests(bool resolved) {
    std::vector<Stored_Request> requests;
    for (const Stored_Request& request :
            Load_Requests_By_Type(Grievance_Request_Type)) {
        bool can_respond = false;
        if (Is_Resolved(request.status) == resolved &&
            Current_User_Can_View_Request(request, &can_respond)) {
            requests.push_back(request);
        }
    }
    return requests;
}

void Display_And_Select_Request(
        const std::vector<Stored_Request>& requests,
        bool allow_decision,
        const std::string& return_option_label) {
    if (requests.empty()) {
        std::cout << Display_Info << No_Requests_Found_Message << std::endl;
        return;
    }

    while (true) {
        Display_Request_List_UI(requests, return_option_label);
        const int return_option = static_cast<int>(requests.size()) + 1;
        std::cout << "Select an option (1-"
                  << return_option << "): ";
        int selection = 0;
        if (!Get_Validated_Menu_Choice(1, return_option, &selection)) {
            continue;
        }
        if (selection == return_option) {
            return;
        }

        const Stored_Request& selected =
            requests[static_cast<std::size_t>(selection - 1)];
        Display_Grievance_Request(To_Grievance_Request(selected));
        if (!allow_decision || Is_Resolved(selected.status)) {
            continue;
        }

        bool can_respond = false;
        Current_User_Can_View_Request(selected, &can_respond);
        if (!can_respond) {
            continue;
        }

        int decision = 0;
        while (true) {
            Display_Request_Decision_Menu_UI();
            if (Get_Validated_Menu_Choice(1, 4, &decision)) {
                break;
            }
        }
        if (decision == 4) {
            continue;
        }

        Display_Request_Response_Reason_Prompt_UI();
        std::string reason;
        std::getline(std::cin, reason);
        if (Trim_Copy(reason).empty()) {
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            continue;
        }

        Request_Status status = Request_Status::Approved;
        if (decision == 2) {
            status = Request_Status::Temporarily_Denied;
        } else if (decision == 3) {
            status = Request_Status::Denied;
        }

        const User_Session_Context& session =
            Get_Current_User_Session_Context();
        if (Update_Request_Status(
                Grievance_Request_Type,
                selected.request_id,
                status,
                session.username,
                reason)) {
            std::cout << Display_Success
                      << Request_Status_Update_Success_Message << std::endl;
            return;
        }
        std::cout << Display_Error << Request_Update_Failure_Message
                  << std::endl;
    }
}
}

bool Save_Grievance_Request(
        const Grievance_Details& grievance_details,
        const std::string& submitted_by,
        const std::string& submitted_for,
        std::string* request_id) {
    std::map<std::string, std::string> employee_details;
    if (!Get_Account_Record_Details_For_Username(
            submitted_for, &employee_details)) {
        return false;
    }

    const std::map<std::string, std::string> fields = {
        {"Employee Name", Get_Employee_Name(submitted_for)},
        {"Attending Supervisor",
         grievance_details.Employee_Attending_Supervisor},
        {"Persons Involved", grievance_details.Persons_Involved},
        {"Event Date", grievance_details.Event_Date},
        {"Event Details", grievance_details.Event_Details},
        {"Affect Details", grievance_details.Affect_Details},
        {"Suggestions", grievance_details.Suggestions},
        {"Additional Comments", grievance_details.Additional},
        {"Declaration", grievance_details.Declaration.I_Do},
        {"Signed Date", grievance_details.Declaration.Signed_Date}
    };
    return Save_New_Request(
        Grievance_Request_Type,
        submitted_by,
        submitted_for,
        Get_Detail(employee_details, "Business Role"),
        Get_Detail(employee_details, "Job Role"),
        fields,
        request_id);
}

void Display_Grievance_Request(
        const Grievance_Request& grievance_request) {
    Grievance_Service service;
    service.Display_Form(
        grievance_request.grievance_details,
        Get_Employee_Name(grievance_request.request.submitted_for));
    std::cout << "Request ID: "
              << grievance_request.request.request_id << std::endl;
    std::cout << "Status: "
              << Request_Status_To_String(
                     grievance_request.request.status) << std::endl;
    Display_Request_Status_History_UI(
        grievance_request.request.status_history);
}

void View_Current_User_Open_Grievance_Requests() {
    Display_And_Select_Request(
        Get_Current_User_Requests(false),
        false,
        "Return to HR Main Menu");
}

void View_Current_User_Resolved_Grievance_Requests() {
    Display_And_Select_Request(
        Get_Current_User_Requests(true),
        false,
        "Return to HR Main Menu");
}

void Run_Grievance_Request_Management_Menu() {
    while (true) {
        Display_Request_Management_Menu_UI();
        int choice = 0;
        if (!Get_Validated_Menu_Choice(1, 3, &choice)) {
            continue;
        }
        if (choice == 3) {
            return;
        }
        Display_And_Select_Request(
            Get_Manageable_Requests(choice == 2),
            choice == 1,
            "Return to Request Management Menu");
    }
}