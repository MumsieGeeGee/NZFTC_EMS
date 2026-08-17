// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef IRD_PAYE_H
#define IRD_PAYE_H

#include <string>
// IRD and PAYE Enums
enum class confirmation_Status {
    Unconfirmed,
    Confirmed
};

enum class pay_Frequency {
    Weekly,
    Fortnightly,
    Monthly
};

struct Pay_Period {
    std::string period_start_date;
    std::string period_end_date;
};

// String Conversion Functions
std::string Pay_Frequency_to_String(pay_Frequency frequency);
std::string Confirmation_Status_to_String(confirmation_Status status);
// IRD and PAYE Input Handling Functions
void Register_IRD_Number();
void IRD_Number_Confirmation_Status();
void Continue_Tax_Information_Entry();
void Get_Pay_Frequency(const std::string& IRD_Number);
void Get_PAYE_Table_Version(const std::string& IRD_Number);
void Get_Extra_PAYE_Voluntary_Deduction(const std::string& IRD_Number);
void Get_New_IRD_Number(std::string& IRD_Number);
void Get_Existing_IRD_Number(std::string& IRD_Number);
bool Valid_Pay_Cycle(const std::string& start, const std::string& end);
bool Valid_Pay_Period_for_Payslips(const std::string& input);



#endif // IRD_PAYE_H
