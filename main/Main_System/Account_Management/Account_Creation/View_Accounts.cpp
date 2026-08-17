// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "View_Accounts.h"
#include "Edit_Accounts.h"
#include "../Account_Access/Account_Database.h"
#include "../Account_Management/Input_Validation_Helpers.h"
#include "../../Employee Management/HR_Management/HR_Management.h"
#include "../../NZFTC_EMS/Session_Handling/Session_Handling.h"
#include "../../UI/Account_UI/View_Accounts_UI.h"
#include "../../UI/Account_UI/Edit_Accounts_UI.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/Success_Messages.h"
#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace {
std::vector<std::string> current_role_usernames;
std::string selected_username;

void Load_Role_Usernames(const std::string& prefix) {
    current_role_usernames = Get_Usernames_By_Prefix(prefix);
}

const std::array<std::string, 6> Employee_Detail_Fields = {
    Emergency_Contact_Label,
    Diagnosed_Health_Conditions_Label,
    Allergies_Label,
    Approved_Emergency_Steps_and_Medicines_Label,
    Prior_Training_and_Qualifications_Label,
    First_Aid_Certification_Label
};

void Display_Account_Update_Result_Error(Account_Update_Result update_result) {
    if (update_result == Account_Update_Result::User_Not_Found) {
        std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
	    std::cout << Display_Error << Username_Invalid << std::endl;
        return;
    }
    if (update_result == Account_Update_Result::Invalid_Input) {
        std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
        return;
    }
    if (update_result == Account_Update_Result::Missing_IRD_Mapping) {
        std::cout << Display_Error << IRD_Tax_Information_Not_Found << std::endl;
        return;
    }
    if (update_result == Account_Update_Result::Write_Failure ||
        update_result == Account_Update_Result::Index_Update_Failure) {
        std::cout << Display_Error << Account_Update_Failure << std::endl;
        return;
    }

    std::cout << Display_Error << Account_Update_Failure << std::endl;
}

bool Read_Employee_Detail_Value(const std::string& detail_label, std::string& value_out) {
    while (true) {
        Display_Employee_Detail_Input_Prompt_UI(detail_label);
        std::string input;
        std::getline(std::cin, input);
        input = Trim_Copy(input);

        if (input.empty()) {
            std::cout << Display_Error << Invalid_Empty_Input << std::endl;
            continue;
        }

        if (detail_label == First_Aid_Certification_Label) {
            if (!Yes_or_No(input)) {
                continue;
            }

            value_out = Is_Yes_Input(input) ? Certified_Label : Uncertified_Label;
            return true;
        }

        value_out = input;
        return true;
    }
}

bool Update_Employee_Detail_Field_For_User(const std::string& username,
                                           const std::string& detail_label) {
    std::string detail_value;
    if (!Read_Employee_Detail_Value(detail_label, detail_value)) {
        return false;
    }

    Account_Update_Result update_result = Account_Update_Result::Success;
    if (!Update_Account_Record_Field_For_Username(username, detail_label, detail_value, &update_result)) {
        Display_Account_Update_Result_Error(update_result);
        return false;
    }

    std::cout << Display_Success << Account_Update_Success_Message << std::endl;
    return true;
}

bool Update_All_Employee_Detail_Fields_For_User(const std::string& username) {
    for (const std::string& detail_label : Employee_Detail_Fields) {
        if (!Update_Employee_Detail_Field_For_User(username, detail_label)) {
            return false;
        }
    }
    return true;
}

std::string Get_Username_Role_Token(const std::string& username) {
    const std::size_t business_role_separator = username.find('_');
    if (business_role_separator == std::string::npos) {
        return "";
    }

    const std::size_t role_start = business_role_separator + 1;
    const std::size_t role_separator = username.find('_', role_start);
    if (role_separator == std::string::npos) {
        return "";
    }

    return username.substr(role_start, role_separator - role_start);
}

bool Current_User_Has_Full_Employee_Detail_Access() {
    const User_Session_Context& session = Get_Current_User_Session_Context();
    if (!session.authenticated) {
        return false;
    }

    const std::string role_token = Get_Username_Role_Token(session.username);
    return role_token == "hr" ||
           role_token == "jlm" ||
           role_token == "slm" ||
           role_token == "pr" ||
           role_token == "jfm" ||
           role_token == "sfm" ||
           role_token == "jtm" ||
           role_token == "stm";
}

bool Display_Employee_Details_Read_Only(const std::string& username, bool current_user) {
    std::map<std::string, std::string> employee_details;
    if (!Get_Account_Record_Details_For_Username(username, &employee_details)) {
        std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
        return false;
    }

    if (current_user) {
        Display_Current_Users_Employee_Details_UI(username, employee_details);
    } else {
        Display_Selected_Users_Employee_Details_UI(username, employee_details);
    }
    return true;
}

bool Edit_Employee_Details(const std::string& username, bool current_user, bool allow_tax_information) {
    while (true) {
        std::map<std::string, std::string> employee_details;
        if (!Get_Account_Record_Details_For_Username(username, &employee_details)) {
            std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
            return false;
        }

        if (current_user) {
            Display_Current_Users_Employee_Details_UI(username, employee_details);
        } else {
            Display_Selected_Users_Employee_Details_UI(username, employee_details);
        }

        if (allow_tax_information) {
            Display_Full_Employee_Details_Edit_Menu_UI();
        } else {
            Display_Basic_Employee_Details_Edit_Menu_UI();
        }

        int choice = 0;
        const int maximum_choice = allow_tax_information ? 4 : 3;
        if (!Get_Validated_Menu_Choice(1, maximum_choice, &choice)) {
            continue;
        }

        if (choice == 1) {
            Update_All_Employee_Detail_Fields_For_User(username);
            continue;
        }
        if (choice == 2) {
            Display_Employee_Details_Edit_Field_Menu_UI();
            int field_choice = 0;
            if (!Get_Validated_Menu_Choice(1, 7, &field_choice)) {
                continue;
            }

            if (field_choice == 7) {
                continue;
            }

            const std::string& selected_detail_label =
                Employee_Detail_Fields[static_cast<std::size_t>(field_choice - 1)];
            Update_Employee_Detail_Field_For_User(username, selected_detail_label);
            continue;
        }
        if (allow_tax_information && choice == 3) {
            Get_Edit_Selected_User_Tax_Information(username);
            continue;
        }

        return true;
    }
}

bool Resolve_Selected_Username_For_Employee_Details() {
    if (!Trim_Copy(selected_username).empty()) {
        return true;
    }

    Display_Employee_Detail_Input_Prompt_UI(Username_Label);
    std::string input_username;
    std::getline(std::cin, input_username);
    input_username = Trim_Copy(input_username);
    if (input_username.empty()) {
        std::cout << Display_Error << Username_Empty << std::endl;
        return false;
    }

    selected_username = input_username;
    return true;
}
}

void Get_Current_Users_Details_Menu_Options() {
    while (true) {
        int choice = 0;
        if (!Get_Validated_Menu_Choice(1, 2, &choice)) {
            continue;
        }

        if (choice == 1) {
            const User_Session_Context& session = Get_Current_User_Session_Context();
            if (!session.authenticated || Trim_Copy(session.username).empty()) {
                std::cout << Display_Error << Username_Empty << std::endl;
                continue;
            }

            Edit_Current_User_Account_Details(session.username);
            continue;
        }

        return;
    }
}

void Get_Selected_User_Details_Menu_Options() {
    while (true) {
        int choice = 0;
        if (!Get_Validated_Menu_Choice(1, 3, &choice)) {
            continue;
        }

        if (choice == 1) {
            Edit_Selected_User_Account_Details(selected_username);
            continue;
        }

        if (choice == 2) {
            const std::string username_to_delete = Trim_Copy(selected_username);
            if (username_to_delete.empty()) {
                std::cout << Display_Error << Username_Empty << std::endl;
                continue;
            }

            std::cout << "Are you sure you want to delete account '" << username_to_delete << "'? (Yes/No): ";
            std::string confirmation;
            std::getline(std::cin, confirmation);

            if (!Yes_or_No(confirmation)) {
                std::cout << Display_Error << Yes_No_Input << std::endl;
                continue;
            }

            if (Yes_Or_No_Input_to_String(confirmation) != "Yes") {
                continue;
            }

            const std::string deletion_ird_number = Get_IRD_Number_For_Username(username_to_delete);
            if (Trim_Copy(deletion_ird_number).empty()) {
                std::cout << Display_Error << IRD_Tax_Information_Not_Found << std::endl;
                continue;
            }

            if (!Delete_Account_And_IRD_Records_For_Username_and_IRD(username_to_delete, deletion_ird_number)) {
                std::cout << Display_Error << Account_Deletion_Failure << std::endl;
                continue;
            }

            std::cout << Display_Success << Account_Deletion_Success_Message << std::endl;
            selected_username.clear();
            return;
        }

        return;
    }
}

void Display_Personal_Details() {
    // Display the Current Users Personal Account Details
    const User_Session_Context& session = Get_Current_User_Session_Context();
    if (!session.authenticated || Trim_Copy(session.username).empty()) {
        std::cout << Display_Error << Username_Empty << std::endl;
        return;
    }

    Display_Current_User_Account_UI(session.username);
    Display_Details_Menu_Options_UI();
    Get_Current_Users_Details_Menu_Options();
}

void Display_Selected_Users_Account_Details() {
    // Display the Selected Users Account Details
    if (selected_username.empty()) {
        std::cout << Display_Error << Username_Empty << std::endl;
        return;
    }

    Display_Selected_User_Account_UI(selected_username);
    Display_Details_Menu_Options_UI();
    Get_Selected_User_Details_Menu_Options();
}

void Display_View_Admin_Accounts_UI() {
    Load_Role_Usernames("admin_");
    Display_Dynamic_Username_List_UI(current_role_usernames, Admin_Accounts_Label);
}

void Display_View_Finance_Accounts_UI() {
    Load_Role_Usernames("finance_");
    Display_Dynamic_Username_List_UI(current_role_usernames, Finance_Accounts_Label);
}

void Display_View_Trade_Accounts_UI () {
    Load_Role_Usernames("trade_");
    Display_Dynamic_Username_List_UI(current_role_usernames, Trade_Accounts_Label);
}

void Display_Account_Details_from_Selection() {
    Display_Selected_Users_Account_Details();
}

void Display_View_Accounts_Options_After_Business_Role_Selection_Menu_UI() {
    if (current_role_usernames.empty()) {
        std::cout << Display_Error << No_Accounts_Found << std::endl;
        return;
    }

    while (true) {
        Display_Account_Selection_Input_Prompt_UI(current_role_usernames.size());

        int choice = 0;
        if (!Get_Validated_Menu_Choice(1, static_cast<int>(current_role_usernames.size()), &choice)) {
            continue;
        }

        selected_username = current_role_usernames[static_cast<std::size_t>(choice - 1)];
        Display_Account_Details_from_Selection();
    }
}

void Get_View_All_Accounts_Menu_Options() {
    while (true) {
        int choice = 0;
        if (!Get_Validated_Menu_Choice(1, 4, &choice)) {
            continue;
        }

        if (choice == 1) {
            Display_View_Admin_Accounts_UI();
            Display_View_Accounts_Options_After_Business_Role_Selection_Menu_UI();
            continue;
        }
        if (choice == 2) {
            Display_View_Finance_Accounts_UI();
            Display_View_Accounts_Options_After_Business_Role_Selection_Menu_UI();
            continue;
        }
        if (choice == 3) {
            Display_View_Trade_Accounts_UI();
            Display_View_Accounts_Options_After_Business_Role_Selection_Menu_UI();
            continue;
        }
        if (choice == 4) {
            return;
        }
    }
}

void View_Current_Users_Employee_Details() {
    const User_Session_Context& session = Get_Current_User_Session_Context();
    if (!session.authenticated || Trim_Copy(session.username).empty()) {
        std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
		std::cout << Display_Error << Username_Invalid << std::endl;
        return;
    }

    Display_Employee_Details_Read_Only(session.username, true);
}

void Edit_Current_Users_Employee_Details() {
    const User_Session_Context& session = Get_Current_User_Session_Context();
    if (!session.authenticated || Trim_Copy(session.username).empty()) {
        std::cout << Display_Error << Account_Retrieval_Failure << std::endl;
        std::cout << Display_Error << Username_Invalid << std::endl;
        return;
    }

    Edit_Employee_Details(session.username, true, false);
}

void View_Selected_Users_Employee_Details() {
    if (!Resolve_Selected_Username_For_Employee_Details()) {
        std::cout << Display_Error << Employee_Record_Not_Found << std::endl;
        return;
    }

    Display_Employee_Details_Read_Only(selected_username, false);
}

void Edit_Selected_Users_Employee_Details() {
    if (!Current_User_Has_Full_Employee_Detail_Access()) {
        std::cout << Display_Error << Role_Type_Access_Failed << std::endl;
        return;
    }
    if (!Resolve_Selected_Username_For_Employee_Details()) {
        std::cout << Display_Error << Employee_Record_Not_Found << std::endl;
        return;
    }

    Edit_Employee_Details(selected_username, false, true);
}
