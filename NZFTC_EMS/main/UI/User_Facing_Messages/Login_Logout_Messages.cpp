#include "NZFTC_EMS\main\UI\User_Facing_Messages\Login_Logout_Messages.h"
#include <string>

// Message Functions
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
