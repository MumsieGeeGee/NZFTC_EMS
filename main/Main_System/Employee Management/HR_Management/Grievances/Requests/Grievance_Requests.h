// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef GRIEVANCE_REQUESTS_H
#define GRIEVANCE_REQUESTS_H

#include "../../../../Requests_Management/Request_Types.h"
#include <string>

bool Save_Grievance_Request(
    const Grievance_Details& grievance_details,
    const std::string& submitted_by,
    const std::string& submitted_for,
    std::string* request_id);
void Display_Grievance_Request(const Grievance_Request& grievance_request);
void View_Current_User_Open_Grievance_Requests();
void View_Current_User_Resolved_Grievance_Requests();
void Run_Grievance_Request_Management_Menu();

#endif