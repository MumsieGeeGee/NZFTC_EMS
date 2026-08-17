// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Admin_Salary_Packages.h"
#include "../../../../UI/Employee_Management_Menu_UI.h"
#include "../../../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../Role_and_Salary_Details.h"
#include "../../Leave_and_Holiday.h"
#include <string>
#include <iostream>

// Global salary package variables
int Approximate_Annual_Salary = 0;
int KiwiSaver_Employer_Contribution = 0;
int Annual_Leave_Entitlement = 0;
int Sick_Leave_Entitlement = 0;
int Public_Holidays_Entitlement = 0;
int Parental_Leave_Entitlement = 0;
int Special_Leave_Entitlement = 0;  // Funerals, Bereavement, Jury Duty, Military Service, Voting, Blood Donation, Marriage, Domestic Violence etc
int Superannuation_Contribution = 0; // Extra/On-top of Kiwisaver Employer Contribution
int Health_Insurance_Contribution = 0; // Monetary contribution to health insurance (if applicable)
int Professional_Development_Contribution = 0; // Monetary contribution (if applicable) (e.g., courses, certifications, workshops, conferences, etc.)
int Employee_Assistance_Program_Contribution = 0; // Monetary contribution (if applicable) (e.g., counselling, mental health support, wellness programs, etc.)

// Admin Salary Packages
void Junior_General_Administrator_Package() {
    Approximate_Annual_Salary = 51000; // $NZD
    KiwiSaver_Employer_Contribution = 3; // $
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

void Senior_General_Administrator_Package() {
    Approximate_Annual_Salary = 57000;
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

void Junior_Trainee_Administrator_Package() {
    Approximate_Annual_Salary = 48000;
    KiwiSaver_Employer_Contribution = 3;
    Annual_Leave_Entitlement = 20;
    Sick_Leave_Entitlement = 10;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 0;
    Special_Leave_Entitlement = 3;
    Superannuation_Contribution = 0;
    Health_Insurance_Contribution = 0;
    Professional_Development_Contribution = 1000;
    Employee_Assistance_Program_Contribution = 800;
}

void Senior_Trainee_Administrator_Package() {
    Approximate_Annual_Salary = 54000;
    KiwiSaver_Employer_Contribution = 3;
    Annual_Leave_Entitlement = 20;
    Sick_Leave_Entitlement = 10;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 0;
    Special_Leave_Entitlement = 3;
    Superannuation_Contribution = 0;
    Health_Insurance_Contribution = 0;
    Professional_Development_Contribution = 2000;
    Employee_Assistance_Program_Contribution = 1000;
}

void Junior_Casual_Administrator_Package() {
    Approximate_Annual_Salary = 48000;
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

void Senior_Casual_Administrator_Package() {
    Approximate_Annual_Salary = 54000;
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

void Receptionist_Package() {
    Approximate_Annual_Salary = 50000;
    KiwiSaver_Employer_Contribution = 3;
    Annual_Leave_Entitlement = 22;
    Sick_Leave_Entitlement = 12;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 1;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 3;
    Health_Insurance_Contribution = 500;
    Professional_Development_Contribution = 500;
    Employee_Assistance_Program_Contribution = 800;
}

void Call_Centre_Operator_Package() {
    Approximate_Annual_Salary = 52000;
    KiwiSaver_Employer_Contribution = 3;
    Annual_Leave_Entitlement = 22;
    Sick_Leave_Entitlement = 12;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 1;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 3;
    Health_Insurance_Contribution = 500;
    Professional_Development_Contribution = 500;
    Employee_Assistance_Program_Contribution = 800;
}

void Executive_Assistant_Package() {
    Approximate_Annual_Salary = 59000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 25;
    Sick_Leave_Entitlement = 14;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 800;
    Professional_Development_Contribution = 500;
    Employee_Assistance_Program_Contribution = 800;
}

void Personal_Assistant_Package() {
    Approximate_Annual_Salary = 59000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 25;
    Sick_Leave_Entitlement = 14;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 800;
    Professional_Development_Contribution = 500;
    Employee_Assistance_Program_Contribution = 800;
}

void Human_Resources_Administrator_Package() {
    Approximate_Annual_Salary = 61000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 25;
    Sick_Leave_Entitlement = 14;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 800;
    Professional_Development_Contribution = 1000;
    Employee_Assistance_Program_Contribution = 800;
}

void Junior_IT_Support_Administrator_Package() {
    Approximate_Annual_Salary = 54000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 25;
    Sick_Leave_Entitlement = 14;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 800;
    Professional_Development_Contribution = 1000;
    Employee_Assistance_Program_Contribution = 800;
}

void Senior_IT_Support_Administrator_Package() {
    Approximate_Annual_Salary = 63000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 28;
    Sick_Leave_Entitlement = 16;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 3;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 1000;
    Professional_Development_Contribution = 2000;
    Employee_Assistance_Program_Contribution = 1000;
}

void Junior_Admin_Manager_Package() {
    Approximate_Annual_Salary = 58000;
    KiwiSaver_Employer_Contribution = 4;
    Annual_Leave_Entitlement = 28;
    Sick_Leave_Entitlement = 16;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 2;
    Special_Leave_Entitlement = 4;
    Superannuation_Contribution = 4;
    Health_Insurance_Contribution = 800;
    Professional_Development_Contribution = 1000;
    Employee_Assistance_Program_Contribution = 800;
}

void Senior_Admin_Manager_Package() {
    Approximate_Annual_Salary = 65000;
    KiwiSaver_Employer_Contribution = 5;
    Annual_Leave_Entitlement = 30;
    Sick_Leave_Entitlement = 18;
    Public_Holidays_Entitlement = Calculate_Public_Holidays_Count();
    Parental_Leave_Entitlement = 4;
    Special_Leave_Entitlement = 5;
    Superannuation_Contribution = 5;
    Health_Insurance_Contribution = 1000;
    Professional_Development_Contribution = 2000;
    Employee_Assistance_Program_Contribution = 1000;
}

// Main dispatcher function to get salary package based on Admin Role
void Get_Admin_Salary_Package(Admin_Role admin_role) {
    switch (admin_role) {
        case Admin_Role::Junior_General_Administrator:
            Junior_General_Administrator_Package();
            break;
        case Admin_Role::Senior_General_Administrator:
            Senior_General_Administrator_Package();
            break;
        case Admin_Role::Junior_Trainee_Administrator:
            Junior_Trainee_Administrator_Package();
            break;
        case Admin_Role::Senior_Trainee_Administrator:
            Senior_Trainee_Administrator_Package();
            break;
        case Admin_Role::Junior_Casual_Administrator:
            Junior_Casual_Administrator_Package();
            break;
        case Admin_Role::Senior_Casual_Administrator:
            Senior_Casual_Administrator_Package();
            break;
        case Admin_Role::Receptionist:
            Receptionist_Package();
            break;
        case Admin_Role::Call_Centre_Operator:
            Call_Centre_Operator_Package();
            break;
        case Admin_Role::Executive_Assistant:
            Executive_Assistant_Package();
            break;
        case Admin_Role::Personal_Assistant:
            Personal_Assistant_Package();
            break;
        case Admin_Role::Human_Resources_Administrator:
            Human_Resources_Administrator_Package();
            break;
        case Admin_Role::Junior_IT_Support_Administrator:
            Junior_IT_Support_Administrator_Package();
            break;
        case Admin_Role::Senior_IT_Support_Administrator:
            Senior_IT_Support_Administrator_Package();
            break;
        case Admin_Role::Junior_Level_Manager:
            Junior_Admin_Manager_Package();
            break;
        case Admin_Role::Senior_Level_Manager:
            Senior_Admin_Manager_Package();
            break;
        default:
            std::cout << Display_Error << "Invalid Admin Role selection." << std::endl;
            break;
    }
}
