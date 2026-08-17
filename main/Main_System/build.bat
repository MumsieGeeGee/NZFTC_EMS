1. @echo off
2. setlocal
3. pushd "%~dp0"
4. 
5. where cl >nul 2>&1
6. if %errorlevel% neq 0 (
7. 	set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
8. 	if exist "%VSWHERE%" (
9. 		for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSINSTALL=%%I"
10. 	)
11. 
12. 	if defined VSINSTALL (
13. 		if exist "%VSINSTALL%\Common7\Tools\VsDevCmd.bat" (
14. 			call "%VSINSTALL%\Common7\Tools\VsDevCmd.bat" -no_logo -arch=x64 -host_arch=x64 >nul
15. 		)
16. 	)
17. )
18. 
19. where cl >nul 2>&1
20. if %errorlevel% neq 0 (
21. 	echo MSVC compiler not found. Install Visual Studio Build Tools with C++ or run this from a Developer Command Prompt.
22. 	popd
23. 	endlocal
24. 	exit /b 1
25. )
26. 
27. cl /std:c++17 /EHsc /I. ^
28. 	"NZFTC_EMS\main.cpp" ^
29. 	"NZFTC_EMS\Session_Handling\Session_Handling.cpp" ^
30. 	"Login\Login_Menu.cpp" ^
31. 	"Tax Calculator\Calculator\Calculator.cpp" ^
32. 	"Tax Calculator\Calculator\Main_Menu.cpp" ^
33. 	"Tax Calculator\Calculator\Other_Menus.cpp" ^
34. 	"Tax Calculator\Components\IRD_PAYE.cpp" ^
35. 	"Tax Calculator\Components\Tax_Codes.cpp" ^
36. 	"Tax Calculator\Components\ACC.cpp" ^
37. 	"Tax Calculator\Components\Student_Loan.cpp" ^
38. 	"Tax Calculator\Components\Child_Support.cpp" ^
39. 	"Tax Calculator\Components\Kiwisaver.cpp" ^
40. 	"Tax Calculator\Components\GST.cpp" ^
41. 	"Tax Calculator\Components\ESCT.cpp" ^
42. 	"UI\Shared_UI_Messaging\IRD_PAYE_UI.cpp" ^
43. 	"Account_Management\Account_Management\Save_Functions.cpp" ^
44. 	"Account_Management\Account_Management\Database.cpp" ^
45. 	"Account_Management\Account_Management\Input_Validation_Helpers.cpp" ^
46. 	"Account_Management\Account_Management\Security_Protocols.cpp" ^
47. 	"Account_Management\Account_Management\Save_Account_Details.cpp" ^
48. 	"Account_Management\Account_Management\Save_Payslip.cpp" ^
49. 	"Account_Management\Account_Creation\Create_Account.cpp" ^
50. 	"Account_Management\Account_Creation\View_Accounts.cpp" ^
51. 	"Account_Management\Account_Creation\Edit_Accounts.cpp" ^
52. 	"Account_Management\Account_Creation\Delete_Accounts.cpp" ^
53. 	"Account_Management\Account_Access\Account_Database.cpp" ^
54. 	"Account_Management\Account_Access\Component_Access.cpp" ^
55. 	"Account_Management\Account_Access\Admin_Dashboard.cpp" ^
56. 	"Account_Management\Account_Access\Employee_Dashboard.cpp" ^
57. 	"Account_Management\Account_Types\Admin_Accounts.cpp" ^
58. 	"Account_Management\Account_Types\Finance_Accounts.cpp" ^
59. 	"Account_Management\Account_Types\Trade_Accounts.cpp" ^
60. 	"Account_Form\Account_Creation_Form_Labels.cpp" ^
61. 	"Account_Form\Account_Creation_Form_UI.cpp" ^
62. 	"Employee Management\HR_Management\HR_Management.cpp" ^
63. 	"Employee Management\HR_Management\HR_Management_UI.cpp" ^
64. 	"Employee Management\HR_Management\Leave\Leave_Requests.cpp" ^
65. 	"Employee Management\HR_Management\Leave\Leave_Requests_UI.cpp" ^
66. 	"Employee Management\HR_Management\Grievances\Grievance_Form\Grievance_Form_UI.cpp" ^
67. 	"Employee Management\HR_Management\Grievances\Grievance_Form\Grievance_Service.cpp" ^
68. 	"Employee Management\HR_Management\Grievances\Grievance_Form\Grievance_Workflow.cpp" ^
69. 	"Employee Management\HR_Management\Grievances\Requests\Grievance_Requests.cpp" ^
70. 	"Employee Management\HR_Management\Payslips\Payslip_Generation.cpp" ^
71. 	"Employee Management\HR_Management\Payslips\Payslip_Generation_UI.cpp" ^
72. 	"Requests_Management\Requests_Management.cpp" ^
73. 	"Requests_Management\Requests_Management_UI.cpp" ^
74. 	"Employee Management\Role_Management\Role_and_Salary_Details.cpp" ^
75. 	"Employee Management\Role_Management\Leave_and_Holiday.cpp" ^
76. 	"Employee Management\Role_Management\Department_Roles\Admin\Admin_Salary_Packages.cpp" ^
77. 	"Employee Management\Role_Management\Department_Roles\Admin\Admin_Salary_UI.cpp" ^
78. 	"Employee Management\Role_Management\Department_Roles\Finance\Finance_Salary_Packages.cpp" ^
79. 	"Employee Management\Role_Management\Department_Roles\Finance\Finance_Salary_UI.cpp" ^
80. 	"Employee Management\Role_Management\Department_Roles\Trade\Trade_Salary_Packages.cpp" ^
81. 	"Employee Management\Role_Management\Department_Roles\Trade\Trade_Salary_UI.cpp" ^
82. 	"UI\Account_UI\Admin_Dashboard_UI.cpp" ^
83. 	"UI\Account_UI\Employee_Dashboard_UI.cpp" ^
84. 	"UI\Account_UI\Edit_Accounts_UI.cpp" ^
85. 	"UI\Account_UI\Delete_Accounts_UI.cpp" ^
86. 	"UI\Account_UI\View_Accounts_UI.cpp" ^
87. 	"UI\Employee_Management_Menu_UI.cpp" ^
88. 	"UI\IRD_Tax_Record_Menu_UI.cpp" ^
89. 	"UI\Login_Menu_UI.cpp" ^
90. 	"UI\Shared_UI_Messaging\Form_Labels_UI.cpp" ^
91. 	"UI\Shared_UI_Messaging\Error_Messages.cpp" ^
92. 	"U