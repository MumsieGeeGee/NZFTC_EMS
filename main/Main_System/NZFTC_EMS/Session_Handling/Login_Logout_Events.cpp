// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Login_Logout_Events.h"

namespace {
constexpr const char UnknownUserLoginAttemptMessage[] = "Unknown user login attempt.";
constexpr const char IncorrectPasswordLoginAttemptMessage[] = "Incorrect password login attempt.";
constexpr const char SecondaryIncorrectPasswordLoginAttemptMessage[] = "Secondary incorrect password login attempt.";
constexpr const char ThirdIncorrectPasswordLoginAttemptMessage[] = "Third incorrect password login attempt.";
constexpr const char TooManyIncorrectPasswordLoginAttemptsMessage[] = "Too many incorrect password login attempts.";
constexpr const char AccountLockedDueToFailedLoginAttemptsMessage[] = "Account locked due to failed login attempts.";
constexpr const char LockedAccountLoginAttemptMessage[] = "Login attempted on locked account.";
constexpr const char AccountUnlockedFromAdminDashboardMessage[] = "Account unlocked from admin dashboard.";
constexpr const char PasswordChangedAfterAdminUnlockMessage[] = "Password changed after admin unlock.";
constexpr const char SuccessfulLogoutMessage[] = "Successful logout.";
constexpr const char SessionEndedNormallyMessage[] = "Session ended normally.";
constexpr const char UnsuccessfulLogoutMessage[] = "Unsuccessful logout.";
constexpr const char SessionEndedDueToTimeoutMessage[] = "Session ended due to timeout.";
constexpr const char SessionEndedDueToForcedLogoutMessage[] = "Session ended due to forced logout.";
}

std::string Unknown_User_Login_Attempt() { return UnknownUserLoginAttemptMessage; }
std::string Incorrect_Password_Login_Attempt() { return IncorrectPasswordLoginAttemptMessage; }
std::string Secondary_Incorrect_Password_Login_Attempt() { return SecondaryIncorrectPasswordLoginAttemptMessage; }
std::string Third_Incorrect_Password_Login_Attempt() { return ThirdIncorrectPasswordLoginAttemptMessage; }
std::string Too_Many_Incorrect_Password_Login_Attempts() { return TooManyIncorrectPasswordLoginAttemptsMessage; }
std::string Account_Locked_Due_to_Failed_Login_Attempts() { return AccountLockedDueToFailedLoginAttemptsMessage; }
std::string Locked_Account_Login_Attempt() { return LockedAccountLoginAttemptMessage; }
std::string Account_Unlocked_From_Admin_Dashboard() { return AccountUnlockedFromAdminDashboardMessage; }
std::string Password_Changed_After_Admin_Unlock() { return PasswordChangedAfterAdminUnlockMessage; }
std::string Successful_Logout() { return SuccessfulLogoutMessage; }
std::string Session_Ended_Normally() { return SessionEndedNormallyMessage; }
std::string Unsuccessful_Logout() { return UnsuccessfulLogoutMessage; }
std::string Session_Ended_Due_to_Timeout() { return SessionEndedDueToTimeoutMessage; }
std::string Session_Ended_Due_to_Forced_Logout() { return SessionEndedDueToForcedLogoutMessage; }
