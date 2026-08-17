// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Employee_Dashboard.h"
#include "../Account_Creation/View_Accounts.h"
#include "../Account_Creation/Edit_Accounts.h"
#include "../Account_Management/Input_Validation_Helpers.h"
#include "../../NZFTC_EMS/Session_Handling/Session_Handling.h"
#include "../../Employee Management/HR_Management/Grievances/Grievance_Form/Grievance_Workflow.h"
#include "../../Employee Management/HR_Management/Grievances/Requests/Grievance_Requests.h"
#include "../../Employee Management/HR_Management/HR_Management.h"
#include "../../Employee Management/HR_Management/HR_Management_UI.h"
#include "../../Employee Management/HR_Management/Leave/Leave_Requests.h"
#include "../../UI/Account_UI/Employee_Dashboard_UI.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/Success_Messages.h"

#include <iostream>

namespace {
void Get_Employee_Account_Details_Menu_Choice() {
    while (true) {
        Display_Employee_Account_Details_Menu_UI();
        std::cout << Please_Select_an_Option_Label << " (1-3): ";

        int choice = 0;
        if (!Get_Validated_Menu_Choice(1, 3, &choice)) {
            continue;
        }

        if (choice == 1) {
            Display_Personal_Details();
            continue;
        }

        if (choice == 2) {
            const User_Session_Context& session = Get_Current_User_Session_Context();
            if (!session.authenticated || Trim_Copy(session.username).empty()) {
                std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
                continue;
            }

            Edit_Current_User_Account_Details(session.username);
            continue;
        }

        return;
    }
}

void Get_Employee_Information_Menu_Choice() {
    while (true) {
        Display_Employee_Information_Menu_UI();
        std::cout << Please_Select_an_Option_Label << " (1-3): ";

        int choice = 0;
        if (!Get_Validated_Menu_Choice(1, 3, &choice)) {
            continue;
        }

        if (choice == 1) {
            View_Current_Users_Employee_Details();
            continue;
        }

        if (choice == 2) {
            Edit_Current_Users_Employee_Details();
            continue;
        }

        return;
    }
}

void Get_Employee_HR_Information_Menu_Choice() {
    while (true) {
        Display_Employee_HR_Information_Menu_UI();
        std::cout << Please_Select_an_Option_Label << " (1-10): ";

        int choice = 0;
        if (!Get_Validated_Menu_Choice(1, 10, &choice)) {
            continue;
        }

        if (choice == 1) {
            Get_Current_User_Leave_and_Holiday_Entitlements();
        } else if (choice == 2) {
            Start_Current_User_Leave_Request_Workflow();
        } else if (choice == 3) {
            View_Current_User_Open_Leave_Requests();
        } else if (choice == 4) {
            View_Current_User_Temp_Denied_Leave_Requests();
        } else if (choice == 5) {
            View_Current_User_Resolved_Leave_Requests();
        } else if (choice == 6) {
            Display_Current_User_Payslips_UI();
        } else if (choice == 7) {
            View_Current_User_Open_Grievance_Requests();
        } else if (choice == 8) {
            View_Current_User_Resolved_Grievance_Requests();
        } else if (choice == 9) {
            Start_Grievance_Workflow();
        } else {
            return;
        }
    }
}
}

void Get_Employee_Dashboard_Main_Menu_Choice() {
    while (true) {
        Display_Employee_Dashboard_Main_Menu_UI();
        std::cout << Please_Select_an_Option_Label << " (1-4): ";

        int choice = 0;
        if (!Get_Validated_Menu_Choice(1, 4, &choice)) {
            continue;
        }

        if (choice == 1) {
            Get_Employee_Account_Details_Menu_Choice();
            continue;
        }

        if (choice == 2) {
            Get_Employee_Information_Menu_Choice();
            continue;
        }

        if (choice == 3) {
            Get_Employee_HR_Information_Menu_Choice();
            continue;
        }

        std::cout << Display_Success << Successful_Logout << std::endl;
        return;
    }
}
