using Microsoft.AspNetCore.Mvc;
using NZFTC_EMS.Data;
using NZFTC_EMS.Data.Entities;
using NZFTC_EMS.Models;
using NZFTC_EMS.Services;
using NZFTC_EMS.Utilities;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;

namespace NZFTC_EMS.Controllers
{
    public class AdminController : PortalControllerBase
    {
        private const string ViewOwnAccountActionKey = "view-own-account-details";
        private const string EditOwnAccountActionKey = "edit-own-account-details";
        private const string ViewSelectedEmployeeAccountActionKey = "view-selected-employee-account-details";
        private const string EditSelectedEmployeeAccountActionKey = "edit-selected-employee-account-details";
        private const string CreateEmployeeAccountActionKey = "create-employee-account";
        private const string DeleteEmployeeAccountActionKey = "delete-employee-account";
        private const string ViewOwnEmployeeActionKey = "view-own-employee-details";
        private const string EditOwnEmployeeActionKey = "edit-own-employee-details";
        private const string ViewSelectedEmployeeRecordActionKey = "view-selected-employee-employee-details";
        private const string EditSelectedEmployeeRecordActionKey = "edit-selected-employee-employee-details";
        private const string ViewMyOpenGrievanceActionKey = "view-my-open-grievance-reports";
        private const string ViewMyResolvedGrievanceActionKey = "view-my-resolved-grievance-reports";
        private const string SubmitGrievanceActionKey = "submit-grievance-report";
        private const string ViewMyLeaveEntitlementActionKey = "view-my-leave-and-holiday-entitlements";
        private const string RequestLeaveActionKey = "request-leave";
        private const string ViewMyOpenLeaveRequestsActionKey = "view-my-open-leave-requests";
        private const string ViewMyTemporarilyDeniedLeaveRequestsActionKey = "view-my-temporarily-denied-leave-requests";
        private const string ViewMyResolvedLeaveRequestsActionKey = "view-my-resolved-leave-requests";
        private const string ViewMyPayslipsActionKey = "view-my-payslips";
        private const string ViewMyTaxInformationActionKey = "view-my-tax-information";
        private const string ViewUsersOpenGrievanceActionKey = "view-users-open-grievance-reports";
        private const string ViewUsersResolvedGrievanceActionKey = "view-users-resolved-grievance-reports";
        private const string ReviewGrievanceActionKey = "review-grievance-report";
        private const string ReviewLeaveRequestActionKey = "review-leave-request";
        private const string EditLeaveRequestActionKey = "edit-leave-request";
        private const string DeleteLeaveRequestActionKey = "delete-leave-request";
        private const string SubmitGrievanceOnBehalfOfUserActionKey = "submit-grievance-report-on-behalf-of-user";
        private const string ViewSelectedEmployeeLeaveEntitlementActionKey = "view-selected-employee-leave-and-holiday-entitlements";
        private const string RequestLeaveOnBehalfOfUserActionKey = "request-leave-on-behalf-of-user";
        private const string ViewSelectedEmployeeOpenLeaveRequestsActionKey = "view-selected-employee-open-leave-requests";
        private const string ViewSelectedEmployeeTemporarilyDeniedLeaveRequestsActionKey = "view-selected-employee-temporarily-denied-leave-requests";
        private const string ViewSelectedEmployeeResolvedLeaveRequestsActionKey = "view-selected-employee-resolved-leave-requests";
        private const string ViewSelectedEmployeePayslipsActionKey = "view-selected-employee-payslips";
        private const string ViewSelectedEmployeeTaxInformationActionKey = "view-selected-employee-tax-information";
        private const string EditSelectedEmployeeTaxInformationActionKey = "edit-selected-employee-tax-information";
        private const string CreatePayslipForEmployeeActionKey = "create-payslip-for-employee";
        private const string ReviewLeaveRequestPartialPath = "~/Views/Admin/HR_Management/Leave/Review_Leave_Request.cshtml";
        private const string EditLeaveRequestPartialPath = "~/Views/Admin/HR_Management/Leave/Edit_Leave_Request.cshtml";
        private const string DeleteLeaveRequestPartialPath = "~/Views/Admin/HR_Management/Leave/Delete_Leave_Request.cshtml";
        private const string ViewOwnAccountPartialPath = "~/Views/Employee/Account_Management/View_Own_Account_Details.cshtml";
        private const string EditOwnAccountPartialPath = "~/Views/Employee/Account_Management/Edit_Own_Account_Details.cshtml";
        private const string ViewSelectedEmployeeAccountPartialPath = "~/Views/Admin/Account_Management/View_Selected_Employee_Account_Details.cshtml";
        private const string EditSelectedEmployeeAccountPartialPath = "~/Views/Admin/Account_Management/Edit_Selected_Employee_Account_Details.cshtml";
        private const string CreateEmployeeAccountPartialPath = "~/Views/Admin/Account_Management/Create_New_Account.cshtml";
        private const string DeleteEmployeeAccountPartialPath = "~/Views/Admin/Account_Management/Delete_an_Account.cshtml";
        private const string ViewOwnEmployeePartialPath = "~/Views/Employee/Employee_Management/View_Own_Employee_Details.cshtml";
        private const string EditOwnEmployeePartialPath = "~/Views/Employee/Employee_Management/Edit_Own_Employee_Details.cshtml";
        private const string ViewSelectedEmployeeRecordPartialPath = "~/Views/Admin/Employee_Management/View_Selected_Employee_Employee_Details.cshtml";
        private const string EditSelectedEmployeeRecordPartialPath = "~/Views/Admin/Employee_Management/Edit_Selected_Employee_Employee_Details.cshtml";
        private const string ViewMyOpenGrievancePartialPath = "~/Views/Employee/HR_Management/Grievance/View_My_Open_Grievance_Reports.cshtml";
        private const string ViewMyResolvedGrievancePartialPath = "~/Views/Employee/HR_Management/Grievance/View_My_Resolved_Grievance_Reports.cshtml";
        private const string SubmitGrievancePartialPath = "~/Views/Employee/HR_Management/Grievance/Submit_Grievance_Report.cshtml";
        private const string SubmitGrievanceOnBehalfPartialPath = "~/Views/Admin/HR_Management/Grievance/Submit_Grievance_Report_On_Behalf_Of_Employee.cshtml";
        private const string ViewUsersOpenGrievancePartialPath = "~/Views/Admin/HR_Management/Grievance/View_Users_Open_Grievance_Reports.cshtml";
        private const string ViewUsersResolvedGrievancePartialPath = "~/Views/Admin/HR_Management/Grievance/View_Users_Resolved_Grievance_Reports.cshtml";
        private const string ViewSelectedEmployeeLeaveEntitlementPartialPath = "~/Views/Admin/HR_Management/Leave/View_Selected_Employee_Leave_And_Holiday_Entitlements.cshtml";
        private const string RequestLeaveOnBehalfOfUserPartialPath = "~/Views/Admin/HR_Management/Leave/Request_Leave_On_Behalf_Of_Employee.cshtml";
        private const string ViewSelectedEmployeeOpenLeaveRequestsPartialPath = "~/Views/Admin/HR_Management/Leave/View_Selected_Employee_Open_Leave_Requests.cshtml";
        private const string ViewSelectedEmployeeTemporarilyDeniedLeaveRequestsPartialPath = "~/Views/Admin/HR_Management/Leave/View_Selected_Employee_Temporarily_Denied_Leave_Requests.cshtml";
        private const string ViewSelectedEmployeeResolvedLeaveRequestsPartialPath = "~/Views/Admin/HR_Management/Leave/View_Selected_Employee_Resolved_Leave_Requests.cshtml";
        private const string ViewSelectedEmployeePayslipsPartialPath = "~/Views/Admin/HR_Management/PAYE/View_Selected_Employee_Payslips.cshtml";
        private const string ViewSelectedEmployeeTaxInformationPartialPath = "~/Views/Admin/HR_Management/PAYE/View_Selected_Employee_Tax_Information.cshtml";
        private const string EditSelectedEmployeeTaxInformationPartialPath = "~/Views/Admin/HR_Management/PAYE/Edit_Selected_Employee_Tax_Information.cshtml";
        private const string CreatePayslipPartialPath = "~/Views/Admin/HR_Management/PAYE/Create_Payslip.cshtml";
        private const string ViewMyLeaveEntitlementPartialPath = "~/Views/Employee/HR_Management/Leave/View_Own_Leave_And_Holiday_Entitlements.cshtml";
        private const string RequestLeavePartialPath = "~/Views/Employee/HR_Management/Leave/Request_Leave.cshtml";
        private const string ViewMyOpenLeaveRequestsPartialPath = "~/Views/Employee/HR_Management/Leave/View_Own_Open_Leave_Requests.cshtml";
        private const string ViewMyTemporarilyDeniedLeaveRequestsPartialPath = "~/Views/Employee/HR_Management/Leave/View_Own_Temporarily_Denied_Leave_Requests.cshtml";
        private const string ViewMyResolvedLeaveRequestsPartialPath = "~/Views/Employee/HR_Management/Leave/View_Own_Resolved_Leave_Requests.cshtml";
        private const string ViewMyPayslipsPartialPath = "~/Views/Employee/HR_Management/PAYE/View_Own_Payslips.cshtml";
        private const string ViewMyTaxInformationPartialPath = "~/Views/Employee/HR_Management/PAYE/View_Own_Tax_Information.cshtml";

        private readonly EmployeeAccountRecordService _employeeAccountRecordService;
        private readonly GrievanceRequestService _grievanceRequestService;
        private readonly LeaveRequestService _leaveRequestService;
        private readonly PasswordResetRequestService _passwordResetRequestService;
        private readonly MySqlRepository _mySqlRepository;
        private readonly PayrollService _payrollService;

        public AdminController(
            EmployeeAccountRecordService employeeAccountRecordService,
            GrievanceRequestService grievanceRequestService,
            LeaveRequestService leaveRequestService,
            PasswordResetRequestService passwordResetRequestService,
            MySqlRepository mySqlRepository,
            PayrollService payrollService)
        {
            _employeeAccountRecordService = employeeAccountRecordService;
            _grievanceRequestService = grievanceRequestService;
            _leaveRequestService = leaveRequestService;
            _passwordResetRequestService = passwordResetRequestService;
            _mySqlRepository = mySqlRepository;
            _payrollService = payrollService;
        }

        public async Task<IActionResult> Dashboard()
        {
            var authResult = PrepareAccessContext(AccessProfileSessionHelper.IsAdminPortalProfile);
            if (!authResult.IsAuthenticated)
            {
                return RedirectToAction("Login", "Login");
            }
            if (!authResult.IsPortalAllowed)
            {
                return RedirectToPermittedDashboard(authResult.AccountType);
            }
            SetSectionMenuOptions("dashboard");
            var username = HttpContext.Session.GetString("Username") ?? string.Empty;
            var dashboardAccessProfile = ViewBag.AccessProfile as AccessProfile;
            var notifications = _grievanceRequestService.GetNotificationsForUser(username)
                .Where(notification =>
                {
                    if (notification.IsCaseUpdate)
                    {
                        return true;
                    }

                    var grievance = _grievanceRequestService.GetById(notification.GrievanceId);
                    return grievance != null &&
                           dashboardAccessProfile != null &&
                           CanReviewGrievance(dashboardAccessProfile, username, grievance);
                })
                .Select(notification => new DashboardNotification
                {
                    Category = "Grievance",
                    Title = notification.IsCaseUpdate ? "Your grievance update" : "Grievance review needed",
                    Message = notification.IsCaseUpdate
                        ? notification.Message
                        : "A grievance requires attention.",
                    Tone = notification.IsCaseUpdate ? "info" : "warning",
                    LinkUrl = notification.IsCaseUpdate
                        ? null
                        : Url.Action(nameof(ReviewGrievanceReport), "Admin", new { grievanceId = notification.GrievanceId }),
                    LinkText = notification.IsCaseUpdate ? null : "Review grievance report",
                    CreatedOn = notification.CreatedOn
                })
                .ToList();
            notifications.AddRange(_leaveRequestService.GetDashboardNotificationsForUser(username));
            notifications.AddRange(_passwordResetRequestService.GetDashboardNotificationsForUser(username));
            if (dashboardAccessProfile != null && CanManageHrOnBehalf(dashboardAccessProfile))
            {
                notifications.AddRange(
                    _leaveRequestService.GetOpenRequestsForTeam()
                        .Where(request => CanReviewLeaveRequest(dashboardAccessProfile, username, request))
                        .Take(6)
                        .Select(request => new DashboardNotification
                        {
                            Category = "Leave",
                            Title = "Leave request awaiting review",
                            Message = $"{request.EmployeeName} submitted {request.LeaveType.ToLowerInvariant()} from {request.StartDate:dd MMM yyyy} to {request.EndDate:dd MMM yyyy}.",
                            Tone = "pending",
                            LinkText = "Review leave request",
                            LinkUrl = Url.Action(nameof(HRManagement), "Admin", new { actionKey = ReviewLeaveRequestActionKey, leaveRequestId = request.Id }),
                            CreatedOn = request.UpdatedOn ?? request.RequestedOn
                        }));
            }
            if (dashboardAccessProfile != null && dashboardAccessProfile.CanManageAllAccounts)
            {
                notifications.AddRange(
                    _employeeAccountRecordService.GetLockedAccounts()
                        .Where(account => CanManageAnotherEmployee(dashboardAccessProfile, username, account.Username))
                        .Take(4)
                        .Select(account => new DashboardNotification
                        {
                            Category = "Account",
                            Title = "Locked account needs review",
                            Message = $"{account.DisplayName} is currently locked and can be unlocked from account management.",
                            Tone = "warning",
                            LinkText = "Open account details",
                            LinkUrl = Url.Action(
                                nameof(AccountManagement),
                                "Admin",
                                new
                                {
                                    actionKey = EditSelectedEmployeeAccountActionKey,
                                    group = account.GroupName,
                                    username = account.Username
                                }),
                            CreatedOn = DateTime.UtcNow
                        }));
            }
            if (_employeeAccountRecordService.TryGetAccountDetails(username, out var accountDetails)
                && accountDetails.TryGetValue("Account Status", out var accountStatus)
                && string.Equals(accountStatus.Trim(), "Locked", StringComparison.OrdinalIgnoreCase))
            {
                notifications.Add(new DashboardNotification
                {
                    Category = "Account",
                    Title = "Account locked",
                    Message = "Your account is locked. Contact HR or support before continuing.",
                    Tone = "critical",
                    LinkText = "Open account details",
                    LinkUrl = Url.Action("AccountManagement", "Admin"),
                    CreatedOn = DateTime.UtcNow
                });
            }
            ViewBag.DashboardNotifications = notifications
                .OrderByDescending(notification => notification.CreatedOn)
                .Take(6)
                .ToList();

            try
            {
                ViewBag.MySqlStatus = await _mySqlRepository.GetStatusSnapshotAsync();
            }
            catch (Exception ex)
            {
                ViewBag.MySqlStatusError = ex.Message;
            }

            return View();
        }

        public IActionResult AccountManagement()
        {
            var authResult = PrepareAccessContext(AccessProfileSessionHelper.IsAdminPortalProfile);
            if (!authResult.IsAuthenticated)
            {
                return RedirectToAction("Login", "Login");
            }
            if (!authResult.IsPortalAllowed)
            {
                return RedirectToPermittedDashboard(authResult.AccountType);
            }
            if (!authResult.CanAccessAccount)
            {
                return RedirectToAction(nameof(Dashboard));
            }
            SetSectionMenuOptions("account");
            ViewBag.AccountManagementContentEndpoint = Url.Action(nameof(AccountManagementContent), "Admin");

            return View("~/Views/Admin/Account_Management/Account_Management.cshtml");
        }

        [HttpGet]
        public IActionResult AccountManagementContent(string actionKey, string? group, string? username)
        {
            var sessionUsername = HttpContext.Session.GetString("Username");
            var accountType = HttpContext.Session.GetString("AccountType");
            var irdNumber = HttpContext.Session.GetString("IRDNumber");
            var loginTime = HttpContext.Session.GetString("LoginTime");

            if (string.IsNullOrWhiteSpace(sessionUsername) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return Unauthorized();
            }

            var moduleAccess = AccessProfileSessionHelper.GetModuleAccess(accessProfile);
            if (!moduleAccess.CanAccessAccount)
            {
                return Forbid();
            }

            if (actionKey == ViewOwnAccountActionKey || actionKey == EditOwnAccountActionKey)
            {
                var personalModel = BuildPersonalAccountDetailsModel(sessionUsername, accountType, irdNumber, loginTime);
                return actionKey == ViewOwnAccountActionKey
                    ? PartialView(ViewOwnAccountPartialPath, personalModel)
                    : PartialView(EditOwnAccountPartialPath, personalModel);
            }

            if (actionKey == ViewSelectedEmployeeAccountActionKey)
            {
                if (!accessProfile.CanManageAllAccounts)
                {
                    return Forbid();
                }

                var model = BuildSelectedEmployeeAccountModel(group, username);
                return PartialView(ViewSelectedEmployeeAccountPartialPath, model);
            }

            if (actionKey == EditSelectedEmployeeAccountActionKey)
            {
                if (!accessProfile.CanManageAllAccounts)
                {
                    return Forbid();
                }

                var model = BuildSelectedEmployeeAccountModel(group, username);
                return PartialView(EditSelectedEmployeeAccountPartialPath, model);
            }

            if (actionKey == CreateEmployeeAccountActionKey)
            {
                if (!accessProfile.CanManageAllAccounts)
                {
                    return Forbid();
                }

                return PartialView(CreateEmployeeAccountPartialPath);
            }

            if (actionKey == DeleteEmployeeAccountActionKey)
            {
                if (!accessProfile.CanManageAllAccounts)
                {
                    return Forbid();
                }

                var model = BuildSelectedEmployeeAccountModel(group, username);
                return PartialView(DeleteEmployeeAccountPartialPath, model);
            }

            return NotFound();
        }

        [HttpGet]
        public IActionResult EmployeeManagementContent(string actionKey, string? group, string? username)
        {
            var sessionUsername = HttpContext.Session.GetString("Username");
            var accountType = HttpContext.Session.GetString("AccountType");
            var irdNumber = HttpContext.Session.GetString("IRDNumber");
            var loginTime = HttpContext.Session.GetString("LoginTime");

            if (string.IsNullOrWhiteSpace(sessionUsername) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return Unauthorized();
            }

            var moduleAccess = AccessProfileSessionHelper.GetModuleAccess(accessProfile);
            if (!moduleAccess.CanAccessEmployee)
            {
                return Forbid();
            }

            if (actionKey == ViewOwnEmployeeActionKey || actionKey == EditOwnEmployeeActionKey)
            {
                var personalModel = BuildEmployeeRecordDetailsModel(sessionUsername);
                return actionKey == ViewOwnEmployeeActionKey
                    ? PartialView(ViewOwnEmployeePartialPath, personalModel)
                    : PartialView(EditOwnEmployeePartialPath, personalModel);
            }

            if (actionKey == ViewSelectedEmployeeRecordActionKey)
            {
                if (!accessProfile.CanManageAllEmployees && !accessProfile.CanManageAllHr)
                {
                    return Forbid();
                }

                var model = BuildSelectedEmployeeRecordModel(group, username, nameof(EmployeeManagementContent));
                return PartialView(ViewSelectedEmployeeRecordPartialPath, model);
            }

            if (actionKey == EditSelectedEmployeeRecordActionKey)
            {
                if (!accessProfile.CanManageAllEmployees)
                {
                    return Forbid();
                }

                var model = BuildSelectedEmployeeRecordModel(group, username, nameof(EmployeeManagementContent));
                return PartialView(EditSelectedEmployeeRecordPartialPath, model);
            }

            return NotFound();
        }

        public IActionResult EmployeeManagement()
        {
            var authResult = PrepareAccessContext(AccessProfileSessionHelper.IsAdminPortalProfile);
            if (!authResult.IsAuthenticated)
            {
                return RedirectToAction("Login", "Login");
            }
            if (!authResult.IsPortalAllowed)
            {
                return RedirectToPermittedDashboard(authResult.AccountType);
            }
            if (!authResult.CanAccessEmployee)
            {
                return RedirectToAction(nameof(Dashboard));
            }
            SetSectionMenuOptions("employee");
            ViewBag.EmployeeManagementContentEndpoint = Url.Action(nameof(EmployeeManagementContent), "Admin");

            return View("~/Views/Admin/Employee_Management/Employee_Management.cshtml");
        }

        public IActionResult HRManagement()
        {
            var authResult = PrepareAccessContext(AccessProfileSessionHelper.IsAdminPortalProfile);
            if (!authResult.IsAuthenticated)
            {
                return RedirectToAction("Login", "Login");
            }
            if (!authResult.IsPortalAllowed)
            {
                return RedirectToPermittedDashboard(authResult.AccountType);
            }
            if (!authResult.CanAccessHr)
            {
                return RedirectToAction(nameof(Dashboard));
            }
            SetSectionMenuOptions("hr");
            ViewBag.HRManagementContentEndpoint = Url.Action(nameof(HRManagementContent), "Admin");

            return View("~/Views/Admin/HR_Management/HR_Management.cshtml");
        }

        [HttpGet]
        public IActionResult HRManagementContent(string actionKey, string? group, string? username, int? leaveRequestId = null, int? grievanceId = null)
        {
            var sessionUsername = HttpContext.Session.GetString("Username");
            if (string.IsNullOrWhiteSpace(sessionUsername) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return Unauthorized();
            }

            var moduleAccess = AccessProfileSessionHelper.GetModuleAccess(accessProfile);
            if (!moduleAccess.CanAccessHr)
            {
                return Forbid();
            }

            switch (actionKey)
            {
                case ViewMyOpenGrievanceActionKey:
                    return PartialView(ViewMyOpenGrievancePartialPath, _grievanceRequestService.GetOpenReportsForUser(sessionUsername));
                case ViewMyResolvedGrievanceActionKey:
                    return PartialView(ViewMyResolvedGrievancePartialPath, _grievanceRequestService.GetResolvedReportsForUser(sessionUsername));
                case SubmitGrievanceActionKey:
                    return PartialView(SubmitGrievancePartialPath, new GrievanceRequest { SubmittedByUsername = sessionUsername, SubmittedForUsername = sessionUsername });
                case ViewMyLeaveEntitlementActionKey:
                    return PartialView(
                        ViewMyLeaveEntitlementPartialPath,
                        _leaveRequestService.GetEntitlementForRole(sessionUsername, accessProfile.BusinessRole, accessProfile.JobRole));
                case RequestLeaveActionKey:
                    return PartialView(RequestLeavePartialPath, new LeaveRequest { SubmittedByUsername = sessionUsername, SubmittedForUsername = sessionUsername });
                case ViewMyOpenLeaveRequestsActionKey:
                    return PartialView(ViewMyOpenLeaveRequestsPartialPath, _leaveRequestService.GetOpenRequestsForUser(sessionUsername));
                case ViewMyTemporarilyDeniedLeaveRequestsActionKey:
                    return PartialView(ViewMyTemporarilyDeniedLeaveRequestsPartialPath, _leaveRequestService.GetTemporarilyDeniedRequestsForUser(sessionUsername));
                case ViewMyResolvedLeaveRequestsActionKey:
                    return PartialView(ViewMyResolvedLeaveRequestsPartialPath, _leaveRequestService.GetResolvedRequestsForUser(sessionUsername));
                case ViewMyPayslipsActionKey:
                    return PartialView(ViewMyPayslipsPartialPath, _payrollService.GetPayslipsForUser(sessionUsername));
                case ViewMyTaxInformationActionKey:
                    return PartialView(ViewMyTaxInformationPartialPath, _payrollService.GetTaxInformationForUser(sessionUsername));
                case ViewUsersOpenGrievanceActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    return PartialView(
                        ViewUsersOpenGrievancePartialPath,
                        _grievanceRequestService.GetOpenReportsForTeam()
                            .Where(report => CanReviewGrievance(accessProfile, sessionUsername, report))
                            .ToList());
                case ViewUsersResolvedGrievanceActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    return PartialView(
                        ViewUsersResolvedGrievancePartialPath,
                        _grievanceRequestService.GetResolvedReportsForTeam()
                            .Where(report => CanReviewGrievance(accessProfile, sessionUsername, report))
                            .ToList());
                case SubmitGrievanceOnBehalfOfUserActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    return PartialView(SubmitGrievanceOnBehalfPartialPath, BuildSelectedEmployeeActionTargetModel(group, username));
                case ViewSelectedEmployeeLeaveEntitlementActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    return PartialView(
                        ViewSelectedEmployeeLeaveEntitlementPartialPath,
                        BuildSelectedEmployeeLeaveModel(group, username));
                case RequestLeaveOnBehalfOfUserActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    return PartialView(RequestLeaveOnBehalfOfUserPartialPath, BuildSelectedEmployeeActionTargetModel(group, username));
                case ViewSelectedEmployeeOpenLeaveRequestsActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    return PartialView(
                        ViewSelectedEmployeeOpenLeaveRequestsPartialPath,
                        BuildSelectedEmployeeLeaveRequestsModel(group, username, _leaveRequestService.GetOpenRequestsForTeam, ViewSelectedEmployeeOpenLeaveRequestsActionKey));
                case ViewSelectedEmployeeTemporarilyDeniedLeaveRequestsActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    return PartialView(
                        ViewSelectedEmployeeTemporarilyDeniedLeaveRequestsPartialPath,
                        BuildSelectedEmployeeLeaveRequestsModel(group, username, _leaveRequestService.GetTemporarilyDeniedRequestsForTeam, ViewSelectedEmployeeTemporarilyDeniedLeaveRequestsActionKey));
                case ViewSelectedEmployeeResolvedLeaveRequestsActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    return PartialView(
                        ViewSelectedEmployeeResolvedLeaveRequestsPartialPath,
                        BuildSelectedEmployeeLeaveRequestsModel(group, username, _leaveRequestService.GetResolvedRequestsForTeam, ViewSelectedEmployeeResolvedLeaveRequestsActionKey));
                case ReviewLeaveRequestActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    if (!leaveRequestId.HasValue)
                    {
                        return BadRequest();
                    }
                    var leaveRequest = _leaveRequestService.GetById(leaveRequestId.Value);
                    if (leaveRequest == null)
                    {
                        return NotFound();
                    }
                    if (!CanReviewLeaveRequest(accessProfile, sessionUsername, leaveRequest))
                    {
                        return Forbid();
                    }
                    return PartialView(ReviewLeaveRequestPartialPath, leaveRequest);
                case EditLeaveRequestActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    if (!leaveRequestId.HasValue)
                    {
                        return BadRequest();
                    }
                    var leaveRequestToEdit = _leaveRequestService.GetById(leaveRequestId.Value);
                    if (leaveRequestToEdit == null)
                    {
                        return NotFound();
                    }
                    if (!CanManageAnotherEmployee(accessProfile, sessionUsername, leaveRequestToEdit.SubmittedForUsername))
                    {
                        return Forbid();
                    }
                    return PartialView(EditLeaveRequestPartialPath, leaveRequestToEdit);
                case DeleteLeaveRequestActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    if (!leaveRequestId.HasValue)
                    {
                        return BadRequest();
                    }
                    var leaveRequestToDelete = _leaveRequestService.GetById(leaveRequestId.Value);
                    if (leaveRequestToDelete == null)
                    {
                        return NotFound();
                    }
                    if (!CanManageAnotherEmployee(accessProfile, sessionUsername, leaveRequestToDelete.SubmittedForUsername))
                    {
                        return Forbid();
                    }
                    return PartialView(DeleteLeaveRequestPartialPath, leaveRequestToDelete);
                case ViewSelectedEmployeePayslipsActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    var targetUserForPayslips = NormalizeManagedTargetUsername(accessProfile, sessionUsername, username);
                    return PartialView(
                        ViewSelectedEmployeePayslipsPartialPath,
                        string.IsNullOrWhiteSpace(targetUserForPayslips)
                            ? new List<PayslipRecord>()
                            : _payrollService.GetPayslipsForUser(targetUserForPayslips));
                case ViewSelectedEmployeeTaxInformationActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    var targetUserForTax = NormalizeManagedTargetUsername(accessProfile, sessionUsername, username);
                    return PartialView(
                        ViewSelectedEmployeeTaxInformationPartialPath,
                        string.IsNullOrWhiteSpace(targetUserForTax)
                            ? new PayrollTaxInformation()
                            : _payrollService.GetTaxInformationForUser(targetUserForTax));
                case EditSelectedEmployeeTaxInformationActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    var targetUserForEditTax = NormalizeManagedTargetUsername(accessProfile, sessionUsername, username);
                    return PartialView(
                        EditSelectedEmployeeTaxInformationPartialPath,
                        string.IsNullOrWhiteSpace(targetUserForEditTax)
                            ? new PayrollTaxInformation()
                            : _payrollService.GetTaxInformationForUser(targetUserForEditTax));
                case CreatePayslipForEmployeeActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    return PartialView(CreatePayslipPartialPath, BuildSelectedEmployeePayslipModel(group, username));
                case ReviewGrievanceActionKey:
                    if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                    {
                        return Forbid();
                    }
                    if (!grievanceId.HasValue)
                    {
                        return BadRequest();
                    }
                    var grievanceRecord = _grievanceRequestService.GetById(grievanceId.Value);
                    if (grievanceRecord == null)
                    {
                        return NotFound();
                    }

                    if (!CanReviewGrievance(accessProfile, sessionUsername, grievanceRecord))
                    {
                        return Forbid();
                    }

                    return PartialView("~/Views/Admin/HR_Management/Grievance/Review_Grievance_Report.cshtml", grievanceRecord);
                default:
                    return NotFound();
            }
        }

        [HttpGet]
        public IActionResult ViewPayslip(int payslipId)
        {
            var sessionUsername = HttpContext.Session.GetString("Username");
            if (string.IsNullOrWhiteSpace(sessionUsername) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return RedirectToAction("Login", "Login");
            }

            if (!AccessProfileSessionHelper.IsAdminPortalProfile(accessProfile) ||
                !CanManageHrOnBehalf(accessProfile))
            {
                return Forbid();
            }

            var payslip = _payrollService.GetPayslipById(payslipId);
            if (payslip == null)
            {
                return NotFound();
            }

            if (!CanAccessManagedPayslip(accessProfile, sessionUsername, payslip))
            {
                return Forbid();
            }

            return View("~/Views/Shared/PAYE/View_Payslip_Details.cshtml", payslip);
        }

        [HttpGet]
        public IActionResult DownloadPayslip(int payslipId)
        {
            var sessionUsername = HttpContext.Session.GetString("Username");
            if (string.IsNullOrWhiteSpace(sessionUsername) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return RedirectToAction("Login", "Login");
            }

            if (!AccessProfileSessionHelper.IsAdminPortalProfile(accessProfile) ||
                !CanManageHrOnBehalf(accessProfile))
            {
                return Forbid();
            }

            var payslip = _payrollService.GetPayslipById(payslipId);
            if (payslip == null)
            {
                return NotFound();
            }

            if (!CanAccessManagedPayslip(accessProfile, sessionUsername, payslip))
            {
                return Forbid();
            }

            var exportText = _payrollService.GetPayslipExportText(payslipId);
            if (string.IsNullOrWhiteSpace(exportText))
            {
                return NotFound();
            }

            return File(Encoding.UTF8.GetBytes(exportText), "text/plain", $"Payslip_{payslip.Id:000000}.txt");
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult SubmitGrievanceReport(
            string submittedForUsername,
            string attendingSupervisor,
            string personsInvolved,
            string eventDate,
            string eventDetails,
            string affectDetails,
            string? suggestions,
            string? additionalComments,
            string declarationAgreement,
            string signedDate)
        {
            var sessionUsername = HttpContext.Session.GetString("Username");
            if (string.IsNullOrWhiteSpace(sessionUsername) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return RedirectToAction("Login", "Login");
            }

            if (!CanManageHrOnBehalf(accessProfile))
            {
                return Forbid();
            }

            var targetUsername = NormalizeManagedTargetUsername(accessProfile, sessionUsername, submittedForUsername);
            if (string.IsNullOrWhiteSpace(targetUsername))
            {
                TempData["GrievanceError"] = "Please select another employee before submitting a grievance report on behalf of an employee.";
                return RedirectToAction(nameof(HRManagement));
            }

            if (string.IsNullOrWhiteSpace(submittedForUsername) ||
                string.IsNullOrWhiteSpace(attendingSupervisor) ||
                string.IsNullOrWhiteSpace(personsInvolved) ||
                string.IsNullOrWhiteSpace(eventDate) ||
                string.IsNullOrWhiteSpace(eventDetails) ||
                string.IsNullOrWhiteSpace(affectDetails) ||
                string.IsNullOrWhiteSpace(declarationAgreement) ||
                string.IsNullOrWhiteSpace(signedDate))
            {
                TempData["GrievanceError"] = "Please complete the grievance complaint form before submitting it.";
                return RedirectToAction(nameof(HRManagement));
            }

            if (!string.Equals(declarationAgreement.Trim(), "I do", StringComparison.OrdinalIgnoreCase))
            {
                TempData["GrievanceError"] = "You must enter 'I do' to confirm the complaint declaration.";
                return RedirectToAction(nameof(HRManagement));
            }

            var description = BuildComplaintDescription(
                attendingSupervisor,
                personsInvolved,
                eventDate,
                eventDetails,
                affectDetails,
                suggestions,
                additionalComments,
                declarationAgreement,
                signedDate);

            _grievanceRequestService.CreateReport(sessionUsername, targetUsername, "NZFTC Employee Complaint Form", description);
            TempData["GrievanceSuccess"] = "Your grievance report has been submitted successfully.";
            return RedirectToAction(nameof(HRManagement));
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult SubmitLeaveRequestOnBehalfOfUser(string leaveType, DateTime? startDate, DateTime? endDate, string reason, string? submittedForUsername)
        {
            var sessionUsername = HttpContext.Session.GetString("Username");
            if (string.IsNullOrWhiteSpace(sessionUsername) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return RedirectToAction("Login", "Login");
            }

            if (!CanManageHrOnBehalf(accessProfile))
            {
                return Forbid();
            }

            var targetUsername = NormalizeManagedTargetUsername(accessProfile, sessionUsername, submittedForUsername);
            if (string.IsNullOrWhiteSpace(targetUsername))
            {
                TempData["LeaveError"] = "Please select another employee before submitting a leave request on behalf of an employee.";
                return RedirectToAction(nameof(HRManagement));
            }

            var requestedStart = startDate ?? DateTime.Today;
            var requestedEnd = endDate ?? requestedStart.AddDays(1);

            if (requestedEnd < requestedStart)
            {
                TempData["LeaveError"] = "The leave end date must be on or after the start date.";
                return RedirectToAction(nameof(HRManagement));
            }

            if (!LeaveRequestService.CanRequestLeaveType(leaveType))
            {
                TempData["LeaveError"] = "Public holidays are statutory entitlements and are not submitted as leave requests.";
                return RedirectToAction(nameof(HRManagement));
            }

            var request = _leaveRequestService.CreateRequest(sessionUsername, targetUsername, leaveType, requestedStart, requestedEnd, reason);
            var employeeName = string.IsNullOrWhiteSpace(request.EmployeeName) ? targetUsername : request.EmployeeName;
            TempData["LeaveSuccess"] = $"Leave request submitted for {employeeName}: {request.LeaveType} from {request.StartDate:dd MMM yyyy} to {request.EndDate:dd MMM yyyy} ({request.TotalDays} days).";
            return RedirectToAction(nameof(HRManagement));
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult CreatePayslipForEmployee(
            string? submittedForUsername,
            decimal basePay,
            decimal overtimePay,
            decimal bonusPay,
            decimal allowancePay,
            decimal preTaxDeductions,
            decimal postTaxDeductions,
            string? payPeriod,
            string? payFrequency)
        {
            var sessionUsername = HttpContext.Session.GetString("Username");
            if (string.IsNullOrWhiteSpace(sessionUsername) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return RedirectToAction("Login", "Login");
            }

            if (!CanManageHrOnBehalf(accessProfile))
            {
                return Forbid();
            }

            var targetUsername = NormalizeManagedTargetUsername(accessProfile, sessionUsername, submittedForUsername);
            if (string.IsNullOrWhiteSpace(targetUsername))
            {
                TempData["PayrollError"] = "Please select another employee before creating a payslip.";
                return RedirectToAction(nameof(HRManagement));
            }

            _payrollService.CreatePayslip(
                sessionUsername,
                targetUsername,
                basePay,
                overtimePay,
                bonusPay,
                allowancePay,
                preTaxDeductions,
                postTaxDeductions,
                payPeriod ?? "Current period",
                payFrequency ?? "Fortnightly");
            TempData["PayrollSuccess"] = "The payslip has been created successfully.";
            return RedirectToAction(nameof(HRManagement));
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult EditSelectedEmployeeTaxInformation(PayrollTaxInformation updatedTaxInfo)
        {
            var sessionUsername = HttpContext.Session.GetString("Username");
            if (string.IsNullOrWhiteSpace(sessionUsername) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return RedirectToAction("Login", "Login");
            }

            if (!CanManageHrOnBehalf(accessProfile))
            {
                return Forbid();
            }

            if (string.IsNullOrWhiteSpace(updatedTaxInfo.Username))
            {
                TempData["PayrollError"] = "A target employee must be selected before editing tax information.";
                return RedirectToAction(nameof(HRManagement));
            }

            if (!CanManageAnotherEmployee(accessProfile, sessionUsername, updatedTaxInfo.Username))
            {
                TempData["PayrollError"] = "You cannot use the employee tax management flow for your own record.";
                return RedirectToAction(nameof(HRManagement));
            }

            if (!_payrollService.UpdateTaxInformation(updatedTaxInfo.Username, updatedTaxInfo))
            {
                TempData["PayrollError"] = "The employee tax information could not be updated.";
                return RedirectToAction(nameof(HRManagement));
            }

            TempData["PayrollSuccess"] = "The employee tax information has been updated successfully.";
            return RedirectToAction(nameof(HRManagement));
        }

        [HttpGet]
        public IActionResult ReviewGrievanceReport(int grievanceId)
            {
                var sessionUsername = HttpContext.Session.GetString("Username");
                if (string.IsNullOrWhiteSpace(sessionUsername) ||
                    !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
                {
                    return RedirectToAction("Login", "Login");
                }

                if (!CanManageHrOnBehalf(accessProfile))
                {
                    return Forbid();
                }

                var grievance = _grievanceRequestService.GetById(grievanceId);
                if (grievance == null)
                {
                    TempData["GrievanceError"] = "The selected grievance report could not be found.";
                    return RedirectToAction(nameof(HRManagement));
                }

                if (!CanReviewGrievance(accessProfile, sessionUsername, grievance))
                {
                    TempData["GrievanceError"] = "You cannot review your own grievance report.";
                    return RedirectToAction(nameof(HRManagement));
                }

                return RedirectToAction(nameof(HRManagement), new
                {
                    actionKey = ReviewGrievanceActionKey,
                    grievanceId
                });
            }

            [HttpPost]
            [ValidateAntiForgeryToken]
            public IActionResult UpdateGrievanceStatus(int grievanceId, string status, string outcome, string decisionReason)
            {
                var sessionUsername = HttpContext.Session.GetString("Username");
                if (string.IsNullOrWhiteSpace(sessionUsername) ||
                    !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
                {
                    return RedirectToAction("Login", "Login");
                }

                if (!CanManageHrOnBehalf(accessProfile))
                {
                    return Forbid();
                }

                var currentGrievance = _grievanceRequestService.GetById(grievanceId);
                if (currentGrievance == null)
                {
                    TempData["GrievanceError"] = "The grievance record could not be updated.";
                    return RedirectToAction(nameof(HRManagement));
                }

                if (!CanReviewGrievance(accessProfile, sessionUsername, currentGrievance))
                {
                    TempData["GrievanceError"] = "You cannot review your own grievance report.";
                    return RedirectToAction(nameof(HRManagement));
                }

                var grievance = _grievanceRequestService.UpdateStatus(grievanceId, status, outcome, decisionReason, sessionUsername);
                if (grievance == null)
                {
                    TempData["GrievanceError"] = "The grievance record could not be updated.";
                    return RedirectToAction(nameof(HRManagement));
                }

                TempData["GrievanceSuccess"] = $"Grievance #{grievance.Id} was updated to '{grievance.Status}'.";
                return RedirectToAction(nameof(HRManagement));
            }

            [HttpGet]
            public IActionResult ReviewLeaveRequest(int leaveRequestId)
            {
                var sessionUsername = HttpContext.Session.GetString("Username");
                if (string.IsNullOrWhiteSpace(sessionUsername) ||
                    !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
                {
                    return RedirectToAction("Login", "Login");
                }

                if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                {
                    return Forbid();
                }

                var leaveRequest = _leaveRequestService.GetById(leaveRequestId);
                if (leaveRequest == null)
                {
                    TempData["LeaveError"] = "The selected leave request could not be found.";
                    return RedirectToAction(nameof(HRManagement));
                }
                if (!CanReviewLeaveRequest(accessProfile, sessionUsername, leaveRequest))
                {
                    TempData["LeaveError"] = "You cannot review your own leave request.";
                    return RedirectToAction(nameof(HRManagement));
                }

                return RedirectToAction(nameof(HRManagement), new
                {
                    actionKey = ReviewLeaveRequestActionKey,
                    leaveRequestId
                });
            }

            [HttpGet]
            public IActionResult EditLeaveRequest(int leaveRequestId)
            {
                var sessionUsername = HttpContext.Session.GetString("Username");
                if (string.IsNullOrWhiteSpace(sessionUsername) ||
                    !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
                {
                    return RedirectToAction("Login", "Login");
                }

                if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                {
                    return Forbid();
                }

                var leaveRequest = _leaveRequestService.GetById(leaveRequestId);
                if (leaveRequest == null)
                {
                    TempData["LeaveError"] = "The selected leave request could not be found.";
                    return RedirectToAction(nameof(HRManagement));
                }
                if (!CanManageAnotherEmployee(accessProfile, sessionUsername, leaveRequest.SubmittedForUsername))
                {
                    TempData["LeaveError"] = "You cannot use the employee leave management edit flow for your own request.";
                    return RedirectToAction(nameof(HRManagement));
                }

                return RedirectToAction(nameof(HRManagement), new
                {
                    actionKey = EditLeaveRequestActionKey,
                    leaveRequestId
                });
            }

            [HttpPost]
            [ValidateAntiForgeryToken]
            public IActionResult UpdateLeaveRequest(int leaveRequestId, string leaveType, DateTime startDate, DateTime endDate, string reason)
            {
                var sessionUsername = HttpContext.Session.GetString("Username");
                if (string.IsNullOrWhiteSpace(sessionUsername) ||
                    !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
                {
                    return RedirectToAction("Login", "Login");
                }

                if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                {
                    return Forbid();
                }

                var currentLeaveRequest = _leaveRequestService.GetById(leaveRequestId);
                if (currentLeaveRequest == null)
                {
                    TempData["LeaveError"] = "The leave request could not be updated.";
                    return RedirectToAction(nameof(HRManagement));
                }
                if (!CanManageAnotherEmployee(accessProfile, sessionUsername, currentLeaveRequest.SubmittedForUsername))
                {
                    TempData["LeaveError"] = "You cannot use the employee leave management edit flow for your own request.";
                    return RedirectToAction(nameof(HRManagement));
                }

                var leaveRequest = _leaveRequestService.UpdateRequest(leaveRequestId, leaveType, startDate, endDate, reason);
                if (leaveRequest == null)
                {
                    TempData["LeaveError"] = "The leave request could not be updated.";
                    return RedirectToAction(nameof(HRManagement));
                }

                TempData["LeaveSuccess"] = $"Leave request #{leaveRequest.Id} has been updated.";
                return RedirectToAction(nameof(HRManagement));
            }

            [HttpPost]
            [ValidateAntiForgeryToken]
            public IActionResult UpdateSelectedEmployeeEmployeeDetails(
                string group,
                string username,
                string emergencyContactName,
                string emergencyContactRelationship,
                string emergencyContactPhoneNumber,
                string diagnosedHealthConditions,
                string allergies,
                string approvedEmergencyStepsAndMedicines,
                string priorTrainingAndQualifications,
                string firstAidCertification)
            {
                var sessionUsername = HttpContext.Session.GetString("Username");
                if (string.IsNullOrWhiteSpace(sessionUsername) ||
                    !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
                {
                    return RedirectToAction("Login", "Login");
                }

                if (!accessProfile.CanManageAllEmployees)
                {
                    return Forbid();
                }

                var normalizedGroup = NormalizeGroup(group);
                var normalizedUsername = (username ?? string.Empty).Trim();
                if (string.IsNullOrWhiteSpace(normalizedGroup) || string.IsNullOrWhiteSpace(normalizedUsername))
                {
                    TempData["EmployeeError"] = "Please select a department and employee before saving the employee details.";
                    return RedirectToAction(nameof(HRManagement));
                }

                if (string.IsNullOrWhiteSpace(emergencyContactName) ||
                    string.IsNullOrWhiteSpace(emergencyContactRelationship) ||
                    string.IsNullOrWhiteSpace(emergencyContactPhoneNumber) ||
                    string.IsNullOrWhiteSpace(diagnosedHealthConditions) ||
                    string.IsNullOrWhiteSpace(allergies) ||
                    string.IsNullOrWhiteSpace(approvedEmergencyStepsAndMedicines) ||
                    string.IsNullOrWhiteSpace(priorTrainingAndQualifications) ||
                    string.IsNullOrWhiteSpace(firstAidCertification))
                {
                    TempData["EmployeeError"] = "All employee detail fields must be completed before saving.";
                    return RedirectToAction(nameof(HRManagement));
                }

                var emergencyContact = $"{emergencyContactName.Trim()} | {emergencyContactRelationship.Trim()} | {emergencyContactPhoneNumber.Trim()}";
                if (!CanManageAnotherEmployee(accessProfile, sessionUsername, normalizedUsername))
                {
                    TempData["EmployeeError"] = "Please select a permitted employee before saving the employee details.";
                    return RedirectToAction(nameof(HRManagement));
                }

                if (!_employeeAccountRecordService.UpdateEmployeeDetails(
                        normalizedUsername,
                        emergencyContact,
                        diagnosedHealthConditions,
                        allergies,
                        approvedEmergencyStepsAndMedicines,
                        priorTrainingAndQualifications,
                        firstAidCertification))
                {
                    TempData["EmployeeError"] = "The selected employee details could not be saved.";
                    return RedirectToAction(nameof(HRManagement));
                }

                TempData["EmployeeSuccess"] = $"Employee details for {normalizedUsername} have been saved successfully.";
                return RedirectToAction(nameof(HRManagement));
            }

            [HttpPost]
            [ValidateAntiForgeryToken]
            public async Task<IActionResult> UnlockSelectedEmployeeAccount(string group, string username, bool unlockLockedAccount)
            {
                var sessionUsername = HttpContext.Session.GetString("Username");
                if (string.IsNullOrWhiteSpace(sessionUsername) ||
                    !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
                {
                    return RedirectToAction("Login", "Login");
                }

                if (!accessProfile.CanManageAllAccounts)
                {
                    return Forbid();
                }

                var normalizedGroup = NormalizeGroup(group);
                var normalizedUsername = (username ?? string.Empty).Trim();
                if (string.IsNullOrWhiteSpace(normalizedGroup) || string.IsNullOrWhiteSpace(normalizedUsername))
                {
                    TempData["AccountError"] = "Please select a department and employee before updating the account.";
                    return RedirectToAction(nameof(AccountManagement));
                }

                if (!CanManageAnotherEmployee(accessProfile, sessionUsername, normalizedUsername))
                {
                    TempData["AccountError"] = "Please select a permitted employee before updating the account.";
                    return RedirectToAction(nameof(AccountManagement));
                }

                if (!_employeeAccountRecordService.TryGetAccountDetails(normalizedUsername, out var details))
                {
                    TempData["AccountError"] = "The selected employee account could not be found.";
                    return RedirectToAction(nameof(AccountManagement), new { actionKey = EditSelectedEmployeeAccountActionKey, group = normalizedGroup, username = normalizedUsername });
                }

                var currentStatus = GetDetail(details, "Account Status");
                if (!string.Equals(currentStatus, "Locked", StringComparison.OrdinalIgnoreCase))
                {
                    TempData["AccountSuccess"] = $"The account for {normalizedUsername} is already active.";
                    return RedirectToAction(nameof(AccountManagement), new { actionKey = EditSelectedEmployeeAccountActionKey, group = normalizedGroup, username = normalizedUsername });
                }

                if (!unlockLockedAccount)
                {
                    TempData["AccountError"] = "Select 'Unlock Locked Account' before applying the account security update.";
                    return RedirectToAction(nameof(AccountManagement), new { actionKey = EditSelectedEmployeeAccountActionKey, group = normalizedGroup, username = normalizedUsername });
                }

                if (!_employeeAccountRecordService.UnlockAccount(normalizedUsername, sessionUsername))
                {
                    TempData["AccountError"] = $"The account for {normalizedUsername} could not be unlocked.";
                    return RedirectToAction(nameof(AccountManagement), new { actionKey = EditSelectedEmployeeAccountActionKey, group = normalizedGroup, username = normalizedUsername });
                }

                await _mySqlRepository.AddAuditEventAsync(new AuditEventEntity
                {
                    EntityType = "account",
                    EntityKey = normalizedUsername,
                    ActionType = "unlock",
                    ActorUsername = sessionUsername,
                    DetailsJson = JsonSerializer.Serialize(new
                    {
                        group = normalizedGroup,
                        status = "Active"
                    }),
                    OccurredOnUtc = DateTime.UtcNow
                });

                TempData["AccountSuccess"] = $"The account for {normalizedUsername} has been unlocked.";
                return RedirectToAction(nameof(AccountManagement), new { actionKey = EditSelectedEmployeeAccountActionKey, group = normalizedGroup, username = normalizedUsername });
            }

            [HttpGet]
            public IActionResult DeleteLeaveRequest(int leaveRequestId)
            {
                var sessionUsername = HttpContext.Session.GetString("Username");
                if (string.IsNullOrWhiteSpace(sessionUsername) ||
                    !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
                {
                    return RedirectToAction("Login", "Login");
                }

                if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                {
                    return Forbid();
                }

                var leaveRequest = _leaveRequestService.GetById(leaveRequestId);
                if (leaveRequest == null)
                {
                    TempData["LeaveError"] = "The selected leave request could not be found.";
                    return RedirectToAction(nameof(HRManagement));
                }
                if (!CanManageAnotherEmployee(accessProfile, sessionUsername, leaveRequest.SubmittedForUsername))
                {
                    TempData["LeaveError"] = "You cannot use the employee leave management delete flow for your own request.";
                    return RedirectToAction(nameof(HRManagement));
                }

                return RedirectToAction(nameof(HRManagement), new
                {
                    actionKey = DeleteLeaveRequestActionKey,
                    leaveRequestId
                });
            }

            [HttpPost]
            [ValidateAntiForgeryToken]
            public IActionResult ConfirmDeleteLeaveRequest(int leaveRequestId)
            {
                var sessionUsername = HttpContext.Session.GetString("Username");
                if (string.IsNullOrWhiteSpace(sessionUsername) ||
                    !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
                {
                    return RedirectToAction("Login", "Login");
                }

                if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                {
                    return Forbid();
                }

                var leaveRequest = _leaveRequestService.GetById(leaveRequestId);
                if (leaveRequest == null)
                {
                    TempData["LeaveError"] = "The leave request could not be found.";
                    return RedirectToAction(nameof(HRManagement));
                }
                if (!CanManageAnotherEmployee(accessProfile, sessionUsername, leaveRequest.SubmittedForUsername))
                {
                    TempData["LeaveError"] = "You cannot use the employee leave management delete flow for your own request.";
                    return RedirectToAction(nameof(HRManagement));
                }

                if (!_leaveRequestService.DeleteRequest(leaveRequestId))
                {
                    TempData["LeaveError"] = "The leave request could not be deleted.";
                    return RedirectToAction(nameof(HRManagement));
                }

                var employeeName = string.IsNullOrWhiteSpace(leaveRequest.EmployeeName) ? leaveRequest.SubmittedForUsername : leaveRequest.EmployeeName;
                TempData["LeaveSuccess"] = $"The leave request for {employeeName} has been deleted.";
                return RedirectToAction(nameof(HRManagement));
            }

            [HttpPost]
            [ValidateAntiForgeryToken]
            public IActionResult UpdateLeaveRequestStatus(int leaveRequestId, string status, string decisionReason)
            {
                var sessionUsername = HttpContext.Session.GetString("Username");
                if (string.IsNullOrWhiteSpace(sessionUsername) ||
                    !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
                {
                    return RedirectToAction("Login", "Login");
                }

                if (!accessProfile.CanManageAllHr && !accessProfile.CanManageAllEmployees)
                {
                    return Forbid();
                }

                var currentLeaveRequest = _leaveRequestService.GetById(leaveRequestId);
                if (currentLeaveRequest == null)
                {
                    TempData["LeaveError"] = "The leave request could not be updated.";
                    return RedirectToAction(nameof(HRManagement));
                }
                if (!CanReviewLeaveRequest(accessProfile, sessionUsername, currentLeaveRequest))
                {
                    TempData["LeaveError"] = "You cannot review your own leave request.";
                    return RedirectToAction(nameof(HRManagement));
                }

                var leaveRequest = _leaveRequestService.UpdateStatus(leaveRequestId, status, decisionReason, sessionUsername);
                if (leaveRequest == null)
                {
                    TempData["LeaveError"] = "The leave request could not be updated.";
                    return RedirectToAction(nameof(HRManagement));
                }

                var employeeName = string.IsNullOrWhiteSpace(leaveRequest.EmployeeName) ? leaveRequest.SubmittedForUsername : leaveRequest.EmployeeName;
                var displayStatus = string.Equals(leaveRequest.Status, "Denied", System.StringComparison.OrdinalIgnoreCase)
                    ? "Fully Denied"
                    : leaveRequest.Status;
                TempData["LeaveSuccess"] = $"The leave request for {employeeName} was updated to '{displayStatus}'.";
                return RedirectToAction(nameof(HRManagement));
            }

        private static bool CanManageHrOnBehalf(AccessProfile accessProfile)
        {
            return accessProfile.CanManageAllHr || accessProfile.CanManageAllEmployees;
        }

        private static bool IsDifferentUser(string actorUsername, string? targetUsername)
        {
            if (string.IsNullOrWhiteSpace(actorUsername) || string.IsNullOrWhiteSpace(targetUsername))
            {
                return false;
            }

            return !string.Equals(actorUsername.Trim(), targetUsername.Trim(), System.StringComparison.OrdinalIgnoreCase);
        }

        private IReadOnlySet<string> GetManagedTargetUsernames(AccessProfile accessProfile, string actorUsername)
        {
            var allowedUsernames = accessProfile.AssistantDelegatedScopeOnly
                ? _employeeAccountRecordService.GetAssistantDelegatedUsernames(actorUsername)
                : accessProfile.CanViewAssignedTeam && string.Equals(accessProfile.DashboardMode, "employee-management-only", StringComparison.OrdinalIgnoreCase)
                    ? _employeeAccountRecordService.GetAssignedTeamUsernames(actorUsername)
                    : accessProfile.CanManageAllAccounts || accessProfile.CanManageAllEmployees || accessProfile.CanManageAllHr || accessProfile.CanUsePayrollFeatures
                        ? _employeeAccountRecordService.GetAllUsernames()
                        : new List<string>();

            return new HashSet<string>(
                allowedUsernames
                    .Where(username => IsDifferentUser(actorUsername, username))
                    .Select(username => username.Trim()),
                StringComparer.OrdinalIgnoreCase);
        }

        private bool CanManageAnotherEmployee(AccessProfile accessProfile, string actorUsername, string? targetUsername)
        {
            var normalizedTarget = (targetUsername ?? string.Empty).Trim();
            return IsDifferentUser(actorUsername, normalizedTarget) &&
                   GetManagedTargetUsernames(accessProfile, actorUsername).Contains(normalizedTarget);
        }

        private string NormalizeManagedTargetUsername(AccessProfile accessProfile, string actorUsername, string? targetUsername)
        {
            var normalizedTarget = (targetUsername ?? string.Empty).Trim();
            return CanManageAnotherEmployee(accessProfile, actorUsername, normalizedTarget) ? normalizedTarget : string.Empty;
        }

        private bool CanReviewGrievance(AccessProfile accessProfile, string reviewerUsername, GrievanceRequest grievance)
        {
            if (string.IsNullOrWhiteSpace(reviewerUsername))
            {
                return false;
            }

            var normalizedReviewer = reviewerUsername.Trim();
            return !string.Equals(normalizedReviewer, grievance.SubmittedForUsername?.Trim(), System.StringComparison.OrdinalIgnoreCase)
                && !string.Equals(normalizedReviewer, grievance.SubmittedByUsername?.Trim(), System.StringComparison.OrdinalIgnoreCase)
                && CanManageAnotherEmployee(accessProfile, normalizedReviewer, grievance.SubmittedForUsername);
        }

        private bool CanReviewLeaveRequest(AccessProfile accessProfile, string reviewerUsername, LeaveRequest leaveRequest)
        {
            if (string.IsNullOrWhiteSpace(reviewerUsername))
            {
                return false;
            }

            var normalizedReviewer = reviewerUsername.Trim();
            return !string.Equals(normalizedReviewer, leaveRequest.SubmittedForUsername?.Trim(), System.StringComparison.OrdinalIgnoreCase)
                && !string.Equals(normalizedReviewer, leaveRequest.SubmittedByUsername?.Trim(), System.StringComparison.OrdinalIgnoreCase)
                && CanManageAnotherEmployee(accessProfile, normalizedReviewer, leaveRequest.SubmittedForUsername);
        }

        private bool CanAccessManagedPayslip(AccessProfile accessProfile, string sessionUsername, PayslipRecord payslip)
        {
            return CanManageHrOnBehalf(accessProfile) &&
                   !string.IsNullOrWhiteSpace(payslip.Username) &&
                   CanManageAnotherEmployee(accessProfile, sessionUsername, payslip.Username);
        }

        private PersonalAccountDetailsViewModel BuildPersonalAccountDetailsModel(
            string username,
            string? accountType,
            string? irdNumber,
            string? loginTime)
        {
            var model = new PersonalAccountDetailsViewModel
            {
                Username = username,
                AccountType = accountType ?? string.Empty,
                IrdNumber = irdNumber ?? string.Empty,
                LoginTimeDisplay = loginTime ?? string.Empty
            };

            if (_employeeAccountRecordService.TryGetAccountDetails(username, out var details))
            {
                ApplyRecordDetails(model, details);
            }

            return model;
        }

        private static void ApplyRecordDetails(
            PersonalAccountDetailsViewModel model,
            IReadOnlyDictionary<string, string> details)
        {
            model.AccountType = GetDetail(details, "Account Type", model.AccountType);
            model.BusinessRole = GetDetail(details, "Business Role");
            model.JobRole = GetDetail(details, "Job Role");
            model.FirstName = GetDetail(details, "First Name");
            model.MiddleName = GetDetail(details, "Middle Name");
            model.LastName = GetDetail(details, "Last Name");
            model.DateOfBirth = GetDetail(details, "Date of Birth");
            model.Address = GetDetail(details, "Address");
            model.EmailAddress = GetDetail(details, "Email Address");
            model.PhoneNumber = GetDetail(details, "Phone Number");
            model.AccountStatus = GetDetail(details, "Account Status");
            model.FailedLoginAttempts = GetDetail(details, "Failed Login Attempts");
            model.PasswordResetRequired = GetDetail(details, "Password Reset Required");
            model.LastLoginEvent = GetDetail(details, "Last Login Event");
            model.LastSessionEvent = GetDetail(details, "Last Session Event");
        }

        private static string GetDetail(
            IReadOnlyDictionary<string, string> details,
            string key,
            string fallback = "Not set")
        {
            return details.TryGetValue(key, out var value) && !string.IsNullOrWhiteSpace(value)
                ? value
                : fallback;
        }

        private SelectedEmployeeAccountDetailsViewModel BuildSelectedEmployeeAccountModel(
            string? group,
            string? username,
            string contentActionName = nameof(AccountManagementContent))
        {
            var normalizedGroup = NormalizeGroup(group);
            var sessionUsername = HttpContext.Session.GetString("Username") ?? string.Empty;
            var usernamesByGroup = AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile)
                ? FilterManagedUsernamesByGroup(_employeeAccountRecordService.GetUsernamesByGroup(), accessProfile, sessionUsername)
                : new Dictionary<string, IReadOnlyList<string>>(StringComparer.OrdinalIgnoreCase);
            var usernames = string.IsNullOrEmpty(normalizedGroup)
                ? new List<string>()
                : usernamesByGroup.TryGetValue(normalizedGroup, out var groupedUsernames)
                    ? groupedUsernames.ToList()
                    : new List<string>();
            var selectedUsername = (username ?? string.Empty).Trim();
            var isSelectedUserValid = usernames.Contains(selectedUsername, StringComparer.OrdinalIgnoreCase);
            var usernameDisplayNames = _employeeAccountRecordService.GetEmployeeDisplayNamesByUsername(usernames);

            var model = new SelectedEmployeeAccountDetailsViewModel
            {
                ContentEndpoint = Url.Action(contentActionName, "Admin") ?? string.Empty,
                SelectedGroup = normalizedGroup,
                SelectedUsername = isSelectedUserValid ? selectedUsername : string.Empty,
                Usernames = usernames,
                UsernamesByGroup = usernamesByGroup,
                UsernameDisplayNames = usernameDisplayNames
            };

            if (isSelectedUserValid && _employeeAccountRecordService.TryGetAccountDetails(selectedUsername, out var details))
            {
                var selectedEmployeeModel = new PersonalAccountDetailsViewModel
                {
                    Username = selectedUsername
                };
                ApplyRecordDetails(selectedEmployeeModel, details);
                model.Details = selectedEmployeeModel;
            }

            return model;
        }

        private SelectedEmployeeLeaveDetailsViewModel BuildSelectedEmployeeLeaveModel(
            string? group,
            string? username,
            string contentActionName = nameof(HRManagementContent))
        {
            var selectedEmployeeAccount = BuildSelectedEmployeeAccountModel(group, username, contentActionName);
            var model = new SelectedEmployeeLeaveDetailsViewModel
            {
                ContentEndpoint = selectedEmployeeAccount.ContentEndpoint,
                SelectedGroup = selectedEmployeeAccount.SelectedGroup,
                SelectedUsername = selectedEmployeeAccount.SelectedUsername,
                Usernames = selectedEmployeeAccount.Usernames,
                UsernamesByGroup = selectedEmployeeAccount.UsernamesByGroup,
                UsernameDisplayNames = selectedEmployeeAccount.UsernameDisplayNames
            };

            if (string.IsNullOrWhiteSpace(model.SelectedUsername))
            {
                return model;
            }

            var selectedEmployeeDetails = selectedEmployeeAccount.Details;
            if (selectedEmployeeDetails == null)
            {
                return model;
            }

            model.Details = _leaveRequestService.GetEntitlementForRole(
                model.SelectedUsername,
                selectedEmployeeDetails.BusinessRole,
                selectedEmployeeDetails.JobRole,
                true);

            return model;
        }

        private SelectedEmployeeLeaveDetailsViewModel BuildSelectedEmployeeLeaveRequestsModel(
            string? group,
            string? username,
            Func<IReadOnlyList<LeaveRequest>> requestSelector,
            string contentActionName = nameof(HRManagementContent))
        {
            var selectedEmployeeAccount = BuildSelectedEmployeeAccountModel(group, username, contentActionName);
            var sessionUsername = HttpContext.Session.GetString("Username") ?? string.Empty;
            AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile);
            var model = new SelectedEmployeeLeaveDetailsViewModel
            {
                ContentEndpoint = selectedEmployeeAccount.ContentEndpoint,
                SelectedGroup = selectedEmployeeAccount.SelectedGroup,
                SelectedUsername = selectedEmployeeAccount.SelectedUsername,
                Usernames = selectedEmployeeAccount.Usernames,
                UsernamesByGroup = selectedEmployeeAccount.UsernamesByGroup,
                UsernameDisplayNames = selectedEmployeeAccount.UsernameDisplayNames
            };

            model.Requests = requestSelector()
                .Where(request => CanManageAnotherEmployee(accessProfile, sessionUsername, request.SubmittedForUsername))
                .ToList();
            model.ImpactByRequestId = model.Requests.ToDictionary(request => request.Id, _leaveRequestService.GetBalanceImpactForRequest);

            return model;
        }

        private SelectedEmployeeActionTargetViewModel BuildSelectedEmployeeActionTargetModel(
            string? group,
            string? username,
            string contentActionName = nameof(HRManagementContent))
        {
            var selectedEmployeeAccount = BuildSelectedEmployeeAccountModel(group, username, contentActionName);
            var selectedEmployeeName = string.Empty;
            if (!string.IsNullOrWhiteSpace(selectedEmployeeAccount.SelectedUsername))
            {
                selectedEmployeeName = selectedEmployeeAccount.UsernameDisplayNames.TryGetValue(selectedEmployeeAccount.SelectedUsername, out var displayName)
                    ? displayName
                    : selectedEmployeeAccount.SelectedUsername;
            }

            return new SelectedEmployeeActionTargetViewModel
            {
                ContentEndpoint = selectedEmployeeAccount.ContentEndpoint,
                SelectedGroup = selectedEmployeeAccount.SelectedGroup,
                SelectedUsername = selectedEmployeeAccount.SelectedUsername,
                SelectedEmployeeName = selectedEmployeeName,
                Usernames = selectedEmployeeAccount.Usernames,
                UsernamesByGroup = selectedEmployeeAccount.UsernamesByGroup,
                UsernameDisplayNames = selectedEmployeeAccount.UsernameDisplayNames,
                LeaveEntitlement = selectedEmployeeAccount.Details == null
                    ? null
                    : _leaveRequestService.GetEntitlementForRole(
                        selectedEmployeeAccount.SelectedUsername,
                        selectedEmployeeAccount.Details.BusinessRole,
                        selectedEmployeeAccount.Details.JobRole,
                        true)
            };
        }

        private SelectedEmployeeActionTargetViewModel BuildSelectedEmployeePayslipModel(
            string? group,
            string? username,
            string contentActionName = nameof(HRManagementContent))
        {
            var model = BuildSelectedEmployeeActionTargetModel(group, username, contentActionName);
            if (string.IsNullOrWhiteSpace(model.SelectedUsername))
            {
                return model;
            }

            var targetPayslipTaxInfo = _payrollService.GetTaxInformationForUser(model.SelectedUsername);
            var targetPayslipLeaveInfo = _leaveRequestService.GetEntitlementForUser(model.SelectedUsername);
            model.PayslipDraft = new PayslipRecord
            {
                Username = model.SelectedUsername,
                EmployeeName = string.IsNullOrWhiteSpace(targetPayslipTaxInfo.EmployeeName)
                    ? model.SelectedUsername
                    : targetPayslipTaxInfo.EmployeeName,
                SalaryPackageName = targetPayslipLeaveInfo.SalaryPackageName,
                SalaryPackageDisplayName = targetPayslipLeaveInfo.SalaryPackageDisplayName,
                PayFrequency = targetPayslipTaxInfo.PayFrequency,
                IRDNumber = targetPayslipTaxInfo.IRDNumber,
                PayPeriod = "Current pay period",
                BasePay = 0m,
                OvertimePay = 0m,
                BonusPay = 0m,
                AllowancePay = 0m,
                PreTaxDeductions = 0m,
                PostTaxDeductions = 0m,
                AnnualLeaveLawfulDays = targetPayslipLeaveInfo.AnnualLeaveLawfulDays,
                AnnualLeavePackageExtraDays = targetPayslipLeaveInfo.AnnualLeavePackageExtraDays,
                AnnualLeaveTakenDays = targetPayslipLeaveInfo.AnnualLeaveTakenDays,
                AnnualLeaveScheduledDays = targetPayslipLeaveInfo.AnnualLeaveScheduledDays,
                AnnualLeaveRemainingDays = targetPayslipLeaveInfo.RemainingAnnualLeaveDays,
                SickLeaveLawfulDays = targetPayslipLeaveInfo.SickLeaveLawfulDays,
                SickLeavePackageExtraDays = targetPayslipLeaveInfo.SickLeavePackageExtraDays,
                SickLeaveTakenDays = targetPayslipLeaveInfo.SickLeaveTakenDays,
                SickLeaveScheduledDays = targetPayslipLeaveInfo.SickLeaveScheduledDays,
                SickLeaveRemainingDays = targetPayslipLeaveInfo.RemainingSickLeaveDays,
                SpecialLeaveLawfulDays = targetPayslipLeaveInfo.SpecialLeaveLawfulDays,
                SpecialLeavePackageExtraDays = targetPayslipLeaveInfo.SpecialLeavePackageExtraDays,
                SpecialLeaveTakenDays = targetPayslipLeaveInfo.SpecialLeaveTakenDays,
                SpecialLeaveScheduledDays = targetPayslipLeaveInfo.SpecialLeaveScheduledDays,
                SpecialLeaveRemainingDays = targetPayslipLeaveInfo.RemainingSpecialLeaveDays,
                ParentalLeaveLawfulWeeks = targetPayslipLeaveInfo.ParentalLeaveLawfulWeeks,
                ParentalLeavePackageExtraWeeks = targetPayslipLeaveInfo.ParentalLeavePackageExtraWeeks,
                ParentalLeaveRemainingWeeks = targetPayslipLeaveInfo.RemainingParentalLeaveWeeks,
                PublicHolidayDays = targetPayslipLeaveInfo.PublicHolidayDays,
                PublicHolidayPackageExtraDays = targetPayslipLeaveInfo.PublicHolidayPackageExtraDays
            };

            return model;
        }

        private string ResolveEmployeeDisplayName(string? username)
        {
            var normalizedUsername = string.IsNullOrWhiteSpace(username) ? string.Empty : username.Trim();
            if (string.IsNullOrWhiteSpace(normalizedUsername))
            {
                return string.Empty;
            }

            var displayNames = _employeeAccountRecordService.GetEmployeeDisplayNamesByUsername(new[] { normalizedUsername });
            return displayNames.TryGetValue(normalizedUsername, out var displayName)
                ? displayName
                : normalizedUsername;
        }

        private static string NormalizeGroup(string? group)
        {
            var normalized = (group ?? string.Empty).Trim().ToLowerInvariant();
            return normalized switch
            {
                "admin" => "admin",
                "finance" => "finance",
                "trade" => "trade",
                _ => string.Empty
            };
        }

        private EmployeeRecordDetailsViewModel BuildEmployeeRecordDetailsModel(string username)
        {
            var model = new EmployeeRecordDetailsViewModel
            {
                Username = username
            };

            if (_employeeAccountRecordService.TryGetAccountDetails(username, out var details))
            {
                ApplyEmployeeRecordDetails(model, details);
            }

            return model;
        }

        private SelectedEmployeeRecordDetailsViewModel BuildSelectedEmployeeRecordModel(
            string? group,
            string? username,
            string contentActionName)
        {
            var normalizedGroup = NormalizeGroup(group);
            var sessionUsername = HttpContext.Session.GetString("Username") ?? string.Empty;
            var usernamesByGroup = AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile)
                ? FilterManagedUsernamesByGroup(_employeeAccountRecordService.GetUsernamesByGroup(), accessProfile, sessionUsername)
                : new Dictionary<string, IReadOnlyList<string>>(StringComparer.OrdinalIgnoreCase);
            var usernames = string.IsNullOrEmpty(normalizedGroup)
                ? new List<string>()
                : usernamesByGroup.TryGetValue(normalizedGroup, out var groupedUsernames)
                    ? groupedUsernames.ToList()
                    : new List<string>();
            var selectedUsername = (username ?? string.Empty).Trim();
            var isSelectedUserValid = usernames.Contains(selectedUsername, StringComparer.OrdinalIgnoreCase);
            var usernameDisplayNames = _employeeAccountRecordService.GetEmployeeDisplayNamesByUsername(usernames);

            var model = new SelectedEmployeeRecordDetailsViewModel
            {
                ContentEndpoint = Url.Action(contentActionName, "Admin") ?? string.Empty,
                SelectedGroup = normalizedGroup,
                SelectedUsername = isSelectedUserValid ? selectedUsername : string.Empty,
                Usernames = usernames,
                UsernameDisplayNames = usernameDisplayNames
            };

            if (isSelectedUserValid)
            {
                model.Details = BuildEmployeeRecordDetailsModel(selectedUsername);
            }

            return model;
        }

        private IReadOnlyDictionary<string, IReadOnlyList<string>> FilterManagedUsernamesByGroup(
            IReadOnlyDictionary<string, IReadOnlyList<string>> usernamesByGroup,
            AccessProfile accessProfile,
            string sessionUsername)
        {
            var allowedUsernames = GetManagedTargetUsernames(accessProfile, sessionUsername);
            return usernamesByGroup.ToDictionary(
                pair => pair.Key,
                pair => (IReadOnlyList<string>)pair.Value
                    .Where(allowedUsernames.Contains)
                    .ToList(),
                StringComparer.OrdinalIgnoreCase);
        }

        private static void ApplyEmployeeRecordDetails(
            EmployeeRecordDetailsViewModel model,
            IReadOnlyDictionary<string, string> details)
        {
            model.EmergencyContact = GetDetail(details, "Emergency Contact");
            model.DiagnosedHealthConditions = GetDetail(details, "Diagnosed Health Conditions");
            model.Allergies = GetDetail(details, "Allergies");
            model.ApprovedEmergencyStepsAndMedicines = GetDetail(details, "Approved Emergency Steps and Medicines");
            model.PriorTrainingAndQualifications = GetDetail(details, "Prior Training and Qualifications");
            model.FirstAidCertification = GetDetail(details, "First Aid Certification");
        }
    }
}
