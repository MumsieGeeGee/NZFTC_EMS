// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef ERROR_MESSAGES_H
#define ERROR_MESSAGES_H

#include <string>

extern const std::string Display_Error;

extern const std::string Username_Empty;
extern const std::string Username_Too_Long;
extern const std::string Username_Invalid_Characters;
extern const std::string Username_Invalid;
extern const std::string Secondary_Auth_Username_Invalid;
extern const std::string Secondary_Auth_Username_Different;
extern const std::string Secondary_Auth_Cannot_Proceed;
extern const std::string Secondary_Auth_Delete_Account_Cannot_Proceed;
extern const std::string Invalid_Credentials;

extern const std::string Password_Insecure;
extern const std::string Password_Missing_or_Invalid;
extern const std::string Password_Incorrect;
extern const std::string Password_Must_Contain_1_Upper;
extern const std::string Password_Must_Contain_1_Lower;
extern const std::string Password_Must_Contain_1_Number;
extern const std::string Password_Must_Contain_1_Symbol;
extern const std::string Password_Empty;
extern const std::string Password_Too_Short;
extern const std::string Password_Too_Long;
extern const std::string Password_Cannot_Contain_Space;
extern const std::string Password_Cannot_Contain_Username;
extern const std::string Password_Cannot_Contain_Account_User_Name;
extern const std::string Password_Cannot_Contain_Account_User_DOB;
extern const std::string Password_Cannot_Contain_Account_User_Email;
extern const std::string Password_Cannot_Contain_Account_User_Phone;
extern const std::string Password_Cannot_Contain_Account_User_Address;
extern const std::string Password_Cannot_Contain_Account_User_IRD;
extern const std::string Passwords_Do_Not_Match;

extern const std::string Invalid_Selection;
extern const std::string Invalid_Account_Type;
extern const std::string Invalid_Admin_Role;
extern const std::string Invalid_Finance_Role;
extern const std::string Invalid_Trade_Role;

extern const std::string First_Name_Empty;
extern const std::string First_Name_Too_Long;
extern const std::string First_Name_Invalid_Characters;

extern const std::string Middle_Name_Empty;
extern const std::string Middle_Name_Too_Long;
extern const std::string Middle_Name_Invalid_Characters;

extern const std::string Surname_Empty;
extern const std::string Surname_Too_Long;
extern const std::string Surname_Invalid_Characters;

extern const std::string DOB_Empty;
extern const std::string DOB_Invalid_Format;
extern const std::string DOB_Invalid_Year;

extern const std::string House_Number_Empty;
extern const std::string House_Number_Too_Long;
extern const std::string House_Number_Invalid_Characters;

extern const std::string Street_Name_Empty;
extern const std::string Street_Name_Too_Long;
extern const std::string Street_Name_Invalid_Characters;

extern const std::string Suburb_Empty;
extern const std::string Suburb_Too_Long;
extern const std::string Suburb_Invalid_Characters;

extern const std::string City_Empty;
extern const std::string City_Too_Long;
extern const std::string City_Invalid_Characters;

extern const std::string Postcode_Empty;
extern const std::string Postcode_Incorrect_Length;
extern const std::string Postcode_Invalid_Characters;

extern const std::string Po_Box_Empty;
extern const std::string Po_Box_Too_Long;
extern const std::string Po_Box_Invalid_Characters;

extern const std::string Phone_Number_Empty;
extern const std::string Phone_Number_Too_Long;
extern const std::string Phone_Number_Invalid_Characters;

extern const std::string Email_Address_Empty;
extern const std::string Email_Address_Too_Long;
extern const std::string Email_Address_Invalid_Format;

extern const std::string IRD_Number_Empty;
extern const std::string IRD_Number_Too_Long;
extern const std::string IRD_Number_Invalid_Characters;
extern const std::string IRD_Number_Invalid_Format;
extern const std::string IRD_Number_Input_8;
extern const std::string IRD_Number_Already_Exists;
extern const std::string IRD_Tax_Information_Save_Failure;
extern const std::string IRD_Tax_Information_Update_Failure;
extern const std::string IRD_Tax_Information_Deletion_Failure;
extern const std::string IRD_Write_to_File_Failure;
extern const std::string IRD_Tax_Information_Not_Found;
extern const std::string Payroll_Tax_Information_Not_Confirmed;
extern const std::string Invalid_IRD_Number;
extern const std::string Valid_IRD_Number;

extern const std::string Employee_Tax_Information_Not_Found;
extern const std::string Employee_Tax_Information_Save_Failure;
extern const std::string Employee_Tax_Information_Update_Failure;
extern const std::string Employee_Record_Not_Found;
extern const std::string Employee_Record_Save_Failure;
extern const std::string Employee_Record_Update_Failure;
extern const std::string Employer_Financial_Details_Not_Found;
extern const std::string Employer_Financial_Details_Save_Failure;
extern const std::string Employer_Financial_Details_Update_Failure;

extern const std::string Try_Again;
extern const std::string Yes_No_Input;
extern const std::string Menu_Option_Input;
extern const std::string Invalid_Empty_Input;
extern const std::string Invalid_Negative_Input;
extern const std::string Invalid_Numerical_Input;
extern const std::string Invalid_Date;
extern const std::string Invalid_Date_Format;
extern const std::string Invalid_Pay_Dates_Format;
extern const std::string Invalid_Year;
extern const std::string Invalid_Year_DOB;
extern const std::string Invalid_Percentage_Input;
extern const std::string Invalid_Decimal_Input;
extern const std::string Invalid_Input_Try_Again;
extern const std::string Invalid_Pay_Frequency;
extern const std::string Invalid_PAYE_Table_Version;
extern const std::string Invalid_Extra_PAYE_Voluntary_Deduction;
extern const std::string Invalid_Pay_Period_Too_Short;
extern const std::string Invalid_Pay_Period_Too_Long;
extern const std::string Invalid_Pay_Period_Invalid_Characters;
extern const std::string Invalid_Pay_Period_Invalid_Format;
extern const std::string Invalid_Pay_Period_Invalid_Dates;

extern const std::string Account_Creation_Failure;
extern const std::string Account_Update_Failure;
extern const std::string Request_Save_Failure_Message;
extern const std::string Request_Update_Failure_Message;
extern const std::string Account_Deletion_Failure;
extern const std::string Account_Deletion_Reason_Must_Be_Provided;
extern const std::string Account_Retrieval_Failure;
extern const std::string Account_Write_to_File_Failure;
extern const std::string Payslip_Save_Failure;
extern const std::string Invalid_Admin_Role;
extern const std::string Invalid_Finance_Role;
extern const std::string Invalid_Trade_Role;

extern const std::string Role_Type_Access_Failed;
extern const std::string IRD_Confirmation_Required_For_View;

extern const std::string Unable_to_Find_ACC_Levy_Type_in_Employer_Details;
extern const std::string Invalid_ACC_Levy_Rate;
extern const std::string Invalid_ACC_Levy_Maximum;
extern const std::string Unable_to_Find_ACC_Levy_Rate_in_Employer_Details;
extern const std::string Unable_to_Find_ACC_Levy_Maximum_in_Employer_Details;

extern const std::string Invalid_Child_Support_Status;
extern const std::string Invalid_Child_Support_Standard_Deduction;
extern const std::string Invalid_Child_Support_Voluntary_Deduction;
extern const std::string Invalid_Child_Support_Protected_Net_Earnings;
extern const std::string Invalid_Child_Support_Shortfall_Calculation;
extern const std::string Invalid_Child_Support_IRD_Notice_of_Deduction_Existence;
extern const std::string Invalid_Child_Support_IRD_Notice_of_Deduction_Amount;
extern const std::string Invalid_Child_Support_IRD_Notice_of_Deduction_Effective_Date;
extern const std::string Invalid_Child_Support_IRD_Notice_of_Deduction_Expiry_Date;
extern const std::string Invalid_Child_Support_IRD_Notice_of_Deduction_Reference_Number;

extern const std::string Invalid_ESCT_Tax_Rate;

extern const std::string Invalid_KiwiSaver_Opt_In_Out_Status;
extern const std::string Invalid_KiwiSaver_Employee_Contribution_Rate;
extern const std::string Invalid_KiwiSaver_Employer_Contribution_Rate;
extern const std::string Invalid_KiwiSaver_Opt_Out_Date;

extern const std::string Invalid_Student_Loan_Special_Deduction_Rate;
extern const std::string Invalid_Student_Loan_Repayment_Exemption_Reason;
extern const std::string Invalid_Student_Loan_Repayment_Exemption_Expiry_Date;
extern const std::string Invalid_Student_Loan_Repayment_Threshold;
extern const std::string Invalid_Student_Loan_Repayment_Rate;

extern const std::string Invalid_Tax_Codes_Job_Type_Input;
extern const std::string Invalid_Primary_Tax_Code;
extern const std::string Invalid_Secondary_Tax_Code;
extern const std::string Invalid_Special_Tax_Code;
extern const std::string Invalid_STC_Rate;

extern const std::string Contact_Administrator;
extern const std::string No_Accounts_Found;
extern const std::string Account_Locked;
extern const std::string Account_Inactive;
extern const std::string Too_Many_Attempts;
extern const std::string Incorrect_Password;
extern const std::string Login_Failed;
extern const std::string Logout_Failed;;
extern const std::string Suspicious_User;
extern const std::string Malicious_User;
extern const std::string First_Incorrect_Password;
extern const std::string Second_Incorrect_Password;
extern const std::string Third_Incorrect_Password;
extern const std::string Account_Locked_Due_to_Failed_Attempts;
extern const std::string Locked_Account_Login_Attempt;
extern const std::string Inactive_Account_Login_Attempt;
extern const std::string Forgot_Password_Request_Failed;
extern const std::string Session_Termination_Failed;
extern const std::string Forced_Session_Termination_Failed;
extern const std::string Unknown_User_Force_Logout_Failed;
extern const std::string Suspicious_User_Force_Logout_Failed;
extern const std::string Malicious_User_Force_Logout_Failed;
extern const std::string Role_Type_Not_Found;
extern const std::string Role_Type_Not_Assigned;
extern const std::string Role_Type_Assignment_Failed;
extern const std::string Role_Type_Removal_Failed;
extern const std::string Role_Type_Update_Failed;
extern const std::string Role_Type_Retrieval_Failed;
extern const std::string Role_Type_Already_Assigned;
extern const std::string Role_Type_Not_Assigned_To_Account;

extern const std::string Pay_Period_Empty;
extern const std::string Hours_Worked_Empty;
extern const std::string Invalid_Hours_Format;
extern const std::string Hours_Negative;

#endif // ERROR_MESSAGES_H
