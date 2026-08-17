// Copyright (c) 2026 Georgia Kaye O'Sullivan,
// NZ Aotearoa. All rights reserved.
// Use prohibited except with explicit written permission.

#include "Grievance_Form_UI.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

const std::string Employee_Grievance_Report_Form_Label = "NZFTC Employee Complaint Form";

std::string Get_Attending_Supervisor_Name_UI() {
    std::cout << "What is the name of the 'Attending' Supervisor?\nName: ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::string Get_Person_s_Involved_Details_UI() {
    std::cout << "Please describe in as much detail as possible all persons involved.\nWho was/is involved?: ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::string Get_Event_Date_UI() {
    std::cout << "What date (earliest to your knowledge) did these events occur/begin to occur?\nDate: ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::string Get_Event_Details_UI() {
    std::cout << "Please describe in as much detail as possible the events/details of the complaint.\nEvent/Complaint Details: ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::string Get_Affect_Details_UI() {
    std::cout << "Please describe how the actions/events you complain about have affected you, and your ability to work.\nAffect on Job/State of Mind Details: ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::string Get_Suggestions_from_Applicant_UI() {
    std::cout << "Please describe any positive suggestions you believe can help resolve your complaint.\n"
              << "It is OK if you cannot think of anything, just leave this section blank.\nSuggestions: ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::string Get_Additional_Info_to_Consider_UI() {
    std::cout << "Please provide any additional comments you wish the company to consider when investigating your complaint,\n"
              << "OR if you have any questions you wish to be answered please leave them here.\n"
              << "It is OK if you cannot think of anything, just leave this section blank.\nAdditional Comments/Questions: ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

Declaration_Agreement Get_Declaration_Agreement_UI() {
    Declaration_Agreement have_agreed;

    std::cout << "I declare that the facts set forth in this complaint are true and correct to the best of my knowledge.\n"
              << "I declare that these details are accurate and pursuant to the penalty of perjury under the laws of this country.\n"
              << "Enter 'I do' Here: ";

    std::getline(std::cin, have_agreed.I_Do);
    const std::size_t first =
        have_agreed.I_Do.find_first_not_of(" \t\r\n");
    const std::size_t last =
        have_agreed.I_Do.find_last_not_of(" \t\r\n");
    std::string normalized_declaration =
        first == std::string::npos
            ? ""
            : have_agreed.I_Do.substr(first, last - first + 1);
    std::transform(
        normalized_declaration.begin(),
        normalized_declaration.end(),
        normalized_declaration.begin(),
        [](unsigned char character) {
            return static_cast<char>(std::tolower(character));
        });
    have_agreed.agreed = normalized_declaration == "i do";
    if (have_agreed.agreed) {
        have_agreed.I_Do = "I do";
    }

    std::cout << "Please enter today's date.\nDate Signed: ";
    std::getline(std::cin, have_agreed.Signed_Date);

    return have_agreed;
}

void Display_Entered_Details(const Grievance_Details& details) {
    std::cout << "\n--- Complaint Summary ---\n";
    std::cout << "Attending Supervisor: " << details.Employee_Attending_Supervisor << "\n";
    std::cout << "Persons Involved: " << details.Persons_Involved << "\n";
    std::cout << "Event Date: " << details.Event_Date << "\n";
    std::cout << "Event Details: " << details.Event_Details << "\n";
    std::cout << "Affect Details: " << details.Affect_Details << "\n";
    std::cout << "Suggestions: " << details.Suggestions << "\n";
    std::cout << "Additional Comments: " << details.Additional << "\n";
    std::cout << "Declaration Agreement: " << details.Declaration.I_Do << "\n";
    std::cout << "Signed Date: " << details.Declaration.Signed_Date << "\n";
}

void Display_of_Entered_Details_and_Confirmation_Prompt(const Grievance_Details& details) {
    Display_Entered_Details(details);

    if (!details.Declaration.agreed) {
        std::cout << "\nYou must type 'I do' to submit this complaint.\n";
    } else {
        std::cout << "\nYour complaint has been recorded.\n";
    }
}
