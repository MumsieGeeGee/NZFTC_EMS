// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Main_Menu.h"
#include "Other_Menus.h"
#include "../Components/IRD_PAYE.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include "../../UI/IRD_Tax_Record_Menu_UI.h"
#include "Account_Management/Account_Management/Database.h"
#include <iostream>
#include <cstdlib>


void Main_Menu_Choices() {
    int choice;
    std::cin >> choice;
    std::cin.ignore(); 

    switch (choice) {
        case 1:
            Register_IRD_Number();
            break;
        case 2: {
            std::string ird_to_search;
            std::cout << IRD_Number_Search_Prompt;
            std::getline(std::cin, ird_to_search);
            if (search_IRD_Master_Folder_for_Record(ird_to_search)) {
                View_Update_IRD_Menu_Choices(ird_to_search);
            }
            break;
        }

        default:

            break;
    }
}


