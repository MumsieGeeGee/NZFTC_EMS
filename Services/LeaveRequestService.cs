using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using NZFTC_EMS.Data;
using NZFTC_EMS.Data.Entities;
using NZFTC_EMS.Models;

namespace NZFTC_EMS.Services
{
    public class LeaveRequestService
    {
        private static readonly List<LeaveRequest> Requests = new();
        private static readonly object StorageLock = new();
        private static bool StorageLoaded;
        private const string LeaveStorageFolderName = "Leave";
        private const string OpenRequestsFileName = "Open_Requests.txt";
        private const string TempDeniedRequestsFileName = "Temp_Denied_Requests.txt";
        private const string OpenRequestsSectionHeader = "Current Open Leave Requests:";
        private const string TempDeniedRequestsSectionHeader = "Current Temp Denied Leave Requests:";
        private const string ResolvedRequestsSectionHeader = "Current Resolved Leave Requests:";
        private const string LeaveRequestBlockHeader = "Leave Request:";
        private const string EndLeaveRequestBlock = "End Leave Request";
        private const string StoredDateFormat = "yyyy-MM-dd";
        private const string StoredDateTimeFormat = "yyyy-MM-dd HH:mm:ss";
        private static readonly IReadOnlyDictionary<string, string> RolePackageMap = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase)
        {
            ["Junior General Administrator"] = "Junior_General_Administrator_Package",
            ["Senior General Administrator"] = "Senior_General_Administrator_Package",
            ["Junior Trainee Administrator"] = "Junior_Trainee_Administrator_Package",
            ["Senior Trainee Administrator"] = "Senior_Trainee_Administrator_Package",
            ["Junior Casual Administrator"] = "Junior_Casual_Administrator_Package",
            ["Senior Casual Administrator"] = "Senior_Casual_Administrator_Package",
            ["Receptionist"] = "Receptionist_Package",
            ["Call Centre Operator"] = "Call_Centre_Operator_Package",
            ["Executive Assistant"] = "Executive_Assistant_Package",
            ["Personal Assistant"] = "Personal_Assistant_Package",
            ["Human Resources Administrator"] = "Human_Resources_Administrator_Package",
            ["Junior IT Support Administrator"] = "Junior_IT_Support_Administrator_Package",
            ["Senior IT Support Administrator"] = "Senior_IT_Support_Administrator_Package",
            ["Junior Level Manager"] = "Junior_Admin_Manager_Package",
            ["Senior Level Manager"] = "Senior_Admin_Manager_Package",
            ["Junior General Finance Staff"] = "Junior_General_Finance_Staff_Package",
            ["Senior General Finance Staff"] = "Senior_General_Finance_Staff_Package",
            ["Junior Trainee Finance Staff"] = "Junior_Trainee_Finance_Staff_Package",
            ["Senior Trainee Finance Staff"] = "Senior_Trainee_Finance_Staff_Package",
            ["Junior Casual Finance Staff"] = "Junior_Casual_Finance_Staff_Package",
            ["Senior Casual Finance Staff"] = "Senior_Casual_Finance_Staff_Package",
            ["Payroll Administrator"] = "Payroll_Administrator_Package",
            ["Accounts Payable Administrator"] = "Accounts_Payable_Administrator_Package",
            ["Accounts Receivable Administrator"] = "Accounts_Receivable_Administrator_Package",
            ["Junior Finance Manager"] = "Junior_Finance_Manager_Package",
            ["Senior Finance Manager"] = "Senior_Finance_Manager_Package",
            ["Junior General Trade Staff"] = "Junior_General_Trade_Staff_Package",
            ["Senior General Trade Staff"] = "Senior_General_Trade_Staff_Package",
            ["Junior Trainee Trade Staff"] = "Junior_Trainee_Trade_Staff_Package",
            ["Senior Trainee Trade Staff"] = "Senior_Trainee_Trade_Staff_Package",
            ["Junior Casual Trade Staff"] = "Junior_Casual_Trade_Staff_Package",
            ["Senior Casual Trade Staff"] = "Senior_Casual_Trade_Staff_Package",
            ["Junior Apprentice"] = "Junior_Apprentice_Package",
            ["Senior Mentor"] = "Senior_Mentor_Package",
            ["Safety Officer"] = "Safety_Officer_Package",
            ["Sales Representative"] = "Sales_Representative_Package",
            ["Junior Trade Manager"] = "Junior_Trade_Manager_Package",
            ["Senior Trade Manager"] = "Senior_Trade_Manager_Package"
        };
        private static readonly Lazy<IReadOnlyDictionary<string, SalaryPackageData>> SalaryPackages =
            new(BuildSalaryPackageCatalog);
        private static readonly Regex PackageFunctionRegex = new(
            @"void\s+(?<name>[A-Za-z0-9_]+_Package)\s*\(\)\s*\{(?<body>.*?)\n\}",
            RegexOptions.Compiled | RegexOptions.Singleline | RegexOptions.Multiline);

        private readonly MySqlRepository _mySqlRepository;
        private readonly EmployeeAccountRecordService _employeeAccountRecordService;
        private readonly EmployeeRecordStore _employeeRecordStore;
        private readonly IPublicHolidayCalendarService _publicHolidayCalendarService;

        public LeaveRequestService(
            MySqlRepository mySqlRepository,
            EmployeeAccountRecordService employeeAccountRecordService,
            IPublicHolidayCalendarService publicHolidayCalendarService,
            EmployeeRecordStore employeeRecordStore)
        {
            _mySqlRepository = mySqlRepository;
            _employeeAccountRecordService = employeeAccountRecordService;
            _publicHolidayCalendarService = publicHolidayCalendarService;
            _employeeRecordStore = employeeRecordStore;
            EnsureStorageLoaded();
        }

        public static string NormalizeStatus(string? status)
        {
            if (string.IsNullOrWhiteSpace(status))
            {
                return string.Empty;
            }

            var trimmed = status.Trim();
            if (trimmed.Equals("open", StringComparison.OrdinalIgnoreCase))
            {
                return "Open";
            }

            if (trimmed.Equals("received", StringComparison.OrdinalIgnoreCase))
            {
                return "Received";
            }

            if (trimmed.Equals("approved", StringComparison.OrdinalIgnoreCase))
            {
                return "Approved";
            }

            if (trimmed.Equals("fully denied", StringComparison.OrdinalIgnoreCase)
                || trimmed.Equals("full denied", StringComparison.OrdinalIgnoreCase)
                || trimmed.Equals("denied", StringComparison.OrdinalIgnoreCase)
                || trimmed.Equals("rejected", StringComparison.OrdinalIgnoreCase)
                || trimmed.Equals("declined", StringComparison.OrdinalIgnoreCase))
            {
                return "Denied";
            }

            if (trimmed.Equals("temporarily denied", StringComparison.OrdinalIgnoreCase)
                || trimmed.Equals("temp denied", StringComparison.OrdinalIgnoreCase)
                || trimmed.Equals("temporary denial", StringComparison.OrdinalIgnoreCase)
                || trimmed.Equals("temp_denied", StringComparison.OrdinalIgnoreCase)
                || trimmed.Equals("temporarily_denied", StringComparison.OrdinalIgnoreCase))
            {
                return "Temporarily Denied";
            }

            if (trimmed.Equals("resolved", StringComparison.OrdinalIgnoreCase))
            {
                return "Resolved";
            }

            return trimmed;
        }

        public static bool MatchesStatus(string? status, params string[] acceptedValues)
        {
            var normalizedStatus = NormalizeStatus(status);
            return acceptedValues.Any(value => string.Equals(normalizedStatus, NormalizeStatus(value), StringComparison.OrdinalIgnoreCase));
        }

        private static string NormalizeUsername(string? username)
        {
            return string.IsNullOrWhiteSpace(username) ? string.Empty : username.Trim();
        }

        public LeaveEntitlement GetEntitlementForUser(string username)
        {
            var normalizedUsername = NormalizeUsername(username);
            var entitlement = new LeaveEntitlement
            {
                Username = normalizedUsername
            };

            if (string.IsNullOrWhiteSpace(normalizedUsername))
            {
                entitlement.ResolutionMessage = "No username was provided.";
                return entitlement;
            }

            var accountDetailsResolved = _employeeAccountRecordService.TryGetAccountDetails(normalizedUsername, out var details);
            var businessRole = accountDetailsResolved ? GetDetail(details, "Business Role") : string.Empty;
            var jobRole = accountDetailsResolved ? GetDetail(details, "Job Role") : string.Empty;
            return GetEntitlementForRole(normalizedUsername, businessRole, jobRole, accountDetailsResolved);
        }

        public LeaveEntitlement GetEntitlementForRole(string username, string businessRole, string jobRole, bool roleDetailsResolved = true)
        {
            var normalizedUsername = NormalizeUsername(username);
            var normalizedBusinessRole = NormalizeRoleName(businessRole);
            var normalizedJobRole = NormalizeRoleName(jobRole);
            var today = DateTime.Today;
            var entitlement = new LeaveEntitlement
            {
                Username = normalizedUsername,
                BusinessRole = normalizedBusinessRole,
                JobRole = normalizedJobRole,
                PublicHolidayDays = _publicHolidayCalendarService.GetPublicHolidayEvents(DateTime.UtcNow.Year).Count
            };

            var package = ResolveSalaryPackage(normalizedJobRole);
            if (package == null)
            {
                entitlement.ResolutionMessage = BuildUnresolvedPackageMessage(normalizedBusinessRole, normalizedJobRole, roleDetailsResolved);
                return entitlement;
            }

            entitlement.SalaryPackageName = package.PackageName;
            entitlement.SalaryPackageDisplayName = FormatPackageDisplayName(package.PackageName);
            entitlement.ApproximateAnnualSalary = package.ApproximateAnnualSalary;
            entitlement.KiwiSaverEmployerContribution = package.KiwiSaverEmployerContribution;
            entitlement.AnnualLeaveLawfulDays = 20;
            entitlement.AnnualLeavePackageExtraDays = Math.Max(0, package.AnnualLeaveDays - entitlement.AnnualLeaveLawfulDays);
            entitlement.AnnualLeaveDays = entitlement.AnnualLeaveLawfulDays + entitlement.AnnualLeavePackageExtraDays;

            entitlement.SickLeaveLawfulDays = 10;
            entitlement.SickLeavePackageExtraDays = Math.Max(0, package.SickLeaveDays - entitlement.SickLeaveLawfulDays);
            entitlement.SickLeaveDays = entitlement.SickLeaveLawfulDays + entitlement.SickLeavePackageExtraDays;

            entitlement.PublicHolidayLawfulDays = entitlement.PublicHolidayDays;
            entitlement.PublicHolidayPackageExtraDays = 0;

            entitlement.SpecialLeaveLawfulDays = 0;
            entitlement.SpecialLeavePackageExtraDays = Math.Max(0, package.SpecialLeaveDays);
            entitlement.SpecialLeaveDays = entitlement.SpecialLeaveLawfulDays + entitlement.SpecialLeavePackageExtraDays;

            entitlement.ParentalLeaveLawfulWeeks = 26;
            entitlement.ParentalLeavePackageExtraWeeks = Math.Max(0, package.ParentalLeaveWeeks);
            entitlement.ParentalLeaveWeeks = entitlement.ParentalLeaveLawfulWeeks + entitlement.ParentalLeavePackageExtraWeeks;

            entitlement.SuperannuationContribution = package.SuperannuationContribution;
            entitlement.HealthInsuranceContribution = package.HealthInsuranceContribution;
            entitlement.ProfessionalDevelopmentContribution = package.ProfessionalDevelopmentContribution;
            entitlement.EmployeeAssistanceProgramContribution = package.EmployeeAssistanceProgramContribution;
            entitlement.HasResolvedPackage = true;

            entitlement.AnnualLeaveTakenDays = GetConsumedRequestedDays(normalizedUsername, IsAnnualLeaveType, today);
            entitlement.AnnualLeaveScheduledDays = GetScheduledApprovedRequestedDays(normalizedUsername, IsAnnualLeaveType, today);
            entitlement.RemainingAnnualLeaveDays = Math.Max(0, entitlement.AnnualLeaveDays - entitlement.AnnualLeaveTakenDays);

            entitlement.SickLeaveTakenDays = GetConsumedRequestedDays(normalizedUsername, IsSickLeaveType, today);
            entitlement.SickLeaveScheduledDays = GetScheduledApprovedRequestedDays(normalizedUsername, IsSickLeaveType, today);
            entitlement.RemainingSickLeaveDays = Math.Max(0, entitlement.SickLeaveDays - entitlement.SickLeaveTakenDays);

            entitlement.SpecialLeaveTakenDays = GetConsumedRequestedDays(normalizedUsername, IsSpecialLeaveType, today);
            entitlement.SpecialLeaveScheduledDays = GetScheduledApprovedRequestedDays(normalizedUsername, IsSpecialLeaveType, today);
            entitlement.RemainingSpecialLeaveDays = Math.Max(0, entitlement.SpecialLeaveDays - entitlement.SpecialLeaveTakenDays);

            entitlement.ParentalLeaveTakenWeeks = GetConsumedRequestedWeeks(normalizedUsername, IsParentalLeaveType, today);
            entitlement.ParentalLeaveScheduledWeeks = GetScheduledApprovedRequestedWeeks(normalizedUsername, IsParentalLeaveType, today);
            entitlement.RemainingParentalLeaveWeeks = Math.Max(0,
                entitlement.ParentalLeaveWeeks - GetConsumedRequestedWeeks(normalizedUsername, IsParentalLeaveType, today));

            return entitlement;
        }

        public IReadOnlyList<LeaveRequest> GetOpenRequestsForUser(string username)
        {
            return Requests
                .Where(r => string.Equals(r.SubmittedForUsername, username, StringComparison.OrdinalIgnoreCase)
                    && MatchesStatus(r.Status, "Open", "Received"))
                .OrderByDescending(r => r.RequestedOn)
                .ToList();
        }

        public IReadOnlyList<LeaveRequest> GetTemporarilyDeniedRequestsForUser(string username)
        {
            return Requests
                .Where(r => string.Equals(r.SubmittedForUsername, username, StringComparison.OrdinalIgnoreCase)
                    && MatchesStatus(r.Status, "Temporarily Denied", "Temp Denied"))
                .OrderByDescending(r => r.RequestedOn)
                .ToList();
        }

        public IReadOnlyList<LeaveRequest> GetResolvedRequestsForUser(string username)
        {
            return Requests
                .Where(r => string.Equals(r.SubmittedForUsername, username, StringComparison.OrdinalIgnoreCase)
                    && MatchesStatus(r.Status, "Resolved", "Approved", "Denied"))
                .OrderByDescending(r => r.RequestedOn)
                .ToList();
        }

        public IReadOnlyList<LeaveRequest> GetOpenRequestsForTeam()
        {
            return Requests
                .Where(r => MatchesStatus(r.Status, "Open", "Received"))
                .OrderByDescending(r => r.RequestedOn)
                .ToList();
        }

        public IReadOnlyList<LeaveRequest> GetTemporarilyDeniedRequestsForTeam()
        {
            return Requests
                .Where(r => MatchesStatus(r.Status, "Temporarily Denied", "Temp Denied"))
                .OrderByDescending(r => r.RequestedOn)
                .ToList();
        }

        public IReadOnlyList<LeaveRequest> GetResolvedRequestsForTeam()
        {
            return Requests
                .Where(r => MatchesStatus(r.Status, "Resolved", "Approved", "Denied"))
                .OrderByDescending(r => r.RequestedOn)
                .ToList();
        }

        public IReadOnlyList<LeaveRequest> GetAllRequests()
        {
            return Requests
                .OrderByDescending(r => r.RequestedOn)
                .ToList();
        }

        public static bool CanRequestLeaveType(string? leaveType)
        {
            return LeaveTypeMatches(leaveType, "Annual Leave", "Sick Leave", "Parental Leave", "Special Leave");
        }

        public static string CanonicalizeLeaveType(string? leaveType)
        {
            if (LeaveTypeMatches(leaveType, "Annual Leave"))
            {
                return "Annual Leave";
            }

            if (LeaveTypeMatches(leaveType, "Sick Leave"))
            {
                return "Sick Leave";
            }

            if (LeaveTypeMatches(leaveType, "Parental Leave", "Adoptive Parental Leave", "Adoptive Leave", "Maternity Leave", "Paternity Leave", "Shared Parental Leave"))
            {
                return "Parental Leave";
            }

            if (LeaveTypeMatches(
                leaveType,
                "Special Leave",
                "Personal Leave",
                "Bereavement Leave",
                "Funeral Leave",
                "Jury Service",
                "Military Service Leave",
                "Voting Leave",
                "Blood Donation Leave",
                "Marriage Leave",
                "Domestic Violence Leave"))
            {
                return "Special Leave";
            }

            return NormalizeLeaveType(leaveType);
        }

        public LeaveRequest CreateRequest(string submittedByUsername, string submittedForUsername, string leaveType, DateTime startDate, DateTime endDate, string reason)
        {
            if (!CanRequestLeaveType(leaveType))
            {
                throw new InvalidOperationException("Only standard leave types can be submitted as leave requests.");
            }

            var targetUsername = NormalizeUsername(string.IsNullOrWhiteSpace(submittedForUsername) ? submittedByUsername : submittedForUsername);
            var targetDisplayName = ResolveEmployeeDisplayName(targetUsername, submittedByUsername);
            var request = new LeaveRequest
            {
                Id = Requests.Count == 0 ? 1 : Requests.Max(r => r.Id) + 1,
                SubmittedByUsername = NormalizeUsername(submittedByUsername),
                SubmittedForUsername = targetUsername,
                EmployeeName = string.IsNullOrWhiteSpace(targetDisplayName) ? targetUsername : targetDisplayName,
                LeaveType = CanonicalizeLeaveType(leaveType),
                StartDate = startDate.Date,
                EndDate = endDate.Date,
                Reason = string.IsNullOrWhiteSpace(reason) ? string.Empty : reason.Trim(),
                Status = "Open",
                RequestedOn = DateTime.UtcNow
            };

            Requests.Add(request);
            PersistRequestsToStorage();
            PersistRequestToDatabase(request);
            return request;
        }

        private string ResolveEmployeeDisplayName(string? targetUsername, string? fallbackUsername)
        {
            foreach (var username in new[] { targetUsername, fallbackUsername })
            {
                var normalizedUsername = NormalizeUsername(username);
                if (string.IsNullOrWhiteSpace(normalizedUsername))
                {
                    continue;
                }

                if (_employeeAccountRecordService.TryGetAccountDetails(normalizedUsername, out var details))
                {
                    var firstName = GetDetail(details, "First Name");
                    var middleName = GetDetail(details, "Middle Name");
                    var lastName = GetDetail(details, "Last Name");
                    var displayName = string.Join(" ", new[] { firstName, middleName, lastName }.Where(part => !string.IsNullOrWhiteSpace(part)));
                    if (!string.IsNullOrWhiteSpace(displayName))
                    {
                        return displayName;
                    }
                }
            }

            return string.Empty;
        }

        public LeaveRequest? UpdateRequest(int leaveRequestId, string leaveType, DateTime startDate, DateTime endDate, string reason)
        {
            var request = Requests.FirstOrDefault(r => r.Id == leaveRequestId);
            if (request == null)
            {
                return null;
            }

            if (endDate.Date < startDate.Date)
            {
                return null;
            }

            if (!CanRequestLeaveType(leaveType))
            {
                return null;
            }

            request.LeaveType = CanonicalizeLeaveType(leaveType);
            request.StartDate = startDate.Date;
            request.EndDate = endDate.Date;
            request.Reason = string.IsNullOrWhiteSpace(reason) ? string.Empty : reason.Trim();
            request.UpdatedOn = DateTime.UtcNow;
            PersistRequestsToStorage();
            PersistRequestToDatabase(request);
            return request;
        }

        public LeaveRequestBalanceImpact GetBalanceImpactForRequest(LeaveRequest request)
        {
            var entitlement = GetEntitlementForUser(request.SubmittedForUsername);
            var requestedDays = Math.Max(1, request.TotalDays);

            if (IsParentalLeaveType(request.LeaveType))
            {
                var availableBeforeRequest = Math.Max(0, entitlement.RemainingParentalLeaveWeeks - entitlement.ParentalLeaveScheduledWeeks);
                var requestedWeeks = (int)Math.Ceiling(requestedDays / 7m);
                return new LeaveRequestBalanceImpact
                {
                    UnitLabel = "weeks",
                    RequestedAmount = requestedWeeks,
                    AvailableBeforeRequest = availableBeforeRequest,
                    RemainingAfterRequest = Math.Max(0, availableBeforeRequest - requestedWeeks),
                    ScheduledApprovedAmount = entitlement.ParentalLeaveScheduledWeeks,
                    TakenAmount = entitlement.ParentalLeaveTakenWeeks,
                    TotalEntitlementAmount = entitlement.ParentalLeaveWeeks
                };
            }

            if (IsSickLeaveType(request.LeaveType))
            {
                return BuildDayBasedBalanceImpact(
                    entitlement.SickLeaveDays,
                    entitlement.SickLeaveTakenDays,
                    entitlement.SickLeaveScheduledDays,
                    entitlement.RemainingSickLeaveDays,
                    requestedDays);
            }

            if (IsSpecialLeaveType(request.LeaveType))
            {
                return BuildDayBasedBalanceImpact(
                    entitlement.SpecialLeaveDays,
                    entitlement.SpecialLeaveTakenDays,
                    entitlement.SpecialLeaveScheduledDays,
                    entitlement.RemainingSpecialLeaveDays,
                    requestedDays);
            }

            return BuildDayBasedBalanceImpact(
                entitlement.AnnualLeaveDays,
                entitlement.AnnualLeaveTakenDays,
                entitlement.AnnualLeaveScheduledDays,
                entitlement.RemainingAnnualLeaveDays,
                requestedDays);
        }

        public bool DeleteRequest(int leaveRequestId)
        {
            var request = Requests.FirstOrDefault(r => r.Id == leaveRequestId);
            if (request == null)
            {
                return false;
            }

            var removed = Requests.Remove(request);
            if (removed)
            {
                PersistRequestsToStorage();
                _mySqlRepository.DeleteLeaveRequestAsync(leaveRequestId).GetAwaiter().GetResult();
            }

            return removed;
        }

        public LeaveRequest? GetById(int leaveRequestId)
        {
            return Requests.FirstOrDefault(r => r.Id == leaveRequestId);
        }

        public LeaveRequest? UpdateStatus(int leaveRequestId, string status, string decisionReason, string handledByUsername)
        {
            var request = Requests.FirstOrDefault(r => r.Id == leaveRequestId);
            if (request == null)
            {
                return null;
            }

            var normalizedStatus = NormalizeStatus(status);
            request.Status = normalizedStatus;
            request.DecisionReason = string.IsNullOrWhiteSpace(decisionReason)
                ? request.DecisionReason
                : decisionReason.Trim();
            request.HandledByUsername = string.IsNullOrWhiteSpace(handledByUsername)
                ? request.HandledByUsername
                : handledByUsername.Trim();

            if (MatchesStatus(normalizedStatus, "Approved", "Denied", "Resolved") && string.IsNullOrWhiteSpace(request.DecisionReason))
            {
                request.DecisionReason = string.Empty;
            }

            request.UpdatedOn = DateTime.UtcNow;
            PersistRequestsToStorage();
            PersistRequestToDatabase(request);
            return request;
        }

        private void PersistRequestToDatabase(LeaveRequest request)
        {
            _mySqlRepository.UpsertLeaveRequestAsync(new LeaveRequestEntity
            {
                Id = request.Id,
                SubmittedByUsername = request.SubmittedByUsername,
                SubmittedForUsername = request.SubmittedForUsername,
                EmployeeName = request.EmployeeName,
                LeaveType = request.LeaveType,
                StartDate = request.StartDate,
                EndDate = request.EndDate,
                ReasonEncrypted = request.Reason,
                Status = request.Status,
                DecisionReasonEncrypted = request.DecisionReason,
                RequestedOnUtc = request.RequestedOn == default ? DateTime.UtcNow : request.RequestedOn,
                UpdatedOnUtc = request.UpdatedOn,
                HandledByUsername = request.HandledByUsername
            }).GetAwaiter().GetResult();
        }

        private void EnsureStorageLoaded()
        {
            lock (StorageLock)
            {
                if (StorageLoaded)
                {
                    return;
                }

                Requests.Clear();
                Requests.AddRange(LoadRequestsFromStorage());
                StorageLoaded = true;
            }
        }

        private IEnumerable<LeaveRequest> LoadRequestsFromStorage()
        {
            var requestsById = new Dictionary<int, LeaveRequest>();

            foreach (var request in LoadRequestsFromContainerFile(GetOpenRequestsFilePath()))
            {
                requestsById[request.Id] = request;
            }

            foreach (var request in LoadRequestsFromContainerFile(GetTempDeniedRequestsFilePath()))
            {
                requestsById[request.Id] = request;
            }

            foreach (var request in LoadResolvedRequestsFromEmployeeRecords())
            {
                requestsById[request.Id] = request;
            }

            return requestsById.Values
                .OrderByDescending(request => request.RequestedOn)
                .ToList();
        }

        private IEnumerable<LeaveRequest> LoadRequestsFromContainerFile(string filePath)
        {
            if (!File.Exists(filePath))
            {
                return Array.Empty<LeaveRequest>();
            }

            return ParseRequestBlocks(File.ReadAllLines(filePath))
                .Select(NormalizeLoadedRequest)
                .ToList();
        }

        private IEnumerable<LeaveRequest> LoadResolvedRequestsFromEmployeeRecords()
        {
            var recordRoot = _employeeRecordStore.ResolveEmployeeRecordRoot();
            if (string.IsNullOrWhiteSpace(recordRoot) || !Directory.Exists(recordRoot))
            {
                return Array.Empty<LeaveRequest>();
            }

            var requests = new List<LeaveRequest>();
            foreach (var filePath in _employeeRecordStore.EnumerateEmployeeRecordFiles())
            {
                requests.AddRange(
                    ParseSectionedRequestBlocks(File.ReadAllLines(filePath), ResolvedRequestsSectionHeader)
                        .Select(NormalizeLoadedRequest));
            }

            return requests;
        }

        private IEnumerable<LeaveRequest> ParseRequestBlocks(IEnumerable<string> lines)
        {
            var block = new List<string>();
            var inBlock = false;

            foreach (var rawLine in lines)
            {
                var trimmed = rawLine.Trim();
                if (string.Equals(trimmed, LeaveRequestBlockHeader, StringComparison.OrdinalIgnoreCase))
                {
                    block.Clear();
                    inBlock = true;
                    continue;
                }

                if (string.Equals(trimmed, EndLeaveRequestBlock, StringComparison.OrdinalIgnoreCase))
                {
                    if (inBlock && block.Count > 0)
                    {
                        yield return ParseLeaveRequestBlock(block);
                    }

                    block.Clear();
                    inBlock = false;
                    continue;
                }

                if (inBlock)
                {
                    block.Add(rawLine);
                }
            }
        }

        private IEnumerable<LeaveRequest> ParseSectionedRequestBlocks(string[] lines, string sectionHeader)
        {
            var sectionIndex = Array.FindIndex(lines, line => string.Equals(line.Trim(), sectionHeader, StringComparison.OrdinalIgnoreCase));
            if (sectionIndex < 0)
            {
                return Array.Empty<LeaveRequest>();
            }

            var result = new List<LeaveRequest>();
            var block = new List<string>();
            var inBlock = false;
            for (var index = sectionIndex + 1; index < lines.Length; index++)
            {
                var trimmed = lines[index].Trim();
                if (string.Equals(trimmed, OpenRequestsSectionHeader, StringComparison.OrdinalIgnoreCase) ||
                    string.Equals(trimmed, TempDeniedRequestsSectionHeader, StringComparison.OrdinalIgnoreCase) ||
                    string.Equals(trimmed, ResolvedRequestsSectionHeader, StringComparison.OrdinalIgnoreCase))
                {
                    break;
                }

                if (string.Equals(trimmed, LeaveRequestBlockHeader, StringComparison.OrdinalIgnoreCase))
                {
                    block.Clear();
                    inBlock = true;
                    continue;
                }

                if (string.Equals(trimmed, EndLeaveRequestBlock, StringComparison.OrdinalIgnoreCase))
                {
                    if (inBlock && block.Count > 0)
                    {
                        result.Add(ParseLeaveRequestBlock(block));
                    }

                    block.Clear();
                    inBlock = false;
                    continue;
                }

                if (inBlock)
                {
                    block.Add(lines[index]);
                }
            }

            return result;
        }

        private LeaveRequest ParseLeaveRequestBlock(IReadOnlyList<string> blockLines)
        {
            var values = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
            foreach (var rawLine in blockLines)
            {
                var trimmed = rawLine.Trim();
                if (string.IsNullOrWhiteSpace(trimmed))
                {
                    continue;
                }

                var separatorIndex = trimmed.IndexOf(':');
                if (separatorIndex <= 0 || separatorIndex == trimmed.Length - 1)
                {
                    continue;
                }

                var key = trimmed.Substring(0, separatorIndex).Trim();
                var value = trimmed.Substring(separatorIndex + 1).Trim();
                values[key] = value;
            }

            values.TryGetValue("Request ID", out var requestIdValue);
            values.TryGetValue("Submitted By Username", out var submittedByUsername);
            values.TryGetValue("Submitted For Username", out var submittedForUsername);
            values.TryGetValue("Employee Username", out var employeeUsername);
            values.TryGetValue("Employee Name", out var employeeName);
            values.TryGetValue("Leave Type", out var leaveType);
            values.TryGetValue("Start Date", out var startDateValue);
            values.TryGetValue("End Date", out var endDateValue);
            values.TryGetValue("Reason", out var reason);
            values.TryGetValue("Status", out var status);
            values.TryGetValue("Decision Reason", out var decisionReason);
            values.TryGetValue("Requested On", out var requestedOnValue);
            values.TryGetValue("Updated On", out var updatedOnValue);
            values.TryGetValue("Handled By Username", out var handledByUsername);

            var request = new LeaveRequest
            {
                Id = int.TryParse(requestIdValue, out var id) ? id : 0,
                SubmittedByUsername = NormalizeUsername(submittedByUsername),
                SubmittedForUsername = NormalizeUsername(string.IsNullOrWhiteSpace(submittedForUsername) ? employeeUsername : submittedForUsername),
                EmployeeName = NormalizeText(employeeName),
                LeaveType = NormalizeText(leaveType),
                StartDate = ParseStoredDate(startDateValue),
                EndDate = ParseStoredDate(endDateValue),
                Reason = NormalizeText(reason),
                Status = NormalizeStatus(status),
                DecisionReason = NormalizeText(decisionReason),
                RequestedOn = ParseStoredDateTime(requestedOnValue),
                UpdatedOn = ParseOptionalStoredDateTime(updatedOnValue),
                HandledByUsername = NormalizeUsername(handledByUsername)
            };

            if (string.IsNullOrWhiteSpace(request.EmployeeName) && !string.IsNullOrWhiteSpace(request.SubmittedForUsername))
            {
                request.EmployeeName = ResolveEmployeeDisplayName(request.SubmittedForUsername, request.SubmittedByUsername);
            }

            if (string.IsNullOrWhiteSpace(request.EmployeeName))
            {
                request.EmployeeName = request.SubmittedForUsername;
            }

            return request;
        }

        private LeaveRequest NormalizeLoadedRequest(LeaveRequest request)
        {
            request.SubmittedByUsername = NormalizeUsername(request.SubmittedByUsername);
            request.SubmittedForUsername = NormalizeUsername(request.SubmittedForUsername);
            request.EmployeeName = NormalizeText(request.EmployeeName);
            request.LeaveType = CanonicalizeLeaveType(request.LeaveType);
            request.Reason = NormalizeText(request.Reason);
            request.Status = NormalizeStatus(request.Status);
            request.DecisionReason = NormalizeText(request.DecisionReason);
            request.HandledByUsername = NormalizeUsername(request.HandledByUsername);
            request.RequestedOn = request.RequestedOn == default ? DateTime.UtcNow : request.RequestedOn;
            return request;
        }

        private void PersistRequestsToStorage()
        {
            lock (StorageLock)
            {
                var storageRoot = GetLeaveStorageRoot();
                if (string.IsNullOrWhiteSpace(storageRoot))
                {
                    return;
                }

                Directory.CreateDirectory(storageRoot);

                WriteContainerFile(
                    GetOpenRequestsFilePath(),
                    OpenRequestsSectionHeader,
                    Requests.Where(r => MatchesStatus(r.Status, "Open", "Received")).OrderByDescending(r => r.RequestedOn).ToList());
                WriteContainerFile(
                    GetTempDeniedRequestsFilePath(),
                    TempDeniedRequestsSectionHeader,
                    Requests.Where(r => MatchesStatus(r.Status, "Temporarily Denied", "Temp Denied")).OrderByDescending(r => r.RequestedOn).ToList());

                var recordRoot = _employeeRecordStore.ResolveEmployeeRecordRoot();
                if (string.IsNullOrWhiteSpace(recordRoot) || !Directory.Exists(recordRoot))
                {
                    return;
                }

                foreach (var filePath in _employeeRecordStore.EnumerateEmployeeRecordFiles())
                {
                    if (!TryGetUsernameFromRecordFile(filePath, out var username))
                    {
                        continue;
                    }

                    var resolvedRequests = Requests
                        .Where(request => string.Equals(request.SubmittedForUsername, username, StringComparison.OrdinalIgnoreCase)
                            && MatchesStatus(request.Status, "Resolved", "Approved", "Denied"))
                        .OrderByDescending(request => request.RequestedOn)
                        .ToList();

                    RewriteEmployeeRecordLeaveSections(filePath, resolvedRequests);
                }
            }
        }

        private void WriteContainerFile(string filePath, string sectionHeader, IReadOnlyList<LeaveRequest> requests)
        {
            var lines = new List<string> { sectionHeader };
            if (requests.Count > 0)
            {
                lines.AddRange(BuildRequestBlockLines(requests));
            }

            File.WriteAllLines(filePath, lines);
        }

        private IEnumerable<string> BuildRequestBlockLines(IEnumerable<LeaveRequest> requests)
        {
            foreach (var request in requests)
            {
                foreach (var line in BuildRequestBlockLines(request))
                {
                    yield return line;
                }
            }
        }

        private IEnumerable<string> BuildRequestBlockLines(LeaveRequest request)
        {
            yield return LeaveRequestBlockHeader;
            yield return $"Request ID: {request.Id}";
            yield return $"Submitted By Username: {request.SubmittedByUsername}";
            yield return $"Submitted For Username: {request.SubmittedForUsername}";
            yield return $"Employee Name: {request.EmployeeName}";
            yield return $"Leave Type: {request.LeaveType}";
            yield return $"Start Date: {request.StartDate:yyyy-MM-dd}";
            yield return $"End Date: {request.EndDate:yyyy-MM-dd}";
            yield return $"Reason: {request.Reason}";
            yield return $"Status: {request.Status}";
            yield return $"Decision Reason: {request.DecisionReason}";
            yield return $"Handled By Username: {request.HandledByUsername}";
            yield return $"Requested On: {request.RequestedOn.ToString(StoredDateTimeFormat, CultureInfo.InvariantCulture)}";
            yield return $"Updated On: {(request.UpdatedOn.HasValue ? request.UpdatedOn.Value.ToString(StoredDateTimeFormat, CultureInfo.InvariantCulture) : string.Empty)}";
            yield return EndLeaveRequestBlock;
            yield return string.Empty;
        }

        private void RewriteEmployeeRecordLeaveSections(string filePath, IReadOnlyList<LeaveRequest> resolvedRequests)
        {
            if (!File.Exists(filePath))
            {
                return;
            }

            var lines = File.ReadAllLines(filePath).ToList();
            var openHeaderIndex = lines.FindIndex(line => string.Equals(line.Trim(), OpenRequestsSectionHeader, StringComparison.OrdinalIgnoreCase));
            if (openHeaderIndex < 0)
            {
                if (lines.Count > 0 && !string.IsNullOrWhiteSpace(lines[^1]))
                {
                    lines.Add(string.Empty);
                }

                openHeaderIndex = lines.Count;
            }

            var output = lines.Take(openHeaderIndex).ToList();
            if (output.Count > 0 && !string.IsNullOrWhiteSpace(output[^1]))
            {
                output.Add(string.Empty);
            }

            output.Add(OpenRequestsSectionHeader);
            output.Add(TempDeniedRequestsSectionHeader);
            output.Add(ResolvedRequestsSectionHeader);
            if (resolvedRequests.Count > 0)
            {
                output.AddRange(BuildRequestBlockLines(resolvedRequests));
            }

            File.WriteAllLines(filePath, output);
        }

        private static string NormalizeText(string? value)
        {
            return string.IsNullOrWhiteSpace(value) ? string.Empty : value.Trim();
        }

        private static DateTime ParseStoredDate(string? value)
        {
            var normalized = NormalizeText(value);
            if (DateTime.TryParseExact(normalized, StoredDateFormat, CultureInfo.InvariantCulture, DateTimeStyles.None, out var parsedDate))
            {
                return parsedDate.Date;
            }

            if (DateTime.TryParse(normalized, CultureInfo.InvariantCulture, DateTimeStyles.AssumeLocal, out parsedDate))
            {
                return parsedDate.Date;
            }

            return DateTime.Today;
        }

        private static DateTime ParseStoredDateTime(string? value)
        {
            var normalized = NormalizeText(value);
            if (DateTime.TryParseExact(normalized, StoredDateTimeFormat, CultureInfo.InvariantCulture, DateTimeStyles.AssumeLocal, out var parsedDateTime))
            {
                return DateTime.SpecifyKind(parsedDateTime, DateTimeKind.Utc);
            }

            if (DateTime.TryParse(normalized, CultureInfo.InvariantCulture, DateTimeStyles.AssumeLocal, out parsedDateTime))
            {
                return DateTime.SpecifyKind(parsedDateTime, DateTimeKind.Utc);
            }

            return DateTime.UtcNow;
        }

        private static DateTime? ParseOptionalStoredDateTime(string? value)
        {
            var normalized = NormalizeText(value);
            return string.IsNullOrWhiteSpace(normalized) ? null : ParseStoredDateTime(normalized);
        }

        private string GetLeaveStorageRoot()
        {
            var recordRoot = _employeeRecordStore.ResolveEmployeeRecordRoot();
            return string.IsNullOrWhiteSpace(recordRoot) ? string.Empty : Path.Combine(recordRoot, LeaveStorageFolderName);
        }

        private string GetOpenRequestsFilePath()
        {
            return Path.Combine(GetLeaveStorageRoot(), OpenRequestsFileName);
        }

        private string GetTempDeniedRequestsFilePath()
        {
            return Path.Combine(GetLeaveStorageRoot(), TempDeniedRequestsFileName);
        }

        private bool TryGetUsernameFromRecordFile(string filePath, out string username)
        {
            var parsed = _employeeRecordStore.ParseKeyValueFile(filePath);
            username = parsed.TryGetValue("Username", out var resolvedUsername) ? resolvedUsername.Trim() : string.Empty;
            return !string.IsNullOrWhiteSpace(username);
        }

        public IReadOnlyList<DashboardNotification> GetDashboardNotificationsForUser(string username)
        {
            var normalizedUsername = NormalizeUsername(username);
            if (string.IsNullOrWhiteSpace(normalizedUsername))
            {
                return Array.Empty<DashboardNotification>();
            }

            var today = DateTime.Today;
            var notifications = Requests
                .Where(r => string.Equals(r.SubmittedForUsername, normalizedUsername, StringComparison.OrdinalIgnoreCase))
                .Select(request =>
                {
                    var status = NormalizeStatus(request.Status);
                    if (string.Equals(status, "Approved", StringComparison.OrdinalIgnoreCase))
                    {
                        if (request.EndDate.Date < today)
                        {
                            return new DashboardNotification
                            {
                                Category = "Leave",
                                Title = $"{request.LeaveType} completed",
                                Message = $"Your approved {request.LeaveType.ToLowerInvariant()} from {request.StartDate:dd MMM yyyy} to {request.EndDate:dd MMM yyyy} has been deducted from your balance.",
                                Tone = "approved",
                                CreatedOn = request.UpdatedOn ?? request.RequestedOn
                            };
                        }

                        if (request.StartDate.Date > today)
                        {
                            return new DashboardNotification
                            {
                                Category = "Leave",
                                Title = "Upcoming leave scheduled",
                                Message = $"Your {request.LeaveType.ToLowerInvariant()} from {request.StartDate:dd MMM yyyy} to {request.EndDate:dd MMM yyyy} is approved and will be deducted after it has passed.",
                                Tone = "approved",
                                CreatedOn = request.UpdatedOn ?? request.RequestedOn
                            };
                        }

                        return new DashboardNotification
                        {
                            Category = "Leave",
                            Title = "Leave in progress",
                            Message = $"Your approved {request.LeaveType.ToLowerInvariant()} is currently in progress and will be deducted after it has passed.",
                            Tone = "approved",
                            CreatedOn = request.UpdatedOn ?? request.RequestedOn
                        };
                    }

                    if (string.Equals(status, "Denied", StringComparison.OrdinalIgnoreCase))
                    {
                        return new DashboardNotification
                        {
                            Category = "Leave",
                            Title = "Leave request denied",
                            Message = $"Your {request.LeaveType.ToLowerInvariant()} request from {request.StartDate:dd MMM yyyy} to {request.EndDate:dd MMM yyyy} was denied.",
                            Tone = "rejected",
                            CreatedOn = request.UpdatedOn ?? request.RequestedOn
                        };
                    }

                    if (MatchesStatus(status, "Open", "Received"))
                    {
                        return new DashboardNotification
                        {
                            Category = "Leave",
                            Title = "Leave request pending",
                            Message = $"Your {request.LeaveType.ToLowerInvariant()} request from {request.StartDate:dd MMM yyyy} to {request.EndDate:dd MMM yyyy} is awaiting review.",
                            Tone = "pending",
                            CreatedOn = request.UpdatedOn ?? request.RequestedOn
                        };
                    }

                    return null;
                })
                .Where(notification => notification != null)
                .Select(notification => notification!)
                .OrderByDescending(notification => notification.CreatedOn)
                .Take(6)
                .ToList();

            return notifications;
        }

        private int GetRequestedDays(
            string username,
            Func<string?, bool> leaveTypeFilter,
            params string[] acceptedStatuses)
        {
            return Requests
                .Where(r => string.Equals(r.SubmittedForUsername, username, StringComparison.OrdinalIgnoreCase)
                    && MatchesStatus(r.Status, acceptedStatuses)
                    && leaveTypeFilter(r.LeaveType))
                .Sum(r => r.TotalDays);
        }

        private int GetConsumedRequestedDays(string username, Func<string?, bool> leaveTypeFilter, DateTime today)
        {
            return Requests
                .Where(r => string.Equals(r.SubmittedForUsername, username, StringComparison.OrdinalIgnoreCase)
                    && MatchesStatus(r.Status, "Approved")
                    && leaveTypeFilter(r.LeaveType)
                    && r.EndDate.Date < today.Date)
                .Sum(r => r.TotalDays);
        }

        private int GetScheduledApprovedRequestedDays(string username, Func<string?, bool> leaveTypeFilter, DateTime today)
        {
            return Requests
                .Where(r => string.Equals(r.SubmittedForUsername, username, StringComparison.OrdinalIgnoreCase)
                    && MatchesStatus(r.Status, "Approved")
                    && leaveTypeFilter(r.LeaveType)
                    && r.EndDate.Date >= today.Date)
                .Sum(r => r.TotalDays);
        }

        private int GetConsumedRequestedWeeks(string username, Func<string?, bool> leaveTypeFilter, DateTime today)
        {
            return (int)Math.Ceiling(GetConsumedRequestedDays(username, leaveTypeFilter, today) / 7m);
        }

        private int GetScheduledApprovedRequestedWeeks(string username, Func<string?, bool> leaveTypeFilter, DateTime today)
        {
            return (int)Math.Ceiling(GetScheduledApprovedRequestedDays(username, leaveTypeFilter, today) / 7m);
        }

        private static bool IsAnnualLeaveType(string? leaveType)
        {
            return LeaveTypeMatches(leaveType, "Annual Leave");
        }

        private static bool IsSickLeaveType(string? leaveType)
        {
            return LeaveTypeMatches(leaveType, "Sick Leave");
        }

        private static LeaveRequestBalanceImpact BuildDayBasedBalanceImpact(
            int totalEntitlement,
            int takenAmount,
            int scheduledApprovedAmount,
            int remainingAmount,
            int requestedAmount)
        {
            var availableBeforeRequest = Math.Max(0, remainingAmount - scheduledApprovedAmount);
            return new LeaveRequestBalanceImpact
            {
                UnitLabel = "days",
                RequestedAmount = requestedAmount,
                AvailableBeforeRequest = availableBeforeRequest,
                RemainingAfterRequest = Math.Max(0, availableBeforeRequest - requestedAmount),
                ScheduledApprovedAmount = scheduledApprovedAmount,
                TakenAmount = takenAmount,
                TotalEntitlementAmount = totalEntitlement
            };
        }

        private static bool IsSpecialLeaveType(string? leaveType)
        {
            return LeaveTypeMatches(
                leaveType,
                "Special Leave",
                "Personal Leave",
                "Bereavement Leave",
                "Funeral Leave",
                "Jury Service",
                "Military Service Leave",
                "Voting Leave",
                "Blood Donation Leave",
                "Marriage Leave",
                "Domestic Violence Leave");
        }

        private static bool IsParentalLeaveType(string? leaveType)
        {
            return LeaveTypeMatches(leaveType, "Parental Leave", "Adoptive Parental Leave", "Adoptive Leave", "Maternity Leave", "Paternity Leave", "Shared Parental Leave");
        }

        private static bool LeaveTypeMatches(string? leaveType, params string[] acceptedValues)
        {
            var normalized = NormalizeLeaveType(leaveType);
            return acceptedValues.Any(value => string.Equals(normalized, NormalizeLeaveType(value), StringComparison.OrdinalIgnoreCase));
        }

        private static string NormalizeLeaveType(string? leaveType)
        {
            return string.IsNullOrWhiteSpace(leaveType) ? string.Empty : leaveType.Trim();
        }

        private static string NormalizeRoleName(string? roleName)
        {
            return string.IsNullOrWhiteSpace(roleName) ? string.Empty : roleName.Trim();
        }

        private static string FormatPackageDisplayName(string packageName)
        {
            if (string.IsNullOrWhiteSpace(packageName))
            {
                return string.Empty;
            }

            var display = packageName.Replace("_Package", string.Empty, StringComparison.OrdinalIgnoreCase).Replace('_', ' ');
            return Regex.Replace(display, @"\s+", " ").Trim();
        }

        private SalaryPackageData? ResolveSalaryPackage(string jobRole)
        {
            if (string.IsNullOrWhiteSpace(jobRole))
            {
                return null;
            }

            if (RolePackageMap.TryGetValue(jobRole, out var mappedPackageName))
            {
                return SalaryPackages.Value.TryGetValue(mappedPackageName, out var mappedPackage)
                    ? mappedPackage
                    : null;
            }

            var cleaned = Regex.Replace(jobRole, @"[^A-Za-z0-9]+", "_");
            cleaned = Regex.Replace(cleaned, "_+", "_").Trim('_');
            var packageName = string.IsNullOrWhiteSpace(cleaned) ? string.Empty : $"{cleaned}_Package";
            return SalaryPackages.Value.TryGetValue(packageName, out var package)
                ? package
                : null;
        }

        private static string BuildUnresolvedPackageMessage(string businessRole, string jobRole, bool accountDetailsResolved)
        {
            if (!accountDetailsResolved)
            {
                return "No employee account record was found.";
            }

            if (string.IsNullOrWhiteSpace(jobRole))
            {
                return string.IsNullOrWhiteSpace(businessRole)
                    ? "The employee record did not include a business role or job role."
                    : $"The employee record for business role '{businessRole}' did not include a job role.";
            }

            return string.IsNullOrWhiteSpace(businessRole)
                ? $"No salary package matched the job role '{jobRole}'."
                : $"No salary package matched business role '{businessRole}' and job role '{jobRole}'.";
        }

        private static IReadOnlyDictionary<string, SalaryPackageData> BuildSalaryPackageCatalog()
        {
            var catalog = new Dictionary<string, SalaryPackageData>(StringComparer.OrdinalIgnoreCase);

            foreach (var packageFile in EnumeratePackageFiles())
            {
                var fileText = File.ReadAllText(packageFile);
                foreach (Match match in PackageFunctionRegex.Matches(fileText))
                {
                    var packageName = match.Groups["name"].Value.Trim();
                    var body = match.Groups["body"].Value;
                    var package = ParsePackageData(packageName, body);
                    if (package != null)
                    {
                        catalog[packageName] = package;
                    }
                }
            }

            return catalog;
        }

        private static IEnumerable<string> EnumeratePackageFiles()
        {
            var roots = new[]
            {
                Path.Combine(Directory.GetCurrentDirectory(), "main", "Main_System", "Employee Management", "Role_Management", "Department_Roles"),
                Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "main", "Main_System", "Employee Management", "Role_Management", "Department_Roles")),
                Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "..", "main", "Main_System", "Employee Management", "Role_Management", "Department_Roles"))
            };

            foreach (var root in roots.Distinct(StringComparer.OrdinalIgnoreCase))
            {
                if (!Directory.Exists(root))
                {
                    continue;
                }

                foreach (var filePath in Directory.EnumerateFiles(root, "*_Salary_Packages.cpp", SearchOption.AllDirectories))
                {
                    yield return filePath;
                }
            }
        }

        private static SalaryPackageData? ParsePackageData(string packageName, string body)
        {
            var annualLeaveDays = TryGetIntAssignment(body, "Annual_Leave_Entitlement");
            var sickLeaveDays = TryGetIntAssignment(body, "Sick_Leave_Entitlement");
            var specialLeaveDays = TryGetIntAssignment(body, "Special_Leave_Entitlement");
            var parentalLeaveWeeks = TryGetIntAssignment(body, "Parental_Leave_Entitlement");
            var approximateAnnualSalary = TryGetIntAssignment(body, "Approximate_Annual_Salary");
            var kiwiSaverEmployerContribution = TryGetIntAssignment(body, "KiwiSaver_Employer_Contribution");
            var superannuationContribution = TryGetIntAssignment(body, "Superannuation_Contribution");
            var healthInsuranceContribution = TryGetIntAssignment(body, "Health_Insurance_Contribution");
            var professionalDevelopmentContribution = TryGetIntAssignment(body, "Professional_Development_Contribution");
            var employeeAssistanceProgramContribution = TryGetIntAssignment(body, "Employee_Assistance_Program_Contribution");

            if (!annualLeaveDays.HasValue
                && !sickLeaveDays.HasValue
                && !specialLeaveDays.HasValue
                && !parentalLeaveWeeks.HasValue
                && !approximateAnnualSalary.HasValue
                && !kiwiSaverEmployerContribution.HasValue
                && !superannuationContribution.HasValue
                && !healthInsuranceContribution.HasValue
                && !professionalDevelopmentContribution.HasValue
                && !employeeAssistanceProgramContribution.HasValue)
            {
                return null;
            }

            return new SalaryPackageData
            {
                PackageName = packageName,
                AnnualLeaveDays = annualLeaveDays ?? 0,
                SickLeaveDays = sickLeaveDays ?? 0,
                SpecialLeaveDays = specialLeaveDays ?? 0,
                ParentalLeaveWeeks = parentalLeaveWeeks ?? 0,
                ApproximateAnnualSalary = approximateAnnualSalary ?? 0,
                KiwiSaverEmployerContribution = kiwiSaverEmployerContribution ?? 0,
                PublicHolidayDaysUsesCurrentCalendar = body.Contains("Calculate_Public_Holidays_Count()", StringComparison.OrdinalIgnoreCase),
                SuperannuationContribution = superannuationContribution ?? 0,
                HealthInsuranceContribution = healthInsuranceContribution ?? 0,
                ProfessionalDevelopmentContribution = professionalDevelopmentContribution ?? 0,
                EmployeeAssistanceProgramContribution = employeeAssistanceProgramContribution ?? 0
            };
        }

        private static int? TryGetIntAssignment(string body, string fieldName)
        {
            var match = Regex.Match(body, $@"{Regex.Escape(fieldName)}\s*=\s*([^;]+);", RegexOptions.IgnoreCase);
            if (!match.Success)
            {
                return null;
            }

            var rawValue = match.Groups[1].Value.Trim();
            if (rawValue.Contains("Calculate_Public_Holidays_Count()", StringComparison.OrdinalIgnoreCase))
            {
                return null;
            }

            return int.TryParse(rawValue, out var parsedValue) ? parsedValue : null;
        }

        private static string GetDetail(IReadOnlyDictionary<string, string> details, string key)
        {
            return details.TryGetValue(key, out var value) ? value.Trim() : string.Empty;
        }

        private sealed class SalaryPackageData
        {
            public string PackageName { get; set; } = string.Empty;
            public int ApproximateAnnualSalary { get; set; }
            public int KiwiSaverEmployerContribution { get; set; }
            public int AnnualLeaveDays { get; set; }
            public int SickLeaveDays { get; set; }
            public int SpecialLeaveDays { get; set; }
            public int ParentalLeaveWeeks { get; set; }
            public bool PublicHolidayDaysUsesCurrentCalendar { get; set; }
            public int SuperannuationContribution { get; set; }
            public int HealthInsuranceContribution { get; set; }
            public int ProfessionalDevelopmentContribution { get; set; }
            public int EmployeeAssistanceProgramContribution { get; set; }
        }
    }
}
