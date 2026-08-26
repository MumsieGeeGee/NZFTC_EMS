// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Account_Database.h"
#include "../Account_Management/Input_Validation_Helpers.h"
#include "../Account_Management/Save_Account_Details.h"
#include "../Account_Management/Security_Protocols.h"
#include "../../Path_Utilities.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <vector>

bool Save_IRD_Record_Field(const std::string& IRD_Number, const std::string& fieldLabel, const std::string& value);

namespace {
constexpr const char RecordFileExtension[] = ".txt";
constexpr const char EmployeeRecordSuffix[] = "_Employee_Record";
constexpr const char EmployeeRecordTitle[] = "Employee Account Information Record";
constexpr const char OpenLeaveRequestsHeader[] = "Current Open Leave Requests:";
constexpr const char TempDeniedLeaveRequestsHeader[] = "Current Temp Denied Leave Requests:";
constexpr const char ResolvedLeaveRequestsHeader[] = "Current Resolved Leave Requests:";

struct Account_Record_State {
    std::string ird_number;
    std::string first_name;
    std::string middle_name;
    std::string last_name;
    std::string business_role;
    std::map<std::string, std::string> extra_fields;
    std::filesystem::path employee_record_path;
};

Account_Record_State account_record_state;

const std::array<std::string, 10> OrderedEmployeeRecordLabels = {
    "Account Type",
    "First Name",
    "Middle Name",
    "Last Name",
    "Date of Birth",
    "Address",
    "Email Address",
    "Phone Number",
    "Username",
    "Password"
};

const std::array<std::string, 12> OrderedTaxRecordLabels = {
    "IRD Number",
    "Tax Code",
    "PAYE",
    "KiwiSaver",
    "ACC",
    "Student Loan",
    "Child Support",
    "ESCT",
    "GST",
    "Salary Package",
    "Business Role",
    "Job Role"
};

std::filesystem::path Get_Employee_Records_Directory_Path() {
    return Path_Utilities::Get_Employee_Records_Directory();
}

std::filesystem::path Get_Username_Index_Path() {
    return Get_Employee_Records_Directory_Path() / "Usernames.txt";
}

std::filesystem::path Get_Password_Index_Path() {
    return Get_Employee_Records_Directory_Path() / "Passwords.txt";
}

std::filesystem::path Get_IRD_Username_Index_Path() {
    return Get_Employee_Records_Directory_Path() / "IRD" / "IRD_Username.txt";
}

std::string Normalize_Record_Name_Component(std::string component) {
    component = Trim_Copy(component);
    std::replace(component.begin(), component.end(), ' ', '_');
    return component;
}

std::string Get_Middle_Name_Token() {
    const std::string normalizedMiddle = Normalize_Record_Name_Component(account_record_state.middle_name);
    if (normalizedMiddle.empty()) {
        return "#";
    }
    return normalizedMiddle;
}

bool Has_Identity() {
    return !Trim_Copy(account_record_state.first_name).empty() && !Trim_Copy(account_record_state.last_name).empty();
}

std::string Build_Record_Base_Stem(int suffix) {
    const std::string firstName = Normalize_Record_Name_Component(account_record_state.first_name);
    const std::string lastName = Normalize_Record_Name_Component(account_record_state.last_name);
    std::string middleName = Get_Middle_Name_Token();
    if (suffix > 0) {
        if (middleName == "#") {
            middleName += std::to_string(suffix);
        } else {
            middleName += "#" + std::to_string(suffix);
        }
    }

    return firstName + "_" + middleName + "_" + lastName;
}

std::filesystem::path Get_Business_Role_Directory_Path(const std::string& business_role_value) {
    const std::string business_role = Trim_Copy(business_role_value);
    if (business_role == "Admin" || business_role == "Finance" || business_role == "Trade") {
        return Get_Employee_Records_Directory_Path() / business_role;
    }
    return std::filesystem::path();
}

std::filesystem::path Get_Business_Role_Directory_Path() {
    return Get_Business_Role_Directory_Path(account_record_state.business_role);
}

std::filesystem::path Build_Employee_Record_Path_From_Stem(const std::string& baseStem) {
    return Get_Business_Role_Directory_Path() / (baseStem + EmployeeRecordSuffix + RecordFileExtension);
}

bool Resolve_Record_Paths() {
    if (!account_record_state.employee_record_path.empty()) {
        return true;
    }

    const std::filesystem::path business_role_directory = Get_Business_Role_Directory_Path();
    if (!Has_Identity() || business_role_directory.empty()) {
        return false;
    }
    std::filesystem::create_directories(business_role_directory);

    for (int suffix = 0; ; ++suffix) {
        const std::string baseStem = Build_Record_Base_Stem(suffix);
        const std::filesystem::path employeeCandidate = Build_Employee_Record_Path_From_Stem(baseStem);
        if (!std::filesystem::exists(employeeCandidate)) {
            account_record_state.employee_record_path = employeeCandidate;
            return true;
        }
    }
}

bool Move_Employee_Record_To_Business_Role_Directory(
        const std::filesystem::path& current_path,
        const std::string& business_role) {
    const std::filesystem::path destination_directory =
        Get_Business_Role_Directory_Path(business_role);
    if (destination_directory.empty()) {
        return false;
    }
    if (current_path.parent_path() == destination_directory) {
        return true;
    }

    std::filesystem::create_directories(destination_directory);
    const std::filesystem::path destination_path =
        destination_directory / current_path.filename();
    if (std::filesystem::exists(destination_path)) {
        return false;
    }

    std::error_code move_error;
    std::filesystem::rename(current_path, destination_path, move_error);
    return !move_error;
}

std::string To_Lower_Copy(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

bool Is_Tax_Field_Label(const std::string& fieldLabel) {
    static const std::array<std::string, 11> taxLabels = {
        "IRD Number",
        "Tax Code",
        "PAYE",
        "KiwiSaver",
        "ACC",
        "Student Loan",
        "Child Support",
        "ESCT",
        "GST",
        "Salary Package",
        "Tax Information"
    };

    if (std::find(taxLabels.begin(), taxLabels.end(), fieldLabel) != taxLabels.end()) {
        return true;
    }

    const std::string normalized = To_Lower_Copy(fieldLabel);
    return Starts_With(normalized, "tax ") || Starts_With(normalized, "tax_");
}

bool Upsert_Record_Field_In_Order(const std::filesystem::path& recordPath,
                                  const char* recordTitle,
                                  const std::array<std::string, 10>& orderedLabels,
                                  const std::string& fieldLabel,
                                  const std::string& fieldValue) {
    std::vector<std::string> lines;
    {
        std::ifstream inFile(recordPath);
        if (inFile) {
            std::string line;
            while (std::getline(inFile, line)) {
                lines.push_back(line);
            }
        }
    }

    if (lines.empty()) {
        lines.push_back(recordTitle);
    }

    const std::string linePrefix = fieldLabel + ": ";
    const std::string newLine = linePrefix + fieldValue;

    bool replaced = false;
    for (std::string& existingLine : lines) {
        if (Starts_With(existingLine, linePrefix)) {
            existingLine = newLine;
            replaced = true;
            break;
        }
    }

    if (!replaced) {
        std::size_t insertPos = lines.size();
        for (std::size_t lineIndex = 0; lineIndex < lines.size(); ++lineIndex) {
            if (lines[lineIndex] == OpenLeaveRequestsHeader) {
                insertPos = lineIndex;
                if (insertPos > 0 && lines[insertPos - 1].empty()) {
                    --insertPos;
                }
                break;
            }
        }
        std::size_t currentIndex = orderedLabels.size();
        for (std::size_t i = 0; i < orderedLabels.size(); ++i) {
            if (orderedLabels[i] == fieldLabel) {
                currentIndex = i;
                break;
            }
        }

        if (currentIndex < orderedLabels.size()) {
            for (std::size_t i = currentIndex + 1; i < orderedLabels.size(); ++i) {
                const std::string nextPrefix = orderedLabels[i] + ": ";
                for (std::size_t lineIndex = 0; lineIndex < lines.size(); ++lineIndex) {
                    if (Starts_With(lines[lineIndex], nextPrefix)) {
                        insertPos = lineIndex;
                        i = orderedLabels.size();
                        break;
                    }
                }
            }
        }

        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(insertPos), newLine);
    }

    std::ofstream outFile(recordPath, std::ios::trunc);
    if (!outFile) {
        return false;
    }

    for (const std::string& line : lines) {
        outFile << line << '\n';
    }

    return true;
}

bool Ensure_Leave_Request_Sections(
        const std::filesystem::path& record_path) {
    std::vector<std::string> lines;
    std::ifstream input(record_path);
    if (!input.is_open()) {
        return false;
    }

    bool has_open_section = false;
    bool has_temp_denied_section = false;
    bool has_resolved_section = false;
    std::string line;
    while (std::getline(input, line)) {
        lines.push_back(line);
        has_open_section = has_open_section ||
            line == OpenLeaveRequestsHeader;
        has_temp_denied_section = has_temp_denied_section ||
            line == TempDeniedLeaveRequestsHeader;
        has_resolved_section = has_resolved_section ||
            line == ResolvedLeaveRequestsHeader;
    }
    if (has_open_section && has_temp_denied_section &&
        has_resolved_section) {
        return true;
    }

    while (!lines.empty() && lines.back().empty()) {
        lines.pop_back();
    }
    lines.push_back("");
    if (!has_open_section) {
        lines.push_back(OpenLeaveRequestsHeader);
    }
    if (!has_temp_denied_section) {
        lines.push_back(TempDeniedLeaveRequestsHeader);
    }
    if (!has_resolved_section) {
        lines.push_back(ResolvedLeaveRequestsHeader);
    }

    std::ofstream output(record_path, std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }
    for (const std::string& output_line : lines) {
        output << output_line << '\n';
    }
    return static_cast<bool>(output);
}

bool Persist_Field(const std::string& fieldLabel, const std::string& fieldValue) {
    if (Is_Tax_Field_Label(fieldLabel)) {
        const std::string normalizedIRD = Trim_Copy(account_record_state.ird_number);
        if (normalizedIRD.empty()) {
            return false;
        }
        return Save_IRD_Record_Field(normalizedIRD, fieldLabel, fieldValue);
    }

    if (!Resolve_Record_Paths()) {
        return false;
    }

    const bool saved = Upsert_Record_Field_In_Order(
        account_record_state.employee_record_path,
        EmployeeRecordTitle,
        OrderedEmployeeRecordLabels,
        fieldLabel,
        fieldValue);
    if (!saved) {
        return false;
    }
    return fieldLabel != "Password" ||
        Ensure_Leave_Request_Sections(
            account_record_state.employee_record_path);
}

void Persist_Cached_Fields() {
    for (const std::string& label : OrderedEmployeeRecordLabels) {
        const auto it = account_record_state.extra_fields.find(label);
        if (it != account_record_state.extra_fields.end()) {
            Persist_Field(label, it->second);
        }
    }

    for (const std::string& label : OrderedTaxRecordLabels) {
        const auto it = account_record_state.extra_fields.find(label);
        if (it != account_record_state.extra_fields.end()) {
            Persist_Field(label, it->second);
        }
    }

    for (const auto& entry : account_record_state.extra_fields) {
        const bool inEmployeeOrder = std::find(OrderedEmployeeRecordLabels.begin(), OrderedEmployeeRecordLabels.end(), entry.first) != OrderedEmployeeRecordLabels.end();
        const bool inTaxOrder = std::find(OrderedTaxRecordLabels.begin(), OrderedTaxRecordLabels.end(), entry.first) != OrderedTaxRecordLabels.end();

        if (!inEmployeeOrder && !inTaxOrder) {
            Persist_Field(entry.first, entry.second);
        }
    }
}

void Store_Field(const std::string& detail_name, const std::string& detail_value) {
    account_record_state.extra_fields[detail_name] = detail_value;

    if (detail_name == "IRD Number") {
        account_record_state.ird_number = detail_value;
    } else if (detail_name == "First Name") {
        account_record_state.first_name = detail_value;
    } else if (detail_name == "Middle Name") {
        account_record_state.middle_name = detail_value;
    } else if (detail_name == "Last Name") {
        account_record_state.last_name = detail_value;
    } else if (detail_name == "Business Role") {
        account_record_state.business_role = detail_value;
    }

    Persist_Cached_Fields();
}

void Set_Account_Update_Result(Account_Update_Result* resultOut, Account_Update_Result result) {
    if (resultOut != nullptr) {
        *resultOut = result;
    }
}

bool Remove_All_Matching_Lines_From_File(const std::string& filePath, const std::string& valueToRemove) {
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        return false;
    }

    std::vector<std::string> retainedLines;
    std::string line;
    while (std::getline(inFile, line)) {
        if (Trim_Copy(line) == valueToRemove) {
            continue;
        }
        retainedLines.push_back(line);
    }

    std::ofstream outFile(filePath, std::ios::trunc);
    if (!outFile.is_open()) {
        return false;
    }

    for (const std::string& retainedLine : retainedLines) {
        outFile << retainedLine << '\n';
    }

    return true;
}

bool Remove_First_Matching_Line_From_File(const std::string& filePath, const std::string& valueToRemove) {
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        return false;
    }

    std::vector<std::string> retainedLines;
    std::string line;
    bool removed = false;
    while (std::getline(inFile, line)) {
        if (!removed && Trim_Copy(line) == valueToRemove) {
            removed = true;
            continue;
        }

        retainedLines.push_back(line);
    }

    if (!removed) {
        return false;
    }

    std::ofstream outFile(filePath, std::ios::trunc);
    if (!outFile.is_open()) {
        return false;
    }

    for (const std::string& retainedLine : retainedLines) {
        outFile << retainedLine << '\n';
    }

    return true;
}

std::filesystem::path Find_Employee_Record_Path_For_Username(const std::string& username) {
    const std::filesystem::path recordsDirectory =
        Get_Employee_Records_Directory_Path();
    if (!std::filesystem::exists(recordsDirectory)) {
        return std::filesystem::path();
    }

    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(recordsDirectory)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".txt") {
            continue;
        }

        std::ifstream inFile(entry.path());
        if (!inFile.is_open()) {
            continue;
        }

        std::string line;
        while (std::getline(inFile, line)) {
            const std::string trimmedLine = Trim_Copy(line);
            if (!Starts_With(trimmedLine, "Username:")) {
                continue;
            }

            const std::string discoveredUsername = Trim_Copy(trimmedLine.substr(std::string("Username:").length()));
            if (discoveredUsername == username) {
                return entry.path();
            }
        }
    }

    return std::filesystem::path();
}
}

void Reset_Account_Record_State() {
    account_record_state = Account_Record_State{};
}

bool Save_Account_Record_Field(const std::string& fieldLabel, const std::string& fieldValue) {
    Store_Field(fieldLabel, fieldValue);
    return true;
}

bool Find_Record_For_Username(const std::string& username,
                              std::string* accountType,
                              std::string* passwordHash) {
    const std::filesystem::path recordsDirectory =
        Get_Employee_Records_Directory_Path();
    if (!std::filesystem::exists(recordsDirectory)) {
        return false;
    }

    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(recordsDirectory)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".txt") {
            continue;
        }

        std::ifstream inFile(entry.path());
        if (!inFile.is_open()) {
            continue;
        }

        bool usernameMatches = false;
        std::string discoveredAccountType;
        std::string discoveredPasswordHash;
        std::string line;
        while (std::getline(inFile, line)) {
            const std::string trimmedLine = Trim_Copy(line);
            if (Starts_With(trimmedLine, "Username:")) {
                const std::string discoveredUsername = Trim_Copy(trimmedLine.substr(std::string("Username:").length()));
                usernameMatches = (discoveredUsername == username);
            } else if (Starts_With(trimmedLine, "Account Type:")) {
                discoveredAccountType = Trim_Copy(trimmedLine.substr(std::string("Account Type:").length()));
            } else if (Starts_With(trimmedLine, "Password:")) {
                discoveredPasswordHash = Trim_Copy(trimmedLine.substr(std::string("Password:").length()));
            }
        }

        if (!usernameMatches) {
            continue;
        }

        if (accountType != nullptr) {
            *accountType = discoveredAccountType;
        }
        if (passwordHash != nullptr) {
            *passwordHash = discoveredPasswordHash;
        }
        return true;
    }

    return false;
}

std::vector<std::string> Get_Usernames_By_Prefix(const std::string& prefix) {
    std::vector<std::string> usernames;

    std::ifstream inFile(Get_Username_Index_Path());
    if (!inFile.is_open()) {
        return usernames;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        const std::string username = Trim_Copy(line);
        if (username.empty()) {
            continue;
        }

        if (Starts_With(username, prefix)) {
            usernames.push_back(username);
        }
    }

    return usernames;
}

std::vector<Role_Assignment_Option> Get_Role_Assignment_Options(
        const std::vector<std::string>& job_roles,
        const std::string& business_role) {
    std::vector<Role_Assignment_Option> options;
    const std::vector<std::string> usernames = Get_Usernames_By_Prefix("");

    for (const std::string& username : usernames) {
        std::map<std::string, std::string> details;
        if (!Get_Account_Record_Details_For_Username(username, &details)) {
            continue;
        }

        const std::string candidate_job_role = Trim_Copy(details["Job Role"]);
        const std::string candidate_business_role = Trim_Copy(details["Business Role"]);
        if (std::find(job_roles.begin(), job_roles.end(), candidate_job_role) == job_roles.end()) {
            continue;
        }
        if (!Trim_Copy(business_role).empty() &&
            candidate_business_role != Trim_Copy(business_role)) {
            continue;
        }

        std::string display_name = Trim_Copy(
            details["First Name"] + " " + details["Middle Name"] + " " + details["Last Name"]);
        options.push_back({
            username,
            display_name,
            candidate_business_role,
            candidate_job_role
        });
    }

    std::sort(options.begin(), options.end(),
        [](const Role_Assignment_Option& left, const Role_Assignment_Option& right) {
            return left.display_name < right.display_name;
        });
    return options;
}

std::vector<Role_Assignment_Option> Get_Assigned_Team_Members(
        const std::string& supervisor_username) {
    std::vector<Role_Assignment_Option> team_members;
    const std::string normalized_supervisor = Trim_Copy(supervisor_username);
    if (normalized_supervisor.empty()) {
        return team_members;
    }

    const std::vector<std::string> usernames = Get_Usernames_By_Prefix("");
    for (const std::string& username : usernames) {
        std::map<std::string, std::string> details;
        if (!Get_Account_Record_Details_For_Username(username, &details)) {
            continue;
        }

        const bool assigned_to_supervisor =
            Trim_Copy(details["Main Manager"]) == normalized_supervisor ||
            Trim_Copy(details["Assigned Manager"]) == normalized_supervisor ||
            Trim_Copy(details["Assigned Mentor"]) == normalized_supervisor;
        if (!assigned_to_supervisor) {
            continue;
        }

        team_members.push_back({
            username,
            Trim_Copy(details["First Name"] + " " + details["Middle Name"] + " " + details["Last Name"]),
            Trim_Copy(details["Business Role"]),
            Trim_Copy(details["Job Role"])
        });
    }

    std::sort(team_members.begin(), team_members.end(),
        [](const Role_Assignment_Option& left, const Role_Assignment_Option& right) {
            return left.display_name < right.display_name;
        });
    return team_members;
}

std::vector<Role_Assignment_Option> Get_Managers_Assistant_Can_Act_On_Behalf_Of(
        const std::string& assistant_username) {
    std::vector<Role_Assignment_Option> targets;
    std::map<std::string, std::string> assistant_details;
    if (!Get_Account_Record_Details_For_Username(
            assistant_username, &assistant_details)) {
        return targets;
    }

    const std::string assistant_job_role =
        Trim_Copy(assistant_details["Job Role"]);
    if (assistant_job_role == "Executive Assistant") {
        targets = Get_Role_Assignment_Options({
            "Senior Level Manager",
            "Senior Finance Manager",
            "Senior Trade Manager"
        });
        const std::string main_manager =
            Trim_Copy(assistant_details["Main Manager"]);
        std::stable_sort(
            targets.begin(),
            targets.end(),
            [&main_manager](
                    const Role_Assignment_Option& left,
                    const Role_Assignment_Option& right) {
                return left.username == main_manager &&
                       right.username != main_manager;
            });
        return targets;
    }
    if (assistant_job_role != "Personal Assistant") {
        return targets;
    }

    const std::string assigned_manager =
        Trim_Copy(assistant_details["Assigned Manager"]);
    if (assigned_manager.empty()) {
        return targets;
    }

    std::map<std::string, std::string> manager_details;
    if (!Get_Account_Record_Details_For_Username(
            assigned_manager, &manager_details)) {
        return targets;
    }

    const std::string manager_job_role =
        Trim_Copy(manager_details["Job Role"]);
    const bool junior_manager =
        manager_job_role == "Junior Level Manager" ||
        manager_job_role == "Junior Finance Manager" ||
        manager_job_role == "Junior Trade Manager";
    if (!junior_manager) {
        return targets;
    }

    targets.push_back({
        assigned_manager,
        Trim_Copy(
            manager_details["First Name"] + " " +
            manager_details["Middle Name"] + " " +
            manager_details["Last Name"]),
        Trim_Copy(manager_details["Business Role"]),
        manager_job_role
    });
    return targets;
}

bool Get_Username_From_Employee_Records(const std::string& username) {
    const std::string target = Trim_Copy(username);
    if (target.empty()) {
        return false;
    }

    std::ifstream inFile(Get_Username_Index_Path());
    if (!inFile.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (Trim_Copy(line) == target) {
            return true;
        }
    }

    return false;
}

bool Get_Password_From_Employee_Records(const std::string& password_hash) {
    const std::string target = Trim_Copy(password_hash);
    const std::string maskedTarget = Mask_Password_Index_Entry(target);
    if (target.empty()) {
        return false;
    }

    std::ifstream inFile(Get_Password_Index_Path());
    if (!inFile.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        const std::string entry = Trim_Copy(line);
        if (entry == target || entry == maskedTarget) {
            return true;
        }
    }

    return false;
}

bool Get_IRD_Username_Match(const std::string& IRD_number, const std::string& username) {
    const std::string normalizedIRD = Trim_Copy(IRD_number);
    const std::string normalizedUsername = Trim_Copy(username);
    std::string target = normalizedIRD + " : " + normalizedUsername;
    if (target.empty()) {
        return false;
    }

    std::ifstream inFile(Get_IRD_Username_Index_Path());
    if (!inFile.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (Trim_Copy(line) == target) {
            return true;
        }
    }

    return false;
}

std::string Get_IRD_Number_For_Username(const std::string& username) {
    const std::string normalizedUsername = Trim_Copy(username);
    if (normalizedUsername.empty()) {
        return "";
    }

    std::ifstream inFile(Get_IRD_Username_Index_Path());
    if (!inFile.is_open()) {
        return "";
    }

    std::string line;
    while (std::getline(inFile, line)) {
        const std::string trimmedLine = Trim_Copy(line);
        const std::size_t separator = trimmedLine.find(':');
        if (separator == std::string::npos) {
            continue;
        }

        const std::string discoveredIRD = Trim_Copy(trimmedLine.substr(0, separator));
        const std::string discoveredUsername = Trim_Copy(trimmedLine.substr(separator + 1));
        if (discoveredUsername == normalizedUsername) {
            return discoveredIRD;
        }
    }

    return "";
}

bool Check_Account_Exists(const std::string& username, const std::string& password, const std::string& IRD_number) {
    const std::string normalizedIRD = Trim_Copy(IRD_number);
    if (!normalizedIRD.empty() && !Get_IRD_Username_Match(normalizedIRD, username)) {
        return false;
    }

    std::string storedPasswordHash;
    if (!Find_Record_For_Username(username, nullptr, &storedPasswordHash)) {
        return false;
    }

    bool needs_rehash = false;
    const bool password_matches = Verify_Password_Against_Stored_Hash(
        password,
        storedPasswordHash,
        &needs_rehash);
    if (!password_matches) {
        return false;
    }

    if (needs_rehash) {
        Update_Account_Password_For_Username(username, password, nullptr);
    }

    return true;
}

bool Check_Account_Exists_In_Indexes(const std::string& username, const std::string& password, const std::string& IRD_number) {
    return Check_Account_Exists(username, password, IRD_number);
}

std::string Get_Account_Type_From_Employee_Records(const std::string& username) {
    const std::string target = Trim_Copy(username);
    if (target.empty()) {
        return "";
    }

    std::string savedType;
    if (Find_Record_For_Username(target, &savedType, nullptr)) {
        if (savedType == "Admin" || target.rfind("admin_", 0) == 0) {
            return "Admin";
        }
        return "Employee";
    }

    if (target.rfind("admin_", 0) == 0) {
        return "Admin";
    }

    return "Employee";
}

bool Get_Account_Record_Details_For_Username(const std::string& username,
                                                 std::map<std::string, std::string>* detailsOut) {
    const std::string target = Trim_Copy(username);
    if (target.empty() || detailsOut == nullptr) {
        return false;
    }

    const std::filesystem::path recordsDirectory =
        Get_Employee_Records_Directory_Path();
    if (!std::filesystem::exists(recordsDirectory)) {
        return false;
    }

    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(recordsDirectory)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".txt") {
            continue;
        }

        std::ifstream inFile(entry.path());
        if (!inFile.is_open()) {
            continue;
        }

        std::map<std::string, std::string> discoveredDetails;
        bool usernameMatches = false;
        std::string line;
        while (std::getline(inFile, line)) {
            const std::string trimmedLine = Trim_Copy(line);
            const std::size_t separator = trimmedLine.find(':');
            if (separator == std::string::npos) {
                continue;
            }

            const std::string label = Trim_Copy(trimmedLine.substr(0, separator));
            const std::string value = Trim_Copy(trimmedLine.substr(separator + 1));
            discoveredDetails[label] = value;

            if (label == "Username" && value == target) {
                usernameMatches = true;
            }
        }

        if (!usernameMatches) {
            continue;
        }

        const std::string associatedIRD = Get_IRD_Number_For_Username(target);
        if (!associatedIRD.empty()) {
            const std::filesystem::path irdRecordPath =
                Get_Employee_Records_Directory_Path() / "IRD" / (associatedIRD + ".txt");

            std::ifstream irdFile(irdRecordPath);
            if (irdFile.is_open()) {
                std::string irdLine;
                while (std::getline(irdFile, irdLine)) {
                    const std::string trimmedIRDLine = Trim_Copy(irdLine);
                    const std::size_t separator = trimmedIRDLine.find(':');
                    if (separator == std::string::npos) {
                        continue;
                    }

                    const std::string label = Trim_Copy(trimmedIRDLine.substr(0, separator));
                    const std::string value = Trim_Copy(trimmedIRDLine.substr(separator + 1));
                    if (label == "Business Role" || label == "Job Role") {
                        discoveredDetails[label] = value;
                    }
                }
            }
        }

        *detailsOut = discoveredDetails;
        return true;
    }

    return false;
}

std::string Get_Employee_Record_File_Path_For_Username(
        const std::string& username) {
    return Find_Employee_Record_Path_For_Username(
        Trim_Copy(username)).string();
}

bool Update_Account_Record_Field_For_Username(const std::string& username,
                                              const std::string& fieldLabel,
                                              const std::string& fieldValue,
                                              Account_Update_Result* resultOut) {
    const std::string target = Trim_Copy(username);
    const std::string normalizedLabel = Trim_Copy(fieldLabel);
    if (target.empty() || normalizedLabel.empty()) {
        Set_Account_Update_Result(resultOut, Account_Update_Result::Invalid_Input);
        return false;
    }

    if (Is_Tax_Field_Label(normalizedLabel)) {
        const std::string associatedIRD = Get_IRD_Number_For_Username(target);
        if (Trim_Copy(associatedIRD).empty()) {
            Set_Account_Update_Result(resultOut, Account_Update_Result::Missing_IRD_Mapping);
            return false;
        }

        if (!Save_IRD_Record_Field(associatedIRD, normalizedLabel, fieldValue)) {
            Set_Account_Update_Result(resultOut, Account_Update_Result::Write_Failure);
            return false;
        }

        Set_Account_Update_Result(resultOut, Account_Update_Result::Success);
        return true;
    }

    const std::filesystem::path recordsDirectory =
        Get_Employee_Records_Directory_Path();
    if (!std::filesystem::exists(recordsDirectory)) {
        Set_Account_Update_Result(resultOut, Account_Update_Result::Write_Failure);
        return false;
    }

    for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(recordsDirectory)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".txt") {
            continue;
        }

        std::ifstream inFile(entry.path());
        if (!inFile.is_open()) {
            continue;
        }

        bool usernameMatches = false;
        std::string line;
        while (std::getline(inFile, line)) {
            const std::string trimmedLine = Trim_Copy(line);
            if (!Starts_With(trimmedLine, "Username:")) {
                continue;
            }

            const std::string discoveredUsername = Trim_Copy(trimmedLine.substr(std::string("Username:").length()));
            if (discoveredUsername == target) {
                usernameMatches = true;
                break;
            }
        }

        if (!usernameMatches) {
            continue;
        }

        if (!Upsert_Record_Field_In_Order(entry.path(),
                                          EmployeeRecordTitle,
                                          OrderedEmployeeRecordLabels,
                                          normalizedLabel,
                                          fieldValue)) {
            Set_Account_Update_Result(resultOut, Account_Update_Result::Write_Failure);
            return false;
        }
        if (normalizedLabel == "Business Role" &&
            !Move_Employee_Record_To_Business_Role_Directory(entry.path(), fieldValue)) {
            Set_Account_Update_Result(resultOut, Account_Update_Result::Write_Failure);
            return false;
        }

        Set_Account_Update_Result(resultOut, Account_Update_Result::Success);
        return true;
    }

    Set_Account_Update_Result(resultOut, Account_Update_Result::User_Not_Found);
    return false;
}

bool Update_Account_Password_For_Username(const std::string& username,
                                          const std::string& password,
                                          Account_Update_Result* resultOut) {
    const std::string normalizedUsername = Trim_Copy(username);
    if (normalizedUsername.empty()) {
        Set_Account_Update_Result(resultOut, Account_Update_Result::Invalid_Input);
        return false;
    }

    const std::string hashedPassword = Password_Save(password);
    Account_Update_Result fieldUpdateResult = Account_Update_Result::Success;
    if (!Update_Account_Record_Field_For_Username(normalizedUsername, "Password", hashedPassword, &fieldUpdateResult)) {
        Set_Account_Update_Result(resultOut, fieldUpdateResult);
        return false;
    }

    if (!Save_Password_To_Index(hashedPassword)) {
        Set_Account_Update_Result(resultOut, Account_Update_Result::Index_Update_Failure);
        return false;
    }

    Set_Account_Update_Result(resultOut, Account_Update_Result::Success);
    return true;
}

bool Update_Account_Role_For_Username(const std::string& username,
                                      const std::string& business_role,
                                      const std::string& job_role,
                                      std::string* updated_username,
                                      Account_Update_Result* resultOut) {
    const std::string normalizedUsername = Trim_Copy(username);
    const std::string normalized_business_role = Trim_Copy(business_role);
    const std::string normalized_job_role = Trim_Copy(job_role);
    if (normalizedUsername.empty() || normalized_business_role.empty() || normalized_job_role.empty()) {
        Set_Account_Update_Result(resultOut, Account_Update_Result::Invalid_Input);
        return false;
    }

    std::map<std::string, std::string> accountDetails;
    if (!Get_Account_Record_Details_For_Username(normalizedUsername, &accountDetails)) {
        Set_Account_Update_Result(resultOut, Account_Update_Result::User_Not_Found);
        return false;
    }

    const std::string accountType = accountDetails["Account Type"];
    const std::string firstName = accountDetails["First Name"];
    const std::string middleName = accountDetails["Middle Name"];
    const std::string lastName = accountDetails["Last Name"];
    const std::string irdNumber = Get_IRD_Number_For_Username(normalizedUsername);

    std::string regeneratedUsername;
    if (!Save_Username_From_Role_Edit(normalizedUsername,
                                      accountType,
                                      firstName,
                                      middleName,
                                      lastName,
                                      normalized_business_role,
                                      normalized_job_role,
                                      irdNumber,
                                      regeneratedUsername)) {
        Set_Account_Update_Result(resultOut, Account_Update_Result::Index_Update_Failure);
        return false;
    }

    Account_Update_Result business_role_update_result = Account_Update_Result::Success;
    if (!Update_Account_Record_Field_For_Username(normalizedUsername,
                                                  "Business Role",
                                                  normalized_business_role,
                                                  &business_role_update_result)) {
        Set_Account_Update_Result(resultOut, business_role_update_result);
        return false;
    }

    Account_Update_Result job_role_update_result = Account_Update_Result::Success;
    if (!Update_Account_Record_Field_For_Username(normalizedUsername,
                                                  "Job Role",
                                                  normalized_job_role,
                                                  &job_role_update_result)) {
        Set_Account_Update_Result(resultOut, job_role_update_result);
        return false;
    }

    if (regeneratedUsername != normalizedUsername) {
        Account_Update_Result usernameUpdateResult = Account_Update_Result::Success;
        if (!Update_Account_Record_Field_For_Username(normalizedUsername,
                                                      "Username",
                                                      regeneratedUsername,
                                                      &usernameUpdateResult)) {
            Set_Account_Update_Result(resultOut, usernameUpdateResult);
            return false;
        }
    }

    if (updated_username != nullptr) {
        *updated_username = regeneratedUsername;
    }

    Set_Account_Update_Result(resultOut, Account_Update_Result::Success);
    return true;
}

bool Delete_Account_And_IRD_Records_For_Username_and_IRD(const std::string& username,
                                                 const std::string& ird_number) {
    const std::string normalizedUsername = Trim_Copy(username);
    const std::string normalizedIrd = Trim_Copy(ird_number);
    if (normalizedUsername.empty() || normalizedIrd.empty()) {
        return false;
    }

    std::map<std::string, std::string> accountDetails;
    if (!Get_Account_Record_Details_For_Username(normalizedUsername, &accountDetails)) {
        return false;
    }

    const std::string passwordHash = Trim_Copy(accountDetails["Password"]);
    if (passwordHash.empty()) {
        return false;
    }

    const std::filesystem::path employeeRecordPath = Find_Employee_Record_Path_For_Username(normalizedUsername);
    const std::filesystem::path irdRecordPath =
        Get_Employee_Records_Directory_Path() / "IRD" / (normalizedIrd + ".txt");

    const bool removedIrdUsernameIndex = Remove_All_Matching_Lines_From_File(
        Get_IRD_Username_Index_Path().string(),
        normalizedIrd + " : " + normalizedUsername);
    const bool removedUsernameIndex = Remove_All_Matching_Lines_From_File(
        Get_Username_Index_Path().string(),
        normalizedUsername);

    const std::string maskedPasswordHash = Mask_Password_Index_Entry(passwordHash);
    const bool removedPasswordHash = Remove_First_Matching_Line_From_File(
        Get_Password_Index_Path().string(),
        passwordHash);
    const bool removedMaskedPasswordHash = Remove_First_Matching_Line_From_File(
        Get_Password_Index_Path().string(),
        maskedPasswordHash);

    bool removedEmployeeRecord = false;
    if (!employeeRecordPath.empty() && std::filesystem::exists(employeeRecordPath)) {
        removedEmployeeRecord = std::filesystem::remove(employeeRecordPath);
    }

    bool removedIrdRecord = false;
    if (std::filesystem::exists(irdRecordPath)) {
        removedIrdRecord = std::filesystem::remove(irdRecordPath);
    }

    return removedIrdUsernameIndex &&
           removedUsernameIndex &&
           (removedPasswordHash || removedMaskedPasswordHash) &&
           removedEmployeeRecord &&
           removedIrdRecord;
}
