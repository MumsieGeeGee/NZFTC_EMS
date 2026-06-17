// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
#include "Login_Logout_Errors.h"
#include "Login_Logout_Messages.h"
#include "User_Roles.h"

// Login Errors - User Facing

std::string ERROR_Invalid_Credentials();
std::string ERROR_Account_Locked();
std::string ERROR_Account_Inactive();
std::string ERROR_Too_Many_Attempts();
std::string ERROR_Incorrect_Password();

//Logout Errors - User Facing
std::string ERROR_Logout_Failed();

// Login Errors - System Facing
std::string ERROR_Unknown_User();
std::string ERROR_Suspicious_User();
std::string ERROR_Malicious_User();
std::string ERROR_Email_Field_Empty();
std::string ERROR_Password_Field_Empty();
std::string ERROR_First_Incorrect_Password();
std::string ERROR_Second_Incorrect_Password();
std::string ERROR_Third_Incorrect_Password();
std::string ERROR_Account_Locked_Due_To_Failed_Attempts();
std::string ERROR_Locked_Account_Login_Attempt();
std::string ERROR_Inactive_Account_Login_Attempt();
std::string ERROR_Forgot_Password_Request_Failed();
std::string ERROR_Login_Failed();

// Logout Errors - System Facing
std::string ERROR_Session_Termination_Failed();
std::string ERROR_Forced_Session_Termination_Failed();
std::string ERROR_Unknown_User_Force_Logout_Failed();
std::string ERROR_Suspicious_User_Force_Logout_Failed();
std::string ERROR_Malicious_User_Force_Logout_Failed();
std::string ERROR_Logout_Failed();
