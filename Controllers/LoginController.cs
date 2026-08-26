using Microsoft.AspNetCore.Mvc;
using NZFTC_EMS.Models;
using NZFTC_EMS.Services;
using NZFTC_EMS.Utilities;
using System.Text.Json;

namespace NZFTC_EMS.Controllers
{
    public class LoginController : Controller
    {
        private readonly IAuthenticationService _authService;
        private readonly PasswordResetRequestService _passwordResetRequestService;
        private readonly SessionAuditService _sessionAuditService;
        private readonly ILogger<LoginController> _logger;

        public LoginController(
            IAuthenticationService authService,
            PasswordResetRequestService passwordResetRequestService,
            SessionAuditService sessionAuditService,
            ILogger<LoginController> logger)
        {
            _authService = authService;
            _passwordResetRequestService = passwordResetRequestService;
            _sessionAuditService = sessionAuditService;
            _logger = logger;
        }

        /// <summary>
        /// Displays the login page.
        /// </summary>
        [HttpGet]
        public IActionResult Login()
        {
            // If user is already logged in, redirect to dashboard
            if (HttpContext.Session.GetString("Username") != null)
            {
                return RedirectToDashboard();
            }

            return RenderLoginView();
        }

        /// <summary>
        /// Handles login form submission.
        /// Authenticates user credentials against the backend.
        /// </summary>
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Login(LoginRequest model)
        {
            _logger.LogInformation($"Login POST received. ModelState.IsValid: {ModelState.IsValid}");
            
            if (!ModelState.IsValid)
            {
                var errors = ModelState.Values.SelectMany(v => v.Errors);
                foreach (var error in errors)
                {
                    _logger.LogWarning($"ModelState error: {error.ErrorMessage}");
                }

                SetPageMessage(GetLoginValidationMessage());
                return RenderLoginView(model);
            }

            try
            {
                // Call authentication service
                var (success, message, userSession) = await _authService.AuthenticateUserAsync(model.Username, model.Password);

                if (!success || userSession == null)
                {
                    SetPageMessage(FormatAuthMessage(message));
                    _logger.LogWarning($"Failed login attempt for username: {model.Username}. Reason: {message}");
                    return RenderLoginView(model);
                }

                // Store user session in server-side session
                HttpContext.Session.SetString("Username", userSession.Username);
                HttpContext.Session.SetString("AccountType", userSession.AccountType);
                HttpContext.Session.SetString("IRDNumber", userSession.IRDNumber);
                HttpContext.Session.SetString("IsAuthenticated", userSession.IsAuthenticated.ToString());
                HttpContext.Session.SetString("LoginTime", userSession.LoginTime.ToString("O"));
                HttpContext.Session.SetString("AccessProfile", JsonSerializer.Serialize(userSession.AccessProfile));
                HttpContext.Session.SetString("BusinessRole", userSession.AccessProfile.BusinessRole);
                HttpContext.Session.SetString("JobRole", userSession.AccessProfile.JobRole);
                HttpContext.Session.SetString("DashboardMode", userSession.AccessProfile.DashboardMode);
                HttpContext.Session.SetString("RequiresSecondaryAuth", userSession.AccessProfile.RequiresSecondaryAuth.ToString());
                await _sessionAuditService.RecordLoginAsync(
                    HttpContext.Session.Id,
                    userSession,
                    HttpContext.Connection.RemoteIpAddress?.ToString());

                _logger.LogInformation($"User '{model.Username}' logged in successfully from IP: {HttpContext.Connection.RemoteIpAddress}");

                // Redirect to appropriate dashboard based on account type
                return RedirectToDashboard();
            }
            catch (Exception ex)
            {
                _logger.LogError($"Login error: {ex.Message}\n{ex.StackTrace}");
                SetPageMessage(MainSystemAuthMessages.AsError(MainSystemAuthMessages.LoginFailed));
                return RenderLoginView(model);
            }
        }

        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult ForgotPassword(LoginRequest model)
        {
            var username = model.Username?.Trim() ?? string.Empty;
            if (!_passwordResetRequestService.TryCreateRequest(username, out var message))
            {
                SetPageMessage(FormatAuthMessage(message));
                return RenderLoginView(new LoginRequest { Username = username });
            }

            SetPageMessage(FormatInfoMessage(message), "info");
            return RenderLoginView(new LoginRequest { Username = username });
        }

        [HttpGet]
        [ActionName("Logout")]
        public IActionResult LogoutPage()
        {
            return RenderLogoutView();
        }

        /// <summary>
        /// Logs out the current user and clears the session.
        /// </summary>
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Logout()
        {
            var username = HttpContext.Session.GetString("Username");

            if (string.IsNullOrWhiteSpace(username))
            {
                HttpContext.Session.Clear();
                return RedirectToAction(nameof(Login));
            }

            await _authService.LogoutUserAsync(username);
            await _sessionAuditService.RecordLogoutAsync(
                HttpContext.Session.Id,
                username,
                "logout",
                HttpContext.Connection.RemoteIpAddress?.ToString());
            HttpContext.Session.Clear();

            _logger.LogInformation($"User '{username}' logged out");
            SetRedirectMessage(MainSystemAuthMessages.AsInfo(MainSystemAuthMessages.LoggedOutMessage), "info");

            return RedirectToAction(nameof(Login));
        }

        /// <summary>
        /// Redirects to the appropriate dashboard based on user's account type.
        /// </summary>
        private IActionResult RedirectToDashboard()
        {
            var accountType = HttpContext.Session.GetString("AccountType");
            if (AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                if (AccessProfileSessionHelper.IsAdminPortalProfile(accessProfile))
                {
                    return RedirectToAction("Dashboard", "Admin");
                }

                if (AccessProfileSessionHelper.IsEmployeePortalProfile(accessProfile))
                {
                    return RedirectToAction("Dashboard", "Employee");
                }
            }

            return (accountType ?? string.Empty).Trim().ToLowerInvariant() switch
            {
                "admin" => RedirectToAction("Dashboard", "Admin"),
                "employee" => RedirectToAction("Dashboard", "Employee"),
                "finance" => RedirectToAction("Dashboard", "Employee"),
                "trade" => RedirectToAction("Dashboard", "Employee"),
                _ => RedirectToAction("Login", "Login")
            };
        }

        private IActionResult RenderLoginView(LoginRequest? model = null)
        {
            RestoreRedirectMessage();

            return model == null
                ? View("~/Views/Login_Logout/Login.cshtml")
                : View("~/Views/Login_Logout/Login.cshtml", model);
        }

        private IActionResult RenderLogoutView()
        {
            RestoreRedirectMessage();
            return View("~/Views/Login_Logout/Logout.cshtml");
        }

        private void RestoreRedirectMessage()
        {
            if (TempData["AuthMessage"] is string authMessage)
            {
                ViewBag.AuthMessage = authMessage;
            }

            if (TempData["AuthMessageType"] is string authMessageType)
            {
                ViewBag.AuthMessageType = authMessageType;
            }
        }

        private void SetPageMessage(string message, string messageType = "error")
        {
            ViewBag.AuthMessage = message;
            ViewBag.AuthMessageType = messageType;
        }

        private void SetRedirectMessage(string message, string messageType = "error")
        {
            TempData["AuthMessage"] = message;
            TempData["AuthMessageType"] = messageType;
        }

        private string GetLoginValidationMessage()
        {
            if (ViewData.ModelState["Username"]?.Errors.Count > 0)
            {
                return MainSystemAuthMessages.AsError(MainSystemAuthMessages.UsernameInvalid);
            }

            if (ViewData.ModelState["Password"]?.Errors.Count > 0)
            {
                return MainSystemAuthMessages.AsError(MainSystemAuthMessages.PasswordMissingOrInvalid);
            }

            return MainSystemAuthMessages.AsError(MainSystemAuthMessages.InvalidInputTryAgain);
        }

        private static string FormatAuthMessage(string message)
        {
            if (string.IsNullOrWhiteSpace(message))
            {
                return MainSystemAuthMessages.AsError(MainSystemAuthMessages.LoginFailed);
            }

            if (message.StartsWith(MainSystemAuthMessages.DisplayError, StringComparison.OrdinalIgnoreCase) ||
                message.StartsWith(MainSystemAuthMessages.DisplayInfo, StringComparison.OrdinalIgnoreCase))
            {
                return message;
            }

            return MainSystemAuthMessages.AsError(message);
        }

        private static string FormatInfoMessage(string message)
        {
            if (string.IsNullOrWhiteSpace(message))
            {
                return MainSystemAuthMessages.AsInfo("Request sent.");
            }

            return message.StartsWith(MainSystemAuthMessages.DisplayInfo, StringComparison.OrdinalIgnoreCase)
                ? message
                : MainSystemAuthMessages.AsInfo(message);
        }
    }
}
