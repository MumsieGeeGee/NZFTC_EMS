// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Finance_Salary_Packages.h"
#include "../../../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../../../UI/Employee_Management_Menu_UI.h"
#include "../../Role_and_Salary_Details.h"
#include "../../Leave_and_Holiday.h"
#include <string>
#include <iostream>

// Global salary package variables
extern int Approximate_Annual_Salary;
extern int KiwiSaver_Employer_Contribution;
extern int Annual_Leave_Entitlement;
extern int Sick_Leave_Entitlement;
extern int Public_Holidays_Entitlement;
extern int Parental_Leave_Entitlement;
extern int Special_Leave_Entitlement; // Funerals, Bereavement, Jury Duty, Military Service, Voting, Blood Donation, Marriage, Domestic Violence etc
extern int Superannuation_Contribution;
extern int Health_Insurance_Contribution;
extern int Professional_Development_Contribution;
extern int Employee_Assistance_Program_Contribution;

// Finance Salary Packages
void Junior_General_Finance_Staff_Package() {
    Approximate_Annual_Salary = 55000; // $NZD
    KiwiSaver_Employer_Contribution = 3; // %
    Annual_Leave_Entitlement = 20; // Default/Extra Benefits Total (in days)
    Sick_Leave_Entitlement = 10; // Default/Extra Benefits Total (in days)
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 0; // Employee Extra Benefits (in weeks)
    Special_Leave_Entitlement = 3; // Default/Extra Benefits Total (in days)
    Superannuation_Contribution = 0; // %
    Health_Insurance_Contribution = 0; // $NZD per year
    Professional_Development_Contribution = 500; // $NZD per year
    Employee_Assistance_Program_Contribution = 500; // $NZD per year
}

void Senior_General_Finance_Staff_Package() {
    Approximate_Annual_Salary = 66000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 22;
    Sick_Leave_Entitlement = 12;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 3;
    Health_Insurance_Contribution = 500;
    Professional_Development_Contribution = 1000;
    Employee_Assistance_Program_Contribution = 800;
}

void Junior_Trainee_Finance_Staff_Package() {
    Approximate_Annual_Salary = 52000;
    KiwiSaver_Employer_Contribution = 3; 
    Annual_Leave_Entitlement = 20;
    Sick_Leave_Entitlement = 10;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 0;
    Special_Leave_Entitlement = 3;
    Superannuation_Contribution = 0; 
    Health_Insurance_Contribution = 0; 
    Professional_Development_Contribution = 1000; 
    Employee_Assistance_Program_Contribution = 500; 
}

void Senior_Trainee_Finance_Staff_Package() {
    Approximate_Annual_Salary = 60000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 20;
    Sick_Leave_Entitlement = 10;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 0;
    Special_Leave_Entitlement = 3;
    Superannuation_Contribution = 0;
    Health_Insurance_Contribution = 0;
    Professional_Development_Contribution = 2000;
    Employee_Assistance_Program_Contribution = 800;
}

void Junior_Casual_Finance_Staff_Package() {
    Approximate_Annual_Salary = 52000;
    KiwiSaver_Employer_Contribution = 3;
    Annual_Leave_Entitlement = 20;
    Sick_Leave_Entitlement = 10;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 0; 
    Special_Leave_Entitlement = 3;
    Superannuation_Contribution = 0;
    Health_Insurance_Contribution = 0;
    Professional_Development_Contribution = 0;
    Employee_Assistance_Program_Contribution = 0;
}

void Senior_Casual_Finance_Staff_Package() {
    Approximate_Annual_Salary = 60000;
    KiwiSaver_Employer_Contribution = 3;
    Annual_Leave_Entitlement = 20;
    Sick_Leave_Entitlement = 10;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 0;
    Special_Leave_Entitlement = 3;
    Superannuation_Contribution = 0;
    Health_Insurance_Contribution = 0;
    Professional_Development_Contribution = 0;
    Employee_Assistance_Program_Contribution = 0;
}

void Payroll_Administrator_Package() {
    Approximate_Annual_Salary = 60000;
    KiwiSaver_Employer_Contribution = 3;
    Annual_Leave_Entitlement = 22;
    Sick_Leave_Entitlement = 12;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 3;
    Health_Insurance_Contribution = 500;
    Professional_Development_Contribution = 500;
    Employee_Assistance_Program_Contribution = 500; 
}

void Accounts_Payable_Administrator_Package() {
    Approximate_Annual_Salary = 60000; 
    KiwiSaver_Employer_Contribution = 4; 
    Annual_Leave_Entitlement = 22;
    Sick_Leave_Entitlement = 12;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 500;
    Professional_Development_Contribution = 500;
    Employee_Assistance_Program_Contribution = 500;
}

void Accounts_Receivable_Administrator_Package() {
    Approximate_Annual_Salary = 60000;
    KiwiSaver_Employer_Contribution = 4; 
    Annual_Leave_Entitlement = 22;
    Sick_Leave_Entitlement = 12;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 500; 
    Professional_Development_Contribution = 500;
    Employee_Assistance_Program_Contribution = 500; 
}

void Junior_Finance_Manager_Package() {
    Approximate_Annual_Salary = 70000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 25;
    Sick_Leave_Entitlement = 14;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 800;
    Professional_Development_Contribution = 1000;
    Employee_Assistance_Program_Contribution = 500;
}

void Senior_Finance_Manager_Package() {
    Approximate_Annual_Salary = 85000;
    KiwiSaver_Employer_Contribution = 5;
    Annual_Leave_Entitlement = 28;
    Sick_Leave_Entitlement = 16;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 3;
    Special_Leave_Entitlement = 5;
    Superannuation_Contribution = 5;
    Health_Insurance_Contribution = 1000;
    Professional_Development_Contribution = 2000;
    Employee_Assistance_Program_Contribution = 800;
}

void Get_Finance_Salary_Package(Finance_Role finance_role) {
    switch (finance_role) {
        case Finance_Role::Junior_General_Finance_Staff:
            Junior_General_Finance_Staff_Package();
            break;
        case Finance_Role::Senior_General_Finance_Staff:
            Senior_General_Finance_Staff_Package();
            break;
        case Finance_Role::Junior_Trainee_Finance_Staff:
            Junior_Trainee_Finance_Staff_Package();
            break;
        case Finance_Role::Senior_Trainee_Finance_Staff:
            Senior_Trainee_Finance_Staff_Package();
            break;
        case Finance_Role::Junior_Casual_Finance_Staff:
            Junior_Casual_Finance_Staff_Package();
            break;
        case Finance_Role::Senior_Casual_Finance_Staff:
            Senior_Casual_Finance_Staff_Package();
            break;
        case Finance_Role::Payroll_Administrator:
            Payroll_Administrator_Package();
            break;
        case Finance_Role::Accounts_Payable_Administrator:
            Accounts_Payable_Administrator_Package();
            break;
        case Finance_Role::Accounts_Receivable_Administrator:
            Accounts_Receivable_Administrator_Package();
            break;
        case Finance_Role::Junior_Finance_Manager:
            Junior_Finance_Manager_Package();
            break;
        case Finance_Role::Senior_Finance_Manager:
            Senior_Finance_Manager_Package();
            break;
        default:
            std::cout << Display_Error << "Invalid Finance Role selection." << std::endl;
            break;
    }
}
