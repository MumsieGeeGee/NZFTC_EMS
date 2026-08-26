using Microsoft.EntityFrameworkCore;
using NZFTC_EMS.Data.Entities;

namespace NZFTC_EMS.Data
{
    public sealed class MySqlRepository
    {
        private readonly ApplicationDbContext _db;

        public MySqlRepository(ApplicationDbContext db)
        {
            _db = db;
        }

        public Task<List<EmployeeEntity>> ListEmployeesAsync(CancellationToken cancellationToken = default)
        {
            return _db.Employees.AsNoTracking().OrderBy(x => x.Username).ToListAsync(cancellationToken);
        }

        public Task<EmployeeEntity?> GetEmployeeAsync(string username, CancellationToken cancellationToken = default)
        {
            return _db.Employees.AsNoTracking().FirstOrDefaultAsync(x => x.Username == username, cancellationToken);
        }

        public async Task UpsertEmployeeAsync(EmployeeEntity employee, CancellationToken cancellationToken = default)
        {
            var existing = await _db.Employees.FirstOrDefaultAsync(x => x.Username == employee.Username, cancellationToken);
            if (existing == null)
            {
                _db.Employees.Add(employee);
            }
            else
            {
                _db.Entry(existing).CurrentValues.SetValues(employee);
            }

            await _db.SaveChangesAsync(cancellationToken);
        }

        public async Task<bool> DeleteEmployeeAsync(string username, CancellationToken cancellationToken = default)
        {
            var existing = await _db.Employees.FirstOrDefaultAsync(x => x.Username == username, cancellationToken);
            if (existing == null)
            {
                return false;
            }

            _db.Employees.Remove(existing);
            await _db.SaveChangesAsync(cancellationToken);
            return true;
        }

        public Task<List<EmployeeTaxInformationEntity>> ListTaxInformationAsync(CancellationToken cancellationToken = default)
        {
            return _db.EmployeeTaxInformation.AsNoTracking().OrderBy(x => x.Username).ToListAsync(cancellationToken);
        }

        public Task<EmployeeTaxInformationEntity?> GetTaxInformationAsync(string username, CancellationToken cancellationToken = default)
        {
            return _db.EmployeeTaxInformation.AsNoTracking().FirstOrDefaultAsync(x => x.Username == username, cancellationToken);
        }

        public async Task UpsertTaxInformationAsync(EmployeeTaxInformationEntity taxInformation, CancellationToken cancellationToken = default)
        {
            var existing = await _db.EmployeeTaxInformation.FirstOrDefaultAsync(x => x.Username == taxInformation.Username, cancellationToken);
            if (existing == null)
            {
                _db.EmployeeTaxInformation.Add(taxInformation);
            }
            else
            {
                _db.Entry(existing).CurrentValues.SetValues(taxInformation);
            }

            await _db.SaveChangesAsync(cancellationToken);
        }

        public Task<EmployeeAccountDetailsEntity?> GetEmployeeAccountDetailsAsync(string username, CancellationToken cancellationToken = default)
        {
            return _db.EmployeeAccountDetails.AsNoTracking().FirstOrDefaultAsync(x => x.Username == username, cancellationToken);
        }

        public Task<List<EmployeeAccountDetailsEntity>> ListEmployeeAccountDetailsAsync(CancellationToken cancellationToken = default)
        {
            return _db.EmployeeAccountDetails.AsNoTracking().OrderBy(x => x.Username).ToListAsync(cancellationToken);
        }

        public async Task UpsertEmployeeAccountDetailsAsync(EmployeeAccountDetailsEntity accountDetails, CancellationToken cancellationToken = default)
        {
            var existing = await _db.EmployeeAccountDetails.FirstOrDefaultAsync(x => x.Username == accountDetails.Username, cancellationToken);
            if (existing == null)
            {
                _db.EmployeeAccountDetails.Add(accountDetails);
            }
            else
            {
                _db.Entry(existing).CurrentValues.SetValues(accountDetails);
            }

            await _db.SaveChangesAsync(cancellationToken);
        }

        public Task<LeaveEntitlementEntity?> GetLeaveEntitlementAsync(string username, CancellationToken cancellationToken = default)
        {
            return _db.LeaveEntitlements.AsNoTracking().FirstOrDefaultAsync(x => x.Username == username, cancellationToken);
        }

        public async Task UpsertLeaveEntitlementAsync(LeaveEntitlementEntity entitlement, CancellationToken cancellationToken = default)
        {
            var existing = await _db.LeaveEntitlements.FirstOrDefaultAsync(x => x.Username == entitlement.Username, cancellationToken);
            if (existing == null)
            {
                _db.LeaveEntitlements.Add(entitlement);
            }
            else
            {
                _db.Entry(existing).CurrentValues.SetValues(entitlement);
            }

            await _db.SaveChangesAsync(cancellationToken);
        }

        public Task<List<LeaveRequestEntity>> ListLeaveRequestsAsync(string? username = null, CancellationToken cancellationToken = default)
        {
            var query = _db.LeaveRequests.AsNoTracking().AsQueryable();
            if (!string.IsNullOrWhiteSpace(username))
            {
                query = query.Where(x => x.SubmittedForUsername == username || x.SubmittedByUsername == username);
            }

            return query.OrderByDescending(x => x.RequestedOnUtc).ToListAsync(cancellationToken);
        }

        public Task<LeaveRequestEntity?> GetLeaveRequestAsync(long id, CancellationToken cancellationToken = default)
        {
            return _db.LeaveRequests.AsNoTracking().FirstOrDefaultAsync(x => x.Id == id, cancellationToken);
        }

        public async Task<long> CreateLeaveRequestAsync(LeaveRequestEntity leaveRequest, CancellationToken cancellationToken = default)
        {
            _db.LeaveRequests.Add(leaveRequest);
            await _db.SaveChangesAsync(cancellationToken);
            return leaveRequest.Id;
        }

        public async Task<bool> UpdateLeaveRequestAsync(LeaveRequestEntity leaveRequest, CancellationToken cancellationToken = default)
        {
            var existing = await _db.LeaveRequests.FirstOrDefaultAsync(x => x.Id == leaveRequest.Id, cancellationToken);
            if (existing == null)
            {
                return false;
            }

            _db.Entry(existing).CurrentValues.SetValues(leaveRequest);
            await _db.SaveChangesAsync(cancellationToken);
            return true;
        }

        public async Task UpsertLeaveRequestAsync(LeaveRequestEntity leaveRequest, CancellationToken cancellationToken = default)
        {
            var existing = await _db.LeaveRequests.FirstOrDefaultAsync(x => x.Id == leaveRequest.Id, cancellationToken);
            if (existing == null)
            {
                _db.LeaveRequests.Add(leaveRequest);
            }
            else
            {
                _db.Entry(existing).CurrentValues.SetValues(leaveRequest);
            }

            await _db.SaveChangesAsync(cancellationToken);
        }

        public async Task<bool> DeleteLeaveRequestAsync(long id, CancellationToken cancellationToken = default)
        {
            var existing = await _db.LeaveRequests.FirstOrDefaultAsync(x => x.Id == id, cancellationToken);
            if (existing == null)
            {
                return false;
            }

            _db.LeaveRequests.Remove(existing);
            await _db.SaveChangesAsync(cancellationToken);
            return true;
        }

        public Task<List<GrievanceRequestEntity>> ListGrievanceRequestsAsync(string? username = null, CancellationToken cancellationToken = default)
        {
            var query = _db.GrievanceRequests.AsNoTracking().AsQueryable();
            if (!string.IsNullOrWhiteSpace(username))
            {
                query = query.Where(x => x.SubmittedForUsername == username || x.SubmittedByUsername == username);
            }

            return query.OrderByDescending(x => x.SubmittedOnUtc).ToListAsync(cancellationToken);
        }

        public Task<GrievanceRequestEntity?> GetGrievanceRequestAsync(long id, CancellationToken cancellationToken = default)
        {
            return _db.GrievanceRequests.AsNoTracking().FirstOrDefaultAsync(x => x.Id == id, cancellationToken);
        }

        public async Task<long> CreateGrievanceRequestAsync(GrievanceRequestEntity grievanceRequest, CancellationToken cancellationToken = default)
        {
            _db.GrievanceRequests.Add(grievanceRequest);
            await _db.SaveChangesAsync(cancellationToken);
            return grievanceRequest.Id;
        }

        public async Task UpsertGrievanceRequestAsync(GrievanceRequestEntity grievanceRequest, CancellationToken cancellationToken = default)
        {
            var existing = await _db.GrievanceRequests.FirstOrDefaultAsync(x => x.Id == grievanceRequest.Id, cancellationToken);
            if (existing == null)
            {
                _db.GrievanceRequests.Add(grievanceRequest);
            }
            else
            {
                _db.Entry(existing).CurrentValues.SetValues(grievanceRequest);
            }

            await _db.SaveChangesAsync(cancellationToken);
        }

        public Task<List<PayslipEntity>> ListPayslipsAsync(string? username = null, CancellationToken cancellationToken = default)
        {
            var query = _db.Payslips.AsNoTracking().AsQueryable();
            if (!string.IsNullOrWhiteSpace(username))
            {
                query = query.Where(x => x.Username == username);
            }

            return query.OrderByDescending(x => x.CreatedOnUtc).ToListAsync(cancellationToken);
        }

        public Task<PayslipEntity?> GetPayslipAsync(long id, CancellationToken cancellationToken = default)
        {
            return _db.Payslips.AsNoTracking().FirstOrDefaultAsync(x => x.Id == id, cancellationToken);
        }

        public async Task<long> CreatePayslipAsync(PayslipEntity payslip, CancellationToken cancellationToken = default)
        {
            _db.Payslips.Add(payslip);
            await _db.SaveChangesAsync(cancellationToken);
            return payslip.Id;
        }

        public async Task UpsertPayslipAsync(PayslipEntity payslip, CancellationToken cancellationToken = default)
        {
            var existing = await _db.Payslips.FirstOrDefaultAsync(x => x.Id == payslip.Id, cancellationToken);
            if (existing == null)
            {
                _db.Payslips.Add(payslip);
            }
            else
            {
                _db.Entry(existing).CurrentValues.SetValues(payslip);
            }

            await _db.SaveChangesAsync(cancellationToken);
        }

        public Task<List<SessionRecordEntity>> ListActiveSessionsAsync(CancellationToken cancellationToken = default)
        {
            return _db.SessionRecords.AsNoTracking()
                .Where(x => x.IsActive)
                .OrderByDescending(x => x.LastSeenOnUtc)
                .ToListAsync(cancellationToken);
        }

        public async Task UpsertSessionAsync(SessionRecordEntity session, CancellationToken cancellationToken = default)
        {
            var existing = await _db.SessionRecords.FirstOrDefaultAsync(x => x.SessionId == session.SessionId, cancellationToken);
            if (existing == null)
            {
                _db.SessionRecords.Add(session);
            }
            else
            {
                _db.Entry(existing).CurrentValues.SetValues(session);
            }

            await _db.SaveChangesAsync(cancellationToken);
        }

        public Task<SessionRecordEntity?> GetSessionAsync(string sessionId, CancellationToken cancellationToken = default)
        {
            return _db.SessionRecords.AsNoTracking().FirstOrDefaultAsync(x => x.SessionId == sessionId, cancellationToken);
        }

        public async Task<bool> DeactivateSessionAsync(string sessionId, CancellationToken cancellationToken = default)
        {
            var existing = await _db.SessionRecords.FirstOrDefaultAsync(x => x.SessionId == sessionId, cancellationToken);
            if (existing == null)
            {
                return false;
            }

            existing.IsActive = false;
            existing.LastSeenOnUtc = DateTime.UtcNow;
            await _db.SaveChangesAsync(cancellationToken);
            return true;
        }

        public async Task<long> AddAuditEventAsync(AuditEventEntity auditEvent, CancellationToken cancellationToken = default)
        {
            _db.AuditEvents.Add(auditEvent);
            await _db.SaveChangesAsync(cancellationToken);
            return auditEvent.Id;
        }

        public async Task<MySqlStatusSnapshot> GetStatusSnapshotAsync(int recentAuditEventLimit = 5, CancellationToken cancellationToken = default)
        {
            var recentAuditEvents = await _db.AuditEvents.AsNoTracking()
                .OrderByDescending(x => x.OccurredOnUtc)
                .Take(recentAuditEventLimit)
                .ToListAsync(cancellationToken);

            return new MySqlStatusSnapshot(
                await _db.Database.CanConnectAsync(cancellationToken),
                await _db.Employees.CountAsync(cancellationToken),
                await _db.EmployeeAccountDetails.CountAsync(cancellationToken),
                await _db.EmployeeTaxInformation.CountAsync(cancellationToken),
                await _db.Employees.CountAsync(
                    x => x.AccountStatus != null &&
                         x.AccountStatus.Trim().ToLower() == "locked",
                    cancellationToken),
                await _db.LeaveEntitlements.CountAsync(cancellationToken),
                await _db.LeaveRequests.CountAsync(cancellationToken),
                await _db.GrievanceRequests.CountAsync(cancellationToken),
                await _db.Payslips.CountAsync(cancellationToken),
                await _db.SessionRecords.CountAsync(x => x.IsActive, cancellationToken),
                await _db.AuditEvents.CountAsync(cancellationToken),
                recentAuditEvents);
        }

        public Task<IReadOnlyList<IntegrityIssue>> ValidateIntegrityAsync(CancellationToken cancellationToken = default)
        {
            return Task.FromResult<IReadOnlyList<IntegrityIssue>>(BuildIntegrityIssues());
        }

        private IReadOnlyList<IntegrityIssue> BuildIntegrityIssues()
        {
            var issues = new List<IntegrityIssue>();

            if (_db.Employees.Any(x => string.IsNullOrWhiteSpace(x.Username)))
            {
                issues.Add(new IntegrityIssue("employees", "blank-username", "Employee rows must have a username."));
            }

            if (_db.EmployeeTaxInformation.Any(x => string.IsNullOrWhiteSpace(x.IrdNumber)))
            {
                issues.Add(new IntegrityIssue("employee_tax_information", "blank-ird", "Tax information rows must include an IRD number."));
            }

            if (_db.LeaveRequests.Any(x => x.EndDate.Date < x.StartDate.Date))
            {
                issues.Add(new IntegrityIssue("leave_requests", "invalid-date-range", "Leave requests must not end before they start."));
            }

            if (_db.Payslips.Any(x => x.CreatedOnUtc == default))
            {
                issues.Add(new IntegrityIssue("payslips", "missing-generated-on", "Payslips must record a generation timestamp."));
            }

            if (_db.GrievanceRequests.Any(x => string.IsNullOrWhiteSpace(x.Subject)))
            {
                issues.Add(new IntegrityIssue("grievance_requests", "blank-subject", "Grievance requests must include a subject."));
            }

            return issues;
        }
    }

    public sealed record IntegrityIssue(string TableName, string IssueCode, string Message);
    public sealed record MySqlStatusSnapshot(
        bool CanConnect,
        int EmployeeCount,
        int AccountDetailsCount,
        int TaxInformationCount,
        int LockedAccountCount,
        int LeaveEntitlementCount,
        int LeaveRequestCount,
        int GrievanceRequestCount,
        int PayslipCount,
        int ActiveSessionCount,
        int AuditEventCount,
        IReadOnlyList<AuditEventEntity> RecentAuditEvents);
}
