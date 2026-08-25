using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using NZFTC_EMS.Models;

namespace NZFTC_EMS.Services
{
    public class GrievanceRequestService
    {
        private static readonly List<GrievanceRequest> Requests = new();

        private static readonly List<GrievanceNotification> Notifications = new();

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

        public GrievanceRequest CreateReport(string submittedByUsername, string submittedForUsername, string subject, string description)
        {
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

        private static string ResolveEmployeeDisplayName(string? targetUsername, string? fallbackUsername)
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

        private static IReadOnlyList<string> GetNotificationRecipients(string targetUsername, string submittedByUsername)
        {
            var recipients = new List<string>();
            var root = ResolveEmployeeRecordRoot();
            if (string.IsNullOrWhiteSpace(root) || !Directory.Exists(root))
            {
                return recipients;
            }

            foreach (var filePath in Directory.EnumerateFiles(root, "*_Employee_Record.txt", SearchOption.AllDirectories))
            {
                var parsed = ParseKeyValueFile(filePath);
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

        private static string? TryResolveEmployeeNameFromRecord(string username)
        {
            var root = ResolveEmployeeRecordRoot();
            if (string.IsNullOrWhiteSpace(root) || !Directory.Exists(root))
            {
                return null;
            }

            foreach (var filePath in Directory.EnumerateFiles(root, "*_Employee_Record.txt", SearchOption.AllDirectories))
            {
                var parsed = ParseKeyValueFile(filePath);
                if (!parsed.TryGetValue("Username", out var recordUsername) ||
                    !string.Equals(recordUsername.Trim(), username.Trim(), StringComparison.OrdinalIgnoreCase))
                {
                    continue;
                }

                var firstName = parsed.TryGetValue("First Name", out var first) ? first.Trim() : string.Empty;
                var lastName = parsed.TryGetValue("Last Name", out var last) ? last.Trim() : string.Empty;
                if (string.IsNullOrWhiteSpace(firstName) && string.IsNullOrWhiteSpace(lastName))
                {
                    return null;
                }

                return string.Join(" ", new[] { firstName, lastName }.Where(value => !string.IsNullOrWhiteSpace(value))).Trim();
            }

            return null;
        }

        private static string ResolveEmployeeRecordRoot()
        {
            var candidates = new[]
            {
                Path.Combine(Directory.GetCurrentDirectory(), "main", "Main_System", "Employee Management", "Employee_Records"),
                Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "main", "Main_System", "Employee Management", "Employee_Records")),
                Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "..", "main", "Main_System", "Employee Management", "Employee_Records"))
            };

            return candidates.FirstOrDefault(Directory.Exists) ?? string.Empty;
        }

        private static IReadOnlyDictionary<string, string> ParseKeyValueFile(string filePath)
        {
            var parsed = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
            foreach (var rawLine in File.ReadLines(filePath))
            {
                var line = rawLine.Trim();
                if (string.IsNullOrWhiteSpace(line))
                {
                    continue;
                }

                var separatorIndex = line.IndexOf(':');
                if (separatorIndex <= 0 || separatorIndex == line.Length - 1)
                {
                    continue;
                }

                var key = line.Substring(0, separatorIndex).Trim();
                var value = line.Substring(separatorIndex + 1).Trim();
                if (!string.IsNullOrWhiteSpace(key))
                {
                    parsed[key] = value;
                }
            }

            return parsed;
        }

        public GrievanceRequest? GetById(int grievanceId)
        {
            return Requests.FirstOrDefault(r => r.Id == grievanceId);
        }

        public GrievanceRequest? UpdateStatus(int grievanceId, string status, string? outcome = null, string? decisionReason = null, string? handledByUsername = null)
        {
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

            return request;
        }
    }
}
