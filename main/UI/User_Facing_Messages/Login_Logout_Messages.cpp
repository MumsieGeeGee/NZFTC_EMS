// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
#include "main\UI\User_Facing_Messages\Login_Logout_Messages.h"
#include <string>

// Login Error Message Functions
std::string MSG_Invalid_Credentials() {
    return "invalid credentials";
}

std::string MSG_Please_Try_Again() {
    return "please try again";
}

std::string MSG_Account_Locked() {
    return "account locked";
}

std::string MSG_Contact_Administrator() {
    return "please contact an administrator";
}

std::string MSG_Account_Inactive() {
    return "your account is inactive";
}

std::string MSG_Session_Expiry() {
    return "your session expired";
}

std::string MSG_Login_Again() {
    return "login again to continue";
}

std::string MSG_Logged_Out() {
    return "you have been logged out";
}

// Message Builder Function
std::string MSG_Login_Page_Full(const std::string& part1, const std::string& part2) {
    return part1 + ". " + part2 + ".";
}


// Pre-Built Messages
std::string invalidCredentialsMessage = MSG_Login_Page_Full(MSG_Invalid_Credentials(), MSG_Please_Try_Again());
std::string accountLockedMessage = MSG_Login_Page_Full(MSG_Account_Locked(), MSG_Contact_Administrator());
std::string accountInactiveMessage = MSG_Login_Page_Full(MSG_Account_Inactive(), MSG_Contact_Administrator());
std::string sessionExpiredMessage = MSG_Login_Page_Full(MSG_Session_Expiry(), MSG_Login_Again());


// Session & Notification Event Messaging
// Login Events

std::string MSG_unknown_user_login_attempt() {
    return "unknown user attempted to log in";
}

std::string MSG_invalid_credentials_login_attempt() {
    return "invalid credentials were used in a login attempt";
}

std::string MSG_incorrect_password_login_attempt() {
    return "incorrect password was used in a login attempt";
}

std::string MSG_secondary_incorrect_password_login_attempt() {
    return "incorrect password was used in a secondary login attempt";
}

std::string MSG_third_incorrect_password_login_attempt() {
    return "incorrect password was used in a third login attempt";
}

std::string MSG_too_many_incorrect_password_login_attempts() {
    return "too many incorrect password attempts";
}

std::string MSG_account_locked_due_to_failed_login_attempts() {
    return "account locked due to failed login attempts";
}

std::string MSG_account_locked() {
    return "account locked";
}

// Logout Events

std::string MSG_successful_logout() {
    return "successful logout";
}

std::string MSG_unsuccessful_logout() {
    return "unsuccessful logout";
}

std::string MSG_session_ended_normally() {
    return "session ended normally";
}

std::string MSG_session_ended_due_to_timeout() {
    return "session ended due to timeout";
}