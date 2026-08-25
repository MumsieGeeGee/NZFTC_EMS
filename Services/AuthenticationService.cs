using System.Diagnostics;
using System.Linq;
using System.Text.Json;
using System.Text.Json.Serialization;
using NZFTC_EMS.Models;
using NZFTC_EMS.Utilities;

namespace NZFTC_EMS.Services
{
    public interface IAuthenticationService
    {
        Task<(bool Success, string Message, UserSession? Session)> AuthenticateUserAsync(string username, string password);
        Task<bool> LogoutUserAsync(string username);
    }

    public class AuthenticationService : IAuthenticationService
    {
        private readonly ILogger<AuthenticationService> _logger;
        private const string BridgeUnavailableMessage = "Main_System bridge is unavailable or outdated. Rebuild the Main_System executable with UI bridge support, then retry login.";
        private static readonly TimeSpan BridgeLoginTimeout = TimeSpan.FromSeconds(5);
        private static readonly TimeSpan BridgeActionTimeout = TimeSpan.FromSeconds(5);
        private static readonly JsonSerializerOptions JsonOptions = new()
        {
            PropertyNameCaseInsensitive = true
        };

        public AuthenticationService(ILogger<AuthenticationService> logger)
        {
            _logger = logger;
        }

        public async Task<(bool Success, string Message, UserSession? Session)> AuthenticateUserAsync(string username, string password)
        {
            if (string.IsNullOrWhiteSpace(username) || string.IsNullOrWhiteSpace(password))
            {
                if (string.IsNullOrWhiteSpace(username))
                {
                    return (false, MainSystemAuthMessages.UsernameInvalid, null);
                }

                return (false, MainSystemAuthMessages.PasswordMissingOrInvalid, null);
            }

            var bridgeExePath = ResolveMainSystemBridgeExecutablePath();
            if (!File.Exists(bridgeExePath))
            {
                _logger.LogError("Main_System bridge executable not found: {BridgeExePath}", bridgeExePath);
                return (false, MainSystemAuthMessages.LoginFailed, null);
            }

            try
            {
                var processStartInfo = new ProcessStartInfo
                {
                    FileName = bridgeExePath,
                    Arguments = "--ui-bridge login",
                    RedirectStandardInput = true,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    UseShellExecute = false,
                    CreateNoWindow = true
                };

                using var process = new Process { StartInfo = processStartInfo };
                if (!process.Start())
                {
                    _logger.LogError("Failed to start Main_System bridge process.");
                    return (false, MainSystemAuthMessages.LoginFailed, null);
                }

                await process.StandardInput.WriteLineAsync(username.Trim());
                await process.StandardInput.WriteLineAsync(password);
                await process.StandardInput.WriteLineAsync(string.Empty);
                process.StandardInput.Close();

                var outputTask = process.StandardOutput.ReadToEndAsync();
                var errorTask = process.StandardError.ReadToEndAsync();
                var exited = await WaitForExitOrKillAsync(process, BridgeLoginTimeout, "login", bridgeExePath);
                if (!exited)
                {
                    return (false, BridgeUnavailableMessage, null);
                }

                var output = await outputTask;
                var errorOutput = await errorTask;

                if (!string.IsNullOrWhiteSpace(errorOutput))
                {
                    _logger.LogWarning("Main_System bridge stderr: {BridgeError}", errorOutput);
                }

                if (string.IsNullOrWhiteSpace(output))
                {
                    _logger.LogError("Main_System bridge returned empty output.");
                    return (false, BridgeUnavailableMessage, null);
                }

                BridgeLoginResponse? bridgeResponse;
                try
                {
                    bridgeResponse = JsonSerializer.Deserialize<BridgeLoginResponse>(output, JsonOptions);
                }
                catch (JsonException jsonException)
                {
                    _logger.LogError(jsonException, "Failed to parse Main_System bridge JSON response: {BridgeOutput}", output);
                    return (false, BridgeUnavailableMessage, null);
                }

                if (bridgeResponse == null)
                {
                    _logger.LogError("Main_System bridge response deserialized to null.");
                    return (false, BridgeUnavailableMessage, null);
                }

                if (!bridgeResponse.Success)
                {
                    var failureMessage = string.IsNullOrWhiteSpace(bridgeResponse.Message)
                        ? MainSystemAuthMessages.LoginFailed
                        : bridgeResponse.Message;
                    _logger.LogWarning("Bridge authentication failed for {Username}. Message: {Message}", username, failureMessage);
                    return (false, failureMessage, null);
                }

                var accessProfileResponse = bridgeResponse.AccessProfile ?? new BridgeAccessProfileResponse();
                var session = new UserSession
                {
                    Username = string.IsNullOrWhiteSpace(bridgeResponse.Username) ? username.Trim() : bridgeResponse.Username,
                    AccountType = bridgeResponse.AccountType ?? string.Empty,
                    IRDNumber = bridgeResponse.IrdNumber ?? string.Empty,
                    IsAuthenticated = true,
                    LoginTime = DateTime.Now,
                    AccessProfile = new AccessProfile
                    {
                        Resolved = accessProfileResponse.Resolved,
                        BusinessRole = accessProfileResponse.BusinessRole ?? string.Empty,
                        JobRole = accessProfileResponse.JobRole ?? string.Empty,
                        DashboardMode = accessProfileResponse.DashboardMode ?? string.Empty,
                        CanManageAllAccounts = accessProfileResponse.CanManageAllAccounts,
                        CanManageAllEmployees = accessProfileResponse.CanManageAllEmployees,
                        CanManageAllHr = accessProfileResponse.CanManageAllHr,
                        CanManageRequests = accessProfileResponse.CanManageRequests,
                        CanUsePayrollFeatures = accessProfileResponse.CanUsePayrollFeatures,
                        AssistantDelegatedScopeOnly = accessProfileResponse.AssistantDelegatedScopeOnly,
                        CanViewAssignedTeam = accessProfileResponse.CanViewAssignedTeam,
                        RequiresSecondaryAuth = accessProfileResponse.RequiresSecondaryAuth
                    }
                };

                var successMessage = string.IsNullOrWhiteSpace(bridgeResponse.Message)
                    ? "Login successful."
                    : bridgeResponse.Message;

                _logger.LogInformation("Bridge authentication succeeded for '{Username}' as '{AccountType}'", session.Username, session.AccountType);
                return (true, successMessage, session);
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "Authentication bridge call failed.");
                return (false, MainSystemAuthMessages.LoginFailed, null);
            }
        }

        public async Task<bool> LogoutUserAsync(string username)
        {
            var normalizedUsername = username?.Trim() ?? string.Empty;
            if (string.IsNullOrWhiteSpace(normalizedUsername))
            {
                return false;
            }

            var bridgeExePath = ResolveMainSystemBridgeExecutablePath();
            if (!File.Exists(bridgeExePath))
            {
                _logger.LogWarning("Main_System bridge executable not found for logout: {BridgeExePath}", bridgeExePath);
                return false;
            }

            try
            {
                var processStartInfo = new ProcessStartInfo
                {
                    FileName = bridgeExePath,
                    Arguments = $"--ui-bridge action logout {normalizedUsername}",
                    RedirectStandardOutput = true,
                    RedirectStandardError = true,
                    UseShellExecute = false,
                    CreateNoWindow = true
                };

                using var process = new Process { StartInfo = processStartInfo };
                if (!process.Start())
                {
                    _logger.LogWarning("Failed to start Main_System bridge logout process.");
                    return false;
                }

                var outputTask = process.StandardOutput.ReadToEndAsync();
                var errorTask = process.StandardError.ReadToEndAsync();
                var exited = await WaitForExitOrKillAsync(process, BridgeActionTimeout, "logout", bridgeExePath);
                if (!exited)
                {
                    return false;
                }
                var output = await outputTask;
                var errorOutput = await errorTask;

                if (!string.IsNullOrWhiteSpace(errorOutput))
                {
                    _logger.LogWarning("Main_System bridge logout stderr: {BridgeError}", errorOutput);
                }

                if (string.IsNullOrWhiteSpace(output))
                {
                    return false;
                }

                BridgeActionResponse? bridgeResponse;
                try
                {
                    bridgeResponse = JsonSerializer.Deserialize<BridgeActionResponse>(output, JsonOptions);
                }
                catch (JsonException jsonException)
                {
                    _logger.LogWarning(jsonException, "Failed to parse Main_System bridge logout response: {BridgeOutput}", output);
                    return false;
                }

                return bridgeResponse?.Success ?? false;
            }
            catch (Exception ex)
            {
                _logger.LogWarning(ex, "Main_System bridge logout call failed for {Username}", normalizedUsername);
                return false;
            }
        }

        private async Task<bool> WaitForExitOrKillAsync(
            Process process,
            TimeSpan timeout,
            string operation,
            string bridgeExePath)
        {
            var waitTask = process.WaitForExitAsync();
            var completed = await Task.WhenAny(waitTask, Task.Delay(timeout));
            if (completed == waitTask)
            {
                await waitTask;
                return true;
            }

            _logger.LogError(
                "Main_System bridge {Operation} timed out after {TimeoutSeconds}s. Executable: {BridgeExePath}",
                operation,
                timeout.TotalSeconds,
                bridgeExePath);

            try
            {
                process.Kill(entireProcessTree: true);
            }
            catch (Exception killException)
            {
                _logger.LogWarning(
                    killException,
                    "Failed to terminate timed-out Main_System bridge process for {Operation}.",
                    operation);
            }

            return false;
        }

        private static string ResolveMainSystemBridgeExecutablePath()
        {
            var configuredPath = Environment.GetEnvironmentVariable("NZFTC_MAIN_SYSTEM_BRIDGE_EXE");
            if (!string.IsNullOrWhiteSpace(configuredPath))
            {
                return Path.GetFullPath(configuredPath);
            }

            var repoRoot = Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", ".."));
            var candidates = new[]
            {
                Path.Combine(repoRoot, "main.exe"),
                Path.Combine(repoRoot, "main", "Main_System", "main.exe")
            };

            var existingCandidates = candidates
                .Where(File.Exists)
                .Select(path => new FileInfo(path))
                .OrderByDescending(file => file.LastWriteTimeUtc)
                .ToList();

            if (existingCandidates.Count > 0)
            {
                return existingCandidates[0].FullName;
            }

            return candidates[0];
        }

        private sealed class BridgeLoginResponse
        {
            [JsonPropertyName("success")]
            public bool Success { get; set; }

            [JsonPropertyName("message")]
            public string? Message { get; set; }

            [JsonPropertyName("username")]
            public string? Username { get; set; }

            [JsonPropertyName("accountType")]
            public string? AccountType { get; set; }

            [JsonPropertyName("irdNumber")]
            public string? IrdNumber { get; set; }

            [JsonPropertyName("accessProfile")]
            public BridgeAccessProfileResponse? AccessProfile { get; set; }
        }

        private sealed class BridgeAccessProfileResponse
        {
            [JsonPropertyName("resolved")]
            public bool Resolved { get; set; }

            [JsonPropertyName("businessRole")]
            public string? BusinessRole { get; set; }

            [JsonPropertyName("jobRole")]
            public string? JobRole { get; set; }

            [JsonPropertyName("dashboardMode")]
            public string? DashboardMode { get; set; }

            [JsonPropertyName("canManageAllAccounts")]
            public bool CanManageAllAccounts { get; set; }

            [JsonPropertyName("canManageAllEmployees")]
            public bool CanManageAllEmployees { get; set; }

            [JsonPropertyName("canManageAllHr")]
            public bool CanManageAllHr { get; set; }

            [JsonPropertyName("canManageRequests")]
            public bool CanManageRequests { get; set; }

            [JsonPropertyName("canUsePayrollFeatures")]
            public bool CanUsePayrollFeatures { get; set; }

            [JsonPropertyName("assistantDelegatedScopeOnly")]
            public bool AssistantDelegatedScopeOnly { get; set; }

            [JsonPropertyName("canViewAssignedTeam")]
            public bool CanViewAssignedTeam { get; set; }

            [JsonPropertyName("requiresSecondaryAuth")]
            public bool RequiresSecondaryAuth { get; set; }
        }

        private sealed class BridgeActionResponse
        {
            [JsonPropertyName("success")]
            public bool Success { get; set; }
        }
    }
}
