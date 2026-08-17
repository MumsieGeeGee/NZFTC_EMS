// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef LEAVE_REQUESTS_UI_H
#define LEAVE_REQUESTS_UI_H

#include <string>
#include <vector>
#include "../../../Requests_Management/Request_Types.h"

void Display_Leave_Request_Type_Menu_UI();
void Display_Sick_Leave_Request_UI();
void Display_Sick_Leave_Request_Start_Date_UI();
void Display_Sick_Leave_Request_End_Date_UI();
void Display_Sick_Leave_Request_Brief_Reason_UI();
void Display_Annual_Leave_Request_UI();
void Display_Annual_Leave_Request_Start_Date_UI();
void Display_Annual_Leave_Request_End_Date_UI();
void Display_Annual_Leave_Request_Brief_Reason_UI();
void Display_Parental_Leave_Request_UI();
void Display_Parental_Leave_Type_Question_UI();
void Display_Parental_Leave_Request_Start_Date_UI();
void Display_Parental_Leave_Request_End_Date_UI();
void Display_Parental_Leave_Request_Brief_Reason_UI();
void Display_Special_Leave_Request_UI();
void Display_Special_Leave_Request_Start_Date_UI();
void Display_Special_Leave_Request_End_Date_UI();
void Display_Special_Leave_Request_Brief_Reason_UI();
void Display_Received_Sick_Leave_Request_UI();
void Display_Received_Annual_Leave_Request_UI();
void Display_Received_Parental_Leave_Request_UI();
void Display_Received_Special_Leave_Request_UI();
void Display_Received_Leave_Request_Decision_Options_UI();
void Display_Temp_Denial_Reasons_UI();
void Display_Deny_Request_Reasons_UI();
void Display_Approved_Requests_UI();
void Display_Denied_Requests_UI();
void Display_Temp_Denied_Requests_UI();
void Display_Open_Sick_Leave_Requests_UI();
void Display_Open_Annual_Leave_Requests_UI();
void Display_Open_Parental_Leave_Requests_UI();
void Display_Open_Special_Leave_Requests_UI();
void Display_Resolved_Sick_Leave_Requests_UI();
void Display_Resolved_Annual_Leave_Requests_UI();
void Display_Resolved_Parental_Leave_Requsts_UI();
void Display_Resolved_Special_Leave_Requests_UI();
void Display_Reopen_Temp_Denied_Requests_UI();
void Display_Temp_Denied_Requests(); // placeholder for a dynamic numbered list that displays the "temp denied" requests (open requests area, NOT resolved)
void Display_Reopen_for_Approval_UI();
void Display_Reopen_for_Full_Denial_UI();
void Display_Leave_Request_ID_UI(
    const std::string& request_id);
void Display_Leave_Request_List_UI(
    const std::vector<Leave_Request>& requests,
    const std::string& return_option_label);
void Display_Leave_Request_Details_UI(
    const Leave_Request& request);


#endif // LEAVE_REQUESTS_UI_H
