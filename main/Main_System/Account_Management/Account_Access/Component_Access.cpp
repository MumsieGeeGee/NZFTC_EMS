// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Component_Access.h"
#include "Admin_Dashboard.h"
#include "Employee_Dashboard.h"
#include "../../UI/Account_UI/Admin_Dashboard_UI.h"
#include "../../UI/Account_UI/Employee_Dashboard_UI.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../Account_Management/Input_Validation_Helpers.h"
#include "Employee Management/Role_Management/Role_and_Salary_Details.h"
#include <string>
#include <iostream>

void Display_Menu_for_Role_Type(Admin_Role& admin_role);
void Display_Menu_for_Role_Type(Finance_Role& finance_role);
void Display_Menu_for_Role_Type(Trade_Role& trade_role);

namespace {
bool Username_Has_Token(const std::string& username, const std::string& token) {
    std::size_t start = 0;
    while (start <= username.size()) {
        const std::size_t end = username.find('_', start);
        const std::size_t length = (end == std::string::npos ? username.size() : end) - start;
        if (length == token.size() && username.compare(start, length, token) == 0) {
            return true;
        }
        if (end == std::string::npos) {
            break;
        }
        start = end + 1;
    }
    return false;
    }
}

bool Route_Admin_Role_From_Username(const std::string& username) {
    Admin_Role role = Admin_Role::Junior_General_Administrator;
    if (Username_Has_Token(username, "r")) {
        role = Admin_Role::Receptionist;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "cc")) {
        role = Admin_Role::Call_Centre_Operator;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "ea")) {
        role = Admin_Role::Executive_Assistant;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "pa")) {
        role = Admin_Role::Personal_Assistant;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "hr")) {
        role = Admin_Role::Human_Resources_Administrator;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jit")) {
        role = Admin_Role::Junior_IT_Support_Administrator;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "sit")) {
        role = Admin_Role::Senior_IT_Support_Administrator;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jlm")) {
        role = Admin_Role::Junior_Level_Manager;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "slm")) {
        role = Admin_Role::Senior_Level_Manager;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jt")) {
        role = Admin_Role::Junior_Trainee_Administrator;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "st")) {
        role = Admin_Role::Senior_Trainee_Administrator;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jc")) {
        role = Admin_Role::Junior_Casual_Administrator;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "sc")) {
        role = Admin_Role::Senior_Casual_Administrator;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jg")) {
        role = Admin_Role::Junior_General_Administrator;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "sg")) {
        role = Admin_Role::Senior_General_Administrator;
        Display_Menu_for_Role_Type(role);
    } else {
        return false;
    }

    return true;
}

bool Route_Finance_Role_From_Username(const std::string& username) {
    Finance_Role role = Finance_Role::Junior_General_Finance_Staff;
    if (Username_Has_Token(username, "pr")) {
        role = Finance_Role::Payroll_Administrator;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "ap")) {
        role = Finance_Role::Accounts_Payable_Administrator;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "ar")) {
        role = Finance_Role::Accounts_Receivable_Administrator;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jfm")) {
        role = Finance_Role::Junior_Finance_Manager;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "sfm")) {
        role = Finance_Role::Senior_Finance_Manager;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jt")) {
        role = Finance_Role::Junior_Trainee_Finance_Staff;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "st")) {
        role = Finance_Role::Senior_Trainee_Finance_Staff;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jc")) {
        role = Finance_Role::Junior_Casual_Finance_Staff;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "sc")) {
        role = Finance_Role::Senior_Casual_Finance_Staff;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jg")) {
        role = Finance_Role::Junior_General_Finance_Staff;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "sg")) {
        role = Finance_Role::Senior_General_Finance_Staff;
        Display_Menu_for_Role_Type(role);
    } else {
        return false;
    }

    return true;
}

bool Route_Trade_Role_From_Username(const std::string& username) {
    Trade_Role role = Trade_Role::Junior_General_Trade_Staff;
    if (Username_Has_Token(username, "app")) {
        role = Trade_Role::Junior_Apprentice;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "men")) {
        role = Trade_Role::Senior_Mentor;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "saf")) {
        role = Trade_Role::Safety_Officer;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "sal")) {
        role = Trade_Role::Sales_Representative;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jtm")) {
        role = Trade_Role::Junior_Trade_Manager;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "stm")) {
        role = Trade_Role::Senior_Trade_Manager;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jt")) {
        role = Trade_Role::Junior_Trainee_Trade_Staff;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "st")) {
        role = Trade_Role::Senior_Trainee_Trade_Staff;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jc")) {
        role = Trade_Role::Junior_Casual_Trade_Staff;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "sc")) {
        role = Trade_Role::Senior_Casual_Trade_Staff;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "jg")) {
        role = Trade_Role::Junior_General_Trade_Staff;
        Display_Menu_for_Role_Type(role);
    } else if (Username_Has_Token(username, "sg")) {
        role = Trade_Role::Senior_General_Trade_Staff;
        Display_Menu_for_Role_Type(role);
    } else {
        return false;
    }

    return true;
}

bool Route_Role_Menu_From_Username(const std::string& username) {
    if (Starts_With(username, "admin")) {
        return Route_Admin_Role_From_Username(username);
    }
    if (Starts_With(username, "finance")) {
        return Route_Finance_Role_From_Username(username);
    }
    if (Starts_With(username, "trade")) {
        return Route_Trade_Role_From_Username(username);
    } else {
        return false;
    }
}

bool Secondary_Authenticators(const std::string& business_role, const std::string& job_role) {
    const std::string normalizedBusinessRole = Trim_Copy(business_role);
    const std::string normalizedJobRole = Trim_Copy(job_role);
    if (normalizedBusinessRole.empty() || normalizedJobRole.empty()) {
        return false;
    }

    // Uses role strings from the account record role data.
    if (normalizedBusinessRole == "Admin") {
        return normalizedJobRole == "Junior IT Support Administrator" ||
               normalizedJobRole == "Senior IT Support Administrator" ||
               normalizedJobRole == "Junior Level Manager" ||
               normalizedJobRole == "Senior Level Manager";
    }

    if (normalizedBusinessRole == "Finance") {
        return normalizedJobRole == "Junior Finance Manager" ||
               normalizedJobRole == "Senior Finance Manager";
    }

    if (normalizedBusinessRole == "Trade") {
        return normalizedJobRole == "Junior Trade Manager" ||
               normalizedJobRole == "Senior Trade Manager";
    }

    return false;
}


// Admin Role Access
//
// General
// Calendar sees: Paydays, Public Holidays, Granted Leave - Personal Only
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Receptionist
// Calendar sees: Paydays, Public Holidays, Granted Leave: Personal/All Admin Employees, Finance Managers, Trade Managers/Sales Representatives.
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Call Centre Operator
// Calendar sees: Paydays, Public Holidays, Granted Leave: Personal/All Employees.
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Executive Assistant
// Calendar sees: Paydays, Public Holidays, Granted Leave: Personal/All Managers.
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Personal Assistant
// Calendar sees: Paydays, Public Holidays, Granted Leave: Personal/Corresponding Manager.
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Human Resources Administrator
// Calendar sees: Paydays, Public Holidays, Granted Leave: Personal/All Employees.
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic for Personal, Full for All Employees
// HR Payslip Viewing-Only - Personal/All Employees
// HR Grievance Management - Basic for Personal Only, Full for All Employees
// HR Leave and Holiday Management - Basic for Personal Only, Full for All Employees

//
// IT Support Administrator
// Calendar sees: Paydays, Public Holidays, Granted Leave: Personal Only.
// Account Password Management - Basic for Personal Only, Full for All Employees
// Employee Information Viewing/Editing - Basic/Personal Only
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Manager
// Calendar sees: Paydays, Public Holidays, Granted Leave: Personal/All Employees.
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic for Personal, Full for All Employees
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic for Personal Only, Full for All (Respective) Employees
// HR Leave and Holiday Management - Basic for Personal Only, Full for All (Respective) Employees

void Display_Menu_for_Role_Type(Admin_Role& admin_role) {
    if (admin_role == Admin_Role::Junior_General_Administrator ||
        admin_role == Admin_Role::Senior_General_Administrator || 
        admin_role == Admin_Role::Junior_Trainee_Administrator || 
        admin_role == Admin_Role::Senior_Trainee_Administrator || 
        admin_role == Admin_Role::Junior_Casual_Administrator || 
        admin_role == Admin_Role::Senior_Casual_Administrator || 
        admin_role == Admin_Role::Receptionist || 
        admin_role == Admin_Role::Call_Centre_Operator) {
        Get_Employee_Dashboard_Main_Menu_Choice();
    } else if (admin_role == Admin_Role::Executive_Assistant || admin_role == Admin_Role::Personal_Assistant) {
        while (true) {
            Display_HR_Management_Only_Admin_Dashboard_Main_Menu_UI();
            Display_Personal_Main_Management_Menu_UI1();
            int choice = 0;
            Get_HR_Management_Only_Admin_Dashboard_Main_Menu_Choice(admin_role, Finance_Role::Junior_General_Finance_Staff, Trade_Role::Junior_General_Trade_Staff, choice);
            if (choice == 6) {
                break;
            }
        }
    } else if (admin_role == Admin_Role::Human_Resources_Administrator) {
        while (true) {
            Display_Partial_Admin_Dashboard_Main_Menu_UI();
            Display_Personal_Main_Management_Menu_UI2();
            int choice = 0;
            Get_Partial_Admin_Dashboard_Main_Menu_Choice(admin_role, Finance_Role::Junior_General_Finance_Staff, Trade_Role::Junior_General_Trade_Staff, choice);
            if (choice == 7) {
                break;
            }
        }
    } else if (admin_role == Admin_Role::Junior_IT_Support_Administrator) {
        while (true) {
            Display_Account_Management_Only_Admin_Dashboard_Main_Menu_UI();
            Display_Personal_Main_Management_Menu_UI1();
            int choice = 0;
            Get_Account_Management_Only_Admin_Dashboard_Main_Menu_Choice(admin_role, Finance_Role::Junior_General_Finance_Staff, Trade_Role::Junior_General_Trade_Staff, choice);
            if (choice == 6) {
                break;
            }
        }
    } else if (admin_role == Admin_Role::Junior_Level_Manager) {
        while (true) {
            Display_Employee_Management_Only_Admin_Dashboard_Main_Menu_UI();
            Display_Personal_Main_Management_Menu_UI1();
            int choice = 0;
            Get_Employee_Management_Only_Admin_Dashboard_Main_Menu_Choice(admin_role, Finance_Role::Junior_General_Finance_Staff, Trade_Role::Junior_General_Trade_Staff, choice);
            if (choice == 6) {
                break;
            }
        }
    } else if (admin_role == Admin_Role::Senior_Level_Manager || admin_role == Admin_Role::Senior_IT_Support_Administrator) {
        while (true) {
            Display_FULL_Admin_Dashboard_Main_Menu_UI();
            int choice = 0;
            Get_FULL_Admin_Dashboard_Main_Menu_Choice(admin_role, Finance_Role::Junior_General_Finance_Staff, Trade_Role::Junior_General_Trade_Staff, choice);
            if (choice == 5) {
                break;
            }
        }
    } else {
        std::cout << Display_Error << Role_Type_Access_Failed << std::endl;
    }
}


// Finance Role Access
//
// General
// Calendar sees: Paydays, Public Holidays, Granted Leave - Personal Only
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Payroll Administrator
// Calendar sees: Paydays, Public Holidays, Granted Leave: Personal Only.
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic for Personal, Full for All Employees
// HR Payslip Viewing-Only - Personal/All Employees
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Accounts Payable Administrator
// Calendar sees: Paydays, Public Holidays, Granted Leave - Personal Only
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Accounts Receivable Administrator
// Calendar sees: Paydays, Public Holidays, Granted Leave - Personal Only
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Manager
// Calendar sees: Paydays, Public Holidays, Granted Leave: Personal/All Employees.
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic for Personal, Full for All Employees
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic for Personal Only, Full for All (Respective) Employees
// HR Leave and Holiday Management - Basic for Personal Only, Full for All (Respective) Employees

void Display_Menu_for_Role_Type(Finance_Role& finance_role) {
    if (finance_role == Finance_Role::Junior_General_Finance_Staff ||
        finance_role == Finance_Role::Senior_General_Finance_Staff ||
        finance_role == Finance_Role::Junior_Trainee_Finance_Staff ||
        finance_role == Finance_Role::Senior_Trainee_Finance_Staff ||
        finance_role == Finance_Role::Junior_Casual_Finance_Staff ||
        finance_role == Finance_Role::Senior_Casual_Finance_Staff ||
        finance_role == Finance_Role::Accounts_Payable_Administrator ||
        finance_role == Finance_Role::Accounts_Receivable_Administrator) {
        Get_Employee_Dashboard_Main_Menu_Choice();
    } else if (finance_role == Finance_Role::Payroll_Administrator) {
        while (true) {
            Display_HR_Management_Only_Admin_Dashboard_Main_Menu_UI();
            Display_Personal_Main_Management_Menu_UI1();
            int choice = 0;
            Get_HR_Management_Only_Admin_Dashboard_Main_Menu_Choice(
                Admin_Role::Executive_Assistant,
                Finance_Role::Payroll_Administrator,
                Trade_Role::Junior_General_Trade_Staff,
                choice);
            if (choice == 6) {
                break;
            }
        }
    } else if (finance_role == Finance_Role::Junior_Finance_Manager) {
        while (true) {
            Display_Employee_Management_Only_Admin_Dashboard_Main_Menu_UI();
            Display_Personal_Main_Management_Menu_UI1();
            int choice = 0;
            Get_Employee_Management_Only_Admin_Dashboard_Main_Menu_Choice(
                Admin_Role::Junior_Level_Manager,
                Finance_Role::Junior_General_Finance_Staff,
                Trade_Role::Junior_General_Trade_Staff,
                choice);
            if (choice == 6) {
                break;
            }
        }
    } else if (finance_role == Finance_Role::Senior_Finance_Manager) {
        while (true) {
            Display_Employee_Management_Only_Admin_Dashboard_Main_Menu_UI();
            Display_Personal_Main_Management_Menu_UI1();
            int choice = 0;
            Get_Employee_Management_Only_Admin_Dashboard_Main_Menu_Choice(
                Admin_Role::Junior_Level_Manager,
                Finance_Role::Junior_General_Finance_Staff,
                Trade_Role::Junior_General_Trade_Staff,
                choice);
            if (choice == 6) {
                break;
            }
        }
    } else {
        std::cout << Display_Error << Role_Type_Access_Failed << std::endl;
    }
}

// Trade Role Access
//
// General
// Calendar sees: Paydays, Public Holidays, Granted Leave - Personal Only
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Apprentice
// Calendar sees: Paydays, Public Holidays, Granted Leave - Personal/Respective Mentor Only
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Mentor
// Calendar sees: Paydays, Public Holidays, Granted Leave - Personal/Respective Apprentices Only
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Safety Officer
// Calendar sees: Paydays, Public Holidays, Granted Leave: Personal/All Employees.
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Sales Representative
// Calendar sees: Paydays, Public Holidays, Granted Leave - Personal/Other Sales Representatives Only
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic/Personal Ony
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic/Personal Only
// HR Leave and Holiday Management - Basic/Personal Only

//
// Manager
// Calendar sees: Paydays, Public Holidays, Granted Leave: Personal/All Employees.
// Account Password Management - Basic/Personal Only
// Employee Information Viewing/Editing - Basic for Personal, Full for All Employees
// HR Payslip Viewing-Only - Personal Only
// HR Grievance Management - Basic for Personal Only, Full for All (Respective) Employees
// HR Leave and Holiday Management - Basic for Personal Only, Full for All (Respective) Employees

void Display_Menu_for_Role_Type(Trade_Role& trade_role) {
    if (trade_role == Trade_Role::Junior_General_Trade_Staff ||
        trade_role == Trade_Role::Senior_General_Trade_Staff ||
        trade_role == Trade_Role::Junior_Trainee_Trade_Staff ||
        trade_role == Trade_Role::Senior_Trainee_Trade_Staff ||
        trade_role == Trade_Role::Junior_Casual_Trade_Staff ||
        trade_role == Trade_Role::Senior_Casual_Trade_Staff ||
        trade_role == Trade_Role::Junior_Apprentice ||
        trade_role == Trade_Role::Sales_Representative) {
        Get_Employee_Dashboard_Main_Menu_Choice();
    } else if (trade_role == Trade_Role::Safety_Officer) {
        while (true) {
            Display_Partial_Admin_Dashboard_Main_Menu_UI();
            Display_Personal_Main_Management_Menu_UI2();
            int choice = 0;
            Get_Partial_Admin_Dashboard_Main_Menu_Choice(
                Admin_Role::Human_Resources_Administrator,
                Finance_Role::Junior_General_Finance_Staff,
                Trade_Role::Junior_General_Trade_Staff,
                choice);
            if (choice == 7) {
                break;
            }
        }
    } else if (trade_role == Trade_Role::Junior_Trade_Manager || trade_role == Trade_Role::Senior_Mentor) {
        while (true) {
            Display_Employee_Management_Only_Admin_Dashboard_Main_Menu_UI();
            Display_Personal_Main_Management_Menu_UI1();
            int choice = 0;
            Get_Employee_Management_Only_Admin_Dashboard_Main_Menu_Choice(
                Admin_Role::Junior_Level_Manager,
                Finance_Role::Junior_General_Finance_Staff,
                Trade_Role::Junior_General_Trade_Staff,
                choice);
            if (choice == 6) {
                break;
            }
        }
    } else if (trade_role == Trade_Role::Senior_Trade_Manager) {
        while (true) {
            Display_FULL_Admin_Dashboard_Main_Menu_UI();
            int choice = 0;
            Get_FULL_Admin_Dashboard_Main_Menu_Choice(
                Admin_Role::Senior_Level_Manager,
                Finance_Role::Junior_General_Finance_Staff,
                Trade_Role::Junior_General_Trade_Staff,
                choice);
            if (choice == 5) {
                break;
            }
        }
    } else {
        std::cout << Display_Error << Role_Type_Access_Failed << std::endl; 
    }
}


void Display_Account_Access_For_Type(const std::string& account_type) {
    Display_Account_Access_For_Type(account_type, "");
}

void Display_Account_Access_For_Type(const std::string& account_type, const std::string& username) {
    if (account_type == "Admin") {
        if (Route_Role_Menu_From_Username(username)) {
            return;
        }
        std::cout << Display_Error << Role_Type_Access_Failed << std::endl;
        return;
    }

    if (account_type == "Employee") {
        Get_Employee_Dashboard_Main_Menu_Choice();
        return;
    }

    std::cout << Display_Error << Role_Type_Access_Failed << std::endl;
}
