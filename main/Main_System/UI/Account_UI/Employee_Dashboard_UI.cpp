// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Employee_Dashboard_UI.h"
#include "../Shared_UI_Messaging/Form_Labels_UI.h"

#include <iostream>

void Display_Employee_Dashboard_Main_Menu_UI() {
    std::cout << "Employee Dashboard Main Menu:" << std::endl;
    std::cout << "1. My Account Details" << std::endl;
    std::cout << "2. My Employee Information" << std::endl;
    std::cout << "3. My HR Information" << std::endl;
    std::cout << "4. Logout" << std::endl;
}

void Display_Employee_Account_Details_Menu_UI() {
    std::cout << "Employee Account Details Menu:" << std::endl;
    std::cout << "1. View My Account Details" << std::endl;
    std::cout << "2. Edit My Account Details" << std::endl;
    std::cout << "3. " << Return_to_Employee_Dashboard_Main_Menu_Label << std::endl;
}

void Display_Employee_Information_Menu_UI() {
    std::cout << "Employee Information Menu:" << std::endl;
    std::cout << "1. View My Employee Record" << std::endl;
    std::cout << "2. Edit My Employee Information" << std::endl;
    std::cout << "3. " << Return_to_Employee_Dashboard_Main_Menu_Label << std::endl;
}

void Display_Employee_HR_Information_Menu_UI() {
    std::cout << "Employee HR Information Menu:" << std::endl;
    std::cout << "1. View My Leave and Holiday entitlements" << std::endl;
    std::cout << "2. Apply for Leave" << std::endl;
    std::cout << "3. View My Open Leave Requests" << std::endl;
    std::cout << "4. View My Temporarily Denied Leave Requests" << std::endl;
    std::cout << "5. View My Resolved Leave Requests" << std::endl;
    std::cout << "6. View My Payslips" << std::endl;
    std::cout << "7. View My Open Grievance Reports" << std::endl;
    std::cout << "8. View My Resolved Grievance Reports" << std::endl;
    std::cout << "9. Submit a Grievance Report" << std::endl;
    std::cout << "10. " << Return_to_Employee_Dashboard_Main_Menu_Label << std::endl;
}
