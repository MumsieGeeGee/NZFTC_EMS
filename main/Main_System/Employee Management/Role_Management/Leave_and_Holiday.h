// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef LEAVE_AND_HOLIDAY_H
#define LEAVE_AND_HOLIDAY_H

#include <string>

// Public Holidays Functions
void Get_Current_Years_NZ_Public_Holidays();

std::string Get_New_Years_Day();
std::string Get_Day_After_New_Years_Day();
std::string Get_Waitangi_Day();
std::string Get_Good_Friday();
std::string Get_Easter_Monday();
std::string Get_ANZAC_Day();
std::string Get_Queens_Kings_Birthday();
std::string Get_Labour_Day();
std::string Get_Christmas_Day();
std::string Get_Boxing_Day();

// Calculate total public holidays count
int Calculate_Public_Holidays_Count();

// Leave and Holiday Pay Taxation

// Contracted Weekly Hours (Salaried employees)
// Contracted Hourly Rate (Salaried employees)
// Annual Salary (Salaried employees)
// Monthly Salary (Salaried employees)
// Fortnightly Salary (Salaried employees)
// Weekly Salary (Salaried employees)

// Hourly Rate (Hourly employees/casual employees)
// Overtime Rates (Hourly employees/casual employees, regular from clocked hours, and when scheduled hours exceed contracted hours)

// Accruing Annual Leave and Holiday Pay Taxation
// Sick Leave Entitlements Rates
// Annual Leave Entitlements Rates
// Public Holidays Entitlements Rates
// Parental Leave Entitlements Rates
// Special Leave Entitlements Rates

// Pay-as-you-go Holiday Pay (casuals only)
// Sick Leave Entitlements Rates
// Annual Leave Entitlements Rates
// Public Holidays Entitlements Rates
// Parental Leave Entitlements Rates
// Special Leave Entitlements Rates

#endif // LEAVE_AND_HOLIDAY_H

