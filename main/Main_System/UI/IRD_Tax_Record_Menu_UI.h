// Copyright (c) 2026 Georgia Kaye O'Sullivan, NZ Aotearoa. All rights reserved.
// Use of this source code is prohibited except with explicit written permission from the author.

#ifndef IRD_TAX_RECORD_MENU_UI_H
#define IRD_TAX_RECORD_MENU_UI_H

#include <string>

void Display_View_Update_IRD_Menu_UI(const std::string& IRD_Number);

void Display_Edit_Tax_Record_Fields_Menu_UI(
	const std::string& IRD_Number,
	bool showStudentLoanOptions,
	bool specialTaxCodeActive,
	bool studentLoanActive,
	bool childSupportActive,
	bool kiwiSaverActive,
	bool kiwiSaverOptedOut,
	bool esctActive);

#endif // IRD_TAX_RECORD_MENU_UI_H
