// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef DATABASE_H
#define DATABASE_H

#include <filesystem>
#include <map>
#include <string>
#include <vector>

extern const std::filesystem::path Employer_Details_File_Path;

// Database Handling Functions
bool IRD_Record_Exists_In_Master_Folder(const std::string& IRD_Number);
std::string Get_IRD_Record_File_Path(const std::string& IRD_Number);
bool Read_Record_Field_Value(const std::string& IRD_Number, const std::string& fieldLabel, std::string& outValue);
bool Get_IRD_Record_Confirmation_Status(const std::string& IRD_Number, std::string& statusOut);
bool Get_IRD_Record_File_Contents(const std::string& IRD_Number, std::string& fileContentsOut);
bool Create_IRD_Record_File(const std::string& IRD_Number);
bool Is_Student_Loan_Active(const std::string& IRD_Number);
bool Is_Primary_Tax_Code_ME(const std::string& IRD_Number);
bool Is_Special_Tax_Code_Active(const std::string& IRD_Number);
bool Is_Child_Support_Active(const std::string& IRD_Number);
bool Is_KiwiSaver_Active(const std::string& IRD_Number);
bool Is_KiwiSaver_Opted_Out(const std::string& IRD_Number);
bool Is_ESCT_Active(const std::string& IRD_Number);
bool Is_Salary_Employee(const std::string& IRD_Number);
bool Is_Hourly_Employee(const std::string& IRD_Number);
bool Is_Casual_Employee(const std::string& IRD_Number);
bool Open_Employer_Details_File(const std::string& EmployerDetailsPath);

void Open_IRD_Record_File(const std::string& IRD_Number);

bool search_IRD_Master_Folder_for_Record(const std::string& IRD_Number);


#endif // DATABASE_H
