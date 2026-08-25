using NZFTC_EMS.Models;

namespace NZFTC_EMS.Utilities
{
    public static class CalendarAccessScopeResolver
    {
        public static string ResolveScope(AccessProfile profile)
        {
            if (!profile.Resolved)
            {
                return "none";
            }

            if (profile.AssistantDelegatedScopeOnly)
            {
                return "personal-and-delegated-managers";
            }

            if (profile.DashboardMode == "full-admin-dashboard" ||
                profile.DashboardMode == "partial-admin-dashboard")
            {
                return "personal-and-all-employees";
            }

            if (profile.DashboardMode == "employee-management-only")
            {
                return profile.CanViewAssignedTeam
                    ? "personal-and-assigned-team"
                    : "personal-and-all-employees";
            }

            if (profile.CanViewAssignedTeam)
            {
                return "personal-and-assigned-team";
            }

            return "personal-only";
        }
    }
}
