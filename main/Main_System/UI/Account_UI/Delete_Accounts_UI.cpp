// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Delete_Accounts_UI.h"
#include "../Shared_UI_Messaging/Form_Labels_UI.h"

#include <iostream>

void Display_Delete_Accounts_Menu_UI() {
	std::cout << "Delete Existing Account:" << std::endl;
	std::cout << "1. Delete Account (requires secondary authentication)" << std::endl;
	std::cout << "2. " << Return_to_Account_Management_Menu_Label << std::endl;
	std::cout << Please_Select_an_Option_Label << " (1-2): ";
}

void Display_Secondary_Authentication_For_Deletion_Prompt_UI() {
	std::cout << "Deletion of Accounts Requires a Secondary Admins Login Credentials at the beginning and at the end." << std::endl;
	std::cout << "Please get a colleague with Secondary Admin status to do this with you for security reasons." << std::endl; // "Fake" status, Job_Role/Acount_Type dependant. 
	std::cout << "Reason for Deletion: ";
}

void Display_Delete_Account_Prompt() {
	std::cout << "Please Search for the Account you wish to Delete using the Username and IRD Number." << std::endl;
}

void Deletion_Confirmation_Prompt() {
	std::cout << "Are you sure you wish to delete the following account?" << std::endl;
	std::cout << Yes_No_Question_Input_Label;
}
