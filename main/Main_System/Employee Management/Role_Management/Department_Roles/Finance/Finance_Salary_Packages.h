// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.
#ifndef FINANCE_SALARY_PACKAGES_H
#define FINANCE_SALARY_PACKAGES_H

#include <string>
#include "../../Employee_Profile.h"
#include "../../Role_and_Salary_Details.h"

void Junior_General_Finance_Staff_Package();
void Senior_General_Finance_Staff_Package();
void Junior_Trainee_Finance_Staff_Package();
void Senior_Trainee_Finance_Staff_Package();
void Junior_Casual_Finance_Staff_Package();
void Senior_Casual_Finance_Staff_Package();
void Payroll_Administrator_Package();
void Accounts_Payable_Administrator_Package();
void Accounts_Receivable_Administrator_Package();
void Junior_Finance_Manager_Package();
void Senior_Finance_Manager_Package();
void Get_Finance_Salary_Package(Finance_Role finance_role);

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

#endif // FINANCE_SALARY_PACKAGES_H
