// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Create_Account.h"
#include "../../Account_Form/Account_Creation_Form_UI.h"
#include "../Account_Management/Save_Account_Details.h"
#include "../Account_Access/Account_Database.h"
#include "../Account_Management/Security_Protocols.h"
#include "../Account_Management/Input_Validation_Helpers.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/Shared_UI_Messaging/Info_Messages.h"
#include "../../UI/Shared_UI_Messaging/Success_Messages.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include "../../UI/Employee_Management_Menu_UI.h"
#include "../../Tax Calculator/Components/IRD_PAYE.h"
#include <string>
#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>



std::string Account_Type_To_String(Account_Type account_type) {
    switch (account_type) {
        case Account_Type::Admin:
            return "Admin";
        case Account_Type::User:
            return "Employee";
    }
    return "Unknown";
}

void Account_Type_Options(Account_Type& choice) {
    Display_Account_Access_Type_Options_UI();
    while (true) {
        int input;
        if (!(std::cin >> input)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << Display_Error << Invalid_Account_Type << std::endl;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (input) {
            case 1:
                choice = Account_Type::Admin;
                return;
            case 2:
                choice = Account_Type::User;
                return;
            default:
                std::cout << Display_Error << Invalid_Account_Type << std::endl;
                break;
        }
    }
}

void Get_Account_Type(Account_Type& choice) {
    Account_Type_Options(choice);
}

std::string Business_Role_To_String(int business_role_selection) {
    if (business_role_selection == 1) {
        return "Admin";
    }
    if (business_role_selection == 2) {
        return "Finance";
    }
    if (business_role_selection == 3) {
        return "Trade";
    }
    return "";
}

bool Get_Business_Role_Selection(int& business_role_selection) {
    while (true) {
        Display_Employee_Role_Question_UI();
        int input = 0;
        if (!(std::cin >> input)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl; 
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (input >= 1 && input <= 3) {
            business_role_selection = input;
            return true;
        }
        std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
    }
}

std::string Job_Role_To_String(int business_role_selection, int job_role_selection) {
    if (business_role_selection == 1) {
        switch (job_role_selection) {
            case 1: return "Junior General Administrator";
            case 2: return "Senior General Administrator";
            case 3: return "Junior Trainee Administrator";
            case 4: return "Senior Trainee Administrator";
            case 5: return "Junior Casual Administrator";
            case 6: return "Senior Casual Administrator";
            case 7: return "Receptionist";
            case 8: return "Call Centre Operator";
            case 9: return "Executive Assistant";
            case 10: return "Personal Assistant";
            case 11: return "Human Resources Administrator";
            case 12: return "Junior IT Support Administrator";
            case 13: return "Senior IT Support Administrator";
            case 14: return "Junior Level Manager";
            case 15: return "Senior Level Manager";
            default: return "";
        }
    }

    if (business_role_selection == 2) {
        switch (job_role_selection) {
            case 1: return "Junior General Finance Staff";
            case 2: return "Senior General Finance Staff";
            case 3: return "Junior Trainee Finance Staff";
            case 4: return "Senior Trainee Finance Staff";
            case 5: return "Junior Casual Finance Staff";
            case 6: return "Senior Casual Finance Staff";
            case 7: return "Payroll Administrator";
            case 8: return "Accounts Payable Administrator";
            case 9: return "Accounts Receivable Administrator";
            case 10: return "Junior Finance Manager";
            case 11: return "Senior Finance Manager";
            default: return "";
        }
    }

    if (business_role_selection == 3) {
        switch (job_role_selection) {
            case 1: return "Junior General Trade Staff";
            case 2: return "Senior General Trade Staff";
            case 3: return "Junior Trainee Trade Staff";
            case 4: return "Senior Trainee Trade Staff";
            case 5: return "Junior Casual Trade Staff";
            case 6: return "Senior Casual Trade Staff";
            case 7: return "Junior Apprentice";
            case 8: return "Senior Mentor";
            case 9: return "Safety Officer";
            case 10: return "Sales Representative";
            case 11: return "Junior Trade Manager";
            case 12: return "Senior Trade Manager";
            default: return "";
        }
    }

    return "";
}

bool Account_Type_Allows_Job_Role(Account_Type account_type,
                                  int business_role_selection,
                                  int job_role_selection) {
    bool requires_admin_account = false;
    if (business_role_selection == 1) {
        requires_admin_account = job_role_selection >= 9;
    } else if (business_role_selection == 2) {
        requires_admin_account = job_role_selection == 7 ||
                                 job_role_selection >= 10;
    } else if (business_role_selection == 3) {
        requires_admin_account = job_role_selection == 8 ||
                                 job_role_selection == 9 ||
                                 job_role_selection >= 11;
    }

    return requires_admin_account
        ? account_type == Account_Type::Admin
        : account_type == Account_Type::User;
}

bool Get_Job_Role_Selection(int business_role_selection, int& job_role_selection) {
    while (true) {
        if (business_role_selection == 1) {
            Display_Employee_Admin_Role_Question_UI();
        } else if (business_role_selection == 2) {
            Display_Employee_Finance_Role_Question_UI();
        } else if (business_role_selection == 3) {
            Display_Employee_Trade_Role_Question_UI();
        } else {
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            return false;
        }

        int input = 0;
        if (!(std::cin >> input)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (business_role_selection == 1) {
            if (input >= 1 && input <= 15) {
                job_role_selection = input;
                return true;
            }
            std::cout << Display_Error << Invalid_Admin_Role << std::endl;
            continue;
        }

        if (business_role_selection == 2) {
            if (input >= 1 && input <= 11) {
                job_role_selection = input;
                return true;
            }
            std::cout << Display_Error << Invalid_Finance_Role << std::endl;
            continue;
        }

        if (input >= 1 && input <= 12) {
            job_role_selection = input;
            return true;
        }
        std::cout << Display_Error << Invalid_Trade_Role << std::endl;
    }
}

void Get_First_Name(std::string& first_name) {
    std::cout << First_Name_Label << ": ";
    std::getline(std::cin, first_name);
    if (first_name.empty()) {
        std::cout << Display_Error << First_Name_Empty << std::endl;
        Get_First_Name(first_name);
        return;
    }
    if (first_name.length() > 50) {
        std::cout << Display_Error << First_Name_Too_Long << std::endl;
        Get_First_Name(first_name);
        return;
    }
    if (!std::all_of(first_name.begin(), first_name.end(), [](char c) { return std::isalpha(c) || std::isspace(c) || c == '\'' || c == '-'; })) {
        std::cout << Display_Error << First_Name_Invalid_Characters << std::endl;
        Get_First_Name(first_name);
        return;
    }

}

void Get_Middle_Name(std::string& middle_name) {
    std::cout << Middle_Name_Label << ": ";
    std::getline(std::cin, middle_name);
    const std::string normalized_middle_name = Trim_Copy(middle_name);
    if (normalized_middle_name == "N/A" || normalized_middle_name == "n/a" || normalized_middle_name == "N/a" || normalized_middle_name == "n/A") {
        middle_name = "";
        return;
    }
    if (middle_name.empty()) {
        std::cout << Display_Error << Middle_Name_Empty << std::endl;
        Get_Middle_Name(middle_name);
        return;
    }
    if (middle_name.length() > 50) {
        std::cout << Display_Error << Middle_Name_Too_Long << std::endl;
        Get_Middle_Name(middle_name);
        return;
    }
    if (!std::all_of(middle_name.begin(), middle_name.end(), [](char c) { return std::isalpha(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c)) || c == '\'' || c == '-'; })) {
        std::cout << Display_Error << Middle_Name_Invalid_Characters << std::endl;
        Get_Middle_Name(middle_name);
        return;
    }

}

void Get_Surname(std::string& surname) {
    std::cout << Last_Name_Label << ": ";
    std::getline(std::cin, surname);
    if (surname.empty()) {
        std::cout << Display_Error << Surname_Empty << std::endl;
        Get_Surname(surname);
        return;
    }
    if (surname.length() > 50) {
        std::cout << Display_Error << Surname_Too_Long << std::endl;
        Get_Surname(surname);
        return;
    }
    if (!std::all_of(surname.begin(), surname.end(), [](char c) { return std::isalpha(c) || std::isspace(c) || c == '/' || c == '\'' || c == '-'; })) {
        std::cout << Display_Error << Surname_Invalid_Characters << std::endl;
        Get_Surname(surname);
        return;
    }

}

void Get_Date_of_Birth(std::string& date_of_birth) {
    std::cout << Date_of_Birth_Label << ": ";
    std::getline(std::cin, date_of_birth);
    if (date_of_birth.empty()) {
        std::cout << Display_Error << DOB_Empty << std::endl;
        Get_Date_of_Birth(date_of_birth);
        return;
    }
    if (!Is_Date_Format(date_of_birth)) {
        Get_Date_of_Birth(date_of_birth);
        return;
    }
    const std::string year_value = date_of_birth.substr(6, 4);
    if (!Is_Valid_DOB_Year(year_value)) {
        Get_Date_of_Birth(date_of_birth);
        return;
    }

}

void Get_House_Number(std::string& house_number) {
    std::cout << House_Number_Label << ": ";
    std::getline(std::cin, house_number);
    if (house_number.empty()) {
        std::cout << Display_Error << House_Number_Empty << std::endl;
        Get_House_Number(house_number);
        return;
    }
    if (house_number.length() > 10) {
        std::cout << Display_Error << House_Number_Too_Long << std::endl;
        Get_House_Number(house_number);
        return;
    }
    if (!std::all_of(house_number.begin(), house_number.end(), [](char c) { return std::isalnum(c) || c == '/'; })) {
        std::cout << Display_Error << House_Number_Invalid_Characters << std::endl;
        Get_House_Number(house_number);
        return;
    }
}

void Get_Street_Name(std::string& street_name) {
    std::cout << Street_Name_Label << ": ";
    std::getline(std::cin, street_name);
    if (street_name.empty()) {
        std::cout << Display_Error << Street_Name_Empty << std::endl;
        Get_Street_Name(street_name);
        return;
    }
    if (street_name.length() > 50) {
        std::cout << Display_Error << Street_Name_Too_Long << std::endl;
        Get_Street_Name(street_name);
        return;
    }
    if (!std::all_of(street_name.begin(), street_name.end(), [](char c) { return std::isalpha(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c)); })) {
        std::cout << Display_Error << Street_Name_Invalid_Characters << std::endl;
        Get_Street_Name(street_name);
        return;
    }
}

void Get_Suburb(std::string& suburb) {
    std::cout << Suburb_Label << ": ";
    std::getline(std::cin, suburb);
    if (suburb.empty()) {
        std::cout << Display_Error << Suburb_Empty << std::endl;
        Get_Suburb(suburb);
        return;
    }
    if (suburb.length() > 50) {
        std::cout << Display_Error << Suburb_Too_Long << std::endl;
        Get_Suburb(suburb);
        return;
    }
    if (!std::all_of(suburb.begin(), suburb.end(), [](char c) { return std::isalpha(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c)); })) {
        std::cout << Display_Error << Suburb_Invalid_Characters << std::endl;
        Get_Suburb(suburb);
        return;
    }
}

void Get_City(std::string& city) {
    std::cout << City_Label << ": ";
    std::getline(std::cin, city);
    if (city.empty()) {
        std::cout << Display_Error << City_Empty << std::endl;
        Get_City(city);
        return;
    }
    if (city.length() > 50) {
        std::cout << Display_Error << City_Too_Long << std::endl;
        Get_City(city);
        return;
    }
    if (!std::all_of(city.begin(), city.end(), [](char c) { return std::isalpha(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c)); })) {
        std::cout << Display_Error << City_Invalid_Characters << std::endl;
        Get_City(city);
        return;
    }
}

void Get_Postcode(std::string& postcode) {
    std::cout << Postcode_Label << ": ";
    std::getline(std::cin, postcode);
    if (postcode.empty()) {
        std::cout << Display_Error << Postcode_Empty << std::endl;
        Get_Postcode(postcode);
        return;
    }
    if (postcode.length() > 4) {
        std::cout << Display_Error << Postcode_Incorrect_Length << std::endl;
        Get_Postcode(postcode);
        return;
    }
    if (postcode.length() < 4) {
        std::cout << Display_Error << Postcode_Incorrect_Length << std::endl;
        Get_Postcode(postcode);
        return;
    }
    if (!std::all_of(postcode.begin(), postcode.end(), [](char c) { return std::isdigit(c) || std::isalpha(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c)) || c == '/' || c == '\'' || c == '-'; })) {
        std::cout << Display_Error << Postcode_Invalid_Characters << std::endl;
        Get_Postcode(postcode);
        return;
    }
}

void Get_Post_Box(std::string& post_box) {
    std::cout << Post_Box_Label << ": ";
    std::getline(std::cin, post_box);
    const std::string normalized_post_box = Trim_Copy(post_box);
    if (normalized_post_box == "None" || normalized_post_box == "none" || normalized_post_box == "NONE" || normalized_post_box == "None") {
        post_box = "None";
        return;
    }
    if (post_box.empty()) {
        std::cout << Display_Error << Po_Box_Empty << std::endl;
        Get_Post_Box(post_box);
        return;
    }
    if (post_box.length() > 50) {
        std::cout << Display_Error << Po_Box_Too_Long << std::endl;
        Get_Post_Box(post_box);
        return;
    }
    if (!std::all_of(post_box.begin(), post_box.end(), [](char c) { return std::isdigit(c); })) {
        std::cout << Display_Error << Po_Box_Invalid_Characters << std::endl;
        Get_Post_Box(post_box);
        return;
    }

}

void Get_Phone_Number(std::string& phone_number) {
    std::cout << Phone_Number_Label << ": ";
    std::getline(std::cin, phone_number);
    phone_number = Trim_Copy(phone_number);
    if (phone_number.empty()) {
        std::cout << Display_Error << Phone_Number_Empty << std::endl;
        Get_Phone_Number(phone_number);
        return;
    }
    if (phone_number.length() > 20) {
        std::cout << Display_Error << Phone_Number_Too_Long << std::endl;
        Get_Phone_Number(phone_number);
        return;
    }
    if (!std::all_of(phone_number.begin(), phone_number.end(), [](char c) {
        return std::isdigit(static_cast<unsigned char>(c)) || c == ' ' || c == '-';
    })) {
        std::cout << Display_Error << Phone_Number_Invalid_Characters << std::endl;
        Get_Phone_Number(phone_number);
        return;
    }
    if (!std::any_of(phone_number.begin(), phone_number.end(), [](char c) { return std::isdigit(static_cast<unsigned char>(c)); })) {
        std::cout << Display_Error << Phone_Number_Invalid_Characters << std::endl;
        Get_Phone_Number(phone_number);
        return;
    }

}

void Get_Email_Address(std::string& email_address) {
    std::cout << Email_Address_Label << ": ";
    std::getline(std::cin, email_address);
    if (email_address.empty()) {
        std::cout << Display_Error << Email_Address_Empty << std::endl;
        Get_Email_Address(email_address);
        return;
    }
    if (email_address.length() > 100) {
        std::cout << Display_Error << Email_Address_Too_Long << std::endl;
        Get_Email_Address(email_address);
        return;
    }
    if (email_address.find('@') == std::string::npos || email_address.find('.') == std::string::npos) {
        std::cout << Display_Error << Email_Address_Invalid_Format << std::endl;
        Get_Email_Address(email_address);
        return;
    }

}

void Get_Full_Address(std::string& address) {
    std::string house_number, street_name, suburb, city, postcode, post_box;
    Get_House_Number(house_number);
    Get_Street_Name(street_name);
    Get_Suburb(suburb);
    Get_City(city);
    Get_Postcode(postcode);
    Get_Post_Box(post_box);

    address = house_number + " " + street_name + ", " + suburb + ", " + city + ", " + postcode + ", PO Box: " + post_box;
}

void Get_Continue_Account_Creation_Question_Options() {
    while (true) {
        int input;
        std::cin >> input;
        if (input == 1) {
            Create_Account();
            return;
        }
        if (input == 2) {
            std::cout << Display_Info << Account_Creation_Cancelled << std::endl;
            std::cout << Display_Info << Returning_to_Account_Management_Main_Menu << std::endl;
            return;
        }
        std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
    }
}

void Get_IRD_Number(std::string& IRD_Number) {
    std::cout << IRD_Number_Label << ": ";
    std::getline(std::cin, IRD_Number);
    if (IRD_Number.empty()) {
        std::cout << Display_Error << IRD_Number_Empty << std::endl;
        Get_IRD_Number(IRD_Number);
        return;
    }
    if (IRD_Number.length() > 9) {
        std::cout << Display_Error << IRD_Number_Too_Long << std::endl;
        Get_IRD_Number(IRD_Number);
        return;
    }
    if (!std::all_of(IRD_Number.begin(), IRD_Number.end(), [](char c) { return std::isdigit(c); })) {
        std::cout << Display_Error << IRD_Number_Invalid_Characters << std::endl;
        Get_IRD_Number(IRD_Number);
        return;
    }
    if (!Is_IRD_Number_Format(IRD_Number)) {
        std::cout << Display_Error << IRD_Number_Invalid_Format << std::endl;
        Get_IRD_Number(IRD_Number);
        return;
    }
}

bool Get_Username(std::string& username,
                  Account_Type account_type,
                  const std::string& first_name,
                  const std::string& middle_name,
                  const std::string& last_name,
                  std::string& business_role,
                  std::string& job_role) {
    while (true) {
        int business_role_selection = 0;
        if (!Get_Business_Role_Selection(business_role_selection)) {
            continue;
        }

        int job_role_selection = 0;
        if (!Get_Job_Role_Selection(business_role_selection, job_role_selection)) {
            continue;
        }
        if (!Account_Type_Allows_Job_Role(account_type, business_role_selection, job_role_selection)) {
            std::cout << Display_Error << Invalid_Account_Type << std::endl;
            continue;
        }

        const std::string generated_username = Generate_Role_Based_Username(business_role_selection,
                                             job_role_selection,
                                             first_name,
                                             middle_name,
                                             last_name);
        if (generated_username.empty()) {
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            continue;
        }

        const std::string business_role_value = Business_Role_To_String(business_role_selection);
        const std::string job_role_value = Job_Role_To_String(business_role_selection, job_role_selection);
        if (business_role_value.empty() || job_role_value.empty()) {
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
            continue;
        }

        std::string assignedUsername;
        if (Save_Username_From_Creation(generated_username,
                                        Account_Type_To_String(account_type),
                                        assignedUsername)) {
            username = assignedUsername;
            business_role = business_role_value;
            job_role = job_role_value;
            return true;
        }
    }
}

void Get_Password(std::string& password) {
    Password_Rules_Full_UI();
    std::cout << Password_Label << ": ";
    password = Read_Password_Input_From_Console();
    Password_Making_Rules(password, "", "", "", "", "", "", "", "");
    Password_Rules(password);
}

bool Get_Required_Role_Assignment(
        const std::string& business_role,
        const std::string& job_role,
        std::string& assignment_field,
        std::string& assigned_username,
        const std::string& username_to_exclude) {
    assignment_field.clear();
    assigned_username.clear();

    std::string question;
    std::string candidate_business_role;
    std::vector<std::string> candidate_job_roles;
    if (job_role == Admin_Executive_Assistant) {
        assignment_field = "Main Manager";
        question = "Who is your main Manager?";
        candidate_job_roles = {
            "Senior Level Manager",
            Senior_Finance_Manager,
            Senior_Trade_Manager
        };
    } else if (job_role == Admin_Personal_Assistant) {
        assignment_field = "Assigned Manager";
        question = "Who is your assigned Manager?";
        candidate_job_roles = {
            "Junior Level Manager",
            Junior_Finance_Manager,
            Junior_Trade_Manager
        };
    } else if (business_role == Role_Type_Label_Trade &&
               job_role == Junior_Apprentice) {
        assignment_field = "Assigned Mentor";
        question = "Who is your assigned Mentor?";
        candidate_business_role = Role_Type_Label_Trade;
        candidate_job_roles = {Senior_Mentor};
    } else {
        return true;
    }

    const std::vector<Role_Assignment_Option> available_candidates =
        Get_Role_Assignment_Options(candidate_job_roles, candidate_business_role);
    std::vector<Role_Assignment_Option> candidates;
    for (const Role_Assignment_Option& candidate : available_candidates) {
        if (candidate.username != username_to_exclude) {
            candidates.push_back(candidate);
        }
    }
    if (candidates.empty()) {
        std::cout << Display_Error << No_Accounts_Found << std::endl;
        return false;
    }

    std::vector<std::string> candidate_labels;
    for (const Role_Assignment_Option& candidate : candidates) {
        candidate_labels.push_back(
            candidate.display_name + " - " + candidate.job_role +
            " (" + candidate.business_role + ")");
    }
    Display_Role_Assignment_Options_UI(question, candidate_labels);

    int selection = 0;
    if (!Get_Validated_Menu_Choice(
            1, static_cast<int>(candidates.size()), &selection)) {
        return false;
    }

    assigned_username =
        candidates[static_cast<std::size_t>(selection - 1)].username;
    return true;
}

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
                          const std::string& password) {
    Save_Account_Type(account_type);
    Save_IRD_Number(IRD_number);
    Save_First_Name(first_name);
    Save_Middle_Name(middle_name);
    Save_Last_Name(last_name);
    Save_Date_of_Birth(date_of_birth);
    Save_Detail_To_File("Business Role", business_role);
    Save_Detail_To_File("Job Role", job_role);
    if (!assignment_field.empty() && !assigned_username.empty()) {
        Save_Detail_To_File(assignment_field, assigned_username);
    }
    Save_Address(address);
    Save_Email_Address(email_address);
    Save_Phone_Number(phone_number);
    Save_Username(username);
    Save_Password(password);
    if (Save_IRD_and_Username_To_Index(IRD_number, username)) {
        std::cout << Display_Success << Account_Creation_Success << std::endl;
    } else {
        std::cout << Display_Error << Account_Creation_Failure << std::endl;
    }
    Reset_Account_Record();
}

void Create_Account() {
    Reset_Account_Record();
    Account_Type account_type = Account_Type::User;
    std::string first_name;
    std::string middle_name;
    std::string last_name;
    std::string date_of_birth;
    std::string address;
    std::string email_address;
    std::string phone_number;
    std::string IRD_number;
    std::string business_role;
    std::string job_role;
    std::string assignment_field;
    std::string assigned_username;
    std::string username;
    std::string password;

    Get_Account_Type(account_type);
    Get_First_Name(first_name);
    Get_Middle_Name(middle_name);
    Get_Surname(last_name);
    Get_Date_of_Birth(date_of_birth);
    Get_Full_Address(address);
    Get_Email_Address(email_address);
    Get_Phone_Number(phone_number);
    Get_IRD_Number(IRD_number);
    if (!Get_Username(username, account_type, first_name, middle_name, last_name, business_role, job_role)) {
        return;
    }
    if (!Get_Required_Role_Assignment(
            business_role, job_role, assignment_field, assigned_username)) {
        return;
    }
    Get_Password(password);
    std::cout << Display_Info << Finalizing_Account_Creation_Message << std::endl;

    Save_Created_Account(Account_Type_To_String(account_type),
                         first_name,
                         middle_name,
                         last_name,
                         date_of_birth,
                         IRD_number,
                         business_role,
                         job_role,
                         assignment_field,
                         assigned_username,
                         address,
                         email_address,
                         phone_number,
                         username,
                         password);
}
