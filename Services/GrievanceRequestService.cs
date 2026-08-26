using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using NZFTC_EMS.Data;
using NZFTC_EMS.Data.Entities;
using NZFTC_EMS.Models;

namespace NZFTC_EMS.Services
{
    public class GrievanceRequestService
    {
        private static readonly List<GrievanceRequest> Requests = new();
        private static readonly List<GrievanceNotification> Notifications = new();
        private static readonly object RequestsLock = new();
        private static bool RequestsLoaded;
        private readonly MySqlRepository _mySqlRepository;
        private readonly EmployeeRecordStore _employeeRecordStore;

        public GrievanceRequestService(MySqlRepository mySqlRepository, EmployeeRecordStore employeeRecordStore)
        {
            _mySqlRepository = mySqlRepository;
            _employeeRecordStore = employeeRecordStore;
            EnsureRequestsLoaded();
        }

        public IReadOnlyList<GrievanceRequest> GetOpenReportsForUser(string username)
        {
            return Requests
                .Where(r => string.Equals(r.SubmittedForUsername, username, StringComparison.OrdinalIgnoreCase) &&
                            string.Equals(r.Status, "Open", StringComparison.OrdinalIgnoreCase))
                .OrderByDescending(r => r.SubmittedOn)
                .ToList();
        }

        public IReadOnlyList<GrievanceRequest> GetResolvedReportsForUser(string username)
        {
            return Requests
                .Where(r => string.Equals(r.SubmittedForUsername, username, StringComparison.OrdinalIgnoreCase) &&
                            string.Equals(r.Status, "Resolved", StringComparison.OrdinalIgnoreCase))
                .OrderByDescending(r => r.SubmittedOn)
                .ToList();
        }

        public IReadOnlyList<GrievanceRequest> GetOpenReportsForTeam()
        {
            return Requests
                .Where(r => string.Equals(r.Status, "Open", StringComparison.OrdinalIgnoreCase))
                .OrderByDescending(r => r.SubmittedOn)
                .ToList();
        }

        public IReadOnlyList<GrievanceRequest> GetResolvedReportsForTeam()
        {
            return Requests
                .Where(r => string.Equals(r.Status, "Resolved", StringComparison.OrdinalIgnoreCase))
                .OrderByDescending(r => r.SubmittedOn)
                .ToList();
        }

        public IReadOnlyList<GrievanceNotification> GetNotificationsForUser(string username)
        {
            return Notifications
                .Where(n => string.Equals(n.RecipientUsername, username, StringComparison.OrdinalIgnoreCase))
                .OrderByDescending(n => n.CreatedOn)
                .ToList();
        }

        public IReadOnlyList<DashboardNotification> GetDashboardNotificationsForUser(string username)
        {
            var notifications = new List<DashboardNotification>();
            notifications.AddRange(
                Notifications
                    .Where(n => string.Equals(n.RecipientUsername, username, StringComparison.OrdinalIgnoreCase))
                    .OrderByDescending(n => n.CreatedOn)
                    .Select(n => new DashboardNotification
                    {
                        Category = "Grievance",
                        Title = n.IsCaseUpdate ? "Your grievance update" : "Grievance review needed",
                        Message = n.IsCaseUpdate
                            ? n.Message
                            : "A grievance requires attention.",
                        Tone = n.IsCaseUpdate ? "info" : "warning",
                        LinkUrl = n.IsCaseUpdate ? null : "/Admin/HRManagement",
                        LinkText = n.IsCaseUpdate ? null : "Open grievance management",
                        CreatedOn = n.CreatedOn
                    }));

            return notifications
                .OrderByDescending(n => n.CreatedOn)
                .Take(6)
                .ToList();
        }

        public IReadOnlyList<GrievanceRequest> GetAllReports()
        {
            EnsureRequestsLoaded();
            return Requests
                .OrderByDescending(r => r.SubmittedOn)
                .ToList();
        }

        public GrievanceRequest CreateReport(string submittedByUsername, string submittedForUsername, string subject, string description)
        {
            EnsureRequestsLoaded();
            var targetUsername = string.IsNullOrWhiteSpace(submittedForUsername) ? submittedByUsername : submittedForUsername;
            var targetName = ResolveEmployeeDisplayName(targetUsername, submittedByUsername);

            var request = new GrievanceRequest
            {
                Id = Requests.Count == 0 ? 1 : Requests.Max(r => r.Id) + 1,
                SubmittedByUsername = submittedByUsername,
                SubmittedForUsername = targetUsername,
                EmployeeName = targetName,
                Subject = subject,
                Description = description,
                Category = string.Empty,
                Severity = string.Empty,
                Status = "Open",
                CreatedByRole = "Employee",
                NotificationGroup = "Employee"
            };

            Requests.Add(request);
            PersistRequestToDatabase(request);

            foreach (var recipient in GetNotificationRecipients(targetUsername, submittedByUsername).Distinct(StringComparer.OrdinalIgnoreCase))
            {
                Notifications.Add(new GrievanceNotification
                {
                    GrievanceId = request.Id,
                    RecipientUsername = recipient,
                    RecipientRole = "Manager",
                    Message = "A grievance requires attention.",
                    IsCaseUpdate = false,
                    CreatedOn = DateTime.UtcNow
                });
            }

            Notifications.Add(new GrievanceNotification
            {
                GrievanceId = request.Id,
                RecipientUsername = targetUsername,
                RecipientRole = "Employee",
                Message = $"Your grievance report '{request.Subject}' has been received and is now under review.",
                IsCaseUpdate = true,
                CreatedOn = DateTime.UtcNow
            });

            return request;
        }

        private string ResolveEmployeeDisplayName(string? targetUsername, string? fallbackUsername)
        {
            var candidateUsernames = new[]
            {
                targetUsername,
                fallbackUsername
            };

            foreach (var username in candidateUsernames)
            {
                if (string.IsNullOrWhiteSpace(username))
                {
                    continue;
                }

                var resolvedName = TryResolveEmployeeNameFromRecord(username);
                if (!string.IsNullOrWhiteSpace(resolvedName))
                {
                    return resolvedName;
                }
            }

            return string.Empty;
        }

        private IReadOnlyList<string> GetNotificationRecipients(string targetUsername, string submittedByUsername)
        {
            var recipients = new List<string>();
            var root = _employeeRecordStore.ResolveEmployeeRecordRoot();
            if (string.IsNullOrWhiteSpace(root) || !Directory.Exists(root))
            {
                return recipients;
            }

            foreach (var filePath in _employeeRecordStore.EnumerateEmployeeRecordFiles())
            {
                var parsed = _employeeRecordStore.ParseKeyValueFile(filePath);
                if (!parsed.TryGetValue("Username", out var username) ||
                    string.IsNullOrWhiteSpace(username) ||
                    string.Equals(username.Trim(), targetUsername, StringComparison.OrdinalIgnoreCase) ||
                    string.Equals(username.Trim(), submittedByUsername, StringComparison.OrdinalIgnoreCase))
                {
                    continue;
                }

                if (GetBusinessRole(parsed) != "Employee")
                {
                    recipients.Add(username.Trim());
                }
            }

            return recipients;
        }

        private static string GetBusinessRole(IReadOnlyDictionary<string, string> record)
        {
            if (record.TryGetValue("Business Role", out var businessRole) && !string.IsNullOrWhiteSpace(businessRole))
            {
                return businessRole.Trim();
            }

            return string.Empty;
        }

        private string? TryResolveEmployeeNameFromRecord(string username)
        {
            if (!_employeeRecordStore.TryGetEmployeeRecord(username, out var parsed))
            {
                return null;
            }

            var firstName = EmployeeRecordStore.TryGetValue(parsed, "First Name");
            var lastName = EmployeeRecordStore.TryGetValue(parsed, "Last Name");
            if (string.IsNullOrWhiteSpace(firstName) && string.IsNullOrWhiteSpace(lastName))
            {
                return null;
            }

            return string.Join(" ", new[] { firstName, lastName }.Where(value => !string.IsNullOrWhiteSpace(value))).Trim();
        }

        public GrievanceRequest? GetById(int grievanceId)
        {
            return Requests.FirstOrDefault(r => r.Id == grievanceId);
        }

        public GrievanceRequest? UpdateStatus(int grievanceId, string status, string? outcome = null, string? decisionReason = null, string? handledByUsername = null)
        {
            EnsureRequestsLoaded();
            var request = Requests.FirstOrDefault(r => r.Id == grievanceId);
            if (request == null)
            {
                return null;
            }

            request.Status = string.IsNullOrWhiteSpace(status) ? request.Status : status;
            request.Outcome = string.IsNullOrWhiteSpace(outcome) ? request.Outcome : outcome;
            request.DecisionReason = string.IsNullOrWhiteSpace(decisionReason) ? request.DecisionReason : decisionReason;
            request.HandledByUsername = string.IsNullOrWhiteSpace(handledByUsername) ? request.HandledByUsername : handledByUsername;
            request.UpdatedOn = DateTime.UtcNow;

            var recipient = request.SubmittedForUsername;
            var message = string.IsNullOrWhiteSpace(request.Outcome)
                ? $"Your grievance '{request.Subject}' has been updated to status '{request.Status}'."
                : $"Your grievance '{request.Subject}' has been updated to status '{request.Status}'. Outcome: {request.Outcome}";

            Notifications.Add(new GrievanceNotification
            {
                GrievanceId = request.Id,
                RecipientUsername = recipient,
                RecipientRole = "Employee",
                Message = message,
                IsCaseUpdate = true,
                CreatedOn = DateTime.UtcNow
            });
            PersistRequestToDatabase(request);

            return request;
        }

        private void EnsureRequestsLoaded()
        {
            lock (RequestsLock)
            {
                if (RequestsLoaded)
                {
                    return;
                }

                Requests.Clear();
                Requests.AddRange(
                    _mySqlRepository.ListGrievanceRequestsAsync()
                        .GetAwaiter()
                        .GetResult()
                        .Select(MapToModel)
                        .OrderBy(r => r.Id));
                RequestsLoaded = true;
            }
        }

        private void PersistRequestToDatabase(GrievanceRequest request)
        {
            _mySqlRepository.UpsertGrievanceRequestAsync(new GrievanceRequestEntity
            {
                Id = request.Id,
                SubmittedByUsername = request.SubmittedByUsername,
                SubmittedForUsername = request.SubmittedForUsername,
                EmployeeName = request.EmployeeName,
                Subject = request.Subject,
                DescriptionEncrypted = request.Description,
                Category = request.Category,
                Severity = request.Severity,
                Status = request.Status,
                OutcomeEncrypted = request.Outcome,
                DecisionReasonEncrypted = request.DecisionReason,
                SubmittedOnUtc = request.SubmittedOn == default ? DateTime.UtcNow : request.SubmittedOn,
                UpdatedOnUtc = request.UpdatedOn,
                CreatedByRole = request.CreatedByRole,
                NotificationGroup = request.NotificationGroup,
                HandledByUsername = request.HandledByUsername
            }).GetAwaiter().GetResult();
        }

        private static GrievanceRequest MapToModel(GrievanceRequestEntity entity)
        {
            return new GrievanceRequest
            {
                Id = checked((int)entity.Id),
                SubmittedByUsername = entity.SubmittedByUsername,
                SubmittedForUsername = entity.SubmittedForUsername,
                EmployeeName = entity.EmployeeName,
                Subject = entity.Subject,
                Description = entity.DescriptionEncrypted,
                Category = entity.Category,
                Severity = entity.Severity,
                Status = entity.Status,
                Outcome = entity.OutcomeEncrypted,
                DecisionReason = entity.DecisionReasonEncrypted,
                SubmittedOn = entity.SubmittedOnUtc,
                UpdatedOn = entity.UpdatedOnUtc,
                CreatedByRole = entity.CreatedByRole,
                NotificationGroup = entity.NotificationGroup,
                HandledByUsername = entity.HandledByUsername
            };
        }
    }
}
