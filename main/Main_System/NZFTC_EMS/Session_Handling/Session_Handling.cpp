// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Session_Handling.h"

namespace {
User_Session_Context current_user_session_context;
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


// Session Handing Functions 
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
// log session success
// log session timeout
// log session end
// log session start
// log session duration
// log session activity
// log session inactivity
// log security event
// log database event
// log system event