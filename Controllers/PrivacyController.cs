using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using NZFTC_EMS.Models;

namespace NZFTC_EMS.Controllers;

public class PrivacyController : Controller
{
    public IActionResult Privacy_Policy()
    {
        return View();
    }

    [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
    public IActionResult Error()
    {
        return View("Error", new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
    }
}
