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

#endif // LOGIN_LOGOUT_MESSAGES_H
