// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.
#ifndef ADMIN_SALARY_PACKAGES_H
#define ADMIN_SALARY_PACKAGES_H

#include <string>
#include "../../Employee_Profile.h"
#include "../../Role_and_Salary_Details.h"

// Function declarations for Admin salary packages
void Junior_General_Administrator_Package();
void Senior_General_Administrator_Package();
void Junior_Trainee_Administrator_Package();
void Senior_Trainee_Administrator_Package();
void Junior_Casual_Administrator_Package();
void Senior_Casual_Administrator_Package();
void Receptionist_Package();
void Call_Centre_Operator_Package();
void Executive_Assistant_Package();
void Personal_Assistant_Package();
void Human_Resources_Administrator_Package();
void Junior_IT_Support_Administrator_Package();
void Senior_IT_Support_Administrator_Package();
void Junior_Admin_Manager_Package();
void Senior_Admin_Manager_Package();
void Get_Admin_Salary_Package(Admin_Role admin_role);

// Global salary variables
extern int Approximate_Annual_Salary;
extern int KiwiSaver_Employer_Contribution;
extern int Annual_Leave_Entitlement;
extern int Sick_Leave_Entitlement;
extern int Public_Holidays_Entitlement;
extern int Parental_Leave_Entitlement;
extern int Special_Leave_Entitlement;
extern int Superannuation_Contribution;
extern int Health_Insurance_Contribution;
extern int Professional_Development_Contribution;
extern int Employee_Assistance_Program_Contribution;

#endif // ADMIN_SALARY_PACKAGES_H
