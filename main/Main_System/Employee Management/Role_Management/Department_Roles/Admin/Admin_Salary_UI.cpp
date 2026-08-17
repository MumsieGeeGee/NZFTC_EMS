// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Admin_Salary_UI.h"
#include "Admin_Salary_Packages.h"
#include "../../../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../Role_and_Salary_Details.h"
#include <string>
#include <iostream>

namespace {
void Package_Helper(int approximate_annual_salary, int kiwiSaver_employer_contribution, int annual_leave_entitlement, int sick_leave_entitlement, int public_holidays_entitlement, int parental_leave_entitlement, int special_leave_entitlement, int superannuation_contribution, int health_insurance_entitlement, int professional_development_contribution, int employee_assistance_program_contribution) {
    std::cout << "Approximate " << Annual_Salary_Label << ": $" << approximate_annual_salary << ". \n";
    std::cout << KiwiSaver_Employer_Contribution_Label << ": " << kiwiSaver_employer_contribution << "%. \n";
    std::cout << Annual_Leave_Entitlement_Label << ": " << annual_leave_entitlement << "days total. (Default + Employee Benefits = Total) \n";
    std::cout << Sick_Leave_Entitlement_Label << ": " << sick_leave_entitlement << "days total. (Default + Employee Benefits = Total) \n";
    std::cout << Public_Holidays_Entitlement_Label << ": " << public_holidays_entitlement << "days total. \n";
    std::cout << Parental_Leave_Entitlement_Label << ": Default 26 weeks + " << parental_leave_entitlement << "weeks extra. (Benefits from Employer) \n";
    std::cout << Special_Leave_Entitlement_Label << ": " << special_leave_entitlement << " days total. (Default + Employee Benefits = Total) \n";
    std::cout << Superannuation_Contribution_Label << ": " << superannuation_contribution << "%. Separate Scheme/On-top of Kiwisaver. \n";
    std::cout << Health_Insurance_Contribution_Label << ": $" << health_insurance_entitlement << " per year. \n";
    std::cout << Professional_Development_Contribution_Label << ": $" << professional_development_contribution << " per year. \n";
    std::cout << Employee_Assistance_Program_Contribution_Label << ": $" << employee_assistance_program_contribution << " per year. \n";
}
}

// Admin Salary Packages UI
void Junior_General_Administrator_Package_UI() {
    std::cout << Junior_General_Administrator_Role_Label << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}
void Senior_General_Administrator_Package_UI() {
    std::cout << Senior_General_Administrator_Role_Label << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Junior_Trainee_Administrator_Package_UI() {
    std::cout << Junior_Trainee_Administrator_Role_Label << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_Trainee_Administrator_Package_UI() {
    std::cout << Senior_Trainee_Administrator_Role_Label << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Junior_Casual_Administrator_Package_UI() {
    std::cout << Junior_Casual_Administrator_Role_Label << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_Casual_Administrator_Package_UI() {
    std::cout << Senior_Casual_Administrator_Role_Label << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Receptionist_Package_UI() {
    std::cout << Admin_Receptionist << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Call_Centre_Operator_Package_UI() {
    std::cout << Admin_Call_Centre_Operator << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Executive_Assistant_Package_UI() {
    std::cout << Admin_Executive_Assistant << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Personal_Assistant_Package_UI() {
    std::cout << Admin_Personal_Assistant << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Human_Resources_Administrator_Package_UI() {
    std::cout << Admin_Human_Resources << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Junior_IT_Support_Administrator_Package_UI() {
    std::cout << Junior_IT_Support_Administrator_Role_Label << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_IT_Support_Administrator_Package_UI() {
    std::cout << Senior_IT_Support_Administrator_Role_Label << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Junior_Admin_Manager_Package_UI() {
    std::cout << Admin_Junior_Manager << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_Admin_Manager_Package_UI() {
    std::cout << Admin_Senior_Manager << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

// Admin Salary by Role UI
void Display_Employee_Salary_Type_Admin_UI(Admin_Role admin_role) {
    Get_Admin_Salary_Package(admin_role);
    std::cout << "Admin Package:\n";
    switch (admin_role) {
        case Admin_Role::Junior_General_Administrator:
            Junior_General_Administrator_Package_UI();
            break;
        case Admin_Role::Senior_General_Administrator:
            Senior_General_Administrator_Package_UI();
            break;
        case Admin_Role::Junior_Trainee_Administrator:
            Junior_Trainee_Administrator_Package_UI();
            break;
        case Admin_Role::Senior_Trainee_Administrator:
            Senior_Trainee_Administrator_Package_UI();
            break;
        case Admin_Role::Junior_Casual_Administrator:
            Junior_Casual_Administrator_Package_UI();
            break;
        case Admin_Role::Senior_Casual_Administrator:
            Senior_Casual_Administrator_Package_UI();
            break;
        case Admin_Role::Receptionist:
            Receptionist_Package_UI();
            break;
        case Admin_Role::Call_Centre_Operator:
            Call_Centre_Operator_Package_UI();
            break;
        case Admin_Role::Executive_Assistant:
            Executive_Assistant_Package_UI();
            break;
        case Admin_Role::Personal_Assistant:
            Personal_Assistant_Package_UI();
            break;
        case Admin_Role::Human_Resources_Administrator:
            Human_Resources_Administrator_Package_UI();
            break;
        case Admin_Role::Junior_IT_Support_Administrator:
            Junior_IT_Support_Administrator_Package_UI();
            break;
        case Admin_Role::Senior_IT_Support_Administrator:
            Senior_IT_Support_Administrator_Package_UI();
            break;
        case Admin_Role::Junior_Level_Manager:
            Junior_Admin_Manager_Package_UI();
            break;
        case Admin_Role::Senior_Level_Manager:
            Senior_Admin_Manager_Package_UI();
            break;
        default:
            std::cout << Display_Error << Invalid_Admin_Role << std::endl;
    }
}
