#ifndef LOGIN_LOGOUT_ERRORS_H
#define LOGIN_LOGOUT_ERRORS_H

#include <string>

// Login Errors
std::string ERROR_Invalid_Credentials();
std::string ERROR_Account_Locked();
std::string ERROR_Account_Inactive();
std::string ERROR_Too_Many_Attempts();
std::string ERROR_Incorrect_Password();

// Logout Errors
std::string ERROR_Logout_Failed();

#endif // LOGIN_LOGOUT_ERRORS_H