using Microsoft.AspNetCore.Mvc;

namespace NZFTC_EMS.Controllers
{
    public class SessionController : Controller
    {
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
        public IActionResult TimeoutLogout()
        {
            HttpContext.Session.Clear();
            return Ok(new { redirectUrl = Url.Action("Login", "Login") });
        }
    }
}