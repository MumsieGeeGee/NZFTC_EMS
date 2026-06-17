#ifndef LOGIN_LOGOUT_MESSAGES_H
#define LOGIN_LOGOUT_MESSAGES_H

#include <string>

// Message Functions
std::string MSG_Invalid_Credentials();
std::string MSG_Please_Try_Again();
std::string MSG_Account_Locked();
std::string MSG_Contact_Administrator();
std::string MSG_Account_Inactive();
std::string MSG_Session_Expiry();
std::string MSG_Login_Again();
std::string MSG_Logged_Out();

// Message Builder Function
std::string MSG_Login_Page_Full(const std::string& part1, const std::string& part2);
// Pre-Built Messages
extern std::string invalidCredentialsMessage;
extern std::string accountLockedMessage;
extern std::string accountInactiveMessage;
extern std::string sessionExpiredMessage;

// Session & Notification Event Messaging
// Login Events
std::string MSG_unknown_user_login_attempt();
std::string MSG_invalid_credentials_login_attempt();
std::string MSG_incorrect_password_login_attempt();
std::string MSG_secondary_incorrect_password_login_attempt();
std::string MSG_third_incorrect_password_login_attempt();
std::string MSG_too_many_incorrect_password_login_attempts();
std::string MSG_account_locked_due_to_failed_login_attempts();
std::string MSG_account_locked();

// Logout Event Messages
std::string MSG_successful_logout();
std::string MSG_unsuccessful_logout();
std::string MSG_session_ended_normally();
std::string MSG_session_ended_due_to_timeout();

#endif // LOGIN_LOGOUT_MESSAGES_H
