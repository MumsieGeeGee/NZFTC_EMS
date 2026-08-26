using System.Text.Json;
using Microsoft.AspNetCore.Http;
using NZFTC_EMS.Models;
using System.Collections.Generic;

namespace NZFTC_EMS.Utilities
{
    public static class AccessProfileSessionHelper
    {
        private static readonly JsonSerializerOptions JsonOptions = new()
        {
            PropertyNameCaseInsensitive = true
        };

        public static bool TryGetAccessProfile(ISession session, out AccessProfile profile)
        {
            profile = new AccessProfile();
            var accessProfileJson = session.GetString("AccessProfile");
            if (string.IsNullOrWhiteSpace(accessProfileJson))
            {
                return false;
            }

            try
            {
                var deserialized = JsonSerializer.Deserialize<AccessProfile>(accessProfileJson, JsonOptions);
                if (deserialized == null)
                {
                    return false;
                }

                profile = deserialized;
                return true;
            }
            catch (JsonException)
            {
                return false;
            }
        }

        public static (bool CanAccessAccount, bool CanAccessEmployee, bool CanAccessHr) GetModuleAccess(AccessProfile profile)
        {
            if (!profile.Resolved)
            {
                return (false, false, false);
            }

            return profile.DashboardMode switch
            {
                "full-admin-dashboard" => (true, true, true),
                "partial-admin-dashboard" => (true, true, true),
                "account-management-only" => (true, true, true),
                "employee-management-only" => (true, true, true),
                "hr-management-only" => (true, true, true),
                "employee-dashboard" => (true, true, true),
                _ => (false, false, false)
            };
        }

        public static bool IsAdminPortalProfile(AccessProfile profile)
        {
            if (!profile.Resolved)
            {
                return false;
            }

            return profile.DashboardMode switch
            {
                "full-admin-dashboard" => true,
                "partial-admin-dashboard" => true,
                "account-management-only" => true,
                "employee-management-only" => true,
                "hr-management-only" => true,
                _ => false
            };
        }

        public static bool IsEmployeePortalProfile(AccessProfile profile)
        {
            if (!profile.Resolved)
            {
                return false;
            }

            return profile.DashboardMode == "employee-dashboard";
        }

        public static IReadOnlyList<string> GetMainContentMenuOptions(AccessProfile profile, string section)
        {
            var normalizedSection = (section ?? string.Empty).Trim().ToLowerInvariant();
            var options = new List<string>();

            switch (normalizedSection)
            {
                case "dashboard":
                    options.Add("Account Management");
                    options.Add("Employee Management");
                    options.Add("HR Management");

                    if (!profile.CanManageAllAccounts &&
                        !profile.CanManageAllEmployees &&
                        !profile.CanManageAllHr &&
                        !profile.CanUsePayrollFeatures &&
                        !profile.AssistantDelegatedScopeOnly)
                    {
                        options.Add("Access scope: Self-service only (your own Account, Employee & HR records and requests).");
                    }
                    else
                    {
                        options.Add("Access scope: Self-service (your own Account, Employee & HR records and requests) " +
                                    "& Management of permitted team/user records.");
                    }
                    break;

                case "account":
                    options.Add("View My Account Details");
                    options.Add("Edit My Account Details");
                    if (profile.CanManageAllAccounts)
                    {
                        options.Add("View an Employee's Account Details");
                        options.Add("Create New Account");
                        options.Add("Edit an Employee's Account Details");
                        options.Add("Delete Account");
                    }
                    break;

                case "employee":
                    options.Add("View My Employee Record");
                    options.Add("Edit My Employee Information");
                    if (profile.CanManageAllEmployees)
                    {
                        options.Add("View an Employee's Details");
                        options.Add("Edit an Existing Employee's Details");
                    }
                    break;

                case "hr":
                    options.Add("My Leave and Holiday Entitlements");
                    options.Add("Apply for Leave");
                    options.Add("My Open Leave Requests");
                    options.Add("My Temporarily Denied Leave Requests");
                    options.Add("My Resolved Leave Requests");
                    options.Add("My Payslips");
                    options.Add("My Tax Information");
                    options.Add("Submit a Grievance");
                    options.Add("My Open Grievance Reports");
                    options.Add("My Resolved Grievance Reports");

                    if (profile.CanManageAllHr || profile.CanManageAllEmployees)
                    {
                        options.Add("View an Employee's Leave and Holiday Entitlements");
                        options.Add("Request Leave for an Employee");
                        options.Add("View Employee Open Leave Requests");
                        options.Add("View Employee Temporarily Denied Leave Requests");
                        options.Add("View Employee Resolved Leave Requests");
                        options.Add("Generate an Employee's Payslip");
                        options.Add("View an Employee's Payslips");
                        options.Add("View an Employee's Tax Information");
                        options.Add("Edit an Employee's Tax Information");
                        options.Add("Submit a Grievance on behalf of an Employee");
                        options.Add("Review an Employee's Grievance Report");
                        options.Add("View Employee Open Grievance Reports");
                        options.Add("View Employee Resolved Grievance Reports");
                    }
                    break;
            }

            return options;
        }
    }
}
