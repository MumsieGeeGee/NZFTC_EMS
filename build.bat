@echo off
setlocal
pushd "%~dp0"

where cl >nul 2>&1
if %errorlevel% neq 0 (
	set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
	if exist "%VSWHERE%" (
		for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSINSTALL=%%I"
	)

	if defined VSINSTALL (
		if exist "%VSINSTALL%\Common7\Tools\VsDevCmd.bat" (
			call "%VSINSTALL%\Common7\Tools\VsDevCmd.bat" -no_logo -arch=x64 -host_arch=x64 >nul
		)
	)
)

where cl >nul 2>&1
if %errorlevel% neq 0 (
	echo MSVC compiler not found. Install Visual Studio Build Tools with C++ or run this from a Developer Command Prompt.
	popd
	endlocal
	exit /b 1
)

cl /std:c++17 /EHsc /I"main\Main_System" ^
	"main\Main_System\NZFTC_EMS\main.cpp" ^
	"main\Main_System\NZFTC_EMS\Session_Handling\Session_Handling.cpp" ^
	"main\Main_System\NZFTC_EMS\Session_Handling\Login_Logout_Events.cpp" ^
	"main\Main_System\Login\Login_Menu.cpp" ^
	"main\Main_System\Tax Calculator\Calculator\Calculator.cpp" ^
	"main\Main_System\Tax Calculator\Calculator\Main_Menu.cpp" ^
	"main\Main_System\Tax Calculator\Calculator\Other_Menus.cpp" ^
	"main\Main_System\Tax Calculator\Components\IRD_PAYE.cpp" ^
	"main\Main_System\Tax Calculator\Components\Tax_Codes.cpp" ^
	"main\Main_System\Tax Calculator\Components\ACC.cpp" ^
	"main\Main_System\Tax Calculator\Components\Student_Loan.cpp" ^
	"main\Main_System\Tax Calculator\Components\Child_Support.cpp" ^
	"main\Main_System\Tax Calculator\Components\Kiwisaver.cpp" ^
	"main\Main_System\Tax Calculator\Components\GST.cpp" ^
	"main\Main_System\Tax Calculator\Components\ESCT.cpp" ^
	"main\Main_System\UI\Shared_UI_Messaging\IRD_PAYE_UI.cpp" ^
	"main\Main_System\Account_Management\Account_Management\Save_Functions.cpp" ^
	"main\Main_System\Account_Management\Account_Management\Database.cpp" ^
	"main\Main_System\Account_Management\Account_Management\Input_Validation_Helpers.cpp" ^
	"main\Main_System\Account_Management\Account_Management\Security_Protocols.cpp" ^
	"main\Main_System\Account_Management\Account_Management\Save_Account_Details.cpp" ^
	"main\Main_System\Account_Management\Account_Management\Save_Payslip.cpp" ^
	"main\Main_System\Account_Management\Account_Creation\Create_Account.cpp" ^
	"main\Main_System\Account_Management\Account_Creation\View_Accounts.cpp" ^
	"main\Main_System\Account_Management\Account_Creation\Edit_Accounts.cpp" ^
	"main\Main_System\Account_Management\Account_Creation\Delete_Accounts.cpp" ^
	"main\Main_System\Account_Management\Account_Access\Account_Database.cpp" ^
	"main\Main_System\Account_Management\Account_Access\Component_Access.cpp" ^
	"main\Main_System\Account_Management\Account_Access\Admin_Dashboard.cpp" ^
	"main\Main_System\Account_Management\Account_Access\Employee_Dashboard.cpp" ^
	"main\Main_System\Account_Management\Account_Types\Admin_Accounts.cpp" ^
	"main\Main_System\Account_Management\Account_Types\Finance_Accounts.cpp" ^
	"main\Main_System\Account_Management\Account_Types\Trade_Accounts.cpp" ^
	"main\Main_System\Account_Form\Account_Creation_Form_Labels.cpp" ^
	"main\Main_System\Account_Form\Account_Creation_Form_UI.cpp" ^
	"main\Main_System\Employee Management\HR_Management\HR_Management.cpp" ^
	"main\Main_System\Employee Management\HR_Management\HR_Management_UI.cpp" ^
	"main\Main_System\Employee Management\HR_Management\Leave\Leave_Requests.cpp" ^
	"main\Main_System\Employee Management\HR_Management\Leave\Leave_Requests_UI.cpp" ^
	"main\Main_System\Employee Management\HR_Management\Grievances\Grievance_Form\Grievance_Form_UI.cpp" ^
	"main\Main_System\Employee Management\HR_Management\Grievances\Grievance_Form\Grievance_Service.cpp" ^
	"main\Main_System\Employee Management\HR_Management\Grievances\Grievance_Form\Grievance_Workflow.cpp" ^
	"main\Main_System\Employee Management\HR_Management\Grievances\Requests\Grievance_Requests.cpp" ^
	"main\Main_System\Employee Management\HR_Management\Payslips\Payslip_Generation.cpp" ^
	"main\Main_System\Employee Management\HR_Management\Payslips\Payslip_Generation_UI.cpp" ^
	"main\Main_System\Requests_Management\Requests_Management.cpp" ^
	"main\Main_System\Requests_Management\Requests_Management_UI.cpp" ^
	"main\Main_System\Employee Management\Role_Management\Role_and_Salary_Details.cpp" ^
	"main\Main_System\Employee Management\Role_Management\Leave_and_Holiday.cpp" ^
	"main\Main_System\Employee Management\Role_Management\Department_Roles\Admin\Admin_Salary_Packages.cpp" ^
	"main\Main_System\Employee Management\Role_Management\Department_Roles\Admin\Admin_Salary_UI.cpp" ^
	"main\Main_System\Employee Management\Role_Management\Department_Roles\Finance\Finance_Salary_Packages.cpp" ^
	"main\Main_System\Employee Management\Role_Management\Department_Roles\Finance\Finance_Salary_UI.cpp" ^
	"main\Main_System\Employee Management\Role_Management\Department_Roles\Trade\Trade_Salary_Packages.cpp" ^
	"main\Main_System\Employee Management\Role_Management\Department_Roles\Trade\Trade_Salary_UI.cpp" ^
	"main\Main_System\UI\Account_UI\Admin_Dashboard_UI.cpp" ^
	"main\Main_System\UI\Account_UI\Employee_Dashboard_UI.cpp" ^
	"main\Main_System\UI\Account_UI\Edit_Accounts_UI.cpp" ^
	"main\Main_System\UI\Account_UI\Delete_Accounts_UI.cpp" ^
	"main\Main_System\UI\Account_UI\View_Accounts_UI.cpp" ^
	"main\Main_System\UI\Employee_Management_Menu_UI.cpp" ^
	"main\Main_System\UI\IRD_Tax_Record_Menu_UI.cpp" ^
	"main\Main_System\UI\Login_Menu_UI.cpp" ^
	"main\Main_System\UI\Shared_UI_Messaging\Form_Labels_UI.cpp" ^
	"main\Main_System\UI\Shared_UI_Messaging\Error_Messages.cpp" ^
	"main\Main_System\UI\Shared_UI_Messaging\Info_Messages.cpp" ^
	"main\Main_System\UI\Shared_UI_Messaging\Success_Messages.cpp" ^
	"main\Main_System\UI\Shared_UI_Messaging\Warning_Messages.cpp"

if errorlevel 1 (
	echo Build failed. Keeping object files for troubleshooting.
	popd
	endlocal
	exit /b 1
)

del /q *.obj >nul 2>&1
echo Build succeeded. Removed root-level object files.

popd
endlocal
