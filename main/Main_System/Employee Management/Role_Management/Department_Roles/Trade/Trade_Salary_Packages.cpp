// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Trade_Salary_Packages.h"
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

// Trade Salary Packages
void Junior_General_Trade_Staff_Package() {
    Approximate_Annual_Salary = 57000; // $NZD
    KiwiSaver_Employer_Contribution = 3; // %
    Annual_Leave_Entitlement = 20; // Default/Extra Benefits Total (in days)
    Sick_Leave_Entitlement = 10; // Default/Extra Benefits Total (in days)
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 0; // Employee Extra Benefits (in weeks)
    Special_Leave_Entitlement = 3; // Default/Extra Benefits Total (in days)
    Superannuation_Contribution = 0; // %
    Health_Insurance_Contribution = 0; // $NZD per year
    Professional_Development_Contribution = 500; // undecided value
    Employee_Assistance_Program_Contribution = 500; // undecided value
}

void Senior_General_Trade_Staff_Package() {
    Approximate_Annual_Salary = 60000;
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
void Junior_Trainee_Trade_Staff_Package() {
    Approximate_Annual_Salary = 53000;
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

void Senior_Trainee_Trade_Staff_Package() {
    Approximate_Annual_Salary = 56000;
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

void Junior_Casual_Trade_Staff_Package() {
    Approximate_Annual_Salary = 55000;
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

void Senior_Casual_Trade_Staff_Package() {
    Approximate_Annual_Salary = 58000;
    KiwiSaver_Employer_Contribution = 4;
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

void Junior_Apprentice_Package() {
    Approximate_Annual_Salary = 53000;
    KiwiSaver_Employer_Contribution = 3;
    Annual_Leave_Entitlement = 20;
    Sick_Leave_Entitlement = 10;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 1;
    Special_Leave_Entitlement = 3;
    Superannuation_Contribution = 3;
    Health_Insurance_Contribution = 800;
    Professional_Development_Contribution = 5000;
    Employee_Assistance_Program_Contribution = 500;
}

void Senior_Mentor_Package() {
    Approximate_Annual_Salary = 61000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 25;
    Sick_Leave_Entitlement = 14;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 1000;
    Professional_Development_Contribution = 2000;
    Employee_Assistance_Program_Contribution = 800;
}

void Safety_Officer_Package() {
    Approximate_Annual_Salary = 62000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 25;
    Sick_Leave_Entitlement = 14;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 1500;
    Professional_Development_Contribution = 1000;
    Employee_Assistance_Program_Contribution = 800;
}

void Sales_Representative_Package() {
    Approximate_Annual_Salary = 63000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 25;
    Sick_Leave_Entitlement = 14;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 1000;
    Professional_Development_Contribution = 500;
    Employee_Assistance_Program_Contribution = 500;
}

void Junior_Trade_Manager_Package() {
    Approximate_Annual_Salary = 65000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 28;
    Sick_Leave_Entitlement = 16;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 3;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 1000;
    Professional_Development_Contribution = 1000;
    Employee_Assistance_Program_Contribution = 500;
}

void Senior_Trade_Manager_Package() {
    Approximate_Annual_Salary = 70000;
    KiwiSaver_Employer_Contribution = 5;
    Annual_Leave_Entitlement = 30;
    Sick_Leave_Entitlement = 18;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 4;
    Special_Leave_Entitlement = 5;
    Superannuation_Contribution = 5;
    Health_Insurance_Contribution = 1500;
    Professional_Development_Contribution = 2000;
    Employee_Assistance_Program_Contribution = 800;
}

void Get_Trade_Salary_Package(Trade_Role trade_role) {
    switch (trade_role) {
        case Trade_Role::Junior_General_Trade_Staff:
            Junior_General_Trade_Staff_Package();
            break;
        case Trade_Role::Senior_General_Trade_Staff:
            Senior_General_Trade_Staff_Package();
            break;
        case Trade_Role::Junior_Trainee_Trade_Staff:
            Junior_Trainee_Trade_Staff_Package();
            break;
        case Trade_Role::Senior_Trainee_Trade_Staff:
            Senior_Trainee_Trade_Staff_Package();
            break;
        case Trade_Role::Junior_Casual_Trade_Staff:
            Junior_Casual_Trade_Staff_Package();
            break;
        case Trade_Role::Senior_Casual_Trade_Staff:
            Senior_Casual_Trade_Staff_Package();
            break;
        case Trade_Role::Junior_Apprentice:
            Junior_Apprentice_Package();
            break;
        case Trade_Role::Senior_Mentor:
            Senior_Mentor_Package();
            break;
        case Trade_Role::Safety_Officer:
            Safety_Officer_Package();
            break;
        case Trade_Role::Sales_Representative:
            Sales_Representative_Package();
            break;
        case Trade_Role::Junior_Trade_Manager:
            Junior_Trade_Manager_Package();
            break;
        case Trade_Role::Senior_Trade_Manager:
            Senior_Trade_Manager_Package();
            break;
        default:
            std::cout << Display_Error << "Invalid Trade Role selection." << std::endl;
            break;
    }
}

