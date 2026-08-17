// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

// Student Loan
// SL Suffix – Student Loan Required - Set  Via Tax_Code functions
// Default Rate (IRD)
// Special Deduction Rate (IRD letter)
// Repayment Exemption (rare)

#ifndef STUDENT_LOAN_H
#define STUDENT_LOAN_H

#include <string>

// Get Student Loan Existence
bool Get_Student_Loan_Existence(const std::string& IRD_Number);
bool Get_Student_Loan_SDR_IRD_Letter_Existence(const std::string& IRD_Number);
bool Get_Student_Loan_Special_Deduction_Rate(const std::string& IRD_Number);
bool Get_Student_Loan_Repayment_Exemption_Existence(const std::string& IRD_Number);
bool Get_Student_Loan_Repayment_Exemption_Reason(const std::string& IRD_Number);
bool Get_Student_Loan_Repayment_Exemption_Expiry_Date(const std::string& IRD_Number);
bool Get_Student_Loan_Repayment_Threshold(const std::string& IRD_Number);
bool Get_Student_Loan_Repayment_Rate(const std::string& IRD_Number);

#endif // STUDENT_LOAN_H
