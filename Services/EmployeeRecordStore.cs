using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace NZFTC_EMS.Services;

public class EmployeeRecordStore
{
    private static readonly StringComparer FieldComparer = StringComparer.OrdinalIgnoreCase;

    public string ResolveMainSystemRoot()
    {
        var candidates = new[]
        {
            Path.Combine(Directory.GetCurrentDirectory(), "main", "Main_System"),
            Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "main", "Main_System")),
            Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "..", "main", "Main_System"))
        };

        return candidates.FirstOrDefault(Directory.Exists) ?? string.Empty;
    }

    public string ResolveEmployeeRecordRoot()
    {
        var mainSystemRoot = ResolveMainSystemRoot();
        return string.IsNullOrWhiteSpace(mainSystemRoot)
            ? string.Empty
            : Path.Combine(mainSystemRoot, "Employee Management", "Employee_Records");
    }

    public string ResolveIrdDirectory()
    {
        var employeeRecordRoot = ResolveEmployeeRecordRoot();
        return string.IsNullOrWhiteSpace(employeeRecordRoot)
            ? string.Empty
            : Path.Combine(employeeRecordRoot, "IRD");
    }

    public IEnumerable<string> EnumerateEmployeeRecordFiles()
    {
        var employeeRecordRoot = ResolveEmployeeRecordRoot();
        if (string.IsNullOrWhiteSpace(employeeRecordRoot) || !Directory.Exists(employeeRecordRoot))
        {
            yield break;
        }

        foreach (var filePath in Directory.EnumerateFiles(employeeRecordRoot, "*_Employee_Record.txt", SearchOption.AllDirectories))
        {
            if (filePath.IndexOf($"{Path.DirectorySeparatorChar}IRD{Path.DirectorySeparatorChar}", StringComparison.OrdinalIgnoreCase) >= 0)
            {
                continue;
            }

            yield return filePath;
        }
    }

    public IEnumerable<string> EnumerateIrdFiles()
    {
        var irdDirectory = ResolveIrdDirectory();
        if (string.IsNullOrWhiteSpace(irdDirectory) || !Directory.Exists(irdDirectory))
        {
            return Array.Empty<string>();
        }

        return Directory.EnumerateFiles(irdDirectory, "*.txt", SearchOption.TopDirectoryOnly)
            .Where(filePath => !string.Equals(Path.GetFileName(filePath), "IRD_Username.txt", StringComparison.OrdinalIgnoreCase));
    }

    public IReadOnlyDictionary<string, string> ParseKeyValueFile(string filePath)
    {
        var parsed = new Dictionary<string, string>(FieldComparer);
        foreach (var rawLine in File.ReadLines(filePath))
        {
            var line = rawLine.Trim();
            if (string.IsNullOrWhiteSpace(line))
            {
                continue;
            }

            var separatorIndex = line.IndexOf(':');
            if (separatorIndex <= 0 || separatorIndex == line.Length - 1)
            {
                continue;
            }

            var key = line.Substring(0, separatorIndex).Trim();
            var value = line.Substring(separatorIndex + 1).Trim();
            if (!string.IsNullOrWhiteSpace(key))
            {
                parsed[key] = value;
            }
        }

        return parsed;
    }

    public bool TryGetEmployeeRecord(string username, out IReadOnlyDictionary<string, string> details)
    {
        details = new Dictionary<string, string>(FieldComparer);
        var normalizedUsername = username?.Trim() ?? string.Empty;
        if (string.IsNullOrWhiteSpace(normalizedUsername))
        {
            return false;
        }

        foreach (var filePath in EnumerateEmployeeRecordFiles())
        {
            var parsed = ParseKeyValueFile(filePath);
            if (!parsed.TryGetValue("Username", out var fileUsername) ||
                !string.Equals(fileUsername?.Trim(), normalizedUsername, StringComparison.OrdinalIgnoreCase))
            {
                continue;
            }

            details = parsed;
            return true;
        }

        return false;
    }

    public string? ResolveEmployeeDisplayName(string username)
    {
        if (!TryGetEmployeeRecord(username, out var record))
        {
            return null;
        }

        var firstName = TryGetValue(record, "First Name");
        var middleName = TryGetValue(record, "Middle Name");
        var lastName = TryGetValue(record, "Last Name");
        var displayName = string.Join(" ", new[] { firstName, middleName, lastName }.Where(value => !string.IsNullOrWhiteSpace(value))).Trim();
        return string.IsNullOrWhiteSpace(displayName) ? string.Empty : displayName;
    }

    public static string TryGetValue(IReadOnlyDictionary<string, string> details, string key)
    {
        return details.TryGetValue(key, out var value) && !string.IsNullOrWhiteSpace(value)
            ? value.Trim()
            : string.Empty;
    }
}
