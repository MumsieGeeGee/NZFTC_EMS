// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef LOGIN_MENU_H
#define LOGIN_MENU_H

#include <string>
#include "../NZFTC_EMS/Session_Handling/Session_Handling.h"

void Main_Login_Menu(const std::string& username, const std::string& password, const std::string& IRD_number);
void Return_to_Login_Menu();

#endif // LOGIN_MENU_H
