# NZFTC EMS - Final Handover Notes

**Date:** August 26, 2026  
**Project:** NZFTC Employee Management System

---
## Recommended Handover Steps

For handover or marking, the simplest process is:

1. confirm the required VS Code development extension/tooling is installed
2. confirm MySQL Server is running
3. run `Database/mysql-schema.sql`
4. set `MYSQL_PASSWORD`
5. run:
   - `dotnet build`
   - `dotnet run`
6. open the web app
7. log in and review the main portal flows

Username and Password guide:

admin_slm_jane_sarah_doe 	 	Password1#
admin_jlm_barbara_bonnie_button  	Barbiedoll1#
admin_sit_john_david_doe  		Password2#
admin_jit_lily_evie_potter  		SnapeSucks1#
admin_hr_sirius_orion_black  		MoonyProngs1#
admin_pa_remus_james_lupin  		TheMoonSucks1#
admin_ea_dobby_socks_potter 		DobbyIsFree1#
admin_cc_hermione_jean_granger  	BooksAreCool1#
admin_r_avery_morgan_reed		HarbourDesk1#
admin_sc_morgan_elise_hart		KoruShift2#
admin_jc_casey_rowan_bell		TuiRoster3#
admin_st_jordan_mae_quinn		SilverFern4#
admin_jt_riley_noor_shaw		SouthernCross5#
admin_jg_taylor_lee_brooks		Pohutukawa6#
admin_sg_cameron_jade_wells		RimuOffice7#
finance_jg_alex_rose_carter		LedgerKoru16#
finance_sg_blake_june_foster		LedgerKoru17#
finance_jt_drew_kai_morgan		LedgerKoru18#
finance_st_eden_mae_parker		LedgerKoru19#
finance_jc_flynn_noor_hayes		LedgerKoru20#
finance_sc_grace_lee_turner		LedgerKoru21#
finance_pr_harper_quinn_scott		LedgerKoru22#
finance_ap_indie_rae_cooper		LedgerKoru23#
finance_ar_jamie_sky_bennett		LedgerKoru24#
finance_jfm_kendall_eve_murphy		LedgerKoru25#
finance_sfm_logan_jade_collins		LedgerKoru26#
trade_jg_micah_rose_ward		TradeKoru27#
trade_sg_nova_lee_price			TradeKoru28#
trade_jt_oakley_kai_hughes		TradeKoru29#
trade_st_piper_mae_richardson		TradeKoru30#
trade_jc_quinn_noor_bailey		TradeKoru31#
trade_sc_rowan_jade_kelly		TradeKoru32#
trade_app_sage_lee_watson		TradeKoru33#
trade_men_tatum_rae_gray		TradeKoru34#
trade_saf_urban_kai_james		TradeKoru35#
trade_sal_violet_eve_king		TradeKoru36#
trade_jtm_winter_mae_green		TradeKoru37#
trade_stm_zion_lee_baker		TradeKoru38#

---
## Overview
NZFTC EMS is an ASP.NET Core MVC web application for employee management.

Main areas included in the current system:
- login/logout and session handling
- admin and employee portal access
- account and employee management
- HR workflows
- leave requests
- grievance reporting
- payroll and tax-related management

The web application is intended to work across all major modern browsers such as:
- Microsoft Edge
- Google Chrome
- Mozilla Firefox
- Safari

It does not target legacy browsers such as Internet Explorer.

---
## What Is Needed to Run the Project
To review or demonstrate the project successfully, the following are needed:

1. **.NET SDK installed**
2. **VS Code development extension/tooling installed** for working on the project in the console
3. **MySQL Server installed and running**
4. the project database created from the included schema file
5. the MySQL password provided through an environment variable

### Important note about the development environment
For this project use the VS Code development tooling when working on the codebase in console/workspace mode.
The reason for this is that the compiler workflow for this project is not considered reliable from plain PowerShell alone in the intended handover setup, so the VS Code development extension/tooling should be installed before trying to work on the project.
MySQL Workbench is optional.  
It can be used to inspect the database, but the important requirement is that MySQL Server is available and the app can connect to it.

---
## MySQL Setup
The application is currently configured to use:
- **server:** `localhost`
- **database:** `NZFTC_EMS`
- **user:** `Admin1`

The database schema is included here:
- `Database/mysql-schema.sql`
Run that file first to create the database and required tables.

The MySQL password is not stored directly in the main connection string.  
It is provided through the `MYSQL_PASSWORD` environment variable.

Example Command Prompt usage:

```bat
set MYSQL_PASSWORD=your-mysql-password
dotnet build
dotnet run
```

Example PowerShell usage:

```powershell
$env:MYSQL_PASSWORD="your-mysql-password"
dotnet build
dotnet run
```

Relevant configuration files:
- `appsettings.json`
- `Utilities/Program.cs`

---