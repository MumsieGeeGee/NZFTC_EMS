using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace NZFTC_EMS.Services;

public class EmployeeAccountRecordService
{
    private static readonly StringComparer FieldComparer = StringComparer.OrdinalIgnoreCase;
    private readonly ILogger<EmployeeAccountRecordService> _logger;

    public EmployeeAccountRecordService(ILogger<EmployeeAccountRecordService> logger)
    {
        _logger = logger;
    }

    public bool TryGetAccountDetails(string username, out IReadOnlyDictionary<string, string> details)
    {
        details = new Dictionary<string, string>(FieldComparer);
        if (string.IsNullOrWhiteSpace(username))
        {
            return false;
        }

        var recordRoot = ResolveEmployeeRecordRoot();
        if (string.IsNullOrWhiteSpace(recordRoot) || !Directory.Exists(recordRoot))
        {
            _logger.LogWarning("Employee record root not found when resolving account details for {Username}.", username);
            return false;
        }

        foreach (var filePath in EnumerateAccountRecordFiles(recordRoot))
        {
            var parsed = ParseKeyValueFile(filePath);
            if (!parsed.TryGetValue("Username", out var fileUsername) ||
                !string.Equals(fileUsername, username, StringComparison.OrdinalIgnoreCase))
            {
                continue;
            }

            details = parsed;
            return true;
        }

        _logger.LogInformation("No employee account record file matched username {Username}.", username);
        return false;
    }

    public IReadOnlyList<string> GetUsernamesByPrefix(string prefix)
    {
        if (string.IsNullOrWhiteSpace(prefix))
        {
            return Array.Empty<string>();
        }

        var recordRoot = ResolveEmployeeRecordRoot();
        if (string.IsNullOrWhiteSpace(recordRoot) || !Directory.Exists(recordRoot))
        {
            _logger.LogWarning("Employee record root not found when listing usernames for prefix {Prefix}.", prefix);
            return Array.Empty<string>();
        }

        var usernames = new List<string>();
        foreach (var filePath in EnumerateAccountRecordFiles(recordRoot))
        {
            var parsed = ParseKeyValueFile(filePath);
            if (!parsed.TryGetValue("Username", out var fileUsername) ||
                string.IsNullOrWhiteSpace(fileUsername))
            {
                continue;
            }

            if (fileUsername.StartsWith(prefix, StringComparison.OrdinalIgnoreCase))
            {
                usernames.Add(fileUsername.Trim());
            }
        }

        return usernames
            .Distinct(StringComparer.OrdinalIgnoreCase)
            .OrderBy(value => value, StringComparer.OrdinalIgnoreCase)
            .ToList();
    }

    public IReadOnlyDictionary<string, IReadOnlyList<string>> GetUsernamesByGroup()
    {
        var usernamesByGroup = new Dictionary<string, IReadOnlyList<string>>(StringComparer.OrdinalIgnoreCase);
        var recordRoot = ResolveEmployeeRecordRoot();
        if (string.IsNullOrWhiteSpace(recordRoot) || !Directory.Exists(recordRoot))
        {
            _logger.LogWarning("Employee record root not found when listing usernames by group.");
            return usernamesByGroup;
        }

        foreach (var groupDirectory in Directory.EnumerateDirectories(recordRoot))
        {
            var groupName = Path.GetFileName(groupDirectory).Trim();
            if (string.IsNullOrWhiteSpace(groupName))
            {
                continue;
            }

            var usernames = Directory.EnumerateFiles(groupDirectory, "*_Employee_Record.txt", SearchOption.AllDirectories)
                .Where(filePath => filePath.IndexOf($"{Path.DirectorySeparatorChar}IRD{Path.DirectorySeparatorChar}", StringComparison.OrdinalIgnoreCase) < 0)
                .Select(ParseUsernameFromRecordFile)
                .Where(username => !string.IsNullOrWhiteSpace(username))
                .Select(username => username.Trim())
                .Distinct(StringComparer.OrdinalIgnoreCase)
                .OrderBy(value => value, StringComparer.OrdinalIgnoreCase)
                .ToList();

            usernamesByGroup[groupName.ToLowerInvariant()] = usernames;
        }

        return usernamesByGroup;
    }

    public IReadOnlyDictionary<string, string> GetEmployeeDisplayNamesByUsername(IEnumerable<string> usernames)
    {
        var displayNames = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
        foreach (var username in usernames.Where(value => !string.IsNullOrWhiteSpace(value)).Distinct(StringComparer.OrdinalIgnoreCase))
        {
            var normalizedUsername = username.Trim();
            if (!TryGetAccountDetails(normalizedUsername, out var details))
            {
                displayNames[normalizedUsername] = normalizedUsername;
                continue;
            }

            var firstName = GetDetail(details, "First Name");
            var middleName = GetDetail(details, "Middle Name");
            var lastName = GetDetail(details, "Last Name");
            var displayName = string.Join(" ", new[] { firstName, middleName, lastName }.Where(part => !string.IsNullOrWhiteSpace(part)));
            displayNames[normalizedUsername] = string.IsNullOrWhiteSpace(displayName)
                ? normalizedUsername
                : displayName;
        }

        return displayNames;
    }

    public bool UpdateEmployeeDetails(
        string username,
        string emergencyContact,
        string diagnosedHealthConditions,
        string allergies,
        string approvedEmergencyStepsAndMedicines,
        string priorTrainingAndQualifications,
        string firstAidCertification)
    {
        if (string.IsNullOrWhiteSpace(username))
        {
            return false;
        }

        if (!TryGetAccountRecordFilePath(username, out var filePath))
        {
            return false;
        }

        var lines = File.ReadAllLines(filePath).ToList();
        var updates = new Dictionary<string, string>(FieldComparer)
        {
            ["Emergency Contact"] = emergencyContact.Trim(),
            ["Diagnosed Health Conditions"] = diagnosedHealthConditions.Trim(),
            ["Allergies"] = allergies.Trim(),
            ["Approved Emergency Steps and Medicines"] = approvedEmergencyStepsAndMedicines.Trim(),
            ["Prior Training and Qualifications"] = priorTrainingAndQualifications.Trim(),
            ["First Aid Certification"] = NormalizeFirstAidCertification(firstAidCertification)
        };

        for (var index = 0; index < lines.Count; index++)
        {
            var line = lines[index].Trim();
            var separatorIndex = line.IndexOf(':');
            if (separatorIndex <= 0)
            {
                continue;
            }

            var key = line.Substring(0, separatorIndex).Trim();
            if (!updates.TryGetValue(key, out var value))
            {
                continue;
            }

            lines[index] = $"{key}: {value}";
            updates.Remove(key);
        }

        if (updates.Count > 0)
        {
            var insertIndex = lines.FindIndex(line => line.TrimStart().StartsWith("Current Open Leave Requests:", StringComparison.OrdinalIgnoreCase));
            if (insertIndex < 0)
            {
                insertIndex = lines.Count;
            }

            var newEntries = updates.Select(update => $"{update.Key}: {update.Value}").ToList();
            if (insertIndex > 0 && !string.IsNullOrWhiteSpace(lines[insertIndex - 1]))
            {
                newEntries.Insert(0, string.Empty);
            }

            lines.InsertRange(insertIndex, newEntries);
        }

        File.WriteAllLines(filePath, lines);
        return true;
    }

    private static IReadOnlyDictionary<string, string> ParseKeyValueFile(string filePath)
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

    private static string ParseUsernameFromRecordFile(string filePath)
    {
        var parsed = ParseKeyValueFile(filePath);
        return parsed.TryGetValue("Username", out var username) ? username : string.Empty;
    }

    private static string GetDetail(
        IReadOnlyDictionary<string, string> details,
        string key)
    {
        return details.TryGetValue(key, out var value) && !string.IsNullOrWhiteSpace(value)
            ? value.Trim()
            : string.Empty;
    }

    private static string NormalizeFirstAidCertification(string? firstAidCertification)
    {
        return string.Equals(firstAidCertification?.Trim(), "Yes", StringComparison.OrdinalIgnoreCase)
            ? "Certified"
            : "Uncertified";
    }

    private bool TryGetAccountRecordFilePath(string username, out string filePath)
    {
        filePath = string.Empty;
        var recordRoot = ResolveEmployeeRecordRoot();
        if (string.IsNullOrWhiteSpace(recordRoot) || !Directory.Exists(recordRoot))
        {
            return false;
        }

        foreach (var candidate in EnumerateAccountRecordFiles(recordRoot))
        {
            var parsed = ParseKeyValueFile(candidate);
            if (parsed.TryGetValue("Username", out var fileUsername) &&
                string.Equals(fileUsername.Trim(), username.Trim(), StringComparison.OrdinalIgnoreCase))
            {
                filePath = candidate;
                return true;
            }
        }

        return false;
    }

    private static string ResolveEmployeeRecordRoot()
    {
        var candidates = new[]
        {
            Path.Combine(Directory.GetCurrentDirectory(), "main", "Main_System", "Employee Management", "Employee_Records"),
            Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "main", "Main_System", "Employee Management", "Employee_Records")),
            Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "..", "main", "Main_System", "Employee Management", "Employee_Records"))
        };

        return candidates.FirstOrDefault(Directory.Exists) ?? string.Empty;
    }

    private static IEnumerable<string> EnumerateAccountRecordFiles(string recordRoot)
    {
        foreach (var filePath in Directory.EnumerateFiles(recordRoot, "*_Employee_Record.txt", SearchOption.AllDirectories))
        {
            if (filePath.IndexOf($"{Path.DirectorySeparatorChar}IRD{Path.DirectorySeparatorChar}", StringComparison.OrdinalIgnoreCase) >= 0)
            {
                continue;
            }

            yield return filePath;
        }
    }
}
