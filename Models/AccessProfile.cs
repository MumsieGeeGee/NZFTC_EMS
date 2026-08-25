namespace NZFTC_EMS.Models
{
    public class AccessProfile
    {
        public bool Resolved { get; set; }
        public string BusinessRole { get; set; } = string.Empty;
        public string JobRole { get; set; } = string.Empty;
        public string DashboardMode { get; set; } = string.Empty;
        public bool CanManageAllAccounts { get; set; }
        public bool CanManageAllEmployees { get; set; }
        public bool CanManageAllHr { get; set; }
        public bool CanManageRequests { get; set; }
        public bool CanUsePayrollFeatures { get; set; }
        public bool AssistantDelegatedScopeOnly { get; set; }
        public bool CanViewAssignedTeam { get; set; }
        public bool RequiresSecondaryAuth { get; set; }
    }
}
