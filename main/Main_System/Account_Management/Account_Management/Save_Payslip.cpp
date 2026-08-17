// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Save_Payslip.h"

#include "../../Path_Utilities.h"
#include <filesystem>
#include <fstream>

namespace {
std::filesystem::path Get_Payslip_Directory() {
    return Path_Utilities::Get_Payslip_Directory();
}

std::string Sanitize_File_Component(std::string value) {
    for (char& ch : value) {
        switch (ch) {
            case '<':
            case '>':
            case ':':
            case '"':
            case '/':
            case '\\':
            case '|':
            case '?':
            case '*':
                ch = '_';
                break;
            default:
                break;
        }
    }
    return value;
}
}

bool Save_Payslip(const std::string& username, const Pre_Calculation_Payslip& slip) {
    const std::filesystem::path directory = Get_Payslip_Directory();
    std::error_code dir_error;
    std::filesystem::create_directories(directory, dir_error);
    if (dir_error) {
        return false;
    }

    std::string file_name = Sanitize_File_Component(username);
    if (!slip.period.period_start_date.empty() || !slip.period.period_end_date.empty()) {
        file_name += "_" + Sanitize_File_Component(slip.period.period_start_date);
        file_name += "_to_";
        file_name += Sanitize_File_Component(slip.period.period_end_date);
    }
    file_name += ".txt";

    const std::filesystem::path file_path = directory / file_name;
    std::ofstream output(file_path, std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }

    output << "Payslip Summary" << '\n';
    output << "Username: " << username << '\n';
    output << "IRD Number: " << slip.IRD_Number << '\n';
    output << "Pay Period: " << slip.period.period_start_date << " - " << slip.period.period_end_date << '\n';
    output << "Gross Earnings: " << slip.gross_earnings << '\n';
    output << "Taxable Earnings: " << slip.taxable_earnings << '\n';
    output << "PAYE Tax: " << slip.paye << '\n';
    output << "ACC Earners Levy: " << slip.acc << '\n';
    output << "Student Loan Repayment: " << slip.student_loan << '\n';
    output << "Child Support Deduction: " << slip.child_support << '\n';
    output << "KiwiSaver Employee Contribution: " << slip.kiwisaver_employee << '\n';
    output << "KiwiSaver Employer Contribution: " << slip.kiwisaver_employer << '\n';
    output << "ESCT: " << slip.esct << '\n';
    output << "Net Pay: " << slip.net_pay << '\n';
    output << "Sick Leave Remaining: " << slip.sick_leave_remaining << '\n';
    output << "Annual Leave Remaining: " << slip.annual_leave_remaining << '\n';
    output << "Parental Leave Remaining: " << slip.parental_leave_remaining << '\n';
    output << "PDC Remaining: " << slip.pdc_remaining << '\n';
    output << "EAP Remaining: " << slip.eap_remaining << '\n';

    return static_cast<bool>(output);
}