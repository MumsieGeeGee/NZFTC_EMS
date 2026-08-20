using System.ComponentModel.DataAnnotations;

namespace NZFTC_EMS.Models
{
    public class UserSession
    {
        public string Username { get; set; } = string.Empty;
        public string AccountType { get; set; } = string.Empty;
        public string IRDNumber { get; set; } = string.Empty;
        public bool IsAuthenticated { get; set; } = false;
        public DateTime LoginTime { get; set; }
    }
}
