// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Session_Handling.h"
#include "Login_Logout_Events.h"
#include "../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../../Account_Management/Account_Access/Account_Database.h"
#include "../../Path_Utilities.h"

#include <chrono>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
// Session Handling Functions
namespace {
User_Session_Context current_user_session_context;

std::string Build_Session_Log_Timestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm local_time{};
#ifdef _WIN32
    localtime_s(&local_time, &now_time);
#else
    local_time = *std::localtime(&now_time);
#endif

    std::ostringstream timestamp;
    timestamp << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
    return timestamp.str();
}

std::string Sanitize_User_Name_For_File_Name(const std::string& username) {
    const std::string trimmed_username = Trim_Copy(username);
    if (trimmed_username.empty()) {
        return "unknown_user";
    }

    std::string sanitized_name;
    sanitized_name.reserve(trimmed_username.size());
    for (char c : trimmed_username) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-') {
            sanitized_name.push_back(c);
        } else {
            sanitized_name.push_back('_');
        }
    }

    return sanitized_name;
}

std::filesystem::path Get_Session_Log_File_Path(const std::string& username) {
    std::filesystem::path session_logs_directory =
        Path_Utilities::Get_Session_Logs_Directory();
    std::filesystem::create_directories(session_logs_directory);
    return session_logs_directory / (Sanitize_User_Name_For_File_Name(username) + "_Session_Log.txt");
}

void Log_Session_Event(
        const std::string& username,
        const std::string& event_message) {
    if (username.empty() || event_message.empty()) {
        return;
    }

    const std::filesystem::path log_file_path =
        Get_Session_Log_File_Path(username);
    std::ofstream log_file(log_file_path, std::ios::app);
    if (!log_file.is_open()) {
        return;
    }

    log_file << Build_Session_Log_Timestamp()
             << " - "
             << event_message
             << '\n';
}

void Update_Account_Event_Field(
        const std::string& username,
        const std::string& field_label,
        const std::string& event_message) {
    if (username.empty() || field_label.empty() || event_message.empty()) {
        return;
    }

    Update_Account_Record_Field_For_Username(
        username,
        field_label,
        event_message,
        nullptr);
}

void Log_Login_Event(
        const std::string& username,
        const std::string& event_message) {
    Log_Session_Event(username, event_message);
    Update_Account_Event_Field(username, "Last Login Event", event_message);
    Update_Account_Event_Field(username, "Last Session Event", event_message);
}
}

const User_Session_Context& Get_Current_User_Session_Context() {
	return current_user_session_context;
}

void Set_Current_User_Session_Context(const User_Session_Context& sessionContext) {
	current_user_session_context = sessionContext;
}

void Clear_Current_User_Session_Context() {
	current_user_session_context = User_Session_Context{};
}

void Log_Incorrect_Password_Attempt(const std::string& username) {
    if (Trim_Copy(username).empty()) {
        return;
    }
    Log_Login_Event(username, Incorrect_Password_Login_Attempt());
}

void Log_Second_Incorrect_Password_Attempt(const std::string& username) {
    if (Trim_Copy(username).empty()) {
        return;
    }
    Log_Login_Event(username, Secondary_Incorrect_Password_Login_Attempt());
}

void Log_Third_Incorrect_Password_Attempt(const std::string& username) {
    if (Trim_Copy(username).empty()) {
        return;
    }
    Log_Login_Event(username, Third_Incorrect_Password_Login_Attempt());
}

void Log_Unknown_User_Attempt(const std::string& username) {
    if (Trim_Copy(username).empty()) {
        return;
    }
    Log_Session_Event(username, Unknown_User_Login_Attempt());
}

void Log_Account_Locked(const std::string& username) {
    if (Trim_Copy(username).empty()) {
        return;
    }
    Log_Login_Event(username, Account_Locked_Due_to_Failed_Login_Attempts());
}

void Log_Too_Many_Incorrect_Password_Attempts(const std::string& username) {
    if (Trim_Copy(username).empty()) {
        return;
    }
    Log_Login_Event(username, Too_Many_Incorrect_Password_Login_Attempts());
}

void Log_Account_Unlocked(const std::string& username) {
    if (Trim_Copy(username).empty()) {
        return;
    }
    Log_Login_Event(username, Account_Unlocked_From_Admin_Dashboard());
}

void Log_Password_Changed_After_Admin_Unlock(const std::string& username) {
    if (Trim_Copy(username).empty()) {
        return;
    }
    Log_Login_Event(username, Password_Changed_After_Admin_Unlock());
}

void Log_Locked_Account_Login_Attempt(const std::string& username) {
    if (Trim_Copy(username).empty()) {
        return;
    }
    Log_Login_Event(username, Locked_Account_Login_Attempt());
}

void Log_Successful_Logout(const std::string& username) {
    if (Trim_Copy(username).empty()) {
        return;
    }
    Log_Login_Event(username, Successful_Logout());
}

void Log_Session_Ended_Normally(const std::string& username) {
    if (Trim_Copy(username).empty()) {
        return;
    }
    Log_Login_Event(username, Session_Ended_Normally());
}