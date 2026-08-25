using Microsoft.AspNetCore.Mvc;
using NZFTC_EMS.Services;

namespace NZFTC_EMS.Controllers
{
    public class SessionController : Controller
    {
        private readonly IAuthenticationService _authService;

        public SessionController(IAuthenticationService authService)
        {
            _authService = authService;
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
            return Ok();
        }

        [HttpPost]
        public async Task<IActionResult> TimeoutLogout()
        {
            var username = HttpContext.Session.GetString("Username");
            if (!string.IsNullOrWhiteSpace(username))
            {
                await _authService.LogoutUserAsync(username);
            }

            HttpContext.Session.Clear();
            return Ok(new { redirectUrl = Url.Action("Login", "Login") });
        }
    }
}