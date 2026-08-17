#include "Grievance_Service.h"
#include "../Requests/Grievance_Requests.h"
#include <iostream>

bool Grievance_Service::Validate_Form(const Grievance_Details& form) {

    if (form.Employee_Attending_Supervisor.empty()) {
        std::cout << "Error: Attending Supervisor is required.\n";
        return false;
    }

    if (form.Persons_Involved.empty()) {
        std::cout << "Error: Persons involved must be described.\n";
        return false;
    }

    if (form.Event_Date.empty()) {
        std::cout << "Error: Event date is required.\n";
        return false;
    }

    if (form.Event_Details.empty()) {
        std::cout << "Error: Event details are required.\n";
        return false;
    }

    if (!form.Declaration.agreed) {
        std::cout << "Error: You must type 'I do' to submit this complaint.\n";
        return false;
    }

    if (form.Declaration.Signed_Date.empty()) {
        std::cout << "Error: Signed date is required.\n";
        return false;
    }

    return true;
}

bool Grievance_Service::Save_Form(
        const Grievance_Details& form,
        const std::string& submitted_by,
        const std::string& submitted_for,
        std::string* request_id) {
    return Save_Grievance_Request(
        form, submitted_by, submitted_for, request_id);
}

void Grievance_Service::Display_Form(const Grievance_Details& form, const std::string& Employee_Name) {
    std::cout << "\n--- Complaint Summary ---\n";
    std::cout << Employee_Name << " has submitted a complaint.\n";
    std::cout << "Attending Supervisor: " << form.Employee_Attending_Supervisor << "\n";
    std::cout << "Persons Involved: " << form.Persons_Involved << "\n";
    std::cout << "Event Date: " << form.Event_Date << "\n";
    std::cout << "Event Details: " << form.Event_Details << "\n";
    std::cout << "Affect Details: " << form.Affect_Details << "\n";
    std::cout << "Suggestions: " << form.Suggestions << "\n";
    std::cout << "Additional Comments: " << form.Additional << "\n";
    std::cout << "Declaration: " << form.Declaration.I_Do << "\n";
    std::cout << "Signed Date: " << form.Declaration.Signed_Date << "\n";
}
