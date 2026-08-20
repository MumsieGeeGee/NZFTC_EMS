#ifndef PATH_UTILITIES_H
#define PATH_UTILITIES_H

#include <filesystem>
#include <initializer_list>

namespace Path_Utilities {

inline std::filesystem::path Resolve_Data_Path(
        std::initializer_list<std::filesystem::path> relative_candidates) {
    std::error_code current_error;
    std::filesystem::path current =
        std::filesystem::current_path(current_error);
    if (current_error || current.empty()) {
        current = std::filesystem::path(".");
    }

    for (std::filesystem::path probe = current;; probe = probe.parent_path()) {
        for (const std::filesystem::path& relative_candidate : relative_candidates) {
            const std::filesystem::path candidate = probe / relative_candidate;
            std::error_code exists_error;
            if (std::filesystem::exists(candidate, exists_error) &&
                !exists_error) {
                return candidate;
            }
        }

        if (probe.parent_path() == probe) {
            break;
        }
    }

    return current / *relative_candidates.begin();
}

inline std::filesystem::path Get_Employee_Records_Directory() {
    static const std::filesystem::path directory = Resolve_Data_Path({
        "main/Main_System/Employee Management/Employee_Records",
        "Main_System/Employee Management/Employee_Records",
        "Employee Management/Employee_Records"
    });
    return directory;
}

inline std::filesystem::path Get_Employer_Details_File() {
    static const std::filesystem::path file = Resolve_Data_Path({
        "main/Main_System/Employer_Details.txt",
        "Main_System/Employer_Details.txt",
        "Employer_Details.txt"
    });
    return file;
}

inline std::filesystem::path Get_Payslip_Directory() {
    static const std::filesystem::path directory = Resolve_Data_Path({
        "main/Main_System/Employee Management/Employee_Records/Payslips",
        "Main_System/Employee Management/Employee_Records/Payslips",
        "Employee Management/Employee_Records/Payslips"
    });
    return directory;
}

inline std::filesystem::path Get_Session_Logs_Directory() {
    static const std::filesystem::path directory = Resolve_Data_Path({
        "main/Main_System/NZFTC_EMS/Session_Handling/Session_Logs",
        "Main_System/NZFTC_EMS/Session_Handling/Session_Logs",
        "NZFTC_EMS/Session_Handling/Session_Logs"
    });
    return directory;
}

}

#endif // PATH_UTILITIES_H
