using System;
using System.Text;
using NZFTC_EMS.Models;
using NZFTC_EMS.Utilities;

namespace NZFTC_EMS.Services
{
    public interface IAuthenticationService
    {
        Task<(bool Success, string Message, UserSession? Session)> AuthenticateUserAsync(string username, string password);
    }

    public class AuthenticationService : IAuthenticationService
    {
        private readonly ILogger<AuthenticationService> _logger;

        public AuthenticationService(ILogger<AuthenticationService> logger)
        {
            _logger = logger;
        }

        /// <summary>
        /// Authenticates user by reading Main_System's individual employee record files
        /// and using the same FNV-1a hashing algorithm Main_System uses.
        /// Replicates Main_System's Find_Record_For_Username() + Check_Account_Exists() logic.
        /// </summary>
        public async Task<(bool Success, string Message, UserSession? Session)> AuthenticateUserAsync(string username, string password)
        {
            try
            {
                if (string.IsNullOrWhiteSpace(username) || string.IsNullOrWhiteSpace(password))
                {
                    if (string.IsNullOrWhiteSpace(username))
                    {
                        return (false, MainSystemAuthMessages.UsernameInvalid, null);
                    }

                    return (false, MainSystemAuthMessages.PasswordMissingOrInvalid, null);
                }

                // Verify against Main_System's Employee_Records using its hashing algorithm
                var (isValid, accountType, irdNumber, failureMessage) = VerifyCredentialsAgainstMainSystem(username, password);

                if (!isValid)
                {
                    _logger.LogWarning($"Authentication failed for username: {username}");
                    return (false, failureMessage, null);
                }

                // Create user session
                var userSession = new UserSession
                {
                    Username = username,
                    AccountType = accountType,
                    IRDNumber = irdNumber,
                    IsAuthenticated = true,
                    LoginTime = DateTime.Now
                };

                _logger.LogInformation($"User '{username}' authenticated successfully as {accountType}");
                return (true, "Login successful.", userSession);
            }
            catch (Exception ex)
            {
                _logger.LogError($"Authentication error: {ex.Message}");
                return (false, MainSystemAuthMessages.LoginFailed, null);
            }
        }

        /// <summary>
        /// Replicates Main_System's Find_Record_For_Username() logic exactly:
        /// 1. Recursively search ALL .txt files in Employee_Records
        /// 2. Read each file and look for "Username:" field inside
        /// 3. When username matches, extract Account Type and Password from same file
        /// 4. Hash input password with FNV-1a and compare
        /// </summary>
        private (bool isValid, string accountType, string irdNumber, string failureMessage) VerifyCredentialsAgainstMainSystem(string username, string password)
        {
            try
            {
                // Path to Main_System's Employee_Records base
                var employeeRecordsPath = Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "main", "Main_System", "Employee Management", "Employee_Records");
                employeeRecordsPath = Path.GetFullPath(employeeRecordsPath);

                _logger.LogInformation($"Searching Employee_Records at: {employeeRecordsPath}");

                if (!Directory.Exists(employeeRecordsPath))
                {
                    _logger.LogError($"Employee_Records directory not found at: {employeeRecordsPath}");
                    return (false, string.Empty, string.Empty, MainSystemAuthMessages.LoginFailed);
                }

                // Recursively search ALL .txt files for matching username (like Main_System does)
                var recordsDir = new DirectoryInfo(employeeRecordsPath);
                var allRecordFiles = recordsDir.EnumerateFiles("*.txt", SearchOption.AllDirectories).ToList();

                _logger.LogInformation($"Found {allRecordFiles.Count()} employee record files to search");

                string? matchedRecordPath = null;
                string matchedAccountType = string.Empty;
                string matchedPasswordHash = string.Empty;
                string normalizedUsername = username.Trim();
                string computedHash = ComputePasswordHashFnv1a(password);
                bool passwordHashExists = false;

                foreach (var recordFile in allRecordFiles)
                {
                    var (storedUsername, storedAccountType, storedPasswordHash) = ExtractFieldsFromRecord(recordFile.FullName);

                    if (!string.IsNullOrEmpty(storedPasswordHash) &&
                        computedHash.Equals(storedPasswordHash.Trim(), StringComparison.OrdinalIgnoreCase))
                    {
                        passwordHashExists = true;
                    }

                    if (!string.IsNullOrEmpty(storedUsername) &&
                        storedUsername.Equals(normalizedUsername, StringComparison.OrdinalIgnoreCase))
                    {
                        matchedRecordPath = recordFile.FullName;
                        matchedAccountType = storedAccountType;
                        matchedPasswordHash = storedPasswordHash.Trim();
                    }
                }

                if (matchedRecordPath == null)
                {
                    _logger.LogWarning($"No employee record found with username: {username}");
                    return (false, string.Empty, string.Empty, MainSystemAuthMessages.UsernameInvalid);
                }

                _logger.LogInformation($"Found matching record: {Path.GetFileName(matchedRecordPath)}");

                if (string.IsNullOrEmpty(matchedPasswordHash))
                {
                    _logger.LogWarning($"Password hash not found in record file for {username}");
                    return (false, string.Empty, string.Empty, MainSystemAuthMessages.PasswordMissingOrInvalid);
                }

                _logger.LogInformation($"Username: {normalizedUsername}, AccountType: {matchedAccountType}");
                _logger.LogInformation($"Stored hash: {matchedPasswordHash}");
                _logger.LogInformation($"Computed hash: {computedHash}");

                if (!passwordHashExists)
                {
                    _logger.LogWarning($"Password hash not found in any record for {username}");
                    return (false, string.Empty, string.Empty, MainSystemAuthMessages.PasswordMissingOrInvalid);
                }

                if (!computedHash.Equals(matchedPasswordHash, StringComparison.OrdinalIgnoreCase))
                {
                    _logger.LogWarning($"Password hash mismatch for {username}");
                    return (false, string.Empty, string.Empty, MainSystemAuthMessages.InvalidCredentials);
                }

                string irdNumber = GetIRDNumberForUsername(employeeRecordsPath, username);

                _logger.LogInformation($"User '{username}' authenticated successfully as {matchedAccountType}");
                return (true, matchedAccountType, irdNumber, string.Empty);
            }
            catch (Exception ex)
            {
                _logger.LogError($"Credential verification error: {ex.Message}\n{ex.StackTrace}");
                return (false, string.Empty, string.Empty, MainSystemAuthMessages.LoginFailed);
            }
        }

        /// <summary>
        /// Reads an employee record file and extracts Username, Account Type, and Password.
        /// File format contains lines like:
        /// Username: admin_slm_jane_sarah_doe
        /// Account Type: Admin
        /// Password: a1b2c3d4e5f6a7b8
        /// </summary>
        private (string username, string accountType, string passwordHash) ExtractFieldsFromRecord(string recordFilePath)
        {
            try
            {
                var lines = File.ReadAllLines(recordFilePath);
                string username = string.Empty;
                string accountType = string.Empty;
                string passwordHash = string.Empty;

                foreach (var line in lines)
                {
                    var trimmedLine = line.Trim();

                    if (trimmedLine.StartsWith("Username:", StringComparison.OrdinalIgnoreCase))
                    {
                        username = trimmedLine.Substring("Username:".Length).Trim();
                    }
                    else if (trimmedLine.StartsWith("Account Type:", StringComparison.OrdinalIgnoreCase))
                    {
                        accountType = trimmedLine.Substring("Account Type:".Length).Trim();
                    }
                    else if (trimmedLine.StartsWith("Password:", StringComparison.OrdinalIgnoreCase))
                    {
                        passwordHash = trimmedLine.Substring("Password:".Length).Trim();
                    }
                }

                return (username, accountType, passwordHash);
            }
            catch (Exception ex)
            {
                _logger.LogError($"Error reading employee record {recordFilePath}: {ex.Message}");
                return (string.Empty, string.Empty, string.Empty);
            }
        }

        /// <summary>
        /// Reads IRD_Username.txt to get IRD number for username.
        /// Format: "IRD_NUMBER : username"
        /// </summary>
        private string GetIRDNumberForUsername(string employeeRecordsPath, string username)
        {
            try
            {
                var irdIndexPath = Path.Combine(employeeRecordsPath, "IRD", "IRD_Username.txt");
                if (!File.Exists(irdIndexPath))
                {
                    _logger.LogWarning($"IRD_Username.txt not found at: {irdIndexPath}");
                    return "00000000";
                }

                var lines = File.ReadAllLines(irdIndexPath);
                foreach (var line in lines)
                {
                    var parts = line.Split(':');
                    if (parts.Length == 2)
                    {
                        var irdNumber = parts[0].Trim();
                        var recordUsername = parts[1].Trim();
                        if (recordUsername.Equals(username, StringComparison.OrdinalIgnoreCase))
                        {
                            return irdNumber;
                        }
                    }
                }

                return "00000000";
            }
            catch (Exception ex)
            {
                _logger.LogError($"Error reading IRD number: {ex.Message}");
                return "00000000";
            }
        }

        /// <summary>
        /// Implements the exact FNV-1a 64-bit hashing algorithm from Main_System Security_Protocols.cpp
        /// Salt: "NZFTC_EMS_PASSWORD_SALT_v1"
        /// FnvOffsetBasis: 1469598103934665603
        /// FnvPrime: 1099511628211
        /// </summary>
        private string ComputePasswordHashFnv1a(string password)
        {
            const ulong FnvOffsetBasis = 1469598103934665603UL;
            const ulong FnvPrime = 1099511628211UL;
            const string Salt = "NZFTC_EMS_PASSWORD_SALT_v1";

            ulong hash = FnvOffsetBasis;

            // Hash the salt first (Main_System does this)
            foreach (byte b in Encoding.UTF8.GetBytes(Salt))
            {
                hash ^= b;
                hash *= FnvPrime;
            }

            // Then hash the password
            foreach (byte b in Encoding.UTF8.GetBytes(password))
            {
                hash ^= b;
                hash *= FnvPrime;
            }

            // Convert to 16-character lowercase hex string (Main_System format)
            var result = new StringBuilder(16);
            for (int i = 15; i >= 0; i--)
            {
                byte nibble = (byte)((hash >> (i * 4)) & 0x0F);
                result.Append("0123456789abcdef"[nibble]);
            }

            return result.ToString();
        }
    }
}
