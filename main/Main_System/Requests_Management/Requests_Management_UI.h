// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef REQUESTS_MANAGEMENT_UI_H
#define REQUESTS_MANAGEMENT_UI_H

#include "Request_Types.h"
#include <string>
#include <vector>

void Display_Request_Management_Menu_UI();
void Display_Request_List_UI(
    const std::vector<Stored_Request>& requests,
    const std::string& return_option_label);
void Display_Request_Status_History_UI(
    const std::vector<Request_Status_Update>& status_history);
void Display_Request_Decision_Menu_UI();
void Display_Request_Response_Reason_Prompt_UI();

#endif
