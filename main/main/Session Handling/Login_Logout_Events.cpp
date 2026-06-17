#include "NZFTC_EMS\main\main\Session Handling\Login_Logout_Events.h"
#include <string>

// Login Event Messaging

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

// Personalized Login Events

// user [  ] account is inactive
// user [  ] account successfully logged in
// user [  ] account triggered "lock"
// session created for user [  ]
// session expired for user [  ]
// session terminated for user [  ]
// session failed to create for user [  ]
// session failed to expire for user [  ]

// General Login and Logout Event Functions

// log locked account
// log unlocked account
// log active account
// log inactive account
// log time
// log date
// log user
// log account status
// log login event
// log logout event
// log session event
// log session creation
// log session expiry
// log session termination
// log session failure


// Logout Event Messaging

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

// Personalized Logout Events

// user [  ] logged out successfully
// user [  ] redirected to login page after logout
// user [  ] session ended normally
// user [  ] session ended due to timeout

