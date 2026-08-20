using Microsoft.AspNetCore.Mvc;
using NZFTC_EMS.Models;
using NZFTC_EMS.Services;
using NZFTC_EMS.Utilities;

namespace NZFTC_EMS.Controllers
{
    public class LoginController : Controller
    {
        private readonly IAuthenticationService _authService;
        private readonly ILogger<LoginController> _logger;

        public LoginController(IAuthenticationService authService, ILogger<LoginController> logger)
        {
            _authService = authService;
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
                    SetPageMessage(MainSystemAuthMessages.AsError(message));
                    _logger.LogWarning($"Failed login attempt for username: {model.Username}. Reason: {message}");
                    return RenderLoginView(model);
                }

                // Store user session in server-side session
                HttpContext.Session.SetString("Username", userSession.Username);
                HttpContext.Session.SetString("AccountType", userSession.AccountType);
                HttpContext.Session.SetString("IRDNumber", userSession.IRDNumber);
                HttpContext.Session.SetString("IsAuthenticated", userSession.IsAuthenticated.ToString());
                HttpContext.Session.SetString("LoginTime", userSession.LoginTime.ToString("O"));

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
        public IActionResult Logout()
        {
            var username = HttpContext.Session.GetString("Username");

            if (string.IsNullOrWhiteSpace(username))
            {
                HttpContext.Session.Clear();
                return RedirectToAction(nameof(Login));
            }

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

            return accountType switch
            {
                "Admin" => RedirectToAction("Dashboard", "Admin"),
                "User" => RedirectToAction("Dashboard", "Employee"),
                "Finance" => RedirectToAction("Dashboard", "Employee"),
                "Trade" => RedirectToAction("Dashboard", "Employee"),
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
    }
}
