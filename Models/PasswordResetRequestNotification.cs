using System;

namespace NZFTC_EMS.Models
{
    public class PasswordResetRequestNotification
    {
        public string RecipientUsername { get; set; } = string.Empty;
        public string TargetUsername { get; set; } = string.Empty;
        public string TargetDisplayName { get; set; } = string.Empty;
        public string TargetGroupName { get; set; } = string.Empty;
        public DateTime CreatedOn { get; set; } = DateTime.UtcNow;
    }
}
