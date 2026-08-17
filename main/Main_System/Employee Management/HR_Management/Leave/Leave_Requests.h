#ifndef LEAVE_REQUESTS_H
#define LEAVE_REQUESTS_H

#include "../../../Requests_Management/Request_Types.h"
#include <vector>
#include <string>

Leave_Request Create_Leave_Request(
    const std::string& employee_username,
    Leave_Request_Type type,
    const std::string& start_date,
    const std::string& end_date,
    const std::string& reason,
    Parental_Leave_Type parental_type = Parental_Leave_Type::None);

void Receive_Leave_Request(Leave_Request& req, const std::string& admin_username);
void Approve_Leave_Request(Leave_Request& req, const std::string& admin_reason);
void Deny_Leave_Request(Leave_Request& req, const std::string& admin_reason);
void Temp_Deny_Leave_Request(Leave_Request& req, const std::vector<Temp_Denial_Reason>& reasons, const std::string& other_text = "");
void Resolve_Leave_Request(Leave_Request& req);
void Reopen_Temp_Denied_Request(Leave_Request& req);

std::vector<Leave_Request*> Get_Approved_Requests(Leave_Request_Storage& storage, const std::string& username);
std::vector<Leave_Request*> Get_Denied_Requests(Leave_Request_Storage& storage, const std::string& username);
std::vector<Leave_Request*> Get_Temp_Denied_Requests(Leave_Request_Storage& storage, const std::string& username);
std::vector<Leave_Request*> Get_Resolved_Requests(Leave_Request_Storage& storage, const std::string& username);
std::vector<Leave_Request*> Get_Open_Requests(Leave_Request_Storage& storage, const std::string& username);

void Store_Leave_Request(Leave_Request_Storage& storage, const Leave_Request& req);
void Start_Current_User_Leave_Request_Workflow();
void View_Current_User_Open_Leave_Requests();
void View_Current_User_Temp_Denied_Leave_Requests();
void View_Current_User_Resolved_Leave_Requests();

#endif // LEAVE_REQUESTS_H
