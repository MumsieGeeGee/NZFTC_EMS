// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef HR_MANAGEMENT_UI_H
#define HR_MANAGEMENT_UI_H

struct Leave_and_Holiday_Entitlements_Details;
struct Tax_Information_Record_Details;

#include <string>

void Display_Generate_Payslip_Form_UI(const std::string& employee_name);
void Display_Current_User_Payslips_UI();
void Display_Selected_User_Payslips_UI();
void Display_Request_Payslip_Review_UI();
void Personal_Payslip_Review_Request_UI();
void On_Behalf_of_User_Payslip_Review_Request_UI();

void Display_Current_User_Leave_and_Holiday_Entitlements_UI(const Leave_and_Holiday_Entitlements_Details& details);
void Display_Selected_User_Leave_and_Holiday_Entitlements_UI(const Leave_and_Holiday_Entitlements_Details& details);
void Display_View_Current_User_Open_Leave_Requests_UI();
void Display_View_Selected_User_Open_Leave_Requests_UI();
void Display_View_Current_User_Resolved_Leave_Requests_UI();
void Display_Submit_Leave_Request_UI();
void Personal_Submit_Leave_Request_UI();
void On_Behalf_of_User_Submit_Leave_Request_UI();

void Display_View_Current_User_Open_Grievance_Report_UI();
void Display_View_Selected_User_Open_Grievance_Report_UI();
void Display_View_Current_User_Resolved_Grievance_Report_UI();
void Display_View_Selected_User_Resolved_Grievance_Report_UI();
void Display_Submit_Grievance_Report_UI();
void Personal_Submit_Grievance_Report_UI();
void On_Behalf_of_User_Submit_Grievance_Report_UI();

void Display_View_Current_User_Tax_Information_UI(const Tax_Information_Record_Details& details);
void Display_Selected_User_Tax_Information_UI(const Tax_Information_Record_Details& details);
void Display_Edit_Current_User_Tax_Information_UI(const Tax_Information_Record_Details& details);
void Display_Edit_Selected_User_Tax_Information_UI(const Tax_Information_Record_Details& details);


#endif // HR_MANAGEMENT_UI_H
