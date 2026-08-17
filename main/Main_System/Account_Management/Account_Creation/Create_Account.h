// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include <string>

enum class Account_Type {
    Admin, 
    User
};

std::string Account_Type_To_String(Account_Type account_type);
void Account_Type_Options(Account_Type& choice);
void Get_Account_Type(Account_Type& choice);
std::string Business_Role_To_String(int business_role_selection);
bool Get_Business_Role_Selection(int& business_role_selection);
std::string Job_Role_To_String(int business_role_selection, int job_role_selection);
bool Get_Job_Role_Selection(int business_role_selection, int& job_role_selection);

void Get_First_Name(std::string& first_name);
void Get_Middle_Name(std::string& middle_name);
void Get_Surname(std::string& last_name);
void Get_Date_of_Birth(std::string& date_of_birth);

void Get_House_Number(std::string& house_number);
void Get_Street_Name(std::string& street_name);
void Get_Suburb(std::string& suburb);
void Get_City(std::string& city);
void Get_Postcode(std::string& postcode);
void Get_Post_Box(std::string& post_box);
void Get_Phone_Number(std::string& phone_number);
void Get_Email_Address(std::string& email_address);
void Get_Full_Address(std::string& address);

void Get_Continue_Account_Creation_Question_Options();

void Get_IRD_Number(std::string& IRD_Number);
bool Get_Username(std::string& username,
          Account_Type account_type,
                  const std::string& first_name,
                  const std::string& middle_name,
                  const std::string& last_name,
          std::string& business_role,
          std::string& job_role);
void Get_Password(std::string& password);
bool Get_Required_Role_Assignment(
	const std::string& business_role,
	const std::string& job_role,
	std::string& assignment_field,
	std::string& assigned_username,
	const std::string& username_to_exclude = "");
void Create_Account();
void Save_Created_Account(const std::string& account_type,
                        const std::string& first_name,
                        const std::string& middle_name,
                        const std::string& last_name,
                        const std::string& date_of_birth,
                        const std::string& IRD_number,
                        const std::string& business_role,
                        const std::string& job_role,
                        const std::string& assignment_field,
                        const std::string& assigned_username,
                        const std::string& address,
                        const std::string& email_address,
                        const std::string& phone_number,
                        const std::string& username,
                        const std::string& password);


#endif // CREATE_ACCOUNT_H
