using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.Json;
using NZFTC_EMS.Data;
using NZFTC_EMS.Data.Entities;

namespace NZFTC_EMS.Services;

public class EmployeeAccountRecordService
{
    private static readonly StringComparer FieldComparer = StringComparer.OrdinalIgnoreCase;
    private static readonly JsonSerializerOptions SnapshotJsonOptions = new()
    {
        PropertyNameCaseInsensitive = true
    };
    private readonly MySqlRepository _mySqlRepository;
    private readonly EmployeeRecordStore _employeeRecordStore;
    private readonly ILogger<EmployeeAccountRecordService> _logger;

    public EmployeeAccountRecordService(
        MySqlRepository mySqlRepository,
        EmployeeRecordStore employeeRecordStore,
        ILogger<EmployeeAccountRecordService> logger)
    {
        _mySqlRepository = mySqlRepository;
        _employeeRecordStore = employeeRecordStore;
        _logger = logger;
    }

    public bool TryGetAccountDetails(string username, out IReadOnlyDictionary<string, string> details)
    {
        details = new Dictionary<string, string>(FieldComparer);
        if (string.IsNullOrWhiteSpace(username))
        {
            return false;
        }

        if (TryGetAccountDetailsFromDatabase(username, out details))
        {
            return true;
        }

        var recordRoot = _employeeRecordStore.ResolveEmployeeRecordRoot();
        if (string.IsNullOrWhiteSpace(recordRoot) || !Directory.Exists(recordRoot))
        {
            _logger.LogWarning("Employee record root not found when resolving account details for {Username}.", username);
            return false;
        }

        if (_employeeRecordStore.TryGetEmployeeRecord(username, out var record))
        {
            details = record;
            SyncEmployeeSnapshotToDatabase(username, record);
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

        var databaseUsernames = GetAllAccountDetailsByUsername()
            .Keys
            .Where(value => value.StartsWith(prefix, StringComparison.OrdinalIgnoreCase))
            .OrderBy(value => value, StringComparer.OrdinalIgnoreCase)
            .ToList();
        if (databaseUsernames.Count > 0)
        {
            return databaseUsernames;
        }

        var usernames = new List<string>();
        foreach (var filePath in _employeeRecordStore.EnumerateEmployeeRecordFiles())
        {
            var parsed = _employeeRecordStore.ParseKeyValueFile(filePath);
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
        var databaseDetails = _mySqlRepository.ListEmployeeAccountDetailsAsync().GetAwaiter().GetResult();
        if (databaseDetails.Count > 0)
        {
            return databaseDetails
                .Select(entity => new
                {
                    Username = entity.Username?.Trim() ?? string.Empty,
                    GroupName = ResolveGroupName(entity)
                })
                .Where(item => !string.IsNullOrWhiteSpace(item.Username) && !string.IsNullOrWhiteSpace(item.GroupName))
                .GroupBy(item => item.GroupName, StringComparer.OrdinalIgnoreCase)
                .ToDictionary(
                    group => group.Key,
                    group => (IReadOnlyList<string>)group
                        .Select(item => item.Username)
                        .Distinct(StringComparer.OrdinalIgnoreCase)
                        .OrderBy(value => value, StringComparer.OrdinalIgnoreCase)
                        .ToList(),
                    StringComparer.OrdinalIgnoreCase);
        }

        var usernamesByGroup = new Dictionary<string, IReadOnlyList<string>>(StringComparer.OrdinalIgnoreCase);
        var recordRoot = _employeeRecordStore.ResolveEmployeeRecordRoot();
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
                .Select(filePath =>
                {
                    var parsed = _employeeRecordStore.ParseKeyValueFile(filePath);
                    return parsed.TryGetValue("Username", out var username) ? username : string.Empty;
                })
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

    public IReadOnlyList<string> GetAllUsernames()
    {
        return GetUsernamesByGroup()
            .SelectMany(pair => pair.Value)
            .Where(username => !string.IsNullOrWhiteSpace(username))
            .Select(username => username.Trim())
            .Distinct(StringComparer.OrdinalIgnoreCase)
            .OrderBy(value => value, StringComparer.OrdinalIgnoreCase)
            .ToList();
    }

    public IReadOnlyList<string> GetAllRecordUsernames()
    {
        return _employeeRecordStore
            .EnumerateEmployeeRecordFiles()
            .Select(filePath =>
            {
                var parsed = _employeeRecordStore.ParseKeyValueFile(filePath);
                return parsed.TryGetValue("Username", out var username) ? username : string.Empty;
            })
            .Where(username => !string.IsNullOrWhiteSpace(username))
            .Select(username => username.Trim())
            .Distinct(StringComparer.OrdinalIgnoreCase)
            .OrderBy(value => value, StringComparer.OrdinalIgnoreCase)
            .ToList();
    }

    public string GetDisplayNameForUser(string username)
    {
        var normalizedUsername = username?.Trim() ?? string.Empty;
        if (string.IsNullOrWhiteSpace(normalizedUsername) ||
            !TryGetAccountDetails(normalizedUsername, out var details))
        {
            return normalizedUsername;
        }

        return BuildDisplayName(normalizedUsername, details);
    }

    public string GetGroupNameForUser(string username)
    {
        var normalizedUsername = username?.Trim() ?? string.Empty;
        if (string.IsNullOrWhiteSpace(normalizedUsername) ||
            !TryGetAccountDetails(normalizedUsername, out var details))
        {
            return string.Empty;
        }

        return ResolveGroupName(details);
    }

    public IReadOnlyList<string> GetPasswordResetRecipientUsernames(string username)
    {
        var normalizedUsername = username?.Trim() ?? string.Empty;
        if (string.IsNullOrWhiteSpace(normalizedUsername))
        {
            return Array.Empty<string>();
        }

        var recordDetailsByUsername = GetAllRecordAccountDetailsByUsername();
        if (!recordDetailsByUsername.TryGetValue(normalizedUsername, out var targetDetails))
        {
            return Array.Empty<string>();
        }

        var directResponsibilityUsernames = new HashSet<string>(
            new[]
            {
                GetDetail(targetDetails, "Main Manager"),
                GetDetail(targetDetails, "Assigned Manager"),
                GetDetail(targetDetails, "Assigned Mentor")
            }
            .Where(value => !string.IsNullOrWhiteSpace(value))
            .Select(value => value.Trim()),
            StringComparer.OrdinalIgnoreCase);

        var directRecipients = recordDetailsByUsername
            .Where(pair =>
                !string.Equals(pair.Key, normalizedUsername, StringComparison.OrdinalIgnoreCase) &&
                directResponsibilityUsernames.Contains(pair.Key) &&
                HasPasswordResetAuthority(pair.Value))
            .Select(pair => pair.Key)
            .Distinct(StringComparer.OrdinalIgnoreCase)
            .OrderBy(value => value, StringComparer.OrdinalIgnoreCase)
            .ToList();

        if (directRecipients.Count > 0)
        {
            return directRecipients;
        }

        return recordDetailsByUsername
            .Where(pair =>
                !string.Equals(pair.Key, normalizedUsername, StringComparison.OrdinalIgnoreCase) &&
                HasPasswordResetAuthority(pair.Value))
            .Select(pair => pair.Key)
            .Distinct(StringComparer.OrdinalIgnoreCase)
            .OrderBy(value => value, StringComparer.OrdinalIgnoreCase)
            .ToList();
    }

    public IReadOnlyList<LockedAccountSummary> GetLockedAccounts()
    {
        return GetAllAccountDetailsByUsername()
            .Where(pair => string.Equals(GetDetail(pair.Value, "Account Status"), "Locked", StringComparison.OrdinalIgnoreCase))
            .Select(pair => new LockedAccountSummary(
                pair.Key,
                BuildDisplayName(pair.Key, pair.Value),
                ResolveGroupName(pair.Value)))
            .OrderBy(item => item.DisplayName, StringComparer.OrdinalIgnoreCase)
            .ToList();
    }

    public bool UnlockAccount(string username, string actorUsername)
    {
        var normalizedUsername = username?.Trim() ?? string.Empty;
        if (string.IsNullOrWhiteSpace(normalizedUsername) ||
            !TryGetAccountDetails(normalizedUsername, out var details))
        {
            return false;
        }

        var updatedDetails = new Dictionary<string, string>(details, FieldComparer)
        {
            ["Account Status"] = "Active",
            ["Failed Login Attempts"] = "0",
            ["Last Session Event"] = string.IsNullOrWhiteSpace(actorUsername)
                ? "Account unlocked by admin."
                : $"Account unlocked by {actorUsername.Trim()}."
        };

        if (string.IsNullOrWhiteSpace(EmployeeRecordStore.TryGetValue(updatedDetails, "Password Reset Required")))
        {
            updatedDetails["Password Reset Required"] = "No";
        }

        return UpdateAccountRecordFields(normalizedUsername, updatedDetails);
    }

    public bool UpdatePersonalAccountDetails(string username, string address, string emailAddress, string phoneNumber)
    {
        var normalizedUsername = username?.Trim() ?? string.Empty;
        if (string.IsNullOrWhiteSpace(normalizedUsername))
        {
            return false;
        }

        var updates = new Dictionary<string, string>(FieldComparer)
        {
            ["Address"] = address.Trim(),
            ["Email Address"] = emailAddress.Trim(),
            ["Phone Number"] = phoneNumber.Trim()
        };

        return UpdateAccountRecordFields(normalizedUsername, updates);
    }

    public IReadOnlyList<string> GetAssignedTeamUsernames(string supervisorUsername)
    {
        var normalizedSupervisor = supervisorUsername?.Trim() ?? string.Empty;
        if (string.IsNullOrWhiteSpace(normalizedSupervisor))
        {
            return Array.Empty<string>();
        }

        return GetAllAccountDetailsByUsername()
            .Where(pair =>
                !string.Equals(pair.Key, normalizedSupervisor, StringComparison.OrdinalIgnoreCase) &&
                (string.Equals(GetDetail(pair.Value, "Main Manager"), normalizedSupervisor, StringComparison.OrdinalIgnoreCase) ||
                 string.Equals(GetDetail(pair.Value, "Assigned Manager"), normalizedSupervisor, StringComparison.OrdinalIgnoreCase) ||
                 string.Equals(GetDetail(pair.Value, "Assigned Mentor"), normalizedSupervisor, StringComparison.OrdinalIgnoreCase)))
            .Select(pair => pair.Key)
            .Distinct(StringComparer.OrdinalIgnoreCase)
            .OrderBy(value => value, StringComparer.OrdinalIgnoreCase)
            .ToList();
    }

    public IReadOnlyList<string> GetAssistantDelegatedUsernames(string assistantUsername)
    {
        var normalizedAssistant = assistantUsername?.Trim() ?? string.Empty;
        if (string.IsNullOrWhiteSpace(normalizedAssistant) ||
            !TryGetAccountDetails(normalizedAssistant, out var assistantDetails))
        {
            return Array.Empty<string>();
        }

        var assistantJobRole = GetDetail(assistantDetails, "Job Role");
        if (string.Equals(assistantJobRole, "Executive Assistant", StringComparison.OrdinalIgnoreCase))
        {
            var primaryManager = GetDetail(assistantDetails, "Main Manager");
            var allowedRoles = new HashSet<string>(StringComparer.OrdinalIgnoreCase)
            {
                "Senior Level Manager",
                "Senior Finance Manager",
                "Senior Trade Manager"
            };

            return GetAllAccountDetailsByUsername()
                .Where(pair => allowedRoles.Contains(GetDetail(pair.Value, "Job Role")))
                .Select(pair => pair.Key)
                .Distinct(StringComparer.OrdinalIgnoreCase)
                .OrderBy(username => string.Equals(username, primaryManager, StringComparison.OrdinalIgnoreCase) ? 0 : 1)
                .ThenBy(username => username, StringComparer.OrdinalIgnoreCase)
                .ToList();
        }

        if (!string.Equals(assistantJobRole, "Personal Assistant", StringComparison.OrdinalIgnoreCase))
        {
            return Array.Empty<string>();
        }

        var assignedManager = GetDetail(assistantDetails, "Assigned Manager");
        if (string.IsNullOrWhiteSpace(assignedManager) ||
            !TryGetAccountDetails(assignedManager, out var managerDetails))
        {
            return Array.Empty<string>();
        }

        var managerJobRole = GetDetail(managerDetails, "Job Role");
        var isJuniorManager =
            string.Equals(managerJobRole, "Junior Level Manager", StringComparison.OrdinalIgnoreCase) ||
            string.Equals(managerJobRole, "Junior Finance Manager", StringComparison.OrdinalIgnoreCase) ||
            string.Equals(managerJobRole, "Junior Trade Manager", StringComparison.OrdinalIgnoreCase);
        return isJuniorManager
            ? new[] { assignedManager }
            : Array.Empty<string>();
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

        var updates = new Dictionary<string, string>(FieldComparer)
        {
            ["Emergency Contact"] = emergencyContact.Trim(),
            ["Diagnosed Health Conditions"] = diagnosedHealthConditions.Trim(),
            ["Allergies"] = allergies.Trim(),
            ["Approved Emergency Steps and Medicines"] = approvedEmergencyStepsAndMedicines.Trim(),
            ["Prior Training and Qualifications"] = priorTrainingAndQualifications.Trim(),
            ["First Aid Certification"] = NormalizeFirstAidCertification(firstAidCertification)
        };

        return UpdateAccountRecordFields(username, updates);
    }

    private void SyncEmployeeSnapshotToDatabase(string username, IReadOnlyDictionary<string, string> details)
    {
        var normalizedUsername = username?.Trim() ?? string.Empty;
        if (string.IsNullOrWhiteSpace(normalizedUsername))
        {
            return;
        }

        var employee = new EmployeeEntity
        {
            Username = normalizedUsername,
            FirstName = GetDetail(details, "First Name"),
            LastName = GetDetail(details, "Last Name"),
            BusinessRole = GetDetail(details, "Business Role"),
            JobRole = GetDetail(details, "Job Role"),
            AccountStatus = string.IsNullOrWhiteSpace(GetDetail(details, "Account Status"))
                ? "Active"
                : GetDetail(details, "Account Status"),
            Email = GetDetail(details, "Email Address"),
            Phone = GetDetail(details, "Phone Number"),
            UpdatedOnUtc = DateTime.UtcNow
        };

        var accountDetails = new EmployeeAccountDetailsEntity
        {
            Username = normalizedUsername,
            EmergencyContactEncrypted = GetDetail(details, "Emergency Contact"),
            DiagnosedHealthConditionsEncrypted = GetDetail(details, "Diagnosed Health Conditions"),
            AllergiesEncrypted = GetDetail(details, "Allergies"),
            ApprovedEmergencyStepsEncrypted = GetDetail(details, "Approved Emergency Steps and Medicines"),
            PriorTrainingEncrypted = GetDetail(details, "Prior Training and Qualifications"),
            FirstAidCertificationEncrypted = GetDetail(details, "First Aid Certification"),
            GroupName = ResolveGroupName(details),
            RecordSnapshotJson = JsonSerializer.Serialize(details, SnapshotJsonOptions),
            UpdatedOnUtc = DateTime.UtcNow
        };

        _mySqlRepository.UpsertEmployeeAsync(employee).GetAwaiter().GetResult();
        _mySqlRepository.UpsertEmployeeAccountDetailsAsync(accountDetails).GetAwaiter().GetResult();
    }

    private bool TryGetAccountDetailsFromDatabase(string username, out IReadOnlyDictionary<string, string> details)
    {
        details = new Dictionary<string, string>(FieldComparer);
        var normalizedUsername = username?.Trim() ?? string.Empty;
        if (string.IsNullOrWhiteSpace(normalizedUsername))
        {
            return false;
        }

        var accountDetailsEntity = _mySqlRepository.GetEmployeeAccountDetailsAsync(normalizedUsername).GetAwaiter().GetResult();
        var employeeEntity = _mySqlRepository.GetEmployeeAsync(normalizedUsername).GetAwaiter().GetResult();
        if (accountDetailsEntity == null && employeeEntity == null)
        {
            return false;
        }

        details = BuildDetailsDictionary(employeeEntity, accountDetailsEntity);
        return details.Count > 0;
    }

    private static string GetDetail(
        IReadOnlyDictionary<string, string> details,
        string key)
    {
        return EmployeeRecordStore.TryGetValue(details, key);
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
        var recordRoot = _employeeRecordStore.ResolveEmployeeRecordRoot();
        if (string.IsNullOrWhiteSpace(recordRoot) || !Directory.Exists(recordRoot))
        {
            return false;
        }

        foreach (var candidate in _employeeRecordStore.EnumerateEmployeeRecordFiles())
        {
            var parsed = _employeeRecordStore.ParseKeyValueFile(candidate);
            if (parsed.TryGetValue("Username", out var fileUsername) &&
                string.Equals(fileUsername.Trim(), username.Trim(), StringComparison.OrdinalIgnoreCase))
            {
                filePath = candidate;
                return true;
            }
        }

        return false;
    }

    private Dictionary<string, IReadOnlyDictionary<string, string>> GetAllAccountDetailsByUsername()
    {
        var databaseDetails = _mySqlRepository.ListEmployeeAccountDetailsAsync().GetAwaiter().GetResult();
        if (databaseDetails.Count > 0)
        {
            var employees = _mySqlRepository.ListEmployeesAsync().GetAwaiter().GetResult()
                .ToDictionary(entity => entity.Username, StringComparer.OrdinalIgnoreCase);
            return databaseDetails
                .Where(entity => !string.IsNullOrWhiteSpace(entity.Username))
                .ToDictionary(
                    entity => entity.Username.Trim(),
                    entity => (IReadOnlyDictionary<string, string>)BuildDetailsDictionary(
                        employees.TryGetValue(entity.Username, out var employee) ? employee : null,
                        entity),
                    StringComparer.OrdinalIgnoreCase);
        }

        var recordRoot = _employeeRecordStore.ResolveEmployeeRecordRoot();
        var detailsByUsername = new Dictionary<string, IReadOnlyDictionary<string, string>>(StringComparer.OrdinalIgnoreCase);
        if (string.IsNullOrWhiteSpace(recordRoot) || !Directory.Exists(recordRoot))
        {
            return detailsByUsername;
        }

        foreach (var filePath in _employeeRecordStore.EnumerateEmployeeRecordFiles())
        {
            var parsed = _employeeRecordStore.ParseKeyValueFile(filePath);
            if (!parsed.TryGetValue("Username", out var username) || string.IsNullOrWhiteSpace(username))
            {
                continue;
            }

            detailsByUsername[username.Trim()] = parsed;
        }

        return detailsByUsername;
    }

    private Dictionary<string, IReadOnlyDictionary<string, string>> GetAllRecordAccountDetailsByUsername()
    {
        return _employeeRecordStore
            .EnumerateEmployeeRecordFiles()
            .Select(filePath => _employeeRecordStore.ParseKeyValueFile(filePath))
            .Where(parsed => parsed.TryGetValue("Username", out var username) && !string.IsNullOrWhiteSpace(username))
            .ToDictionary(
                parsed => parsed["Username"].Trim(),
                parsed => (IReadOnlyDictionary<string, string>)parsed,
                StringComparer.OrdinalIgnoreCase);
    }

    private bool UpdateAccountRecordFields(string username, IReadOnlyDictionary<string, string> fieldValues)
    {
        var normalizedUsername = username?.Trim() ?? string.Empty;
        if (string.IsNullOrWhiteSpace(normalizedUsername))
        {
            return false;
        }

        if (!TryGetAccountRecordFilePath(normalizedUsername, out var filePath))
        {
            if (!TryGetAccountDetails(normalizedUsername, out var storedDetails))
            {
                return false;
            }

            var databaseOnlyUpdates = new Dictionary<string, string>(storedDetails, FieldComparer);
            foreach (var fieldValue in fieldValues)
            {
                databaseOnlyUpdates[fieldValue.Key] = fieldValue.Value;
            }

            SyncEmployeeSnapshotToDatabase(normalizedUsername, databaseOnlyUpdates);
            return true;
        }

        var pendingUpdates = new Dictionary<string, string>(fieldValues, FieldComparer);
        var lines = File.ReadAllLines(filePath).ToList();
        for (var index = 0; index < lines.Count; index++)
        {
            var line = lines[index].Trim();
            var separatorIndex = line.IndexOf(':');
            if (separatorIndex <= 0)
            {
                continue;
            }

            var key = line.Substring(0, separatorIndex).Trim();
            if (!pendingUpdates.TryGetValue(key, out var value))
            {
                continue;
            }

            lines[index] = $"{key}: {value}";
            pendingUpdates.Remove(key);
        }

        if (pendingUpdates.Count > 0)
        {
            var insertIndex = lines.FindIndex(line => line.TrimStart().StartsWith("Current Open Leave Requests:", StringComparison.OrdinalIgnoreCase));
            if (insertIndex < 0)
            {
                insertIndex = lines.Count;
            }

            var newEntries = pendingUpdates.Select(update => $"{update.Key}: {update.Value}").ToList();
            if (insertIndex > 0 && !string.IsNullOrWhiteSpace(lines[insertIndex - 1]))
            {
                newEntries.Insert(0, string.Empty);
            }

            lines.InsertRange(insertIndex, newEntries);
        }

        File.WriteAllLines(filePath, lines);
        if (_employeeRecordStore.TryGetEmployeeRecord(normalizedUsername, out var updatedRecord))
        {
            SyncEmployeeSnapshotToDatabase(normalizedUsername, updatedRecord);
        }

        return true;
    }

    private Dictionary<string, string> BuildDetailsDictionary(EmployeeEntity? employee, EmployeeAccountDetailsEntity? accountDetails)
    {
        Dictionary<string, string> details = new(FieldComparer);

        if (!string.IsNullOrWhiteSpace(accountDetails?.RecordSnapshotJson))
        {
            try
            {
                var parsed = JsonSerializer.Deserialize<Dictionary<string, string>>(accountDetails.RecordSnapshotJson, SnapshotJsonOptions);
                if (parsed != null)
                {
                    details = new Dictionary<string, string>(parsed, FieldComparer);
                }
            }
            catch (JsonException)
            {
                _logger.LogWarning("Stored account snapshot JSON could not be parsed for {Username}.", accountDetails.Username);
            }
        }

        if (employee != null)
        {
            details["Username"] = employee.Username;
            details["First Name"] = employee.FirstName;
            details["Last Name"] = employee.LastName;
            details["Business Role"] = employee.BusinessRole;
            details["Job Role"] = employee.JobRole;
            details["Account Status"] = employee.AccountStatus;
            if (!string.IsNullOrWhiteSpace(employee.Email))
            {
                details["Email Address"] = employee.Email;
            }

            if (!string.IsNullOrWhiteSpace(employee.Phone))
            {
                details["Phone Number"] = employee.Phone;
            }
        }

        if (accountDetails != null)
        {
            details["Emergency Contact"] = accountDetails.EmergencyContactEncrypted;
            details["Diagnosed Health Conditions"] = accountDetails.DiagnosedHealthConditionsEncrypted;
            details["Allergies"] = accountDetails.AllergiesEncrypted;
            details["Approved Emergency Steps and Medicines"] = accountDetails.ApprovedEmergencyStepsEncrypted;
            details["Prior Training and Qualifications"] = accountDetails.PriorTrainingEncrypted;
            details["First Aid Certification"] = accountDetails.FirstAidCertificationEncrypted;
        }

        return details;
    }

    private static string ResolveGroupName(IReadOnlyDictionary<string, string> details)
    {
        return GetDetail(details, "Business Role").Trim().ToLowerInvariant();
    }

    private static string ResolveGroupName(EmployeeAccountDetailsEntity entity)
    {
        return string.IsNullOrWhiteSpace(entity.GroupName)
            ? string.Empty
            : entity.GroupName.Trim().ToLowerInvariant();
    }

    private static string BuildDisplayName(string username, IReadOnlyDictionary<string, string> details)
    {
        var firstName = GetDetail(details, "First Name");
        var middleName = GetDetail(details, "Middle Name");
        var lastName = GetDetail(details, "Last Name");
        var displayName = string.Join(" ", new[] { firstName, middleName, lastName }.Where(part => !string.IsNullOrWhiteSpace(part)));
        return string.IsNullOrWhiteSpace(displayName) ? username : displayName;
    }

    private static bool HasPasswordResetAuthority(IReadOnlyDictionary<string, string> details)
    {
        var jobRole = GetDetail(details, "Job Role");
        return string.Equals(jobRole, "Junior IT Support Administrator", StringComparison.OrdinalIgnoreCase)
            || string.Equals(jobRole, "Senior IT Support Administrator", StringComparison.OrdinalIgnoreCase)
            || string.Equals(jobRole, "Senior Level Manager", StringComparison.OrdinalIgnoreCase)
            || string.Equals(jobRole, "Senior Trade Manager", StringComparison.OrdinalIgnoreCase);
    }
}

public sealed record LockedAccountSummary(string Username, string DisplayName, string GroupName);
