using System;

namespace NZFTC_EMS.Models
{
    public class GrievanceRequest
    {
        public int Id { get; set; }
        public string SubmittedByUsername { get; set; } = string.Empty;
        public string SubmittedForUsername { get; set; } = string.Empty;
        public string EmployeeName { get; set; } = string.Empty;
        public string Subject { get; set; } = string.Empty;
        public string Description { get; set; } = string.Empty;
        public string Category { get; set; } = "General";
        public string Severity { get; set; } = "Medium";
        public string Status { get; set; } = "Open";
        public string Outcome { get; set; } = string.Empty;
        public string DecisionReason { get; set; } = string.Empty;
        public DateTime SubmittedOn { get; set; } = DateTime.UtcNow;
        public DateTime? UpdatedOn { get; set; }
        public string CreatedByRole { get; set; } = "Employee";
        public string NotificationGroup { get; set; } = "Employee";
        public string HandledByUsername { get; set; } = string.Empty;
    }

    public class GrievanceNotification
    {
        public int GrievanceId { get; set; }
        public string RecipientUsername { get; set; } = string.Empty;
        public string RecipientRole { get; set; } = string.Empty;
        public string Message { get; set; } = string.Empty;
        public bool IsCaseUpdate { get; set; }
        public DateTime CreatedOn { get; set; } = DateTime.UtcNow;
    }

    public class DashboardNotification
    {
        public string Category { get; set; } = "General";
        public string Title { get; set; } = string.Empty;
        public string Message { get; set; } = string.Empty;
        public string Tone { get; set; } = "info";
        public string? LinkUrl { get; set; }
        public string? LinkText { get; set; }
        public DateTime CreatedOn { get; set; } = DateTime.UtcNow;
    }
}
