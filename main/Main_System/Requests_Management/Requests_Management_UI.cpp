// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Requests_Management_UI.h"
#include "Requests_Management.h"
#include "../UI/Shared_UI_Messaging/Form_Labels_UI.h"

#include <iostream>

void Display_Request_Management_Menu_UI() {
    std::cout << "Request Management Menu:" << std::endl;
    std::cout << "1. View Open Requests" << std::endl;
    std::cout << "2. View Resolved Requests" << std::endl;
    std::cout << "3. Return to Dashboard Main Menu" << std::endl;
    std::cout << Please_Select_an_Option_Label << " (1-3): ";
}

void Display_Request_List_UI(
        const std::vector<Stored_Request>& requests,
        const std::string& return_option_label) {
    for (std::size_t i = 0; i < requests.size(); ++i) {
        const Stored_Request& request = requests[i];
        std::cout << (i + 1) << ". "
                  << request.request_id << " - "
                  << request.submitted_for << " - "
                  << Request_Status_To_String(request.status) << std::endl;
    }
    std::cout << (requests.size() + 1) << ". "
              << return_option_label << std::endl;
}

void Display_Request_Status_History_UI(
        const std::vector<Request_Status_Update>& status_history) {
    std::cout << "Request Status History:" << std::endl;
    for (const Request_Status_Update& update : status_history) {
        std::cout << "- " << update.updated_at << " | "
                  << Request_Status_To_String(update.status)
                  << " | " << update.updated_by;
        if (!update.reason.empty()) {
            std::cout << " | " << update.reason;
        }
        std::cout << std::endl;
    }
}

void Display_Request_Decision_Menu_UI() {
    std::cout << "Request Actions:" << std::endl;
    std::cout << "1. Approve Request" << std::endl;
    std::cout << "2. Temporarily Deny Request" << std::endl;
    std::cout << "3. Deny Request" << std::endl;
    std::cout << "4. Return to Request List" << std::endl;
    std::cout << Please_Select_an_Option_Label << " (1-4): ";
}

void Display_Request_Response_Reason_Prompt_UI() {
    std::cout << "Please enter the reason for this decision: ";
}
