using System;
using System.Text.Json;
using System.Threading.Tasks;
using NZFTC_EMS.Data;
using NZFTC_EMS.Data.Entities;
using NZFTC_EMS.Models;

namespace NZFTC_EMS.Services
{
    public class SessionAuditService
    {
        private static readonly JsonSerializerOptions JsonOptions = new()
        {
            WriteIndented = false
        };

        private readonly MySqlRepository _mySqlRepository;
        private readonly ILogger<SessionAuditService> _logger;

        public SessionAuditService(MySqlRepository mySqlRepository, ILogger<SessionAuditService> logger)
        {
            _mySqlRepository = mySqlRepository;
            _logger = logger;
        }

        public async Task RecordLoginAsync(string sessionId, UserSession userSession, string? remoteIpAddress)
        {
            if (string.IsNullOrWhiteSpace(sessionId) || string.IsNullOrWhiteSpace(userSession?.Username))
            {
                return;
            }

            var now = DateTime.UtcNow;
            await _mySqlRepository.UpsertSessionAsync(new SessionRecordEntity
            {
                SessionId = sessionId,
                Username = userSession.Username.Trim(),
                AccountType = userSession.AccountType?.Trim() ?? string.Empty,
                AccessProfileJson = JsonSerializer.Serialize(userSession.AccessProfile, JsonOptions),
                CreatedOnUtc = userSession.LoginTime == default ? now : userSession.LoginTime.ToUniversalTime(),
                LastSeenOnUtc = now,
                ExpiresOnUtc = now.AddMinutes(15),
                IsActive = true
            });

            await WriteAuditEventAsync("session", sessionId, "login", userSession.Username, new
            {
                userSession.AccountType,
                remoteIpAddress,
                accessProfile = userSession.AccessProfile
            });

            _logger.LogInformation(
                "MySQL session recorded for login. SessionId={SessionId}, Username={Username}, AccountType={AccountType}, RemoteIp={RemoteIpAddress}",
                sessionId,
                userSession.Username,
                userSession.AccountType,
                remoteIpAddress ?? "unknown");
        }

        public async Task TouchSessionAsync(string sessionId)
        {
            if (string.IsNullOrWhiteSpace(sessionId))
            {
                return;
            }

            var existing = await _mySqlRepository.GetSessionAsync(sessionId);
            if (existing == null)
            {
                return;
            }

            existing.LastSeenOnUtc = DateTime.UtcNow;
            existing.ExpiresOnUtc = DateTime.UtcNow.AddMinutes(15);
            existing.IsActive = true;
            await _mySqlRepository.UpsertSessionAsync(existing);

            _logger.LogDebug(
                "MySQL session heartbeat updated. SessionId={SessionId}, Username={Username}, ExpiresOnUtc={ExpiresOnUtc:o}",
                existing.SessionId,
                existing.Username,
                existing.ExpiresOnUtc);
        }

        public async Task RecordLogoutAsync(string sessionId, string username, string reason, string? remoteIpAddress)
        {
            if (string.IsNullOrWhiteSpace(username))
            {
                return;
            }

            if (!string.IsNullOrWhiteSpace(sessionId))
            {
                await _mySqlRepository.DeactivateSessionAsync(sessionId);
            }

            await WriteAuditEventAsync("session", string.IsNullOrWhiteSpace(sessionId) ? username : sessionId, reason, username, new
            {
                remoteIpAddress
            });

            _logger.LogInformation(
                "MySQL session recorded for logout. SessionId={SessionId}, Username={Username}, Reason={Reason}, RemoteIp={RemoteIpAddress}",
                string.IsNullOrWhiteSpace(sessionId) ? "n/a" : sessionId,
                username,
                reason,
                remoteIpAddress ?? "unknown");
        }

        private Task WriteAuditEventAsync(string entityType, string entityKey, string actionType, string actorUsername, object details)
        {
            return _mySqlRepository.AddAuditEventAsync(new AuditEventEntity
            {
                EntityType = entityType,
                EntityKey = entityKey,
                ActionType = actionType,
                ActorUsername = actorUsername,
                DetailsJson = JsonSerializer.Serialize(details, JsonOptions),
                OccurredOnUtc = DateTime.UtcNow
            });
        }
    }
}
