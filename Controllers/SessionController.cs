using Microsoft.AspNetCore.Mvc;
using NZFTC_EMS.Services;

namespace NZFTC_EMS.Controllers
{
    public class SessionController : Controller
    {
        private readonly IAuthenticationService _authService;
        private readonly SessionAuditService _sessionAuditService;

        public SessionController(IAuthenticationService authService, SessionAuditService sessionAuditService)
        {
            _authService = authService;
            _sessionAuditService = sessionAuditService;
        }

        [HttpPost]
        public IActionResult Touch()
        {
            var username = HttpContext.Session.GetString("Username");
            if (string.IsNullOrEmpty(username))
            {
                return Unauthorized();
            }

            HttpContext.Session.SetString("LastActivityTime", DateTime.UtcNow.ToString("O"));
            _sessionAuditService.TouchSessionAsync(HttpContext.Session.Id).GetAwaiter().GetResult();
            return Ok();
        }

        [HttpPost]
        public async Task<IActionResult> TimeoutLogout()
        {
            var username = HttpContext.Session.GetString("Username");
            if (!string.IsNullOrWhiteSpace(username))
            {
                await _authService.LogoutUserAsync(username);
                await _sessionAuditService.RecordLogoutAsync(
                    HttpContext.Session.Id,
                    username,
                    "timeout-logout",
                    HttpContext.Connection.RemoteIpAddress?.ToString());
            }

            HttpContext.Session.Clear();
            return Ok(new { redirectUrl = Url.Action("Login", "Login") });
        }
    }
}