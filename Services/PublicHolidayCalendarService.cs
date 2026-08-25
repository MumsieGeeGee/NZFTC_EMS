using System.Globalization;
using System.Collections.Concurrent;

namespace NZFTC_EMS.Services
{
    public sealed class PublicHolidayEvent
    {
        public string Name { get; init; } = string.Empty;
        public string Date { get; init; } = string.Empty;
    }

    public interface IPublicHolidayCalendarService
    {
        IReadOnlyList<PublicHolidayEvent> GetPublicHolidayEvents(int year);
    }

    public sealed class PublicHolidayCalendarService : IPublicHolidayCalendarService
    {
        private const string PublicHolidaysFileExtension = ".txt";
        private const string PublicHolidaysFilePrefix = "Public_Holidays_NZ";
        private const string PublicHolidaysFolderName = "Calendar (NZ Public Holidays)";
        private readonly string _contentRootPath;
        private readonly ConcurrentDictionary<int, IReadOnlyList<PublicHolidayEvent>> _holidayCache = new();

        public PublicHolidayCalendarService(IWebHostEnvironment environment)
        {
            _contentRootPath = environment.ContentRootPath;
        }

        public IReadOnlyList<PublicHolidayEvent> GetPublicHolidayEvents(int year)
        {
            return _holidayCache.GetOrAdd(year, LoadPublicHolidayEvents);
        }

        private IReadOnlyList<PublicHolidayEvent> LoadPublicHolidayEvents(int year)
        {
            var filePath = ResolvePublicHolidayFilePath(year);
            if (filePath == null || !File.Exists(filePath))
            {
                return Array.Empty<PublicHolidayEvent>();
            }

            var holidays = new List<PublicHolidayEvent>();
            foreach (var line in File.ReadLines(filePath))
            {
                if (string.IsNullOrWhiteSpace(line))
                {
                    continue;
                }

                var colonIndex = line.IndexOf(':');
                if (colonIndex < 0)
                {
                    continue;
                }

                var holidayName = line[..colonIndex].Trim();
                var holidayDateText = line[(colonIndex + 1)..].Trim();
                if (string.IsNullOrWhiteSpace(holidayName) || string.IsNullOrWhiteSpace(holidayDateText))
                {
                    continue;
                }

                if (!DateTime.TryParseExact(
                        holidayDateText,
                        "dd/MM/yyyy",
                        CultureInfo.InvariantCulture,
                        DateTimeStyles.None,
                        out var holidayDate))
                {
                    continue;
                }

                holidays.Add(new PublicHolidayEvent
                {
                    Name = holidayName,
                    Date = holidayDate.ToString("yyyy-MM-dd", CultureInfo.InvariantCulture)
                });
            }

            return holidays
                .OrderBy(holiday => holiday.Date, StringComparer.Ordinal)
                .ToArray();
        }

        private string? ResolvePublicHolidayFilePath(int year)
        {
            var fileName = $"{year}_{PublicHolidaysFilePrefix}{PublicHolidaysFileExtension}";
            var relativeCandidates = new[]
            {
                fileName,
                Path.Combine(PublicHolidaysFolderName, fileName),
                Path.Combine("..", PublicHolidaysFolderName, fileName),
                Path.Combine("..", "..", PublicHolidaysFolderName, fileName),
                Path.Combine("main", "Main_System", PublicHolidaysFolderName, fileName)
            };

            foreach (var candidate in relativeCandidates)
            {
                var absolutePath = Path.GetFullPath(Path.Combine(_contentRootPath, candidate));
                if (File.Exists(absolutePath))
                {
                    return absolutePath;
                }
            }

            return null;
        }
    }
}
