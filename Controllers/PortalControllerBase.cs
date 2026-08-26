using System;
using Microsoft.AspNetCore.Mvc;
using NZFTC_EMS.Models;
using NZFTC_EMS.Utilities;

namespace NZFTC_EMS.Controllers
{
    public abstract class PortalControllerBase : Controller
    {
        protected readonly record struct PortalAccessContext(
            bool IsAuthenticated,
            bool IsPortalAllowed,
            bool CanAccessAccount,
            bool CanAccessEmployee,
            bool CanAccessHr,
            string AccountType);

        protected PortalAccessContext PrepareAccessContext(Func<AccessProfile, bool> isPortalAllowed)
        {
            var username = HttpContext.Session.GetString("Username");
            var accountType = HttpContext.Session.GetString("AccountType");
            var loginTime = HttpContext.Session.GetString("LoginTime");

            if (string.IsNullOrWhiteSpace(username) ||
                !AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return new PortalAccessContext(false, false, false, false, false, string.Empty);
            }

            var moduleAccess = AccessProfileSessionHelper.GetModuleAccess(accessProfile);
            ViewBag.Username = username;
            ViewBag.AccountType = accountType;
            ViewBag.LoginTime = loginTime;
            ViewBag.AccessProfile = accessProfile;
            ViewBag.CanAccessAccountModule = moduleAccess.CanAccessAccount;
            ViewBag.CanAccessEmployeeModule = moduleAccess.CanAccessEmployee;
            ViewBag.CanAccessHrModule = moduleAccess.CanAccessHr;

            return new PortalAccessContext(
                true,
                isPortalAllowed(accessProfile),
                moduleAccess.CanAccessAccount,
                moduleAccess.CanAccessEmployee,
                moduleAccess.CanAccessHr,
                accountType ?? string.Empty);
        }

        protected void SetSectionMenuOptions(string section)
        {
            ViewBag.ActiveSection = section;
            if (ViewBag.AccessProfile is AccessProfile accessProfile)
            {
                ViewBag.MainContentMenuOptions =
                    AccessProfileSessionHelper.GetMainContentMenuOptions(accessProfile, section);
            }
        }

        protected IActionResult RedirectToPermittedDashboard(string accountType)
        {
            if (string.Equals(accountType, "Admin", StringComparison.OrdinalIgnoreCase))
            {
                return RedirectToAction("Dashboard", "Admin");
            }

            if (string.Equals(accountType, "Employee", StringComparison.OrdinalIgnoreCase))
            {
                return RedirectToAction("Dashboard", "Employee");
            }

            return RedirectToAction("Login", "Login");
        }

        protected static string BuildComplaintDescription(
            string attendingSupervisor,
            string personsInvolved,
            string eventDate,
            string eventDetails,
            string affectDetails,
            string? suggestions,
            string? additionalComments,
            string declarationAgreement,
            string signedDate)
        {
            return string.Join(Environment.NewLine, new[]
            {
                $"Attending Supervisor: {attendingSupervisor.Trim()}",
                $"Persons Involved: {personsInvolved.Trim()}",
                $"Event Date: {eventDate.Trim()}",
                $"Event Details: {eventDetails.Trim()}",
                $"Affect on Job/State of Mind Details: {affectDetails.Trim()}",
                $"Suggestions: {suggestions?.Trim() ?? string.Empty}",
                $"Additional Comments/Questions: {additionalComments?.Trim() ?? string.Empty}",
                $"Declaration Agreement: {declarationAgreement.Trim()}",
                $"Signed Date: {signedDate.Trim()}"
            });
        }
    }
}
