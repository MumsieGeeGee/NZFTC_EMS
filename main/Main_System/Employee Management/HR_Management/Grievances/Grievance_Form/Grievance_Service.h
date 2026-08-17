// Copyright (c) 2026 Georgia Kaye O'Sullivan,
// NZ Aotearoa. All rights reserved.

#ifndef GRIEVANCE_SERVICE_H
#define GRIEVANCE_SERVICE_H

#include "Grievance_Form_UI.h"

class Grievance_Service {
public:
    bool Validate_Form(const Grievance_Details& form);
    bool Save_Form(
        const Grievance_Details& form,
        const std::string& submitted_by,
        const std::string& submitted_for,
        std::string* request_id);
    void Display_Form(const Grievance_Details& form, const std::string& Employee_Name);
};

#endif
