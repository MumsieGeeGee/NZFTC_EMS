// Copyright (c) 2026 Georgia Kaye O'Sullivan,
// NZ Aotearoa. All rights reserved.

#include "Grievance_Form_UI.h"
#include "Grievance_Service.h"
#include "../../../../Account_Management/Account_Access/Account_Database.h"
#include "../../../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../../../../NZFTC_EMS/Session_Handling/Session_Handling.h"
#include "../../../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../../../UI/Shared_UI_Messaging/Success_Messages.h"
#include <iostream>
#include <map>

void Start_Grievance_Workflow() {
    const User_Session_Context& session = Get_Current_User_Session_Context();
    if (!session.authenticated || Trim_Copy(session.username).empty()) {
        std::cout << Display_Error << Request_Save_Failure_Message << std::endl;
        return;
    }
    Grievance_Details form;

    std::cout << "\n--- Begin Employee Complaint Form ---\n\n";

    form.Employee_Attending_Supervisor = Get_Attending_Supervisor_Name_UI();
    form.Persons_Involved = Get_Person_s_Involved_Details_UI();
    form.Event_Date = Get_Event_Date_UI();
    form.Event_Details = Get_Event_Details_UI();
    form.Affect_Details = Get_Affect_Details_UI();
    form.Suggestions = Get_Suggestions_from_Applicant_UI();
    form.Additional = Get_Additional_Info_to_Consider_UI();
    form.Declaration = Get_Declaration_Agreement_UI();

    Grievance_Service service;

    std::cout << "Validating Complaint Form \n";

    if (!service.Validate_Form(form)) {
        std::cout << "\nForm validation failed. Please correct the errors and try again.\n";
        return;
    }

    std::cout << "Saving Complaint Form \n";
    std::string request_id;
    if (!service.Save_Form(
            form,
            session.username,
            session.username,
            &request_id)) {
        std::cout << Display_Error << Request_Save_Failure_Message << std::endl;
        return;
    }

    std::cout << "Displaying Saved Complaint \n";
    std::map<std::string, std::string> employee_details;
    std::string employee_name = session.username;
    if (Get_Account_Record_Details_For_Username(
            session.username, &employee_details)) {
        employee_name = Trim_Copy(
            employee_details["First Name"] + " " +
            employee_details["Middle Name"] + " " +
            employee_details["Last Name"]);
    }
    service.Display_Form(form, employee_name);

    std::cout << "Request ID: " << request_id << std::endl;
    std::cout << Display_Success
              << Request_Submission_Success_Message << std::endl;
}
