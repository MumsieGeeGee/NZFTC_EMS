using System;
using System.Collections.Generic;

namespace NZFTC_EMS.Models;

public class SelectedEmployeeRecordDetailsViewModel
{
    public string ContentEndpoint { get; set; } = string.Empty;
    public string SelectedGroup { get; set; } = "admin";
    public string SelectedUsername { get; set; } = string.Empty;
    public IReadOnlyList<string> Usernames { get; set; } = Array.Empty<string>();
    public IReadOnlyDictionary<string, string> UsernameDisplayNames { get; set; } = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
    public EmployeeRecordDetailsViewModel? Details { get; set; }
}
