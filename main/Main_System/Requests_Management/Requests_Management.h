// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef REQUESTS_MANAGEMENT_H
#define REQUESTS_MANAGEMENT_H

#include "Request_Types.h"
#include <map>
#include <string>
#include <vector>

std::string Request_Status_To_String(Request_Status status);
bool Save_New_Request(
    const std::string& request_type,
    const std::string& submitted_by,
    const std::string& submitted_for,
    const std::string& business_role,
    const std::string& job_role,
    const std::map<std::string, std::string>& fields,
    std::string* request_id);
std::vector<Stored_Request> Load_Requests_By_Type(
    const std::string& request_type);
bool Update_Request_Status(
    const std::string& request_type,
    const std::string& request_id,
    Request_Status status,
    const std::string& updated_by,
    const std::string& reason);

#endif
