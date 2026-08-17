// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.
#ifndef TRADE_SALARY_PACKAGES_H
#define TRADE_SALARY_PACKAGES_H

#include <string>
#include "../../Employee_Profile.h"
#include "../../Role_and_Salary_Details.h"

void Junior_General_Trade_Staff_Package();
void Senior_General_Trade_Staff_Package();
void Junior_Trainee_Trade_Staff_Package();
void Senior_Trainee_Trade_Staff_Package();
void Junior_Casual_Trade_Staff_Package();
void Senior_Casual_Trade_Staff_Package();
void Junior_Apprentice_Package();
void Senior_Mentor_Package();
void Safety_Officer_Package();
void Sales_Representative_Package();
void Junior_Trade_Manager_Package();
void Senior_Trade_Manager_Package();
void Get_Trade_Salary_Package(Trade_Role trade_role);

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

#endif // TRADE_SALARY_PACKAGES_H
