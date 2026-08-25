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
}
