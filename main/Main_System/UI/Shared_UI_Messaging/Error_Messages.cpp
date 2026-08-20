// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Error_Messages.h"
#include "Form_Labels_UI.h"

// const std::string = "";
const std::string Display_Error = "Error: ";

// Username messages are defined later with label-driven composition.
const std::string Secondary_Auth_Username_Invalid = "Invalid secondary authentication username.";
const std::string Secondary_Auth_Username_Different = "Secondary authentication requires a different admin user.";
const std::string Secondary_Auth_Cannot_Proceed = "Secondary authentication cannot continue without a valid admin user.";
const std::string Secondary_Auth_Delete_Account_Cannot_Proceed = "Secondary authentication is required before deleting an account.";
const std::string Invalid_Credentials = "Invalid credentials. Please try again.";

const std::string Password_Insecure = Password_Label + " not secure.";
const std::string Password_Missing_or_Invalid = Password_Label + " is missing or invalid.";
const std::string Password_Incorrect = Password_Label + " is incorrect.";
const std::string Password_Must_Contain_1_Upper = Password_Label + " must contain at least 1 Uppercase Letter.";
const std::string Password_Must_Contain_1_Lower = Password_Label + " must contain at least 1 Lowercase Letter.";
const std::string Password_Must_Contain_1_Number = Password_Label + " must contain at least 1 Number.";
const std::string Password_Must_Contain_1_Symbol = Password_Label + " must contain at least 1 Symbol/Special Character.";
const std::string Password_Empty = Password_Label + " cannot be empty.";
const std::string Password_Too_Short = Password_Label + " too short, must be at least 8 characters long.";
const std::string Password_Too_Long = Password_Label + " too long, must remain under 30 characters.";
const std::string Password_Cannot_Contain_Space = Password_Label + " cannot contain spaces.";
const std::string Password_Cannot_Contain_Username = Password_Label + " cannot contain Username Details.";
const std::string Password_Cannot_Contain_Account_User_Name = Password_Label + " cannot contain First, Middle or Surname.";
const std::string Password_Cannot_Contain_Account_User_DOB = Password_Label + " cannot contain Date of Birth.";
const std::string Password_Cannot_Contain_Account_User_Email = Password_Label + " cannot contain Email Address.";
const std::string Password_Cannot_Contain_Account_User_Phone = Password_Label + " cannot contain Phone Number.";
const std::string Password_Cannot_Contain_Account_User_Address = Password_Label + " cannot contain Address Details.";
const std::string Password_Cannot_Contain_Account_User_IRD = Password_Label + " cannot contain IRD Number.";
const std::string Passwords_Do_Not_Match = "Passwords do not match.";

const std::string Invalid_Selection = "Invalid selection. Please try again.";
const std::string Invalid_Account_Type = "Invalid account type. Please select a valid account type.";
const std::string Invalid_Admin_Role = "Invalid admin role. Please select a valid admin role.";
const std::string Invalid_Finance_Role = "Invalid finance role. Please select a valid finance role.";
const std::string Invalid_Trade_Role = "Invalid trade role. Please select a valid trade role.";

const std::string First_Name_Empty = "First Name cannot be empty.";
const std::string First_Name_Too_Long = "First Name too long, must remain under 50 characters long.";
const std::string First_Name_Invalid_Characters = "First Name can only contain letters,spaces, '-' & apostrophes (Example: Mc'Donald).";

const std::string Middle_Name_Empty = "Middle Name cannot be empty.";
const std::string Middle_Name_Too_Long = "Middle Name too long, must remain under 50 characters long.";
const std::string Middle_Name_Invalid_Characters = "Middle Name can only contain letters,spaces, '-' & apostrophes (Example: Mc'Donald).";

const std::string Surname_Empty = "Surname cannot be empty.";
const std::string Surname_Too_Long = "Surname too long, must remain under 50 characters long.";
const std::string Surname_Invalid_Characters = "Surname can only contain letters,spaces, '-' & apostrophes (Example: Mc'Donald).";

const std::string DOB_Empty = "Date of Birth cannot be empty.";
const std::string DOB_Invalid_Format = "Date of Birth must be in the format DD/MM/YYYY.";
const std::string DOB_Invalid_Year = "Date of Birth cannot be after 2020 or before 1920.";

const std::string House_Number_Empty = "House Number cannot be empty.";
const std::string House_Number_Too_Long = "House Number too long, must remain under 10 characters long.";
const std::string House_Number_Invalid_Characters = "House Number must be numerical and can only contain '/' as a separator.";

const std::string Street_Name_Empty = "Street Name cannot be empty.";
const std::string Street_Name_Too_Long = "Street Name too long, must remain under 50 characters long.";
const std::string Street_Name_Invalid_Characters = "Street Name can only contain letters,spaces, '-' & apostrophes (Example: Mc'Donald).";

const std::string Suburb_Empty = "Suburb cannot be empty.";
const std::string Suburb_Too_Long = "Suburb too long, must remain under 50 characters long.";
const std::string Suburb_Invalid_Characters = "Suburb can only contain letters,spaces, '-' & apostrophes (Example: Mc'Donald).";

const std::string City_Empty = "City cannot be empty.";
const std::string City_Too_Long = "City too long, must remain under 50 characters long.";
const std::string City_Invalid_Characters = "City can only contain letters,spaces, '-' & apostrophes (Example: Mc'Donald).";

const std::string Postcode_Empty = "Postcode cannot be empty.";
const std::string Postcode_Incorrect_Length = "Postcode too long, must be 4 characters long.";
const std::string Postcode_Invalid_Characters = "Postcode must be numerical.";

const std::string Po_Box_Empty = "Post Office Box cannot be empty.";
const std::string Po_Box_Too_Long = "Post Office Box too long, must remain under 50 characters long.";
const std::string Po_Box_Invalid_Characters = " can only contain letters,spaces, '/', '-' & apostrophes (Example: Mc'Donald).";

const std::string Phone_Number_Empty = "Phone Number cannot be empty.";
const std::string Phone_Number_Too_Long = "Phone Number too long, must remain under 20 characters long.";
const std::string Phone_Number_Invalid_Characters = "Phone Number can only contain numbers, spaces, and dashes.";

const std::string Email_Address_Empty = "Email Address cannot be empty.";
const std::string Email_Address_Too_Long = "Email Address too long, must remain under 100 characters long.";
const std::string Email_Address_Invalid_Format = "Email Address must be in a valid format (e.g., email@gmail.com or email@gmail.co.nz).";

const std::string IRD_Number_Empty = "IRD Number cannot be empty.";
const std::string IRD_Number_Too_Long = "IRD Number too long, must remain under 9 characters long.";
const std::string IRD_Number_Invalid_Characters = "IRD Number must be numerical.";
const std::string IRD_Number_Invalid_Format = "IRD Number must be in a valid format (e.g., 123-456-789).";
const std::string IRD_Number_Input_8 = "This looks like an old 8-digit IRD number. Please add a zero at the beginning to make it a valid 9-digit IRD number (e.g., 012345678).";
const std::string IRD_Number_Already_Exists = "This IRD Number already has a record file in the master folder. Please enter a different IRD Number.";
const std::string IRD_Tax_Information_Save_Failure = "Failed to save IRD Tax Information.";
const std::string IRD_Tax_Information_Update_Failure = "Failed to update IRD Tax Information.";
const std::string IRD_Tax_Information_Deletion_Failure = "Failed to delete IRD Tax Information.";
const std::string IRD_Tax_Information_Not_Found = "IRD Tax Information not found.";
const std::string Payroll_Tax_Information_Not_Confirmed = "Payroll has not yet confirmed and/or uploaded your Tax Information, please return at a later time.";
const std::string IRD_Write_to_File_Failure = "Failed to write IRD Tax Information to file.";
const std::string Invalid_IRD_Number = "Please enter a valid IRD Number.";
const std::string Valid_IRD_Number = "Please enter a valid IRD Number.";

const std::string Employee_Tax_Information_Not_Found = "Employee Tax Information not found.";
const std::string Employee_Tax_Information_Save_Failure = "Failed to save Employee Tax Information.";
const std::string Employee_Tax_Information_Update_Failure = "Failed to update Employee Tax Information.";
const std::string Employee_Record_Not_Found = "Employee Record not found.";
const std::string Employer_Financial_Details_Not_Found = "Employer Financial Details not found.";

const std::string Try_Again = "Please try again.";
const std::string Yes_No_Input = "must be 'Yes' or 'No'.";
const std::string Menu_Option_Input = "must be a valid menu option.";
const std::string Invalid_Empty_Input = "Input cannot be empty.";
const std::string Invalid_Negative_Input = "Input cannot be a negative value.";
const std::string Invalid_Numerical_Input = "Input must be numerical.";
const std::string Invalid_Date = "Input must be a valid date.";
const std::string Invalid_Date_Format = "Invalid date format. Please enter a valid date in the format dd-mm-yyyy, dd/mm/yyyy, or dd.mm.yyyy.";
const std::string Invalid_Pay_Dates_Format = "Invalid backpay format. Use *Start-Date* '+' *End-Date*.";
const std::string Invalid_Year = "Input must be a valid year (between 2025 and 2100).";
const std::string Invalid_Year_DOB = "Invalid year of birth. Year must be between 1920 and 2020.";
const std::string Invalid_Percentage_Input = "Invalid percentage input.";
const std::string Invalid_Decimal_Input = "Input must be a valid decimal number.";
const std::string Invalid_Input_Try_Again = "Invalid input. Please try again.";
const std::string Invalid_Pay_Frequency = "Must select a valid pay frequency (Weekly, Fortnightly, Monthly).";
const std::string Invalid_PAYE_Table_Version = "Must enter a valid PAYE table version (e.g., 2026).";
const std::string Invalid_Extra_PAYE_Voluntary_Deduction = "Must enter a valid numeric value for extra PAYE voluntary deduction (or 0 if none).";
const std::string Invalid_Pay_Period_Too_Short = "Pay period is too short. Minimum is 7 days.";
const std::string Invalid_Pay_Period_Too_Long = "Pay period is too long. Maximum is 31 days.";
const std::string Invalid_Pay_Period_Invalid_Characters = "";
const std::string Invalid_Pay_Period_Invalid_Format = "Pay period must contain 'to'.";
const std::string Invalid_Pay_Period_Invalid_Dates = "Pay period contains invalid dates. Please enter a valid pay period in the format dd-mm-yyyy to dd-mm-yyyy.";

const std::string Account_Creation_Failure = "Account creation failed.";
// Account messages are defined later with label-driven composition.
const std::string Account_Deletion_Reason_Must_Be_Provided = "Account deletion reason must be provided.";
const std::string Account_Details_Unavailable = "Account details unavailable.";
const std::string Account_Retrieval_Failure = "Account retrieval failed.";
const std::string Account_Write_to_File_Failure = "Failed to write account details to file.";
const std::string Role_Type_Access_Failed = "Access failed due to invalid role type.";
const std::string IRD_Confirmation_Required_For_View = "IRD must be confirmed, no file contents yet.";

const std::string Unable_to_Find_ACC_Levy_Type_in_Employer_Details = "Unable to find ACC Levy Type in the employer details file.";
const std::string Invalid_ACC_Levy_Rate = "Invalid ACC levy rate. Please enter a valid percentage value (e.g., 1.75).";
const std::string Invalid_ACC_Levy_Maximum = "Invalid ACC levy maximum earnings threshold. Please enter a numeric value (e.g., 130000).";
const std::string Unable_to_Find_ACC_Levy_Rate_in_Employer_Details = "Unable to find ACC Levy Rate in the employer details file.";
// ACC levy messages are defined later with label-driven composition.

const std::string Invalid_Child_Support_Status = "Invalid child support status. Please enter 'active' or 'inactive'.";
const std::string Invalid_Child_Support_Standard_Deduction = "Invalid child support standard deduction amount. Please enter a numeric value (e.g., 136.50).";
const std::string Invalid_Child_Support_Voluntary_Deduction = "Invalid child support voluntary deduction amount. Please enter a numeric value (or 0 if none).";
const std::string Invalid_Child_Support_Protected_Net_Earnings = "Invalid child support protected net earnings (PNE) amount. Please enter a numeric value (e.g., 50000).";
const std::string Invalid_Child_Support_Shortfall_Calculation = "Invalid child support shortfall calculation amount. Please enter a numeric value (e.g., 100).";
const std::string Invalid_Child_Support_IRD_Notice_of_Deduction_Existence = "Invalid child support IRD Notice of Deduction (NOD) existence input. Please enter 'yes' or 'no'.";
const std::string Invalid_Child_Support_IRD_Notice_of_Deduction_Amount = "Invalid child support IRD Notice of Deduction (NOD) amount. Please enter a numeric value (e.g., 150).";
const std::string Invalid_Child_Support_IRD_Notice_of_Deduction_Effective_Date = "Invalid child support IRD Notice of Deduction (NOD) effective date.";
const std::string Invalid_Child_Support_IRD_Notice_of_Deduction_Expiry_Date = "Invalid child support IRD Notice of Deduction (NOD) expiry date.";
const std::string Invalid_Child_Support_IRD_Notice_of_Deduction_Reference_Number = "Invalid child support IRD Notice of Deduction (NOD) reference number. Please enter a valid reference number.";

const std::string Invalid_ESCT_Tax_Rate = "Invalid ESCT tax rate. Please enter a valid percentage value (e.g., 33%).";

const std::string Invalid_KiwiSaver_Opt_In_Out_Status = "Invalid input for KiwiSaver opt-in/opt-out status. Please enter 'opt-in' or 'opt-out'.";
const std::string Invalid_KiwiSaver_Employee_Contribution_Rate = "Invalid employee contribution rate for KiwiSaver. Please enter a valid percentage value (e.g., 3%).";
const std::string Invalid_KiwiSaver_Employer_Contribution_Rate = "Invalid employer contribution rate for KiwiSaver. Please enter a valid percentage value (e.g., 3%).";
const std::string Invalid_KiwiSaver_Opt_Out_Date = "Invalid KiwiSaver opt-out date. Please enter a valid date in the format DD/MM/YYYY.";

const std::string Invalid_Student_Loan_Special_Deduction_Rate = "Invalid Student Loan Special Deduction Rate (SDR). Please enter a valid numeric value (e.g., 12.5).";
const std::string Invalid_Student_Loan_Repayment_Exemption_Reason = "Invalid Student Loan Repayment Exemption Reason. Please enter a valid reason number.";
const std::string Invalid_Student_Loan_Repayment_Exemption_Expiry_Date = "Invalid Student Loan Repayment Exemption Expiry Date, must be a future date.";
const std::string Invalid_Student_Loan_Repayment_Threshold = "Invalid Student Loan Repayment Threshold. Please enter a valid numeric value (e.g., 20000).";
const std::string Invalid_Student_Loan_Repayment_Rate = "Invalid Student Loan Repayment Rate. Please enter a valid numeric value (e.g., 12.5).";

const std::string Invalid_Tax_Codes_Job_Type_Input = "Invalid input. Please enter 'Primary' or 'Secondary'.";
const std::string Invalid_Primary_Tax_Code = "Invalid primary tax code. Please select a valid tax code.";
const std::string Invalid_Secondary_Tax_Code = "Invalid secondary tax code. Please select a valid tax code.";
const std::string Invalid_Special_Tax_Code = "Invalid special tax code. Please select a valid tax code.";
const std::string Invalid_STC_Rate = "Invalid STC rate. Please enter a valid numeric value.";

const std::string Pay_Period_Empty =  "Pay period cannot be empty.";

const std::string Contact_Administrator = "Please contact the system administrator for assistance.";

// Database Messages
const std::string Unable_to_Access_Master_Folder = "";
const std::string Unable_to_Register_Record_in_Master_Folder = "";
const std::string Unable_to_Create_Record_File = "";
const std::string Unable_to_Open_Record_File = "";
const std::string Unable_to_Open_Employer_Details = "";
// Other Error Messages
const std::string Invalid_Date_Value = "Invalid date. Please enter a valid date in the format dd-mm-yyyy, dd/mm/yyyy, or dd.mm.yyyy.";
const std::string Invalid_ESCT_Classification_Input = "Please enter 'benefit' or 'deduction'.";

const std::string Unable_to_Find_ACC_Levy_Maximum_in_Employer_Details = "Unable to find ACC Levy Maximum in the employer details file.";
const std::string Unable_to_Find_KiwiSaver_Employer_Contribution_Rate_in_Employer_Details = "Unable to find KiwiSaver employer contribution rate in the employer details file.";
const std::string KiwiSaver_Employer_Contribution_Rate_in_Employer_Details_Empty = "KiwiSaver employer contribution rate in the employer details file is empty.";
const std::string Unknown_Role_Type_Salary_Package = "Unknown role type. Cannot display salary package.";
const std::string Unknown_Admin_Role_Salary_Package = "Unknown admin role. Cannot display salary package.";
const std::string Unknown_Finance_Role_Salary_Package = "Unknown finance role. Cannot display salary package.";
const std::string Unknown_Trade_Role_Salary_Package = "Unknown trade role. Cannot display salary package.";
const std::string Account_Update_Failure = "Account update failed.";
const std::string Request_Save_Failure_Message = "Request could not be saved.";
const std::string Request_Update_Failure_Message = "Request status could not be updated.";
const std::string Account_Deletion_Failure = "Account deletion failed.";
const std::string Employee_Record_Save_Failure = "Employee record save failed.";
const std::string Employee_Record_Update_Failure = "Employee record update failed.";
const std::string Employee_Record_Deletion_Failure = "Employee record deletion failed.";
const std::string Payslip_Save_Failure = "Payslip save failed.";

const std::string Cannot_Add_Tax_Details_Without_Confirmation = "Cannot add details without confirmation.";
const std::string Employee_Tax_Information_Not_Saved = "Employee tax information not saved.";
const std::string Employee_Tax_Information_Not_Updated = "Employee tax information not updated.";
const std::string Employee_Tax_Information_Not_Deleted = "Employee tax information not deleted.";
const std::string No_Accounts_Found = "No accounts found.";

const std::string Username_Empty = "Username cannot be empty.";
const std::string Username_Too_Long = "Username cannot exceed 50 characters.";
const std::string Username_Invalid_Characters = "Username can only contain alphanumeric characters, '_' and '#'.";
const std::string Username_Invalid = "Invalid username.";

// Login and Logout and eventually "session" also error messages
const std::string Account_Locked = "Account is locked.";
const std::string Account_Inactive = "Account is inactive.";
const std::string Too_Many_Attempts = "Too many attempts. Please try again later.";
const std::string Incorrect_Password = "Incorrect password.";
const std::string Login_Failed = "Login failed.";
const std::string Logout_Failed = "Logout failed.";
const std::string Unknown_User = "Unknown user.";
const std::string Suspicious_User = "Suspicious user.";
const std::string Malicious_User = "Malicious user.";
const std::string First_Incorrect_Password = "First incorrect password.";
const std::string Second_Incorrect_Password = "Second incorrect password.";
const std::string Third_Incorrect_Password = "Third incorrect password.";
const std::string Account_Locked_Due_to_Failed_Attempts = "Account locked due to failed attempts.";
const std::string Locked_Account_Login_Attempt = "Login attempted on locked account.";
const std::string Inactive_Account_Login_Attempt = "Login attempted on inactive account.";
const std::string Forgot_Password_Request_Failed = "Forgot password request failed.";
const std::string Session_Termination_Failed = "Session termination failed.";
const std::string Forced_Session_Termination_Failed = "Forced session termination failed.";
const std::string Unknown_User_Force_Logout_Failed = "Force logout failed for unknown user.";
const std::string Suspicious_User_Force_Logout_Failed = "Force logout failed for suspicious user.";
const std::string Malicious_User_Force_Logout_Failed = "Force logout failed for malicious user.";
const std::string Role_Type_Not_Found = "Role Type not found.";
const std::string Role_Type_Not_Assigned = "Role Type not assigned.";
const std::string Role_Type_Assignment_Failed = "Failed assignment for role type.";
const std::string Role_Type_Removal_Failed = "Failed to delete role type.";
const std::string Role_Type_Update_Failed = "Failed to update role type.";
const std::string Role_Type_Retrieval_Failed = "Failed to retrieve role type.";
const std::string Role_Type_Already_Assigned = "Role type already assigned.";
const std::string Role_Type_Not_Assigned_To_Account = "Role type not assigned to an account.";


const std::string Hours_Worked_Empty = "Hours worked cannot be empty.";
const std::string Invalid_Hours_Format = "Invalid hours format. Use numbers only.";
const std::string Hours_Negative = "Hours worked cannot be negative.";