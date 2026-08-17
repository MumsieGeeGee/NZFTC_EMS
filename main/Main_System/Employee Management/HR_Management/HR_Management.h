// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef HR_MANAGEMENT_H
#define HR_MANAGEMENT_H

#include <string>

struct Leave_and_Holiday_Entitlements_Details {
	std::string username;
	std::string business_role;
	std::string job_role;
	int annual_leave_entitlement = 0;
	int sick_leave_entitlement = 0;
	int public_holidays_entitlement = 0;
	int parental_leave_entitlement = 0;
	int special_leave_entitlement = 0;
	std::string new_years_day;
	std::string day_after_new_years_day;
	std::string waitangi_day;
	std::string good_friday;
	std::string easter_monday;
	std::string anzac_day;
	std::string queens_kings_birthday;
	std::string labour_day;
	std::string christmas_day;
	std::string boxing_day;
};

struct Tax_Information_Record_Details {
	std::string username;
	std::string ird_number;
	std::string file_contents;
};

void Get_Current_User_Leave_and_Holiday_Entitlements();
void Get_Selected_User_Leave_and_Holiday_Entitlements(const std::string& username);
void Get_View_Current_User_Tax_Information();
void Get_View_Selected_User_Tax_Information(const std::string& username);
void Get_Edit_Current_User_Tax_Information();
void Get_Edit_Selected_User_Tax_Information(const std::string& username);


#endif // HR_MANAGEMENT_H
