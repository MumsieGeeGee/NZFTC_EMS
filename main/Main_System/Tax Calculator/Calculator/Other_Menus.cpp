// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#include "Other_Menus.h"
#include "../../UI/IRD_Tax_Record_Menu_UI.h"
#include "../../Account_Management/Account_Management/Database.h"
#include "../../Account_Management/Account_Management/Save_Functions.h"
#include "../../Account_Management/Account_Management/Input_Validation_Helpers.h"
#include "../Components/IRD_PAYE.h"
#include "../Components/Tax_Codes.h"
#include "../Components/ACC.h"
#include "../Components/Student_Loan.h"
#include "../Components/Child_Support.h"
#include "../Components/Kiwisaver.h"
#include "../Components/ESCT.h"
#include "../../UI/Shared_UI_Messaging/IRD_PAYE_UI.h"
#include "../../UI/Shared_UI_Messaging/Error_Messages.h"
#include "../../UI/Shared_UI_Messaging/Form_Labels_UI.h"
#include <iostream>
#include <limits>

// Second Option From Main Menu - View/Update IRD Number & Tax Information
void View_Update_IRD_Menu_Choices(const std::string& IRD_Number) {
	while (true) {
        Display_View_Update_IRD_Menu_UI(IRD_Number);

		int choice = 0;
		if (!(std::cin >> choice)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
			continue;
		}
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (choice == 1) {
            std::string fileContents;
            if (!Get_IRD_Record_File_Contents(IRD_Number, fileContents)) {
                std::cout << Display_Error << IRD_Tax_Information_Not_Found << std::endl;
                continue;
            }
            std::cout << fileContents << std::endl;
			continue;
		}

		if (choice == 2) {
			Edit_Tax_Record_Fields_Menu_Choices(IRD_Number);
			continue;

        if (choice == 3) {
            // Display HR Management Main Menu/Get Selection
            
            return;
        }
		} else {
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
        }
	}
}

void Edit_Tax_Record_Fields_Menu_Choices(const std::string& IRD_Number) {
	while (true) {
        const bool showStudentLoanOptions = !Is_Primary_Tax_Code_ME(IRD_Number);
        const bool specialTaxCodeActive = Is_Special_Tax_Code_Active(IRD_Number);
        const bool studentLoanActive = showStudentLoanOptions && Is_Student_Loan_Active(IRD_Number);
        const bool childSupportActive = Is_Child_Support_Active(IRD_Number);
        const bool kiwiSaverActive = Is_KiwiSaver_Active(IRD_Number);
        const bool kiwiSaverOptedOut = Is_KiwiSaver_Opted_Out(IRD_Number);
        const bool esctActive = Is_ESCT_Active(IRD_Number);
        Display_Edit_Tax_Record_Fields_Menu_UI(
            IRD_Number,
            showStudentLoanOptions,
            specialTaxCodeActive,
            studentLoanActive,
            childSupportActive,
            kiwiSaverActive,
            kiwiSaverOptedOut,
            esctActive);

		int choice = -1;
		if (!(std::cin >> choice)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
			continue;
		}
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (choice == 0) {
			return;
		}

		int nextChoice = 1;

		if (choice == nextChoice++) {
			Get_Pay_Frequency(IRD_Number);
            continue;
        }

        if (choice == nextChoice++) {
            Get_PAYE_Table_Version(IRD_Number);
            continue;
        }

        if (choice == nextChoice++) {
            Get_Extra_PAYE_Voluntary_Deduction(IRD_Number);
            continue;
        }

        if (choice == nextChoice++) {
            Get_Tax_Code_By_Job_Type(IRD_Number);
            continue;
        }

        if (choice == nextChoice++) {
            Get_Special_Tax_Code(IRD_Number);
            continue;
        }

        if (showStudentLoanOptions) {
            if (choice == nextChoice++) {
                Get_Student_Loan_Existence(IRD_Number);
                continue;
            }
        }

        if (choice == nextChoice++) {
            Get_Child_Support_Status(IRD_Number);
            continue;
        }

        if (choice == nextChoice++) {
            Get_KiwiSaver_Opt_In_Out_Status(IRD_Number);
            continue;
        }

        if (specialTaxCodeActive) {
            if (choice == nextChoice++) {
                Get_STC_Rate(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_STC_Letter_Received(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_STC_Expiry_Date(IRD_Number);
                continue;
            }
        }

        if (showStudentLoanOptions && studentLoanActive) {
            if (choice == nextChoice++) {
                Get_Student_Loan_SDR_IRD_Letter_Existence(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Student_Loan_Special_Deduction_Rate(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Student_Loan_Repayment_Exemption_Existence(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Student_Loan_Repayment_Exemption_Reason(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Student_Loan_Repayment_Exemption_Expiry_Date(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Student_Loan_Repayment_Threshold(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Student_Loan_Repayment_Rate(IRD_Number);
                continue;
            }
        }

        if (childSupportActive) {
            if (choice == nextChoice++) {
                Get_Child_Support_Standard_Deduction(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Child_Support_Voluntary_Deduction(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Child_Support_Protected_Net_Earnings(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Child_Support_Shortfall_Calculation(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Child_Support_IRD_Notice_of_Deduction_Existence(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Child_Support_IRD_Notice_of_Deduction_Amount(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Child_Support_IRD_Notice_of_Deduction_Effective_Date(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Child_Support_IRD_Notice_of_Deduction_Expiry_Date(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_Child_Support_IRD_Notice_of_Deduction_Reference_Number(IRD_Number);
                continue;
            }
        }

        if (kiwiSaverActive) {
            if (choice == nextChoice++) {
                Get_KiwiSaver_Employee_Contribution_Rate(IRD_Number);
                continue;
            }
            if (kiwiSaverOptedOut) {
                if (choice == nextChoice++) {
                    Get_KiwiSaver_Opt_Out_Date(IRD_Number);
                    continue;
                }
            }
        }

        if (choice == nextChoice++) {
            Get_ESCT_Existence(IRD_Number);
            continue;
        }

        if (esctActive) {
            if (choice == nextChoice++) {
                Get_ESCT_Tax_Rate(IRD_Number);
                continue;
            }
            if (choice == nextChoice++) {
                Get_ESCT_Income_Band(IRD_Number);
                continue;
            }
        }
        std::cout << Display_Error << Invalid_Input_Try_Again << std::endl;
	}
}


