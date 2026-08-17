// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Role_and_Salary_Details.h"
#include "../../UI/Employee_Management_Menu_UI.h"
#include "Department_Roles/Admin/Admin_Salary_Packages.h"
#include "Department_Roles/Finance/Finance_Salary_Packages.h"
#include "Department_Roles/Trade/Trade_Salary_Packages.h"
#include "Department_Roles/Admin/Admin_Salary_UI.h"
#include "Department_Roles/Finance/Finance_Salary_UI.h"
#include "Department_Roles/Trade/Trade_Salary_UI.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include <string>
#include <iostream>

void Display_Superannuation_Contribution_UI(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role) {
    std::cout << Superannuation_Contribution_Label << ": " << Superannuation_Contribution << "%. Separate Scheme/On-top of KiwiSaver. \n";
}

void Display_Employee_Health_Insurance_UI(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role) {
    std::cout << Health_Insurance_Contribution_Label << ": $" << Health_Insurance_Contribution << " per year. \n";
}

void Display_Employee_Professional_Development_UI(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role) {
    std::cout << Professional_Development_Contribution_Label << ": $" << Professional_Development_Contribution << " per year. \n";
}

void Display_Employee_Employee_Assistance_Program_UI(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role) {
    std::cout << Employee_Assistance_Program_Contribution_Label << ": $" << Employee_Assistance_Program_Contribution << " per year. \n";
}

void Display_Employee_Extra_Benefits_UI(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role) {
    std::cout << "Extra Benefits for this employee:\n";
    Display_Superannuation_Contribution_UI(admin_role, finance_role, trade_role);
    Display_Employee_Health_Insurance_UI(admin_role, finance_role, trade_role);
    Display_Employee_Professional_Development_UI(admin_role, finance_role, trade_role);
    Display_Employee_Employee_Assistance_Program_UI(admin_role, finance_role, trade_role);
}

// Admin Role (Admin_Role)
void Get_Admin_Role(Admin_Role& admin_role) {
    Display_Employee_Admin_Role_Question_UI();
    std::string admin_role_input;
    std::getline(std::cin, admin_role_input);
    if (admin_role_input == "1") {
        admin_role = Admin_Role::Junior_General_Administrator;
    } else if (admin_role_input == "2") {
        admin_role = Admin_Role::Senior_General_Administrator;
    } else if (admin_role_input == "3") {
        admin_role = Admin_Role::Junior_Trainee_Administrator;
    } else if (admin_role_input == "4") {
        admin_role = Admin_Role::Senior_Trainee_Administrator;
    } else if (admin_role_input == "5") {
        admin_role = Admin_Role::Junior_Casual_Administrator;
    } else if (admin_role_input == "6") {
        admin_role = Admin_Role::Senior_Casual_Administrator;
    } else if (admin_role_input == "7") {
        admin_role = Admin_Role::Receptionist;
    } else if (admin_role_input == "8") {
        admin_role = Admin_Role::Call_Centre_Operator;
    } else if (admin_role_input == "9") {
        admin_role = Admin_Role::Executive_Assistant;
    } else if (admin_role_input == "10") {
        admin_role = Admin_Role::Personal_Assistant;
    } else if (admin_role_input == "11") {
        admin_role = Admin_Role::Human_Resources_Administrator;
    } else if (admin_role_input == "12") {
        admin_role = Admin_Role::Junior_IT_Support_Administrator;
    } else if (admin_role_input == "13") {
        admin_role = Admin_Role::Senior_IT_Support_Administrator;
    } else if (admin_role_input == "14") {
        admin_role = Admin_Role::Junior_Level_Manager;
    } else if (admin_role_input == "15") {
        admin_role = Admin_Role::Senior_Level_Manager;
    } else if (admin_role_input == "16") {
        std::cout << "Exiting Admin Role selection.\n";
        // Return to the previous menu or exit the function as needed
    } else {
        std::cout << Display_Error + " " + Invalid_Selection << std::endl;
        // call the function again to prompt the user for input
        Get_Admin_Role(admin_role);
    }
}

// Finance Role (Finance_Role)
void Get_Finance_Role(Finance_Role& finance_role) {
    Display_Employee_Finance_Role_Question_UI();
    std::string finance_role_input;
    std::getline(std::cin, finance_role_input);
    if (finance_role_input == "1") {
        finance_role = Finance_Role::Junior_General_Finance_Staff;
    } else if (finance_role_input == "2") {
        finance_role = Finance_Role::Senior_General_Finance_Staff;
    } else if (finance_role_input == "3") {
        finance_role = Finance_Role::Junior_Trainee_Finance_Staff;
    } else if (finance_role_input == "4") {
        finance_role = Finance_Role::Senior_Trainee_Finance_Staff;
    } else if (finance_role_input == "5") {
        finance_role = Finance_Role::Junior_Casual_Finance_Staff;
    } else if (finance_role_input == "6") {
        finance_role = Finance_Role::Senior_Casual_Finance_Staff;
    } else if (finance_role_input == "7") {
        finance_role = Finance_Role::Payroll_Administrator;
    } else if (finance_role_input == "8") {
        finance_role = Finance_Role::Accounts_Payable_Administrator;
    } else if (finance_role_input == "9") {
        finance_role = Finance_Role::Accounts_Receivable_Administrator;
    } else if (finance_role_input == "10") {
        finance_role = Finance_Role::Junior_Finance_Manager;
    } else if (finance_role_input == "11") {
        finance_role = Finance_Role::Senior_Finance_Manager;
    } else if (finance_role_input == "12") {
        std::cout << "Exiting Finance Role selection.\n";
        // Return to the previous menu or exit the function as needed
    } else {
        std::cout << Display_Error + " " + Invalid_Selection << std::endl;
        // call the function again to prompt the user for input
        Get_Finance_Role(finance_role);
    }
}
// Trade Role (Trade_Role)
void Get_Trade_Role(Trade_Role& trade_role) {
    Display_Employee_Trade_Role_Question_UI();
    std::string trade_role_input;
    std::getline(std::cin, trade_role_input);
    if (trade_role_input == "1") {
        trade_role = Trade_Role::Junior_General_Trade_Staff;
    } else if (trade_role_input == "2") {
        trade_role = Trade_Role::Senior_General_Trade_Staff;
    } else if (trade_role_input == "3") {
        trade_role = Trade_Role::Junior_Trainee_Trade_Staff;
    } else if (trade_role_input == "4") {
        trade_role = Trade_Role::Senior_Trainee_Trade_Staff;
    } else if (trade_role_input == "5") {
        trade_role = Trade_Role::Junior_Casual_Trade_Staff;
    } else if (trade_role_input == "6") {
        trade_role = Trade_Role::Senior_Casual_Trade_Staff;
    } else if (trade_role_input == "7") {
        trade_role = Trade_Role::Junior_Apprentice;
    } else if (trade_role_input == "8") {
        trade_role = Trade_Role::Senior_Mentor;
    } else if (trade_role_input == "9") {
        trade_role = Trade_Role::Safety_Officer;
    } else if (trade_role_input == "10") {
        trade_role = Trade_Role::Sales_Representative;
    } else if (trade_role_input == "11") {
        trade_role = Trade_Role::Junior_Trade_Manager;
    } else if (trade_role_input == "12") {
        trade_role = Trade_Role::Senior_Trade_Manager;
    } else {
        std::cout << Display_Error + " " + Invalid_Selection << std::endl;
        // call the function again to prompt the user for input
        Get_Trade_Role(trade_role);
    }
}

// Role Type (Admin, Finance, Trade)
void Get_Role_Type(Role_Type& role_type) {
    Display_Employee_Role_Question_UI();
    std::string role_type_input;
    std::getline(std::cin, role_type_input);
    if (role_type_input == "1") {
        role_type = Role_Type::Admin;
        Admin_Role admin_role;
        Get_Admin_Role(admin_role); // Call the function to get the specific admin role
    } else if (role_type_input == "2") {
        role_type = Role_Type::Finance;
        Finance_Role finance_role;
        Get_Finance_Role(finance_role); // Call the function to get the specific finance role
    } else if (role_type_input == "3") {
        role_type = Role_Type::Trade;
        Trade_Role trade_role;
        Get_Trade_Role(trade_role); // Call the function to get the specific trade role
    } else if (role_type_input == "4") {
        std::cout << "Exiting Department Role selection.\n";
        // Return to the previous menu or exit the function as needed
    } else {
        std::cout << Display_Error + " " + Invalid_Selection << std::endl;
        // Optionally, you can call the function again to prompt the user for input
    }
}

// Salary Package Question Menu
void Display_Salary_Package_for_Role_Type_UI(Admin_Role admin_role, Finance_Role finance_role, Trade_Role trade_role, Role_Type role_type) {
    switch (role_type) {
        case Role_Type::Admin:
            Display_Employee_Salary_Type_Admin_UI(admin_role);
            break;
        case Role_Type::Finance:
            Display_Employee_Salary_Type_Finance_UI(finance_role);
            break;
        case Role_Type::Trade:
            Display_Employee_Salary_Type_Trade_UI(trade_role);
            break;
        default:
            std::cout << Display_Error << Role_Type_Access_Failed << std::endl;
            break;
    }
}

void Get_Parental_Leave_Entitlement(Parental_Leave_Type& parental_leave_type) {
    std::cout << "1. " << Maternity_Leave_Label << std::endl;
    std::cout << "2. " << Paternity_Leave_Label << std::endl;
    std::cout << "3. " << Adoptive_Parent_Leave_Label << std::endl;
    std::cout << "4. " << Shared_Parent_Leave_Label << std::endl;
    std::cout << "Enter Here (1, 2, 3 or 4): ";
    std::string parental_leave_input;
    std::getline(std::cin, parental_leave_input);
    if (parental_leave_input == "1") {
        parental_leave_type = Parental_Leave_Type::Maternity_Leave;
    } else if (parental_leave_input == "2") {
        parental_leave_type = Parental_Leave_Type::Paternity_Leave;
    } else if (parental_leave_input == "3") {
        parental_leave_type = Parental_Leave_Type::Adoptive_Parent_Leave;
    } else if (parental_leave_input == "4") {
        parental_leave_type = Parental_Leave_Type::Shared_Parental_Leave;
    } else {
        std::cout << Display_Error + " " + Invalid_Selection << std::endl;
        Get_Parental_Leave_Entitlement(parental_leave_type);
    }
}

// Pay Type (Hourly, Salaried, Casual)
void Get_Pay_Type(Pay_Type& pay_type) {
    Display_Employee_Pay_Type_Question_UI();
    std::string pay_type_input;
    std::getline(std::cin, pay_type_input);
    if (pay_type_input == "1") {
        pay_type = Pay_Type::Hourly;
    } else if (pay_type_input == "2") {
        pay_type = Pay_Type::Salary;
    } else if (pay_type_input == "3") {
        pay_type = Pay_Type::Casual;
    } else {
        std::cout << Display_Error + " " + Invalid_Selection << std::endl;
        // call the function again to prompt the user for input
        Get_Pay_Type(pay_type);
    }
}

void Get_Employment_Type(Employment_Type& employment_type) {
    Display_Employee_Employment_Type_Question_UI();
    std::string employment_type_input;
    std::getline(std::cin, employment_type_input);
    if (employment_type_input == "1") {
        employment_type = Employment_Type::Full_Time;
    } else if (employment_type_input == "2") {
        employment_type = Employment_Type::Part_Time;
    } else if (employment_type_input == "3") {
        employment_type = Employment_Type::Fixed_Term;
    } else if (employment_type_input == "4") {
        employment_type = Employment_Type::Single_Occasion;
    } else if (employment_type_input == "5") {
        employment_type = Employment_Type::Casual;
    } else {
        std::cout << Display_Error + " " + Invalid_Selection << std::endl;
        // call the function again to prompt the user for input
        Get_Employment_Type(employment_type);
    }
}

void Get_Pay_Frequency_Type(Pay_Frequency& pay_frequency_type) {
    Display_Employee_Pay_Frequency_Type_Question_UI();
    std::string pay_frequency_input;
    std::getline(std::cin, pay_frequency_input);
    if (pay_frequency_input == "1") {
        pay_frequency_type = Pay_Frequency::Weekly;
    } else if (pay_frequency_input == "2") {
        pay_frequency_type = Pay_Frequency::Fortnightly;
    } else if (pay_frequency_input == "3") {
        pay_frequency_type = Pay_Frequency::Monthly;
    } else {
        std::cout << Display_Error + " " + Invalid_Selection << std::endl;
        // call the function again to prompt the user for input
        Get_Pay_Frequency_Type(pay_frequency_type);
    }
}

void Get_Contracted_Hours(int& contracted_hours) {
    Display_Employee_Contracted_Hours_Question_UI();
    std::cin >> contracted_hours;
    if (contracted_hours < 0) {
        std::cerr << "Error: Contracted hours cannot be negative." << std::endl;
        Get_Contracted_Hours(contracted_hours);
    }
}

void Get_Schedule_Type(Schedule_Type& schedule_type) {
    Display_Employee_Schedule_Question_UI();
    std::string schedule_input;
    std::getline(std::cin, schedule_input);
    if (schedule_input == "1") {
        schedule_type = Schedule_Type::Fixed;
    } else if (schedule_input == "2") {
        schedule_type = Schedule_Type::Flexible;
    } else {
        std::cout << Display_Error + " " + Invalid_Selection << std::endl;
        // call the function again to prompt the user for input
        Get_Schedule_Type(schedule_type);
    }
}

void Get_Overtime_Rate(int& overtime_rate) {
    Display_Employee_Overtime_Question_UI();
    std::cin >> overtime_rate;
    if (overtime_rate < 0) {
        std::cerr << "Error: Overtime rate cannot be negative." << std::endl;
        Get_Overtime_Rate(overtime_rate);
    } else if (overtime_rate > 200) {
        std::cerr << "Error: Overtime rate cannot exceed 200." << std::endl;
        Get_Overtime_Rate(overtime_rate);
    }
}

void Get_Overtime_Maximum_Hours(int& overtime_maximum_hours) {
    Display_Employee_Overtime_Maximum_Hours_Question_UI();
    std::cin >> overtime_maximum_hours;
    if (overtime_maximum_hours < 0) {
        std::cerr << "Error: Overtime maximum hours cannot be negative." << std::endl;
        Get_Overtime_Maximum_Hours(overtime_maximum_hours);
    } else if (overtime_maximum_hours > 10) {
        std::cerr << "Error: Overtime maximum hours cannot exceed 10 per week." << std::endl;
        Get_Overtime_Maximum_Hours(overtime_maximum_hours);
    }
}


