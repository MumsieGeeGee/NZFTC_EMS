// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Login_Menu_UI.h"
#include "Shared_UI_Messaging/Form_Labels_UI.h"
#include <string>
#include <iostream>

void Display_Main_Login_Menu_UI() {
    std::cout << "Welcome to the NZFTC Employee Management System (EMS)" << std::endl;
    std::cout << Please_Select_an_Option_Label << std::endl;
    std::cout << "1. Login" << std::endl;
    std::cout << "2. Exit" << std::endl;
    std::cout << "Enter your choice (1 or 2): ";
}

void Display_Login_Prompt_UI() {
    std::cout << "Please enter your login credentials." << std::endl;
}

void Display_Username_Prompt_UI() {
    std::cout << Username_Label << ": ";
}

void Display_Password_Prompt_UI() {
    std::cout << Password_Label << ": ";
}

void Display_Login_Success_UI() {
    std::cout << "Login successful." << std::endl;
}
