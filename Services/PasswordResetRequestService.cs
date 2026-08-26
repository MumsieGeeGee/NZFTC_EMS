using System;
using System.Collections.Generic;
using System.Linq;
using NZFTC_EMS.Models;
using NZFTC_EMS.Utilities;

namespace NZFTC_EMS.Services
{
    public class PasswordResetRequestService
    {
        private static readonly List<PasswordResetRequestNotification> Notifications = new();
        private static readonly object NotificationsLock = new();
        private readonly EmployeeAccountRecordService _employeeAccountRecordService;

        public PasswordResetRequestService(EmployeeAccountRecordService employeeAccountRecordService)
        {
            _employeeAccountRecordService = employeeAccountRecordService;
        }

        public bool TryCreateRequest(string username, out string message)
        {
            var normalizedUsername = username?.Trim() ?? string.Empty;
            if (string.IsNullOrWhiteSpace(normalizedUsername))
            {
                message = "Enter your username before requesting a password reset.";
                return false;
            }

            if (!_employeeAccountRecordService.TryGetAccountDetails(normalizedUsername, out _))
            {
                message = MainSystemAuthMessages.UsernameInvalid;
                return false;
            }

            var recipients = _employeeAccountRecordService.GetPasswordResetRecipientUsernames(normalizedUsername);
            if (recipients.Count == 0)
            {
                message = "No responsible administrator could be found for that account.";
                return false;
            }

            var displayName = _employeeAccountRecordService.GetDisplayNameForUser(normalizedUsername);
            var groupName = _employeeAccountRecordService.GetGroupNameForUser(normalizedUsername);
            var createdOn = DateTime.UtcNow;

            lock (NotificationsLock)
            {
                foreach (var recipient in recipients)
                {
                    if (Notifications.Any(existing =>
                            string.Equals(existing.RecipientUsername, recipient, StringComparison.OrdinalIgnoreCase) &&
                            string.Equals(existing.TargetUsername, normalizedUsername, StringComparison.OrdinalIgnoreCase)))
                    {
                        continue;
                    }

                    Notifications.Add(new PasswordResetRequestNotification
                    {
                        RecipientUsername = recipient,
                        TargetUsername = normalizedUsername,
                        TargetDisplayName = displayName,
                        TargetGroupName = groupName,
                        CreatedOn = createdOn
                    });
                }
            }

            message = "Password reset request sent to the responsible administrator.";
            return true;
        }

        public IReadOnlyList<DashboardNotification> GetDashboardNotificationsForUser(string username)
        {
            var normalizedUsername = username?.Trim() ?? string.Empty;
            if (string.IsNullOrWhiteSpace(normalizedUsername))
            {
                return Array.Empty<DashboardNotification>();
            }

            lock (NotificationsLock)
            {
                return Notifications
                    .Where(notification => string.Equals(notification.RecipientUsername, normalizedUsername, StringComparison.OrdinalIgnoreCase))
                    .OrderByDescending(notification => notification.CreatedOn)
                    .Select(notification => new DashboardNotification
                    {
                        Category = "Account",
                        Title = "Password reset request",
                        Message = $"{notification.TargetDisplayName} requested a password reset.",
                        Tone = "warning",
                        LinkText = "Open account details",
                        LinkUrl = $"/Admin/AccountManagement?actionKey=edit-selected-employee-account-details&group={Uri.EscapeDataString(notification.TargetGroupName)}&username={Uri.EscapeDataString(notification.TargetUsername)}",
                        CreatedOn = notification.CreatedOn
                    })
                    .Take(6)
                    .ToList();
            }
        }
    }
}
