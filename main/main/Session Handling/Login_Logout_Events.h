#ifndef LOGIN_LOGOUT_EVENTS_H
#define LOGIN_LOGOUT_EVENTS_H

#include <string>

// Login Event Messages
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

#endif // LOGIN_LOGOUT_EVENTS_H
