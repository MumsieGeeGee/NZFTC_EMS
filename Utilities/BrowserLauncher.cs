namespace NZFTC_EMS.Utilities;
using System.Diagnostics;
using Microsoft.Win32;
/// <summary>
/// Utility class to detect available browsers and launch the application in the user's preferred browser.
/// Supports: Chrome, Edge, Firefox, Opera
/// </summary>
public static class BrowserLauncher
{
    private static readonly Dictionary<string, BrowserInfo> AvailableBrowsers = new()
    {
        // Chrome
        {
            "Chrome", new BrowserInfo
            {
                Name = "Chrome",
                Paths = new[]
                {
                    @"C:\Program Files\Google\Chrome\Application\chrome.exe",
                    @"C:\Program Files (x86)\Google\Chrome\Application\chrome.exe",
                    Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),
                        @"Google\Chrome\Application\chrome.exe")
                },
                RegistryKey = @"HKEY_CURRENT_USER\Software\Google\Chrome\BLBeacon",
                RegistryValue = "version"
            }
        },
        // Edge
        {
            "Edge", new BrowserInfo
            {
                Name = "Edge",
                Paths = new[]
                {
                    @"C:\Program Files\Microsoft\Edge\Application\msedge.exe",
                    @"C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe",
                    Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
                        @"Microsoft\Edge\Application\msedge.exe")
                },
                RegistryKey = @"HKEY_CURRENT_USER\Software\Microsoft\Edge\BLBeacon",
                RegistryValue = "version"
            }
        },
        // Firefox
        {
            "Firefox", new BrowserInfo
            {
                Name = "Firefox",
                Paths = new[]
                {
                    @"C:\Program Files\Mozilla Firefox\firefox.exe",
                    @"C:\Program Files (x86)\Mozilla Firefox\firefox.exe",
                    Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
                        @"Mozilla Firefox\firefox.exe")
                },
                RegistryKey = @"HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\firefox.exe",
                RegistryValue = "Path"
            }
        },
        // Opera
        {
            "Opera", new BrowserInfo
            {
                Name = "Opera",
                Paths = new[]
                {
                    @"C:\Program Files\Opera\opera.exe",
                    @"C:\Program Files (x86)\Opera\opera.exe",
                    Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles),
                        @"Opera\opera.exe")
                },
                RegistryKey = @"HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\opera.exe",
                RegistryValue = "Path"
            }
        }
    };

    /// <summary>
    /// Get list of installed browsers on the system
    /// </summary>
    public static List<BrowserInfo> GetInstalledBrowsers()
    {
        var installedBrowsers = new List<BrowserInfo>();

        foreach (var browser in AvailableBrowsers.Values)
        {
            var executablePath = GetBrowserExecutablePath(browser);
            if (executablePath != null && File.Exists(executablePath))
            {
                browser.ExecutablePath = executablePath;
                installedBrowsers.Add(browser);
            }
        }

        return installedBrowsers;
    }

    /// <summary>
    /// Get the default browser set in Windows
    /// </summary>
    public static BrowserInfo? GetDefaultBrowser()
    {
        if (!OperatingSystem.IsWindows())
            return null;

        try
        {
            const string registryPath = @"Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\.html\UserChoice";
            using var key = Registry.CurrentUser.OpenSubKey(registryPath);
            if (key == null)
                return null;

            var progId = key.GetValue("Progid")?.ToString();
            if (string.IsNullOrEmpty(progId))
                return null;

            // Map ProgID to browser name
            var browserName = progId switch
            {
                var p when p.Contains("Chrome") => "Chrome",
                var p when p.Contains("Edge") => "Edge",
                var p when p.Contains("Firefox") => "Firefox",
                var p when p.Contains("Opera") => "Opera",
                _ => null
            };

            if (browserName != null && AvailableBrowsers.TryGetValue(browserName, out var browser))
            {
                var executablePath = GetBrowserExecutablePath(browser);
                if (executablePath != null && File.Exists(executablePath))
                {
                    browser.ExecutablePath = executablePath;
                    return browser;
                }
            }
        }
        catch
        {
            // Silently fail if unable to read registry
        }

        return null;
    }

    /// <summary>
    /// Launch application in the user's default browser, or a specific browser if provided
    /// </summary>
    public static void LaunchBrowser(string url, BrowserInfo? preferredBrowser = null)
    {
        try
        {
            BrowserInfo? browser = null;

            // Use preferred browser if provided and available
            if (preferredBrowser != null)
            {
                var executablePath = GetBrowserExecutablePath(preferredBrowser);
                if (executablePath != null && File.Exists(executablePath))
                {
                    browser = preferredBrowser;
                    browser.ExecutablePath = executablePath;
                }
            }

            // Fall back to default browser
            browser ??= GetDefaultBrowser();

            // Fall back to first available browser
            if (browser == null)
            {
                var installed = GetInstalledBrowsers();
                if (installed.Count == 0)
                    throw new Exception("No supported browsers found on this system.");
                browser = installed[0];
            }

            // Launch the browser
            if (browser.ExecutablePath != null)
            {
                var processInfo = new ProcessStartInfo
                {
                    FileName = browser.ExecutablePath,
                    Arguments = url,
                    UseShellExecute = true
                };

                Process.Start(processInfo);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Warning: Could not launch browser: {ex.Message}");
            // Fall back to default URL handler
            try
            {
                Process.Start(new ProcessStartInfo
                {
                    FileName = url,
                    UseShellExecute = true
                });
            }
            catch
            {
                Console.WriteLine($"Error: Could not launch application in any browser. Please manually navigate to {url}");
            }
        }
    }

    /// <summary>
    /// Get the executable path for a browser from either local file system or registry
    /// </summary>
    private static string? GetBrowserExecutablePath(BrowserInfo browser)
    {
        // Check local file paths first
        foreach (var path in browser.Paths)
        {
            if (File.Exists(path))
                return path;
        }

        // Try registry (Windows only)
        if (OperatingSystem.IsWindows())
        {
            try
            {
                using var key = Registry.LocalMachine.OpenSubKey(browser.RegistryKey);
                if (key != null)
                {
                    var value = key.GetValue(browser.RegistryValue)?.ToString();
                    if (!string.IsNullOrEmpty(value))
                    {
                        var execPath = value.Contains(@"\") ? value : Path.Combine(value, browser.Name.ToLower() + ".exe");
                        if (File.Exists(execPath))
                            return execPath;
                    }
                }
            }
            catch
            {
                // Silently fail
            }
        }

        return null;
    }
}

/// <summary>
/// Information about an installed browser
/// </summary>
public class BrowserInfo
{
    public string Name { get; set; } = string.Empty;
    public string[] Paths { get; set; } = Array.Empty<string>();
    public string RegistryKey { get; set; } = string.Empty;
    public string RegistryValue { get; set; } = string.Empty;
    public string? ExecutablePath { get; set; }

    public override string ToString() => Name;
}
