using Microsoft.AspNetCore.Mvc;
using NZFTC_EMS.Services;
using NZFTC_EMS.Utilities;

namespace NZFTC_EMS.Controllers
{
    [Route("api/calendar")]
    public class CalendarController : Controller
    {
        private readonly IPublicHolidayCalendarService _holidayService;

        public CalendarController(IPublicHolidayCalendarService holidayService)
        {
            _holidayService = holidayService;
        }

        [HttpGet("events")]
        public IActionResult GetEvents([FromQuery] int? year = null)
        {
            if (string.IsNullOrWhiteSpace(HttpContext.Session.GetString("Username")))
            {
                return Unauthorized();
            }

            if (!AccessProfileSessionHelper.TryGetAccessProfile(HttpContext.Session, out var accessProfile))
            {
                return Unauthorized();
            }

            var selectedYear = year ?? DateTime.Now.Year;
            if (selectedYear < 1900 || selectedYear > 3000)
            {
                return BadRequest(new { message = "Invalid year." });
            }

            var holidays = _holidayService.GetPublicHolidayEvents(selectedYear);
            return Ok(new
            {
                year = selectedYear,
                scope = CalendarAccessScopeResolver.ResolveScope(accessProfile),
                publicHolidays = holidays
            });
        }
    }
}