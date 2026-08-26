// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef ACCOUNT_DATABASE_H
#define ACCOUNT_DATABASE_H

#include <map>
#include <string>
#include <vector>

enum class Account_Update_Result { // maybe add the locked/unlocked state to this enum in the future
	Success,
	Invalid_Input,
	User_Not_Found,
	Missing_IRD_Mapping,
	Write_Failure,
	Index_Update_Failure
};

struct Role_Assignment_Option {
	std::string username;
	std::string display_name;
	std::string business_role;
	std::string job_role;
};

void Reset_Account_Record_State();
bool Save_Account_Record_Field(const std::string& fieldLabel, const std::string& fieldValue);

bool Find_Record_For_Username(const std::string& username,
							  std::string* accountType,
							  std::string* passwordHash);
std::vector<std::string> Get_Usernames_By_Prefix(const std::string& prefix);
std::vector<Role_Assignment_Option> Get_Role_Assignment_Options(
	const std::vector<std::string>& job_roles,
	const std::string& business_role = "");
std::vector<Role_Assignment_Option> Get_Assigned_Team_Members(
	const std::string& supervisor_username);
std::vector<Role_Assignment_Option> Get_Managers_Assistant_Can_Act_On_Behalf_Of(
	const std::string& assistant_username);
bool Get_Username_From_Employee_Records(const std::string& username);
bool Get_Password_From_Employee_Records(const std::string& password_hash);
bool Get_IRD_Username_Match(const std::string& IRD_number, const std::string& username);
std::string Get_IRD_Number_For_Username(const std::string& username);
bool Check_Account_Exists(const std::string& username, const std::string& password, const std::string& IRD_number);
bool Check_Account_Exists_In_Indexes(const std::string& username, const std::string& password, const std::string& IRD_number);
std::string Get_Account_Type_From_Employee_Records(const std::string& username);
bool Get_Account_Record_Details_For_Username(const std::string& username,
												 std::map<std::string, std::string>* detailsOut);
std::string Get_Employee_Record_File_Path_For_Username(
	const std::string& username);
bool Update_Account_Record_Field_For_Username(const std::string& username,
											  const std::string& fieldLabel,
											  const std::string& fieldValue,
											  Account_Update_Result* resultOut = nullptr);
bool Update_Account_Password_For_Username(const std::string& username,
											  const std::string& password,
											  Account_Update_Result* resultOut = nullptr);
bool Update_Account_Role_For_Username(const std::string& username,
										  const std::string& business_role,
										  const std::string& job_role,
										  std::string* updated_username,
										  Account_Update_Result* resultOut = nullptr);
bool Delete_Account_And_IRD_Records_For_Username_and_IRD(const std::string& username,
									  const std::string& ird_number);

#endif // ACCOUNT_DATABASE_H
