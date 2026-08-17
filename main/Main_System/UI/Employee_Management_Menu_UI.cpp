// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Employee_Management_Menu_UI.h"
#include "Shared_UI_Messaging/Form_Labels_UI.h"
#include "../Employee Management/Role_Management/Role_and_Salary_Details.h"
#include "../Employee Management/Role_Management/Leave_and_Holiday.h"
#include "../Employee Management/Role_Management/Department_Roles/Admin/Admin_Salary_Packages.h"
#include "../Employee Management/Role_Management/Department_Roles/Admin/Admin_Salary_UI.h"
#include "../Employee Management/Role_Management/Department_Roles/Finance/Finance_Salary_Packages.h"
#include "../Employee Management/Role_Management/Department_Roles/Finance/Finance_Salary_UI.h"
#include "../Employee Management/Role_Management/Department_Roles/Trade/Trade_Salary_Packages.h"
#include "../Employee Management/Role_Management/Department_Roles/Trade/Trade_Salary_UI.h"
#include <string>
#include <iostream>

// Main Employee Management System Menu
void Display_Employee_Management_Menu_UI() {
    std::cout << "Employee Management System Menu:\n";
    std::cout << "1. View Employees\n";
    std::cout << "2. Add Employee\n";
    std::cout << "3. Update Employee\n";
    std::cout << "4. Delete Employee\n";
    std::cout << "5. Exit\n";
}
// View Employees Menu
void Display_View_Employees_UI() {
    std::cout << "View Employees Menu:\n";
    std::cout << "1. View All Employees\n";
    std::cout << "2. Search Employee by IRD Number\n";
    std::cout << "3. " << Return_to_Employee_Management_Main_Menu_Label << "\n";
}
// Add Employee Menu
void Display_Add_Employee_UI() {
    std::cout << "Add Employee Menu:\n";
    std::cout << "Enter the IRD Number of the employee you wish to add (or '0' to cancel):\n";
}
// Update Employee Menu
void Display_Update_Employee_UI() {
    std::cout << "Update Employee Menu:\n";
    std::cout << "Enter the IRD number of the employee you wish to update (or '0' to cancel):\n";
}
// Delete Employee Menu
void Display_Delete_Employee_UI() {
    std::cout << "Delete Employee Menu:\n";
    std::cout << "Enter the IRD number of the employee you wish to delete (or '0' to cancel):\n";
}
    // If the IRD number exists but is not confirmed...
void Display_Confirm_Existing_Employee_UI() {
    std::cout << "Employee with this IRD number already exists but is not confirmed.\n";
}
    // If the IRD number exists and is confirmed...
void Display_Existing_Employee_UI() {
    std::cout << "Employee with this IRD number already exists and is confirmed.\n";
}
    // If the IRD number does not exist...
void Display_Non_Existing_Employee_UI() {
    std::cout << "Employee with this IRD number does not exist.\n";
}
// Role Type Question Menus
void Display_Employee_Role_Question_UI() {
    std::cout << "What Business Role will this employee have?\n";
    std::cout << "1. " << Role_Type_Label_Admin << "\n";
    std::cout << "2. " << Role_Type_Label_Finance << "\n";
    std::cout << "3. " << Role_Type_Label_Trade << "\n";
    std::cout << Please_Select_an_Option_Label << " (1-3): ";
}
    // Admin Role Question Menu
void Display_Employee_Admin_Role_Question_UI() {
    std::cout << "What Admin Role will this employee have?\n";
    std::cout << "1. " << Junior_General_Administrator_Role_Label << "\n";
    std::cout << "2. " << Senior_General_Administrator_Role_Label << "\n";
    std::cout << "3. " << Junior_Trainee_Administrator_Role_Label << "\n";
    std::cout << "4. " << Senior_Trainee_Administrator_Role_Label << "\n";
    std::cout << "5. " << Junior_Casual_Administrator_Role_Label << "\n";
    std::cout << "6. " << Senior_Casual_Administrator_Role_Label << "\n";
    std::cout << "7. " << Admin_Receptionist << "\n";
    std::cout << "8. " << Admin_Call_Centre_Operator << "\n";
    std::cout << "9. " << Admin_Executive_Assistant << "\n";
    std::cout << "10. " << Admin_Personal_Assistant << "\n";
    std::cout << "11. " << Admin_Human_Resources << "\n";
    std::cout << "12. " << Junior_IT_Support_Administrator_Role_Label << "\n";
    std::cout << "13. " << Senior_IT_Support_Administrator_Role_Label << "\n";
    std::cout << "14. " << Admin_Junior_Manager << "\n";
    std::cout << "15. " << Admin_Senior_Manager << "\n";
    std::cout << Please_Select_an_Option_Label << " (1-15): ";
}
    // Finance Role Question Menu
void Display_Employee_Finance_Role_Question_UI() {
    std::cout << "What Finance Role will this employee have?\n";
    std::cout << "1. " << Junior_General_Finance_Staff_Member << "\n";
    std::cout << "2. " << Senior_General_Finance_Staff_Member << "\n";
    std::cout << "3. " << Junior_Trainee_Finance_Staff_Member << "\n";
    std::cout << "4. " << Senior_Trainee_Finance_Staff_Member << "\n";
    std::cout << "5. " << Junior_Casual_Finance_Staff_Member << "\n";
    std::cout << "6. " << Senior_Casual_Finance_Staff_Member << "\n";
    std::cout << "7. " << Payroll_Administrator << "\n";
    std::cout << "8. " << Accounts_Payable_Administrator << "\n";
    std::cout << "9. " << Accounts_Receivable_Administrator << "\n";
    std::cout << "10. " << Junior_Finance_Manager << "\n";
    std::cout << "11. " << Senior_Finance_Manager << "\n";
    std::cout << Please_Select_an_Option_Label << " (1-11): ";
}
    // Trade Role Question Menu
void Display_Employee_Trade_Role_Question_UI() {
    std::cout << "What Trade Role will this employee have?\n";
    std::cout << "1. " << Junior_General_Trade_Staff_Member << "\n";
    std::cout << "2. " << Senior_General_Trade_Staff_Member << "\n";
    std::cout << "3. " << Junior_Trainee_Trade_Staff_Member << "\n";
    std::cout << "4. " << Senior_Trainee_Trade_Staff_Member << "\n";
    std::cout << "5. " << Junior_Casual_Trade_Staff_Member << "\n";
    std::cout << "6. " << Senior_Casual_Trade_Staff_Member << "\n";
    std::cout << "7. " << Junior_Apprentice << "\n";
    std::cout << "8. " << Senior_Mentor << "\n";
    std::cout << "9. " << Safety_Officer << "\n";
    std::cout << "10. " << Sales_Representative << "\n";
    std::cout << "11. " << Junior_Trade_Manager << "\n";
    std::cout << "12. " << Senior_Trade_Manager << "\n";
    std::cout << Please_Select_an_Option_Label << " (1-12): ";
}

// Pay Type Question Menu (Hourly, Salaried, Casual)
void Display_Employee_Pay_Type_Question_UI() {
    std::cout << "What Pay Type will this employee have?\n";
    std::cout << "eg. Are they paid hourly, by salary or casually?\n";
    std::cout << "1. " << Hourly_Label << "\n";
    std::cout << "2. " << Salary_Label << "\n";
    std::cout << "3. " << Casual_Label << "\n";
}
// Employment Type Question Menu (Full-Time, Part-Time, Fixed-Term, Single-Occasion, Casual)
void Display_Employee_Employment_Type_Question_UI() {
    std::cout << "What Employment Type will this employee have?\n";
    std::cout << "eg. Are they employed full-time, part-time, fixed-term, for a single-occasion or casually?\n";
    std::cout << "1. " << Full_Time_Employment_Type_Label << "\n";
    std::cout << "2. " << Part_Time_Employment_Type_Label << "\n";
    std::cout << "3. " << Fixed_Term_Employment_Type_Label << "\n";
    std::cout << "4. " << Single_Occasion_Employment_Type_Label << "\n";
    std::cout << "5. " << Casual_Label << "\n";
}
// Pay Frequency Question Menu (Weekly, Fortnightly, Monthly)
void Display_Employee_Pay_Frequency_Type_Question_UI() {
    std::cout << "What Pay Frequency will this employee have?\n";
    std::cout << "1. " << Weekly_Label << "\n";
    std::cout << "2. " << Fortnightly_Label << "\n";
    std::cout << "3. " << Monthly_Label << "\n";
}
// Contracted Hours Question Menu
void Display_Employee_Contracted_Hours_Question_UI() {
    std::cout << "What are the Contracted Hours for this employee?\n";
    std::cout << "Enter the number of hours per week (eg. 40, 30, 20, 10, etc.)\n";
    std::cout << "or enter '0' to cancel and return to the previous menu.\n";
    std::cout << "Contracted Hours: ";
}
// Scheduling Question Menu (Fixed Schedule, Flexible Schedule)
void Display_Employee_Schedule_Question_UI() {
    std::cout << "What is the Scheduling for this employee?\n";
    std::cout << "1. Fixed Schedule\n";
    std::cout << "2. Flexible Schedule\n";
}
// Overtime Rate Question Menu (Hourly Rate, Contracted Hourly Rate, Overtime Rate, Annual Salary, Monthly Salary, Fortnightly Salary, Weekly Salary)
void Display_Employee_Overtime_Question_UI() {
    std::cout << "What is the Overtime Rate for this employee?\n";
    std::cout << "Enter the Overtime Rate (eg. 1.5, 2.0, etc.)\n";
    std::cout << "or enter '0' to cancel and return to the previous menu.\n";
    std::cout << "Overtime Rate: ";
}
// Overtime Maximum Hours Question Menu
void Display_Employee_Overtime_Maximum_Hours_Question_UI() {
    std::cout << "What is the Overtime Maximum Hours for this employee?\n";
    std::cout << "Enter the Overtime Maximum Hours (eg. 10, 20, 30, etc.)\n";
    std::cout << "or enter '0' to cancel and return to the previous menu.\n";
    std::cout << "Overtime Maximum Hours: ";
}
