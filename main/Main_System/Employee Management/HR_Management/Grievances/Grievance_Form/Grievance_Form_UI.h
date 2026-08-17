// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa.
// All rights reserved. Use prohibited except with explicit written permission.

#ifndef GRIEVANCE_FORM_UI_H
#define GRIEVANCE_FORM_UI_H

#include <string>

enum class Employee_Grievance_Applicant {
    Employee_Name, 
    Employee_Department,
    Employee_Job_Role
};

struct Declaration_Agreement {
    bool agreed;
    std::string I_Do;
    std::string Signed_Date;
};

struct Grievance_Details {
    std::string Employee_Attending_Supervisor;
    std::string Persons_Involved;
    std::string Event_Date;
    std::string Event_Details;
    std::string Affect_Details;
    std::string Suggestions;
    std::string Additional;
    Declaration_Agreement Declaration;
};

extern const std::string Employee_Grievance_Report_Form_Label;

// UI INPUT FUNCTIONS — these MUST return data
std::string Get_Attending_Supervisor_Name_UI();
std::string Get_Person_s_Involved_Details_UI();
std::string Get_Event_Date_UI();
std::string Get_Event_Details_UI();
std::string Get_Affect_Details_UI();
std::string Get_Suggestions_from_Applicant_UI();
std::string Get_Additional_Info_to_Consider_UI();
Declaration_Agreement Get_Declaration_Agreement_UI();

// DISPLAY FUNCTIONS
void Display_Entered_Details(const Grievance_Details& details);
void Display_of_Entered_Details_and_Confirmation_Prompt(const Grievance_Details& details);

#endif // GRIEVANCE_FORM_UI_H
