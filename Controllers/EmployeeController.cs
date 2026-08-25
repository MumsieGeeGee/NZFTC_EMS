using Microsoft.AspNetCore.Mvc;
using NZFTC_EMS.Models;
using NZFTC_EMS.Services;
using NZFTC_EMS.Utilities;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NZFTC_EMS.Controllers
{
    public class EmployeeController : Controller
    {
        private const string ViewOwnAccountActionKey = "view-own-account-details";
        private const string EditOwnAccountActionKey = "edit-own-account-details";
        private const string ViewOwnEmployeeActionKey = "view-own-employee-details";
        private const string EditOwnEmployeeActionKey = "edit-own-employee-details";
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
        private const string EditMyTaxInformationActionKey = "edit-my-tax-information";
        private const string ViewOwnAccountPartialPath = "~/Views/Employee/Account_Management/View_Own_Account_Details.cshtml";
        private const string EditOwnAccountPartialPath = "~/Views/Employee/Account_Management/Edit_Own_Account_Details.cshtml";
        private const string ViewOwnEmployeePartialPath = "~/Views/Employee/Employee_Management/View_Own_Employee_Details.cshtml";
        private const string EditOwnEmployeePartialPath = "~/Views/Employee/Employee_Management/Edit_Own_Employee_Details.cshtml";
        private const string ViewMyOpenGrievancePartialPath = "~/Views/Employee/HR_Management/Grievance/View_My_Open_Grievance_Reports.cshtml";
        private const string ViewMyResolvedGrievancePartialPath = "~/Views/Employee/HR_Management/Grievance/View_My_Resolved_Grievance_Reports.cshtml";
        private const string SubmitGrievancePartialPath = "~/Views/Employee/HR_Management/Grievance/Submit_Grievance_Report.cshtml";
        private const string ViewMyLeaveEntitlementPartialPath = "~/Views/Employee/HR_Management/Leave/View_Own_Leave_And_Holiday_Entitlements.cshtml";
        private const string RequestLeavePartialPath = "~/Views/Employee/HR_Management/Leave/Request_Leave.cshtml";
        private const string ViewMyOpenLeaveRequestsPartialPath = "~/Views/Employee/HR_Management/Leave/View_Own_Open_Leave_Requests.cshtml";
        private const string ViewMyTemporarilyDeniedLeaveRequestsPartialPath = "~/Views/Employee/HR_Management/Leave/View_Own_Temporarily_Denied_Leave_Requests.cshtml";
        private const string ViewMyResolvedLeaveRequestsPartialPath = "~/Views/Employee/HR_Management/Leave/View_Own_Resolved_Leave_Requests.cshtml";
        private const string ViewMyPayslipsPartialPath = "~/Views/Employee/HR_Management/PAYE/View_Own_Payslips.cshtml";
        private const string ViewMyTaxInformationPartialPath = "~/Views/Employee/HR_Management/PAYE/View_Own_Tax_Information.cshtml";
        private const string EditMyTaxInformationPartialPath = "~/Views/Employee/HR_Management/PAYE/Edit_Own_Tax_Information.cshtml";

        private readonly EmployeeAccountRecordService _employeeAccountRecordService;
        private readonly GrievanceRequestService _grievanceRequestService;
        private readonly LeaveRequestService _leaveRequestService;
        private readonly PayrollService _payrollService;

        public EmployeeController(
            EmployeeAccountRecordService employeeAccountRecordService,
            GrievanceRequestService grievanceRequestService,
            LeaveRequestService leaveRequestService,
            PayrollService payrollService)
        {
            _employeeAccountRecordService = employeeAccountRecordService;
            _grievanceRequestService = grievanceRequestService;
            _leaveRequestService = leaveRequestService;
            _payrollService = payrollService;
        }

        public IActionResult Dashboard()
        {
            var authResult = PrepareAccessContext();
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
            var notifications = _grievanceRequestService.GetDashboardNotificationsForUser(username).ToList();
            notifications.AddRange(_leaveRequestService.GetDashboardNotificationsForUser(username));
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
                    LinkUrl = Url.Action("AccountManagement", "Employee"),
                    CreatedOn = DateTime.UtcNow
                });
            }
            ViewBag.DashboardNotifications = notifications
                .OrderByDescending(notification => notification.CreatedOn)
                .Take(6)
                .ToList();

            return View();
        }

        public IActionResult AccountManagement()
        {
            var authResult = PrepareAccessContext();
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
            ViewBag.AccountManagementContentEndpoint = Url.Action(nameof(AccountManagementContent), "Employee");

            return View("~/Views/Employee/Account_Management/Account_Management.cshtml");
        }

        public IActionResult EmployeeManagement()
        {
            var authResult = PrepareAccessContext();
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
            ViewBag.EmployeeManagementContentEndpoint = Url.Action(nameof(EmployeeManagementContent), "Employee");

            return View("~/Views/Employee/Employee_Management/Employee_Management.cshtml");
        }

        public IActionResult HRManagement()
        {
            var authResult = PrepareAccessContext();
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
            ViewBag.HRManagementContentEndpoint = Url.Action(nameof(HRManagementContent), "Employee");

            return View("~/Views/Employee/HR_Management/HR_Management.cshtml");
        }

        [HttpGet]
        public IActionResult HRManagementContent(string actionKey)
        {
            var username = HttpContext.Session.GetString("Username");
            if (string.IsNullOrWhiteSpace(username) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return Unauthorized();
            }

            var moduleAccess = AccessProfileSessionHelper.GetModuleAccess(accessProfile);
            if (!moduleAccess.CanAccessHr)
            {
                return Forbid();
            }

            return actionKey switch
            {
                ViewMyOpenGrievanceActionKey => PartialView(ViewMyOpenGrievancePartialPath, _grievanceRequestService.GetOpenReportsForUser(username)),
                ViewMyResolvedGrievanceActionKey => PartialView(ViewMyResolvedGrievancePartialPath, _grievanceRequestService.GetResolvedReportsForUser(username)),
                SubmitGrievanceActionKey => PartialView(SubmitGrievancePartialPath, new GrievanceRequest { SubmittedByUsername = username, SubmittedForUsername = username }),
                ViewMyLeaveEntitlementActionKey => PartialView(
                    ViewMyLeaveEntitlementPartialPath,
                    _leaveRequestService.GetEntitlementForRole(username, accessProfile.BusinessRole, accessProfile.JobRole)),
                RequestLeaveActionKey => PartialView(RequestLeavePartialPath, new LeaveRequest { SubmittedByUsername = username, SubmittedForUsername = username }),
                ViewMyOpenLeaveRequestsActionKey => PartialView(ViewMyOpenLeaveRequestsPartialPath, _leaveRequestService.GetOpenRequestsForUser(username)),
                ViewMyTemporarilyDeniedLeaveRequestsActionKey => PartialView(ViewMyTemporarilyDeniedLeaveRequestsPartialPath, _leaveRequestService.GetTemporarilyDeniedRequestsForUser(username)),
                ViewMyResolvedLeaveRequestsActionKey => PartialView(ViewMyResolvedLeaveRequestsPartialPath, _leaveRequestService.GetResolvedRequestsForUser(username)),
                ViewMyPayslipsActionKey => PartialView(ViewMyPayslipsPartialPath, _payrollService.GetPayslipsForUser(username)),
                ViewMyTaxInformationActionKey => PartialView(ViewMyTaxInformationPartialPath, _payrollService.GetTaxInformationForUser(username)),
                EditMyTaxInformationActionKey => PartialView(EditMyTaxInformationPartialPath, _payrollService.GetEditableTaxInformationForUser(username)),
                _ => NotFound()
            };
        }

        [HttpGet]
        public IActionResult ViewPayslip(int payslipId)
        {
            var authResult = PrepareAccessContext();
            if (!authResult.IsAuthenticated)
            {
                return RedirectToAction("Login", "Login");
            }

            if (!authResult.IsPortalAllowed)
            {
                return RedirectToPermittedDashboard(authResult.AccountType);
            }

            var username = HttpContext.Session.GetString("Username") ?? string.Empty;
            var payslip = _payrollService.GetPayslipById(payslipId);
            if (payslip == null || !string.Equals(payslip.Username, username, StringComparison.OrdinalIgnoreCase))
            {
                return NotFound();
            }

            return View("~/Views/Shared/PAYE/View_Payslip_Details.cshtml", payslip);
        }

        [HttpGet]
        public IActionResult DownloadPayslip(int payslipId)
        {
            var authResult = PrepareAccessContext();
            if (!authResult.IsAuthenticated)
            {
                return RedirectToAction("Login", "Login");
            }

            if (!authResult.IsPortalAllowed)
            {
                return RedirectToPermittedDashboard(authResult.AccountType);
            }

            var username = HttpContext.Session.GetString("Username") ?? string.Empty;
            var payslip = _payrollService.GetPayslipById(payslipId);
            if (payslip == null || !string.Equals(payslip.Username, username, StringComparison.OrdinalIgnoreCase))
            {
                return NotFound();
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
        public IActionResult EditMyTaxInformation(PayrollTaxInformation updatedTaxInfo)
        {
            var username = HttpContext.Session.GetString("Username");
            if (string.IsNullOrWhiteSpace(username) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return RedirectToAction("Login", "Login");
            }

            if (!AccessProfileSessionHelper.GetModuleAccess(accessProfile).CanAccessHr)
            {
                return Forbid();
            }

            updatedTaxInfo.Username = username;
            if (!_payrollService.UpdateTaxInformation(username, updatedTaxInfo))
            {
                TempData["PayrollError"] = "The tax information could not be updated.";
                return RedirectToAction(nameof(HRManagement));
            }

            TempData["PayrollSuccess"] = "Your tax information has been updated successfully.";
            return RedirectToAction(nameof(HRManagement));
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult SubmitGrievanceReport(
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
            var username = HttpContext.Session.GetString("Username");
            if (string.IsNullOrWhiteSpace(username) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return RedirectToAction("Login", "Login");
            }

            if (!AccessProfileSessionHelper.GetModuleAccess(accessProfile).CanAccessHr)
            {
                return Forbid();
            }

            if (string.IsNullOrWhiteSpace(attendingSupervisor) ||
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

            var subject = "NZFTC Employee Complaint Form";
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

            _grievanceRequestService.CreateReport(username, username, subject, description);
            TempData["GrievanceSuccess"] = "Your grievance report has been submitted successfully.";
            return RedirectToAction(nameof(HRManagement));
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult SubmitLeaveRequest(string leaveType, DateTime? startDate, DateTime? endDate, string reason)
        {
            var username = HttpContext.Session.GetString("Username");
            if (string.IsNullOrWhiteSpace(username) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return RedirectToAction("Login", "Login");
            }

            if (!AccessProfileSessionHelper.GetModuleAccess(accessProfile).CanAccessHr)
            {
                return Forbid();
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

            var request = _leaveRequestService.CreateRequest(username, username, leaveType, requestedStart, requestedEnd, reason);
            TempData["LeaveSuccess"] = $"Leave request submitted: {request.LeaveType} from {request.StartDate:dd MMM yyyy} to {request.EndDate:dd MMM yyyy} ({request.TotalDays} days).";

            if (AccessProfileSessionHelper.IsAdminPortalProfile(accessProfile))
            {
                return RedirectToAction("HRManagement", "Admin");
            }

            return RedirectToAction(nameof(HRManagement));
        }

        [HttpGet]
        public IActionResult AccountManagementContent(string actionKey)
        {
            var username = HttpContext.Session.GetString("Username");
            var accountType = HttpContext.Session.GetString("AccountType");
            var irdNumber = HttpContext.Session.GetString("IRDNumber");
            var loginTime = HttpContext.Session.GetString("LoginTime");

            if (string.IsNullOrWhiteSpace(username) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return Unauthorized();
            }

            var moduleAccess = AccessProfileSessionHelper.GetModuleAccess(accessProfile);
            if (!moduleAccess.CanAccessAccount)
            {
                return Forbid();
            }

            var model = BuildPersonalAccountDetailsModel(username, accountType, irdNumber, loginTime);

            return actionKey switch
            {
                ViewOwnAccountActionKey => PartialView(ViewOwnAccountPartialPath, model),
                EditOwnAccountActionKey => PartialView(EditOwnAccountPartialPath, model),
                _ => NotFound()
            };
        }

        [HttpGet]
        public IActionResult PersonalAccountContent(string actionKey)
        {
            return AccountManagementContent(actionKey);
        }

        [HttpGet]
        public IActionResult EmployeeManagementContent(string actionKey)
        {
            var username = HttpContext.Session.GetString("Username");

            if (string.IsNullOrWhiteSpace(username) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return Unauthorized();
            }

            var moduleAccess = AccessProfileSessionHelper.GetModuleAccess(accessProfile);
            if (!moduleAccess.CanAccessEmployee)
            {
                return Forbid();
            }

            var model = BuildEmployeeRecordDetailsModel(username);

            return actionKey switch
            {
                ViewOwnEmployeeActionKey => PartialView(ViewOwnEmployeePartialPath, model),
                EditOwnEmployeeActionKey => PartialView(EditOwnEmployeePartialPath, model),
                _ => NotFound()
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

        private static string BuildComplaintDescription(
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
            return string.Join(Environment.NewLine, new[]
            {
                $"Attending Supervisor: {attendingSupervisor.Trim()}",
                $"Persons Involved: {personsInvolved.Trim()}",
                $"Event Date: {eventDate.Trim()}",
                $"Event Details: {eventDetails.Trim()}",
                $"Affect on Job/State of Mind Details: {affectDetails.Trim()}",
                $"Suggestions: {suggestions?.Trim() ?? string.Empty}",
                $"Additional Comments/Questions: {additionalComments?.Trim() ?? string.Empty}",
                $"Declaration Agreement: {declarationAgreement.Trim()}",
                $"Signed Date: {signedDate.Trim()}"
            });
        }

        private (bool IsAuthenticated, bool IsPortalAllowed, bool CanAccessAccount, bool CanAccessEmployee, bool CanAccessHr, string AccountType) PrepareAccessContext()
        {
            var username = HttpContext.Session.GetString("Username");
            var accountType = HttpContext.Session.GetString("AccountType");
            var loginTime = HttpContext.Session.GetString("LoginTime");

            if (string.IsNullOrWhiteSpace(username) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return (false, false, false, false, false, string.Empty);
            }

            var moduleAccess = AccessProfileSessionHelper.GetModuleAccess(accessProfile);
            ViewBag.Username = username;
            ViewBag.AccountType = accountType;
            ViewBag.LoginTime = loginTime;
            ViewBag.AccessProfile = accessProfile;
            ViewBag.CanAccessAccountModule = moduleAccess.CanAccessAccount;
            ViewBag.CanAccessEmployeeModule = moduleAccess.CanAccessEmployee;
            ViewBag.CanAccessHrModule = moduleAccess.CanAccessHr;

            var isPortalAllowed = AccessProfileSessionHelper.IsEmployeePortalProfile(accessProfile);
            return (true, isPortalAllowed, moduleAccess.CanAccessAccount, moduleAccess.CanAccessEmployee, moduleAccess.CanAccessHr, accountType ?? string.Empty);
        }

        private void SetSectionMenuOptions(string section)
        {
            ViewBag.ActiveSection = section;
            if (ViewBag.AccessProfile is AccessProfile accessProfile)
            {
                ViewBag.MainContentMenuOptions =
                    AccessProfileSessionHelper.GetMainContentMenuOptions(accessProfile, section);
            }
        }

        private IActionResult RedirectToPermittedDashboard(string accountType)
        {
            if (string.Equals(accountType, "Admin", StringComparison.OrdinalIgnoreCase))
            {
                return RedirectToAction("Dashboard", "Admin");
            }

            return RedirectToAction("Login", "Login");
        }
    }
}
