// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Admin_Dashboard.h"
#include "../../Account_Form/Account_Creation_Form_UI.h"
#include "../../UI/Account_UI/Admin_Dashboard_UI.h"
#include "../../UI/Account_UI/Employee_Dashboard_UI.h"
#include "../../UI/Account_UI/View_Accounts_UI.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/Success_Messages.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Info_Messages.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include "../../UI/Account_UI/Edit_Accounts_UI.h"
#include "../../UI/Account_UI/Delete_Accounts_UI.h"
#include "../Account_Access/Account_Database.h"
#include "../Account_Creation/Create_Account.h"
#include "../Account_Creation/View_Accounts.h"
#include "../Account_Creation/Edit_Accounts.h"
#include "../Account_Creation/Delete_Accounts.h"
#include "../../Employee Management/HR_Management/Payslips/Payslip_Generation.h"
#include "../../Employee Management/HR_Management/Payslips/Payslip_Generation_UI.h"
#include "../../Tax Calculator/Calculator/Calculator.h"
#include "../../Employee Management/HR_Management/HR_Management_UI.h"
#include "../../Employee Management/HR_Management/HR_Management.h"
#include "../../Employee Management/HR_Management/Grievances/Grievance_Form/Grievance_Workflow.h"
#include "../../Employee Management/HR_Management/Grievances/Requests/Grievance_Requests.h"
#include "../../Employee Management/HR_Management/Leave/Leave_Requests.h"
#include "../../Tax Calculator/Components/IRD_PAYE.h"
#include "../Account_Management/Input_Validation_Helpers.h"
#include "../Account_Management/Security_Protocols.h"
#include "../../NZFTC_EMS/Session_Handling/Session_Handling.h"
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace {
std::string Get_Current_User_Job_Role();

void Log_Current_User_Logout() {
    const User_Session_Context& session = Get_Current_User_Session_Context();
    if (!session.authenticated || Trim_Copy(session.username).empty()) {
        return;
    }

    Log_Successful_Logout(session.username);
    Log_Session_Ended_Normally(session.username);
}

bool Current_User_Is_Assistant() {
    const std::string job_role = Get_Current_User_Job_Role();
    return job_role == Admin_Executive_Assistant ||
           job_role == Admin_Personal_Assistant;
}

std::vector<Role_Assignment_Option> Get_Current_Assistants_Managers() {
    if (!Current_User_Is_Assistant()) {
        return {};
    }
    const User_Session_Context& session = Get_Current_User_Session_Context();
    return Get_Managers_Assistant_Can_Act_On_Behalf_Of(session.username);
}

bool Get_Selected_Username(std::string& username_out) {
    if (Current_User_Is_Assistant()) {
        const std::vector<Role_Assignment_Option> delegated_managers =
            Get_Current_Assistants_Managers();
        if (delegated_managers.empty()) {
            std::cout << Display_Error << No_Accounts_Found << std::endl;
            return false;
        }
        if (delegated_managers.size() == 1) {
            username_out = delegated_managers.front().username;
            return true;
        }

        std::vector<std::string> manager_labels;
        for (const Role_Assignment_Option& manager : delegated_managers) {
            manager_labels.push_back(
                manager.display_name + " - " + manager.job_role +
                " (" + manager.business_role + ")");
        }
        Display_Dynamic_Username_List_UI(
            manager_labels, "Senior Managers:");
        Display_Account_Selection_Input_Prompt_UI(manager_labels.size());

        int manager_choice = 0;
        if (!Get_Validated_Menu_Choice(
                1,
                static_cast<int>(delegated_managers.size()),
                &manager_choice)) {
            return false;
        }
        username_out = delegated_managers[
            static_cast<std::size_t>(manager_choice - 1)].username;
        return true;
    }

    Display_View_Accounts_Menu_UI();

    int business_role_choice = 0;
    if (!Get_Validated_Menu_Choice(1, 4, &business_role_choice)) {
        return false;
    }

    if (business_role_choice == 4) {
        return false;
    }

    std::string username_prefix;
    std::string role_heading;
    if (business_role_choice == 1) {
        username_prefix = "admin_";
        role_heading = Admin_Accounts_Label;
    } else if (business_role_choice == 2) {
        username_prefix = "finance_";
        role_heading = Finance_Accounts_Label;
    } else {
        username_prefix = "trade_";
        role_heading = Trade_Accounts_Label;
    }

    const std::vector<std::string> usernames = Get_Usernames_By_Prefix(username_prefix);
    if (usernames.empty()) {
        std::cout << Display_Error << No_Accounts_Found << std::endl;
        return false;
    }

    Display_Dynamic_Username_List_UI(usernames, role_heading);
    Display_Account_Selection_Input_Prompt_UI(usernames.size());

    int username_choice = 0;
    if (!Get_Validated_Menu_Choice(1, static_cast<int>(usernames.size()), &username_choice)) {
        return false;
    }

    username_out = usernames[static_cast<std::size_t>(username_choice - 1)];
    return true;
}

std::string Get_Current_User_Job_Role() {
    const User_Session_Context& session = Get_Current_User_Session_Context();
    std::map<std::string, std::string> details;
    if (!session.authenticated ||
        !Get_Account_Record_Details_For_Username(session.username, &details)) {
        return "";
    }
    return Trim_Copy(details["Job Role"]);
}

bool Current_User_Has_Payroll_Access() {
    return Get_Current_User_Job_Role() == Payroll_Administrator;
}

bool Display_Current_Users_Assigned_Team() {
    const std::string job_role = Get_Current_User_Job_Role();
    const bool assigned_team_view =
        job_role == Senior_Mentor ||
        job_role == "Junior Level Manager" ||
        job_role == "Senior Level Manager" ||
        job_role == Junior_Finance_Manager ||
        job_role == Senior_Finance_Manager ||
        job_role == Junior_Trade_Manager ||
        job_role == Senior_Trade_Manager;
    if (!assigned_team_view) {
        return false;
    }

    const User_Session_Context& session = Get_Current_User_Session_Context();
    const std::vector<Role_Assignment_Option> team_members =
        Get_Assigned_Team_Members(session.username);
    if (team_members.empty()) {
        std::cout << Display_Info << No_Assigned_Team_Members_Message << std::endl;
        return true;
    }

    std::vector<std::string> team_member_labels;
    for (const Role_Assignment_Option& team_member : team_members) {
        team_member_labels.push_back(
            team_member.display_name + " - " + team_member.job_role +
            " (" + team_member.business_role + ")");
    }
    Display_Assigned_Team_Members_UI(
        job_role == Senior_Mentor
            ? "Assigned Junior Apprentices:"
            : "Team Members Under Your Supervision:",
        team_member_labels);
    return true;
}

void Run_Personal_Account_Management_Menu(
        Admin_Role admin_role,
        Finance_Role finance_role,
        Trade_Role trade_role) {
    Display_Personal_Account_Management_Menu_UI();
    int choice = 0;
    Get_Personal_Account_Management_Menu_Choice(
        admin_role, finance_role, trade_role, choice);
}

void Run_All_Accounts_Management_Menu(
        Admin_Role admin_role,
        Finance_Role finance_role,
        Trade_Role trade_role) {
    int choice = 0;
    std::string first_name;
    Get_Admin_All_Accounts_Management_Menu_Choice(
        admin_role, finance_role, trade_role, choice, first_name);
}

void Run_Personal_Employee_Management_Menu(
        Admin_Role admin_role,
        Finance_Role finance_role,
        Trade_Role trade_role) {
    Display_Personal_Employee_Management_Menu_UI();
    int choice = 0;
    Get_Personal_Employee_Management_Menu_Choice(
        admin_role, finance_role, trade_role, choice);
}

void Run_Personal_HR_Management_Menu(Admin_Role admin_role) {
    Display_Personal_HR_Management_Menu_UI();
    int choice = 0;
    Get_Personal_HR_Management_Menu_Choice(admin_role, choice);
}

void Run_All_Employee_Management_Menu(
        Admin_Role admin_role,
        Finance_Role finance_role,
        Trade_Role trade_role) {
    if (Display_Current_Users_Assigned_Team()) {
        return;
    }
    Display_Admin_Employee_Management_Menu_UI();
    int choice = 0;
    Get_Admin_Employee_Management_Menu_Choice(
        admin_role, finance_role, trade_role, choice);
}

void Run_All_Employees_HR_Management_Menu(
        Admin_Role admin_role,
        Finance_Role finance_role,
        Trade_Role trade_role) {
    if (Current_User_Is_Assistant()) {
        const bool executive_assistant =
            Get_Current_User_Job_Role() == Admin_Executive_Assistant;
        const std::vector<Role_Assignment_Option> managers =
            Get_Current_Assistants_Managers();
        if (managers.empty()) {
            std::cout << Display_Error << No_Accounts_Found << std::endl;
            return;
        }
        std::vector<std::string> manager_descriptions;
        for (std::size_t i = 0; i < managers.size(); ++i) {
            const Role_Assignment_Option& manager = managers[i];
            manager_descriptions.push_back(
                manager.display_name + " - " + manager.job_role +
                " (" + manager.business_role + ") - " +
                (executive_assistant
                    ? (i == 0 ? "Main Manager" : "Available Senior Manager")
                    : "Assigned Manager"));
        }
        Display_Assistant_Delegated_HR_Management_Menu_UI(
            manager_descriptions);
    } else {
        Display_Admin_All_Employees_HR_Management_Menu_UI(
            Current_User_Has_Payroll_Access());
    }
    int choice = 0;
    Pre_Calculation_Payslip slip{};
    Get_Admin_All_Employees_HR_Management_Menu_Choice(
        "", admin_role, finance_role, trade_role, choice, slip);
}
}

void Get_Personal_Account_Management_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice) {
    std::cin >> choice;
    switch (choice) {
        case 1:
            Display_Personal_Details();
            break;
        case 2:
        {
            const User_Session_Context& session = Get_Current_User_Session_Context();
            if (!session.authenticated || Trim_Copy(session.username).empty()) {
                std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
                break;
            }

            Edit_Current_User_Account_Details(session.username);
            break;
        }
        case 3:
            return;
        default:
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            break;
    }
}

void Get_Admin_All_Accounts_Management_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice, std::string& first_name) {
    (void)first_name;
    while (true) {
        Display_Admin_All_Accounts_Management_Menu_UI();
        if (!Get_Validated_Menu_Choice(1, 5, &choice)) {
            continue;
        }

        switch (choice) {
            case 1:
                Display_View_Accounts_Menu_UI();
                Get_View_All_Accounts_Menu_Options();
                break;
            case 2:
                Display_Account_User_Details_Required_UI();
                Display_Continue_Account_Creation_Question_UI();
                Get_Continue_Account_Creation_Question_Options();
                break;
            case 3:
                Display_Edit_Accounts_Menu_UI();
                Get_Edit_Accounts_Menu_Options();
                break;
            case 4:
                Display_Delete_Accounts_Menu_UI();
                Get_Delete_Accounts_Menu_Options();
                break;
            case 5:
                return;
            default:
                std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
                break;
        }
    }
}

void Get_Personal_HR_Management_Menu_Choice(Admin_Role admin_role, int& choice) {
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    switch (choice) {
        case 1:
            // tbc
            break;
        case 2:
            Display_Selected_User_Payslips_UI();
            break;
        case 3:
            Get_Current_User_Leave_and_Holiday_Entitlements();
            break;
        case 4:
            View_Current_User_Open_Leave_Requests();
            break;
        case 5:
            View_Current_User_Temp_Denied_Leave_Requests();
            break;
        case 6:
            View_Current_User_Resolved_Leave_Requests();
            break;
        case 7:
            Start_Current_User_Leave_Request_Workflow();
            break;
        case 8:
            View_Current_User_Open_Grievance_Requests();
            break;
        case 9:
            View_Current_User_Resolved_Grievance_Requests();
            break;
        case 10:
            Start_Grievance_Workflow();
            break;
        case 11:
            Get_View_Current_User_Tax_Information();
            break;
        case 12:
            Get_Edit_Current_User_Tax_Information();
            break;
        case 13:
            return;
        default:
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            break;
    }
}

void Get_Admin_All_Employees_HR_Management_Menu_Choice(const std::string& employee_name, Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice, Pre_Calculation_Payslip& slip) {
    const bool payroll_access = Current_User_Has_Payroll_Access();
    const bool assistant_access = Current_User_Is_Assistant();
    const int maximum_choice = payroll_access ? 13 : (assistant_access ? 5 : 10);
    if (!Get_Validated_Menu_Choice(1, maximum_choice, &choice)) {
        return;
    }
    if (assistant_access) {
        choice = choice == 5 ? 13 : choice + 3;
    } else if (!payroll_access) {
        choice += 3;
    }

    switch (choice) {
        case 1:
        {
            std::string username;
            if (!Get_Selected_Username(username)) {
                break;
            }

            if (!Prepare_Payslip_Context(username, slip)) {
                break;
            }

            Display_Generate_Payslip_Form_UI(employee_name); 
            Display_Pay_Period_Question_UI();
            Get_Pay_Period_Question_Value(slip);
            Get_Selected_User_Hours_Worked(username, slip);
            Get_Additional_Earnings(username, slip);
            Handle_Sick_Leave(slip);
            Handle_Annual_Leave(slip);
            Handle_Public_Holidays(slip);
            Handle_Unpaid_Leave(slip);
            Handle_PD_Contribution(slip);
            Handle_EAP_Contribution(slip);
            Generate_Payslip(username, slip);
            break;
        }
        case 2:
            // tbc
            break;
        case 3:
            // tbc
            break;
        case 4:
        {
            std::string username;
            if (!Get_Selected_Username(username)) {
                break;
            }

            Get_Selected_User_Leave_and_Holiday_Entitlements(username);
            break;
        }
        case 5:
            // tbc
            break;
        case 6:
            // tbc
            break;
        case 7:
            // tbc
            break;
        case 8:
            // tbc
            break;
        case 9:
            // tbc
            break;
        case 10:
            // tbc
            break;
        case 11:
        {
            std::string username;
            if (!Get_Selected_Username(username)) {
                break;
            }

            Get_View_Selected_User_Tax_Information(username);
            break;
        }
        case 12:
        {
            std::string username;
            if (!Get_Selected_Username(username)) {
                break;
            }

            Get_Edit_Selected_User_Tax_Information(username);
            break;
        }
        case 13:
            return;
        default:
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            break;
    }
}

void Get_Personal_Employee_Management_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice) {
    std::cin >> choice;
    switch (choice) {
        case 1:
            // tbc
            break;
        case 2:
            Edit_Current_Users_Employee_Details();
            break;
        case 3:
            return;
        default:
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            break;
    }
}

void Get_Admin_Employee_Management_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice) {
    (void)admin_role;
    (void)finance_role;
    (void)trade_role;
    std::cin >> choice;
    switch (choice) {
        case 1:
            View_Selected_Users_Employee_Details();
            break;
        case 2:
            Edit_Selected_Users_Employee_Details();
            break;
        case 3:
            return;
        default:
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            break;
    }
}

void Get_FULL_Admin_Dashboard_Main_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice) {
    if (!Get_Validated_Menu_Choice(1, 5, &choice)) {
        return;
    }

    switch (choice) {
        case 1:
        {
            Display_Admin_Account_Management_Question_UI();
            int account_management_choice = 0;
            if (!Get_Validated_Menu_Choice(1, 3, &account_management_choice)) {
                return;
            }

            if (account_management_choice == 1) {
                Run_Personal_Account_Management_Menu(
                    admin_role, finance_role, trade_role);
            } else if (account_management_choice == 2) {
                Run_All_Accounts_Management_Menu(
                    admin_role, finance_role, trade_role);
            } else if (account_management_choice == 3) {
                // Return to the main dashboard loop; caller renders the menu once.
            } else {
                std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            }
            break;
        }
        case 2:
        {
            Display_Admin_Employee_Management_Question_UI();
            int employee_management_choice = 0;
            if (!Get_Validated_Menu_Choice(1, 3, &employee_management_choice)) {
                return;
            }
            if (employee_management_choice == 1) {
                Run_Personal_Employee_Management_Menu(
                    admin_role, finance_role, trade_role);
            } else if (employee_management_choice == 2) {
                Run_All_Employee_Management_Menu(
                    admin_role, finance_role, trade_role);
            } else if (employee_management_choice == 3) {
                // Return to the main dashboard loop; caller renders the menu once.
            } else {
                std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            }
            break;
        }
        case 3:
        {
            Display_Admin_HR_Management_Question_UI();
            int hr_management_choice = 0;
            std::cin >> hr_management_choice;
            if (hr_management_choice == 1) {
                Run_Personal_HR_Management_Menu(admin_role);
            } else if (hr_management_choice == 2) {
                Run_All_Employees_HR_Management_Menu(
                    admin_role, finance_role, trade_role);
            } else if (hr_management_choice == 3) {
                // Return to the main dashboard loop; caller renders the menu once.
            } else {
                std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            }
            break;
        }
        case 4:
            Run_Grievance_Request_Management_Menu();
            break;
        case 5:
            Log_Current_User_Logout();
            std::cout << Display_Success << Successful_Logout << std::endl;
            break;
        default:
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            break;
    }
}

void Get_Partial_Admin_Dashboard_Main_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice) {
    if (!Get_Validated_Menu_Choice(1, 7, &choice)) {
        return;
    }
    switch (choice) {
        case 1:
            Run_All_Employee_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 2:
            Run_All_Employees_HR_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 3:
            Run_Personal_Account_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 4:
            Run_Personal_Employee_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 5:
            Run_Personal_HR_Management_Menu(admin_role);
            break;
        case 6:
            Run_Grievance_Request_Management_Menu();
            break;
        case 7:
            Log_Current_User_Logout();
            std::cout << Display_Success << Successful_Logout << std::endl;
            return;
        default:
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            break;
    }
}

void Get_Account_Management_Only_Admin_Dashboard_Main_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice) {
    if (!Get_Validated_Menu_Choice(1, 6, &choice)) {
        return;
    }
    switch (choice) {
        case 1:
            Run_All_Accounts_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 2:
            Run_Personal_Account_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 3:
            Run_Personal_Employee_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 4:
            Run_Personal_HR_Management_Menu(admin_role);
            break;
        case 5:
            Run_Grievance_Request_Management_Menu();
            break;
        case 6:
            Log_Current_User_Logout();
            std::cout << Display_Success << Successful_Logout << std::endl;
            return;
        default:
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            break;
    }
}

void Get_Employee_Management_Only_Admin_Dashboard_Main_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice) {
    if (!Get_Validated_Menu_Choice(1, 6, &choice)) {
        return;
    }
    switch (choice) {
        case 1:
            Run_All_Employee_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 2:
            Run_Personal_Account_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 3:
            Run_Personal_Employee_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 4:
            Run_Personal_HR_Management_Menu(admin_role);
            break;
        case 5:
            Run_Grievance_Request_Management_Menu();
            break;
        case 6:
            Log_Current_User_Logout();
            std::cout << Display_Success << Successful_Logout << std::endl;
            return;
        default:
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            break;
    }
}

void Get_HR_Management_Only_Admin_Dashboard_Main_Menu_Choice(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, int& choice) {
    if (!Get_Validated_Menu_Choice(1, 6, &choice)) {
        return;
    }
    switch (choice) {
        case 1:
            Run_All_Employees_HR_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 2:
            Run_Personal_Account_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 3:
            Run_Personal_Employee_Management_Menu(
                admin_role, finance_role, trade_role);
            break;
        case 4:
            Run_Personal_HR_Management_Menu(admin_role);
            break;
        case 5:
            Run_Grievance_Request_Management_Menu();
            break;
        case 6:
            Log_Current_User_Logout();
            std::cout << Display_Success << Successful_Logout << std::endl;
            return;
        default:
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            break;
    }
}
