// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef SAVE_ACCOUNT_DETAILS_H
#define SAVE_ACCOUNT_DETAILS_H

#include <string>

int Business_Role_To_Selection(const std::string& business_role);
int Job_Role_To_Selection(int business_role_selection, const std::string& job_role);

bool Username_Exists_In_Index(const std::string& username);
bool Password_Exists_In_Index(const std::string& password_hash);
bool IRD_Username_Exists_In_Index(const std::string& IRD_number, const std::string& username);
bool Save_Username_To_Index(const std::string& username);
bool Save_Password_To_Index(const std::string& password_hash);
bool Save_IRD_and_Username_To_Index(const std::string& IRD_number, const std::string& username);

void Reset_Account_Record();
void Save_Detail_To_File(const std::string& detail_name, const std::string& detail_value);
void Save_Account_Type(const std::string& account_type);
void Save_IRD_Number(const std::string& IRD_number);
void Save_First_Name(const std::string& first_name);
void Save_Middle_Name(const std::string& middle_name);
void Save_Last_Name(const std::string& last_name);
void Save_Date_of_Birth(const std::string& date_of_birth);
void Save_Address(const std::string& address);
void Save_Email_Address(const std::string& email_address);
void Save_Phone_Number(const std::string& phone_number);
bool Save_Username_From_Creation(const std::string& finalized_username,
                                 const std::string& account_type,
                                 std::string& assigned_username);
bool Save_Username_From_Role_Edit(const std::string& existing_username,
                                  const std::string& account_type,
                                  const std::string& first_name,
                                  const std::string& middle_name,
                                  const std::string& last_name,
                                  const std::string& business_role,
                                  const std::string& job_role,
                                  const std::string& ird_number,
                                  std::string& assigned_username);
void Save_Username(const std::string& username);
void Save_Password(const std::string& password);
void Check_Account_Details_Completion(const std::string& first_name, 
                                        const std::string& middle_name, 
                                        const std::string& last_name, 
                                        const std::string& date_of_birth, 
                                        const std::string& address, 
                                        const std::string& email_address, 
                                        const std::string& phone_number, 
                                        const std::string& username, 
                                        const std::string& password);


#endif // SAVE_ACCOUNT_DETAILS_H

