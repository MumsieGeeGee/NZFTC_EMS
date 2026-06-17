// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
#ifndef USER_ROLES_H
#define USER_ROLES_H
#pragma once
#include <string>
#include <vector>

// User Roles

namespace Roles {
    inline const std::string Employee = "Employee";
    inline const std::string Admin    = "Admin";

    inline std::vector<std::string> All() {
        return { Employee, Admin };
    }
}

#endif // USER_ROLES_H
