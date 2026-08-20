using Microsoft.AspNetCore.Mvc;

namespace NZFTC_EMS.Controllers
{
    public class EmployeeController : Controller
    {
        private readonly ILogger<EmployeeController> _logger;

        public EmployeeController(ILogger<EmployeeController> logger)
        {
            _logger = logger;
        }

        public IActionResult Dashboard()
        {
            var username = HttpContext.Session.GetString("Username");
            var accountType = HttpContext.Session.GetString("AccountType");
            var loginTime = HttpContext.Session.GetString("LoginTime");

            if (string.IsNullOrEmpty(username))
            {
                return RedirectToAction("Login", "Login");
            }

            ViewBag.Username = username;
            ViewBag.AccountType = accountType;
            ViewBag.LoginTime = loginTime;

            return View();
        }

        public IActionResult AccountManagement()
        {
            var username = HttpContext.Session.GetString("Username");
            var accountType = HttpContext.Session.GetString("AccountType");
            var loginTime = HttpContext.Session.GetString("LoginTime");

            if (string.IsNullOrEmpty(username))
            {
                return RedirectToAction("Login", "Login");
            }

            ViewBag.Username = username;
            ViewBag.AccountType = accountType;
            ViewBag.LoginTime = loginTime;

            return View("Account_Management");
        }

        public IActionResult EmployeeManagement()
        {
            var username = HttpContext.Session.GetString("Username");
            var accountType = HttpContext.Session.GetString("AccountType");
            var loginTime = HttpContext.Session.GetString("LoginTime");

            if (string.IsNullOrEmpty(username))
            {
                return RedirectToAction("Login", "Login");
            }

            ViewBag.Username = username;
            ViewBag.AccountType = accountType;
            ViewBag.LoginTime = loginTime;

            return View("Employee_Management");
        }

        public IActionResult HRManagement()
        {
            var username = HttpContext.Session.GetString("Username");
            var accountType = HttpContext.Session.GetString("AccountType");
            var loginTime = HttpContext.Session.GetString("LoginTime");

            if (string.IsNullOrEmpty(username))
            {
                return RedirectToAction("Login", "Login");
            }

            ViewBag.Username = username;
            ViewBag.AccountType = accountType;
            ViewBag.LoginTime = loginTime;

            return View("HR_Management");
        }
    }
}
