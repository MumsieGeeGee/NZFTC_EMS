// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Requests_Management.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "../Path_Utilities.h"

namespace {
std::string Trim_Copy(const std::string& value) {
    const std::size_t first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

std::string Current_Timestamp() {
    const std::time_t now = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());
    std::tm local_time{};
    localtime_s(&local_time, &now);
    std::ostringstream output;
    output << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
    return output.str();
}

std::string File_Timestamp() {
    const auto now = std::chrono::system_clock::now();
    const auto milliseconds =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count();
    return std::to_string(milliseconds);
}

std::string Normalize_Path_Component(std::string value) {
    for (char& character : value) {
        if (!std::isalnum(static_cast<unsigned char>(character)) &&
            character != '-' && character != '_') {
            character = '_';
        }
    }
    return value;
}

std::string Sanitize_Field_Value(std::string value) {
    std::replace(value.begin(), value.end(), '\r', ' ');
    std::replace(value.begin(), value.end(), '\n', ' ');
    std::replace(value.begin(), value.end(), '\t', ' ');
    return Trim_Copy(value);
}

bool Request_Status_From_String(
        const std::string& value,
        Request_Status* status) {
    if (status == nullptr) {
        return false;
    }
    if (value == "Open") {
        *status = Request_Status::Open;
    } else if (value == "Approved") {
        *status = Request_Status::Approved;
    } else if (value == "Temporarily Denied") {
        *status = Request_Status::Temporarily_Denied;
    } else if (value == "Denied") {
        *status = Request_Status::Denied;
    } else {
        return false;
    }
    return true;
}

std::filesystem::path Get_Request_Directory(
        const std::string& request_type) {
    const std::string directory_name =
        request_type == "Grievance"
            ? "Grievances"
            : Normalize_Path_Component(request_type);
    return Path_Utilities::Get_Employee_Records_Directory() / directory_name;
}

std::filesystem::path Get_Request_Path(
        const std::string& request_type,
        const std::string& request_id) {
    return Get_Request_Directory(request_type) /
        (Normalize_Path_Component(request_id) + ".txt");
}

void Parse_History(
        const std::string& value,
        std::vector<Request_Status_Update>* history) {
    if (history == nullptr) {
        return;
    }
    std::istringstream input(value);
    Request_Status_Update update;
    std::string status;
    std::getline(input, update.updated_at, '\t');
    std::getline(input, status, '\t');
    std::getline(input, update.updated_by, '\t');
    std::getline(input, update.reason);
    if (Request_Status_From_String(status, &update.status)) {
        history->push_back(update);
    }
}
}

std::string Request_Status_To_String(Request_Status status) {
    switch (status) {
        case Request_Status::Open:
            return "Open";
        case Request_Status::Approved:
            return "Approved";
        case Request_Status::Temporarily_Denied:
            return "Temporarily Denied";
        case Request_Status::Denied:
            return "Denied";
    }
    return "Open";
}

bool Save_New_Request(
        const std::string& request_type,
        const std::string& submitted_by,
        const std::string& submitted_for,
        const std::string& business_role,
        const std::string& job_role,
        const std::map<std::string, std::string>& fields,
        std::string* request_id) {
    if (Trim_Copy(request_type).empty() ||
        Trim_Copy(submitted_by).empty() ||
        Trim_Copy(submitted_for).empty()) {
        return false;
    }

    const std::string id =
        Normalize_Path_Component(request_type) + "-" + File_Timestamp();
    const std::filesystem::path directory =
        Get_Request_Directory(request_type);
    std::filesystem::create_directories(directory);

    std::ofstream output(Get_Request_Path(request_type, id));
    if (!output.is_open()) {
        return false;
    }

    const std::string timestamp = Current_Timestamp();
    output << "Request Record\n";
    output << "Request Type: " << Sanitize_Field_Value(request_type) << '\n';
    output << "Request ID: " << id << '\n';
    output << "Status: Open\n";
    output << "Submitted By: " << Sanitize_Field_Value(submitted_by) << '\n';
    output << "Submitted For: " << Sanitize_Field_Value(submitted_for) << '\n';
    output << "Business Role: " << Sanitize_Field_Value(business_role) << '\n';
    output << "Job Role: " << Sanitize_Field_Value(job_role) << '\n';
    output << "Created At: " << timestamp << '\n';
    output << "Updated At: " << timestamp << '\n';
    for (const auto& field : fields) {
        output << field.first << ": "
               << Sanitize_Field_Value(field.second) << '\n';
    }
    output << "History Entry: " << timestamp
           << "\tOpen\t" << Sanitize_Field_Value(submitted_by)
           << "\tRequest submitted\n";

    if (!output) {
        return false;
    }
    if (request_id != nullptr) {
        *request_id = id;
    }
    return true;
}

std::vector<Stored_Request> Load_Requests_By_Type(
        const std::string& request_type) {
    std::vector<Stored_Request> requests;
    const std::filesystem::path directory =
        Get_Request_Directory(request_type);
    if (!std::filesystem::exists(directory)) {
        return requests;
    }

    for (const std::filesystem::directory_entry& entry :
            std::filesystem::directory_iterator(directory)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".txt") {
            continue;
        }
        std::ifstream input(entry.path());
        if (!input.is_open()) {
            continue;
        }

        Stored_Request request;
        std::string line;
        while (std::getline(input, line)) {
            const std::size_t separator = line.find(':');
            if (separator == std::string::npos) {
                continue;
            }
            const std::string label = Trim_Copy(line.substr(0, separator));
            const std::string value = Trim_Copy(line.substr(separator + 1));
            if (label == "Request Type") {
                request.request_type = value;
            } else if (label == "Request ID") {
                request.request_id = value;
            } else if (label == "Status") {
                Request_Status_From_String(value, &request.status);
            } else if (label == "Submitted By") {
                request.submitted_by = value;
            } else if (label == "Submitted For") {
                request.submitted_for = value;
            } else if (label == "Business Role") {
                request.business_role = value;
            } else if (label == "Job Role") {
                request.job_role = value;
            } else if (label == "Created At") {
                request.created_at = value;
            } else if (label == "Updated At") {
                request.updated_at = value;
            } else if (label == "History Entry") {
                Parse_History(value, &request.status_history);
            } else {
                request.fields[label] = value;
            }
        }
        if (!request.request_id.empty()) {
            requests.push_back(request);
        }
    }

    std::sort(
        requests.begin(),
        requests.end(),
        [](const Stored_Request& left, const Stored_Request& right) {
            return left.created_at > right.created_at;
        });
    return requests;
}

bool Update_Request_Status(
        const std::string& request_type,
        const std::string& request_id,
        Request_Status status,
        const std::string& updated_by,
        const std::string& reason) {
    const std::filesystem::path path =
        Get_Request_Path(request_type, request_id);
    std::ifstream input(path);
    if (!input.is_open()) {
        return false;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line)) {
        lines.push_back(line);
    }
    input.close();

    const std::string timestamp = Current_Timestamp();
    for (std::string& existing_line : lines) {
        if (existing_line.rfind("Status: ", 0) == 0) {
            existing_line = "Status: " + Request_Status_To_String(status);
        } else if (existing_line.rfind("Updated At: ", 0) == 0) {
            existing_line = "Updated At: " + timestamp;
        }
    }
    lines.push_back(
        "History Entry: " + timestamp + "\t" +
        Request_Status_To_String(status) + "\t" +
        Sanitize_Field_Value(updated_by) + "\t" +
        Sanitize_Field_Value(reason));

    std::ofstream output(path, std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }
    for (const std::string& output_line : lines) {
        output << output_line << '\n';
    }
    return static_cast<bool>(output);
}
