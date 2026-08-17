// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Trade_Salary_UI.h"
#include "Trade_Salary_Packages.h"
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
    std::cout << Superannuation_Contribution_Label << ": " << superannuation_contribution << "%. Separate Scheme/On-top of KiwiSaver. \n";
    std::cout << Health_Insurance_Contribution_Label << ": $" << health_insurance_entitlement << " per year. \n";
    std::cout << Professional_Development_Contribution_Label << ": $" << professional_development_contribution << " per year. \n";
    std::cout << Employee_Assistance_Program_Contribution_Label << ": $" << employee_assistance_program_contribution << " per year. \n";
}
}

void Junior_General_Trade_Staff_Package_UI() {
    std::cout << Junior_General_Trade_Staff_Member << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_General_Trade_Staff_Package_UI() {
    std::cout << Senior_General_Trade_Staff_Member << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Junior_Trainee_Trade_Staff_Package_UI() {
    std::cout << Junior_Trainee_Trade_Staff_Member << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_Trainee_Trade_Staff_Package_UI() {
    std::cout << Senior_Trainee_Trade_Staff_Member << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Junior_Casual_Trade_Staff_Package_UI() {
    std::cout << Junior_Casual_Trade_Staff_Member << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_Casual_Trade_Staff_Package_UI() {
    std::cout << Senior_Casual_Trade_Staff_Member << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Junior_Apprentice_Package_UI() {
    std::cout << Junior_Apprentice << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_Mentor_Package_UI() {
    std::cout << Senior_Mentor << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Safety_Officer_Package_UI() {
    std::cout << Safety_Officer << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Sales_Representative_Package_UI() {
    std::cout << Sales_Representative << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Junior_Trade_Manager_Package_UI() {
    std::cout << Junior_Trade_Manager << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_Trade_Manager_Package_UI() {
    std::cout << Senior_Trade_Manager << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Display_Employee_Salary_Type_Trade_UI(Trade_Role trade_role) {
    Get_Trade_Salary_Package(trade_role);
    std::cout << "Trade Package:\n";
    switch (trade_role) {
        case Trade_Role::Junior_General_Trade_Staff:
            Junior_General_Trade_Staff_Package_UI();
            break;
        case Trade_Role::Senior_General_Trade_Staff:
            Senior_General_Trade_Staff_Package_UI();
            break;
        case Trade_Role::Junior_Trainee_Trade_Staff:
            Junior_Trainee_Trade_Staff_Package_UI();
            break;
        case Trade_Role::Senior_Trainee_Trade_Staff:
            Senior_Trainee_Trade_Staff_Package_UI();
            break;
        case Trade_Role::Junior_Casual_Trade_Staff:
            Junior_Casual_Trade_Staff_Package_UI();
            break;
        case Trade_Role::Senior_Casual_Trade_Staff:
            Senior_Casual_Trade_Staff_Package_UI();
            break;
        case Trade_Role::Junior_Apprentice:
            Junior_Apprentice_Package_UI();
            break;
        case Trade_Role::Senior_Mentor:
            Senior_Mentor_Package_UI();
            break;
        case Trade_Role::Safety_Officer:
            Safety_Officer_Package_UI();
            break;
        case Trade_Role::Sales_Representative:
            Sales_Representative_Package_UI();
            break;
        case Trade_Role::Junior_Trade_Manager:
            Junior_Trade_Manager_Package_UI();
            break;
        case Trade_Role::Senior_Trade_Manager:
            Senior_Trade_Manager_Package_UI();
            break;
        default:
            std::cout << Display_Error << Invalid_Trade_Role << std::endl;
            break;
    }
}
