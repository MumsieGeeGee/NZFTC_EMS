// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Finance_Salary_UI.h"
#include "Finance_Salary_Packages.h"
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

// Finance Salary Packages UI
void Junior_General_Finance_Staff_Package_UI() {
    std::cout << Junior_General_Finance_Staff_Member << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_General_Finance_Staff_Package_UI() {
    std::cout << Senior_General_Finance_Staff_Member << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Junior_Trainee_Finance_Staff_Package_UI() {
    std::cout << Junior_Trainee_Finance_Staff_Member << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_Trainee_Finance_Staff_Package_UI() {
    std::cout << Senior_Trainee_Finance_Staff_Member << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Junior_Casual_Finance_Staff_Package_UI() {
    std::cout << Junior_Casual_Finance_Staff_Member << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_Casual_Finance_Staff_Package_UI() {
    std::cout << Senior_Casual_Finance_Staff_Member << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Payroll_Administrator_Package_UI() {
    std::cout << Payroll_Administrator << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Accounts_Payable_Administrator_Package_UI() {
    std::cout << Accounts_Payable_Administrator << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Accounts_Receivable_Administrator_Package_UI() {
    std::cout << Accounts_Receivable_Administrator << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Junior_Finance_Manager_Package_UI() {
    std::cout << Junior_Finance_Manager << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Senior_Finance_Manager_Package_UI() {
    std::cout << Senior_Finance_Manager << " Salary Package.\n";
    Package_Helper(Approximate_Annual_Salary, KiwiSaver_Employer_Contribution, Annual_Leave_Entitlement, Sick_Leave_Entitlement, Public_Holidays_Entitlement, Parental_Leave_Entitlement, Special_Leave_Entitlement, Superannuation_Contribution, Health_Insurance_Contribution, Professional_Development_Contribution, Employee_Assistance_Program_Contribution);
}

void Display_Employee_Salary_Type_Finance_UI(Finance_Role finance_role) {
    Get_Finance_Salary_Package(finance_role);
    std::cout << "Finance Package:\n";
    switch (finance_role) {
        case Finance_Role::Junior_General_Finance_Staff:
            Junior_General_Finance_Staff_Package_UI();
            break;
        case Finance_Role::Senior_General_Finance_Staff:
            Senior_General_Finance_Staff_Package_UI();
            break;
        case Finance_Role::Junior_Trainee_Finance_Staff:
            Junior_Trainee_Finance_Staff_Package_UI();
            break;
        case Finance_Role::Senior_Trainee_Finance_Staff:
            Senior_Trainee_Finance_Staff_Package_UI();
            break;
        case Finance_Role::Junior_Casual_Finance_Staff:
            Junior_Casual_Finance_Staff_Package_UI();
            break;
        case Finance_Role::Senior_Casual_Finance_Staff:
            Senior_Casual_Finance_Staff_Package_UI();
            break;
        case Finance_Role::Payroll_Administrator:
            Payroll_Administrator_Package_UI();
            break;
        case Finance_Role::Accounts_Payable_Administrator:
            Accounts_Payable_Administrator_Package_UI();
            break;
        case Finance_Role::Accounts_Receivable_Administrator:
            Accounts_Receivable_Administrator_Package_UI();
            break;
        case Finance_Role::Junior_Finance_Manager:
            Junior_Finance_Manager_Package_UI();
            break;
        case Finance_Role::Senior_Finance_Manager:
            Senior_Finance_Manager_Package_UI();
            break;
        default:
            std::cout << Display_Error << Invalid_Finance_Role << std::endl;
    }
}
