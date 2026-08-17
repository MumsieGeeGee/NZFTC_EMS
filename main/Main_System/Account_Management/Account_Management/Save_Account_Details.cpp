// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Save_Account_Details.h"
#include "../Account_Access/Account_Database.h"
#include "Input_Validation_Helpers.h"
#include "Security_Protocols.h"
#include "../../Path_Utilities.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <random>
#include <vector>

int Business_Role_To_Selection(const std::string& business_role) {
	const std::string normalized_role = Trim_Copy(business_role);
	if (normalized_role == "Admin") {
		return 1;
	}
	if (normalized_role == "Finance") {
		return 2;
	}
	if (normalized_role == "Trade") {
		return 3;
	}
	return 0;
}

int Job_Role_To_Selection(int business_role_selection, const std::string& job_role) {
	const std::string normalized_role = Trim_Copy(job_role);

	if (business_role_selection == 1) {
		if (normalized_role == "Junior General Administrator") return 1;
		if (normalized_role == "Senior General Administrator") return 2;
		if (normalized_role == "Junior Trainee Administrator") return 3;
		if (normalized_role == "Senior Trainee Administrator") return 4;
		if (normalized_role == "Junior Casual Administrator") return 5;
		if (normalized_role == "Senior Casual Administrator") return 6;
		if (normalized_role == "Receptionist") return 7;
		if (normalized_role == "Call Centre Operator") return 8;
		if (normalized_role == "Executive Assistant") return 9;
		if (normalized_role == "Personal Assistant") return 10;
		if (normalized_role == "Human Resources Administrator") return 11;
		if (normalized_role == "Junior IT Support Administrator") return 12;
		if (normalized_role == "Senior IT Support Administrator") return 13;
		if (normalized_role == "Junior Level Manager") return 14;
		if (normalized_role == "Senior Level Manager") return 15;
		return 0;
	}

	if (business_role_selection == 2) {
		if (normalized_role == "Junior General Finance Staff") return 1;
		if (normalized_role == "Senior General Finance Staff") return 2;
		if (normalized_role == "Junior Trainee Finance Staff") return 3;
		if (normalized_role == "Senior Trainee Finance Staff") return 4;
		if (normalized_role == "Junior Casual Finance Staff") return 5;
		if (normalized_role == "Senior Casual Finance Staff") return 6;
		if (normalized_role == "Payroll Administrator") return 7;
		if (normalized_role == "Accounts Payable Administrator") return 8;
		if (normalized_role == "Accounts Receivable Administrator") return 9;
		if (normalized_role == "Junior Finance Manager") return 10;
		if (normalized_role == "Senior Finance Manager") return 11;
		return 0;
	}

	if (business_role_selection == 3) {
		if (normalized_role == "Junior General Trade Staff") return 1;
		if (normalized_role == "Senior General Trade Staff") return 2;
		if (normalized_role == "Junior Trainee Trade Staff") return 3;
		if (normalized_role == "Senior Trainee Trade Staff") return 4;
		if (normalized_role == "Junior Casual Trade Staff") return 5;
		if (normalized_role == "Senior Casual Trade Staff") return 6;
		if (normalized_role == "Junior Apprentice") return 7;
		if (normalized_role == "Senior Mentor") return 8;
		if (normalized_role == "Safety Officer") return 9;
		if (normalized_role == "Sales Representative") return 10;
		if (normalized_role == "Junior Trade Manager") return 11;
		if (normalized_role == "Senior Trade Manager") return 12;
		return 0;
	}

	return 0;
}

namespace {
std::filesystem::path Get_Username_Index_Path() {
	return Path_Utilities::Get_Employee_Records_Directory() / "Usernames.txt";
}

std::filesystem::path Get_Password_Index_Path() {
	return Path_Utilities::Get_Employee_Records_Directory() / "Passwords.txt";
}

std::filesystem::path Get_IRD_Username_Index_Path() {
	return Path_Utilities::Get_Employee_Records_Directory() / "IRD" / "IRD_Username.txt";
}

bool Remove_Username_From_Index(const std::string& username) {
	const std::string normalizedUsername = Trim_Copy(username);
	if (normalizedUsername.empty()) {
		return false;
	}

	std::ifstream inFile(Get_Username_Index_Path());
	if (!inFile.is_open()) {
		return false;
	}

	std::vector<std::string> usernames;
	std::string line;
	while (std::getline(inFile, line)) {
		const std::string value = Trim_Copy(line);
		if (value.empty() || value == normalizedUsername) {
			continue;
		}
		usernames.push_back(value);
	}

	std::ofstream outFile(Get_Username_Index_Path(), std::ios::trunc);
	if (!outFile.is_open()) {
		return false;
	}

	for (const std::string& value : usernames) {
		outFile << value << '\n';
	}

	return true;
}

bool Replace_IRD_Username_Index_Entry(const std::string& ird_number,
								  const std::string& existing_username,
								  const std::string& updated_username) {
	const std::string normalizedIrd = Trim_Copy(ird_number);
	const std::string normalizedExisting = Trim_Copy(existing_username);
	const std::string normalizedUpdated = Trim_Copy(updated_username);
	if (normalizedIrd.empty() || normalizedUpdated.empty()) {
		return false;
	}

	std::filesystem::create_directories(Get_IRD_Username_Index_Path().parent_path());

	std::vector<std::string> lines;
	bool replaced = false;
	{
		std::ifstream inFile(Get_IRD_Username_Index_Path());
		std::string line;
		while (std::getline(inFile, line)) {
			const std::string value = Trim_Copy(line);
			if (value.empty()) {
				continue;
			}

			const std::size_t separator = value.find(':');
			if (separator == std::string::npos) {
				lines.push_back(value);
				continue;
			}

			const std::string entryIrd = Trim_Copy(value.substr(0, separator));
			const std::string entryUsername = Trim_Copy(value.substr(separator + 1));
			if (!replaced && entryIrd == normalizedIrd && (normalizedExisting.empty() || entryUsername == normalizedExisting)) {
				lines.push_back(normalizedIrd + " : " + normalizedUpdated);
				replaced = true;
			} else {
				lines.push_back(value);
			}
		}
	}

	if (!replaced) {
		lines.push_back(normalizedIrd + " : " + normalizedUpdated);
	}

	std::ofstream outFile(Get_IRD_Username_Index_Path(), std::ios::trunc);
	if (!outFile.is_open()) {
		return false;
	}

	for (const std::string& value : lines) {
		outFile << value << '\n';
	}

	return true;
}

}

bool Username_Exists_In_Index(const std::string& username) {
	std::ifstream inFile(Get_Username_Index_Path());
	if (!inFile.is_open()) {
		return false;
	}

	std::string line;
	while (std::getline(inFile, line)) {
		if (Trim_Copy(line) == username) {
			return true;
		}
	}

	return false;
}

bool Password_Exists_In_Index(const std::string& password_hash) {
	const std::string maskedPassword = Mask_Password_Index_Entry(password_hash);
	std::ifstream inFile(Get_Password_Index_Path());
	if (!inFile.is_open()) {
		return false;
	}

	std::string line;
	while (std::getline(inFile, line)) {
		const std::string entry = Trim_Copy(line);
		if (entry == password_hash || entry == maskedPassword) {
			return true;
		}
	}

	return false;
}

bool IRD_Username_Exists_In_Index(const std::string& IRD_number, const std::string& username) {
	std::ifstream inFile(Get_IRD_Username_Index_Path());
	if (!inFile.is_open()) {
		return false;
	}

	std::string line;
	while (std::getline(inFile, line)) {
		if (Trim_Copy(line) == (IRD_number + " : " + username)) {
			return true;
		}
	}

	return false;
}

bool Save_Username_To_Index(const std::string& username) {
	const std::string normalizedUsername = Trim_Copy(username);
	if (normalizedUsername.empty()) {
		return false;
	}

	if (Username_Exists_In_Index(normalizedUsername)) {
		return true;
	}

	std::filesystem::create_directories(Get_Username_Index_Path().parent_path());
	std::ofstream outFile(Get_Username_Index_Path(), std::ios::app);
	if (!outFile.is_open()) {
		return false;
	}

	outFile << normalizedUsername << '\n';
	return true;
}


bool Save_Password_To_Index(const std::string& password_hash) {
	const std::string normalizedPassword = Trim_Copy(password_hash);
	const std::string maskedPassword = Mask_Password_Index_Entry(normalizedPassword);
	if (normalizedPassword.empty()) {
		return false;
	}

	std::filesystem::path passwordIndexPath = Get_Password_Index_Path();
	std::filesystem::create_directories(passwordIndexPath.parent_path());

	std::vector<std::string> lines;
	{
		std::ifstream inFile(passwordIndexPath);
		std::string line;
		while (std::getline(inFile, line)) {
			if (!Trim_Copy(line).empty()) {
				lines.push_back(Trim_Copy(line));
			}
		}
	}

	std::size_t insertPosition = lines.size();
	try {
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_int_distribution<std::size_t> distribution(0, lines.size());
		insertPosition = distribution(generator);
	} catch (...) {
		insertPosition = lines.size();
	}

	lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(insertPosition), maskedPassword);

	std::ofstream outFile(passwordIndexPath, std::ios::trunc);
	if (!outFile.is_open()) {
		return false;
	}

	for (const std::string& line : lines) {
		outFile << line << '\n';
	}

	return true;
}

bool Save_IRD_and_Username_To_Index(const std::string& IRD_number, const std::string& username) {
	const std::string normalizedIRD = Trim_Copy(IRD_number);
	const std::string normalizedUsername = Trim_Copy(username);
	if (normalizedIRD.empty() || normalizedUsername.empty()) {
		return false;
	}
	if (IRD_Username_Exists_In_Index(normalizedIRD, normalizedUsername)) {
		return true;
	}

	std::filesystem::path irdUsernameIndexPath = Get_IRD_Username_Index_Path();
	std::filesystem::create_directories(irdUsernameIndexPath.parent_path());

	std::vector<std::string> lines;
	{
		std::ifstream inFile(irdUsernameIndexPath);
		std::string line;
		while (std::getline(inFile, line)) {
			if (!Trim_Copy(line).empty()) {
				lines.push_back(Trim_Copy(line));
			}
		}
	}

	std::string newEntry = normalizedIRD + " : " + normalizedUsername;
	std::size_t insertPosition = lines.size();
	try {
		std::random_device randomDevice;
		std::mt19937 generator(randomDevice());
		std::uniform_int_distribution<std::size_t> distribution(0, lines.size());
		insertPosition = distribution(generator);
	} catch (...) {
		insertPosition = lines.size();
	}

	lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(insertPosition), newEntry);

	std::ofstream outFile(irdUsernameIndexPath, std::ios::trunc);
	if (!outFile.is_open()) {
		return false;
	}

	for (const std::string& line : lines) {
		outFile << line << '\n';
	}

	return true;
}

void Reset_Account_Record() {
	Reset_Account_Record_State();
}

void Save_Detail_To_File(const std::string& detail_name, const std::string& detail_value) {
	Save_Account_Record_Field(detail_name, detail_value);
}

void Save_Account_Type(const std::string& account_type) {
	Save_Detail_To_File("Account Type", account_type);
}

void Save_IRD_Number(const std::string& IRD_number) {
	Save_Detail_To_File("IRD Number", IRD_number);
}

void Save_First_Name(const std::string& first_name) {
	Save_Detail_To_File("First Name", first_name);
}

void Save_Middle_Name(const std::string& middle_name) {
	Save_Detail_To_File("Middle Name", middle_name);
}

void Save_Last_Name(const std::string& last_name) {
	Save_Detail_To_File("Last Name", last_name);
}

void Save_Date_of_Birth(const std::string& date_of_birth) {
	Save_Detail_To_File("Date of Birth", date_of_birth);
}

void Save_Address(const std::string& address) {
	Save_Detail_To_File("Address", address);
}

void Save_Email_Address(const std::string& email_address) {
	Save_Detail_To_File("Email Address", email_address);
}

void Save_Phone_Number(const std::string& phone_number) {
	Save_Detail_To_File("Phone Number", phone_number);
}

bool Save_Username_From_Creation(const std::string& finalized_username,
								 const std::string& account_type,
								 std::string& assigned_username) {
	(void)account_type;
	assigned_username = Trim_Copy(finalized_username);
	if (!Username_Rules(assigned_username)) {
		return false;
	}
	return true;
}

bool Save_Username_From_Role_Edit(const std::string& existing_username,
								  const std::string& account_type,
								  const std::string& first_name,
								  const std::string& middle_name,
								  const std::string& last_name,
								  const std::string& business_role,
								  const std::string& job_role,
								  const std::string& ird_number,
								  std::string& assigned_username) {
	(void)account_type;

	const int business_role_selection = Business_Role_To_Selection(business_role);
	if (business_role_selection == 0) {
		return false;
	}

	const int job_role_selection = Job_Role_To_Selection(business_role_selection, job_role);
	if (job_role_selection == 0) {
		return false;
	}

	const std::string generated_username = Generate_Role_Based_Username(business_role_selection,
											   job_role_selection,
											   first_name,
											   middle_name,
											   last_name);
	if (generated_username.empty()) {
		return false;
	}

	assigned_username = generated_username;
	if (!Username_Rules(assigned_username)) {
		return false;
	}

	const std::string normalizedExisting = Trim_Copy(existing_username);
	if (normalizedExisting != assigned_username) {
		if (!Remove_Username_From_Index(normalizedExisting)) {
			return false;
		}
		if (!Save_Username_To_Index(assigned_username)) {
			return false;
		}
		if (!Replace_IRD_Username_Index_Entry(ird_number, normalizedExisting, assigned_username)) {
			return false;
		}
	}

	return true;
}

void Save_Username(const std::string& username) {
	Save_Detail_To_File("Username", username);
	Save_Username_To_Index(username);
}

void Save_Password(const std::string& password) {
	const std::string hashedPassword = Password_Save(password);
	Save_Detail_To_File("Password", hashedPassword);
	Save_Password_To_Index(hashedPassword);
}
