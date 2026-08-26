using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using NZFTC_EMS.Data.Entities;
using NZFTC_EMS.Models;
using NZFTC_EMS.Services;

namespace NZFTC_EMS.Data
{
    public static class MySqlDatabaseInitializer
    {
        private static readonly JsonSerializerOptions SnapshotJsonOptions = new()
        {
            PropertyNameCaseInsensitive = true
        };

        public static async Task InitializeAsync(IServiceProvider services, CancellationToken cancellationToken = default)
        {
            await using var scope = services.CreateAsyncScope();
            var logger = scope.ServiceProvider.GetRequiredService<ILoggerFactory>().CreateLogger("MySqlDatabaseInitializer");
            var db = scope.ServiceProvider.GetRequiredService<ApplicationDbContext>();

            await db.Database.EnsureCreatedAsync(cancellationToken);
            await EnsureLegacySchemaCompatibilityAsync(db, logger, cancellationToken);

            var repository = scope.ServiceProvider.GetRequiredService<MySqlRepository>();
            var employeeAccountRecordService = scope.ServiceProvider.GetRequiredService<EmployeeAccountRecordService>();
            var leaveRequestService = scope.ServiceProvider.GetRequiredService<LeaveRequestService>();
            var grievanceRequestService = scope.ServiceProvider.GetRequiredService<GrievanceRequestService>();
            var payrollService = scope.ServiceProvider.GetRequiredService<PayrollService>();

            var usernames = employeeAccountRecordService.GetAllRecordUsernames();
            foreach (var username in usernames)
            {
                if (!employeeAccountRecordService.TryGetAccountDetails(username, out var details))
                {
                    continue;
                }

                var employee = BuildEmployeeEntity(username, details);
                await repository.UpsertEmployeeAsync(employee, cancellationToken);
                await repository.UpsertEmployeeAccountDetailsAsync(BuildAccountDetailsEntity(username, details), cancellationToken);
                await repository.UpsertTaxInformationAsync(BuildTaxEntity(payrollService.GetEditableTaxInformationForUser(username), username), cancellationToken);
                await repository.UpsertLeaveEntitlementAsync(BuildLeaveEntitlementEntity(leaveRequestService.GetEntitlementForUser(username)), cancellationToken);
            }

            var knownEmployeeUsernames = new HashSet<string>(
                await db.Employees
                    .AsNoTracking()
                    .Select(x => x.Username)
                    .Where(username => !string.IsNullOrWhiteSpace(username))
                    .ToListAsync(cancellationToken),
                StringComparer.OrdinalIgnoreCase);

            foreach (var request in leaveRequestService.GetAllRequests())
            {
                if (!HasKnownEmployeeReferences(request.SubmittedByUsername, request.SubmittedForUsername, knownEmployeeUsernames))
                {
                    logger.LogWarning(
                        "MySQL compatibility update: skipping leave request {RequestId} because the referenced employee record is missing. SubmittedBy={SubmittedByUsername}, SubmittedFor={SubmittedForUsername}.",
                        request.Id,
                        request.SubmittedByUsername,
                        request.SubmittedForUsername);
                    continue;
                }

                await repository.UpsertLeaveRequestAsync(BuildLeaveRequestEntity(request), cancellationToken);
            }

            foreach (var grievance in grievanceRequestService.GetAllReports())
            {
                if (!HasKnownEmployeeReferences(grievance.SubmittedByUsername, grievance.SubmittedForUsername, knownEmployeeUsernames))
                {
                    logger.LogWarning(
                        "MySQL compatibility update: skipping grievance request {RequestId} because the referenced employee record is missing. SubmittedBy={SubmittedByUsername}, SubmittedFor={SubmittedForUsername}.",
                        grievance.Id,
                        grievance.SubmittedByUsername,
                        grievance.SubmittedForUsername);
                    continue;
                }

                await repository.UpsertGrievanceRequestAsync(BuildGrievanceRequestEntity(grievance), cancellationToken);
            }

            foreach (var payslip in payrollService.GetAllPayslips())
            {
                if (!knownEmployeeUsernames.Contains(payslip.Username))
                {
                    logger.LogWarning(
                        "MySQL compatibility update: skipping payslip {PayslipId} because employee {Username} is missing from employees.",
                        payslip.Id,
                        payslip.Username);
                    continue;
                }

                await repository.UpsertPayslipAsync(BuildPayslipEntity(payslip), cancellationToken);
            }

            var canConnect = await db.Database.CanConnectAsync(cancellationToken);
            var employeeCount = await db.Employees.CountAsync(cancellationToken);
            var accountDetailsCount = await db.EmployeeAccountDetails.CountAsync(cancellationToken);
            var taxInformationCount = await db.EmployeeTaxInformation.CountAsync(cancellationToken);
            var leaveEntitlementCount = await db.LeaveEntitlements.CountAsync(cancellationToken);
            var leaveRequestCount = await db.LeaveRequests.CountAsync(cancellationToken);
            var grievanceRequestCount = await db.GrievanceRequests.CountAsync(cancellationToken);
            var payslipCount = await db.Payslips.CountAsync(cancellationToken);
            var activeSessionCount = await db.SessionRecords.CountAsync(x => x.IsActive, cancellationToken);
            var auditEventCount = await db.AuditEvents.CountAsync(cancellationToken);

            var integrityIssues = await repository.ValidateIntegrityAsync(cancellationToken);
            if (integrityIssues.Count > 0)
            {
                foreach (var issue in integrityIssues)
                {
                    logger.LogWarning("MySQL integrity issue in {TableName} ({IssueCode}): {Message}", issue.TableName, issue.IssueCode, issue.Message);
                }
            }

            logger.LogInformation(
                "MySQL status: Connected={CanConnect}, Employees={EmployeeCount}, AccountDetails={AccountDetailsCount}, TaxInformation={TaxInformationCount}, LeaveEntitlements={LeaveEntitlementCount}, LeaveRequests={LeaveRequestCount}, Grievances={GrievanceRequestCount}, Payslips={PayslipCount}, ActiveSessions={ActiveSessionCount}, AuditEvents={AuditEventCount}.",
                canConnect,
                employeeCount,
                accountDetailsCount,
                taxInformationCount,
                leaveEntitlementCount,
                leaveRequestCount,
                grievanceRequestCount,
                payslipCount,
                activeSessionCount,
                auditEventCount);
        }

        private static async Task EnsureLegacySchemaCompatibilityAsync(
            ApplicationDbContext db,
            ILogger logger,
            CancellationToken cancellationToken)
        {
            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_account_details",
                "group_name",
                "ALTER TABLE `employee_account_details` ADD COLUMN `group_name` VARCHAR(50) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_account_details",
                "record_snapshot_json",
                "ALTER TABLE `employee_account_details` ADD COLUMN `record_snapshot_json` LONGTEXT NULL;",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "ird_confirmation_status",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `ird_confirmation_status` VARCHAR(30) NOT NULL DEFAULT 'Unconfirmed';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "extra_paye_voluntary_deduction",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `extra_paye_voluntary_deduction` DECIMAL(12,2) NOT NULL DEFAULT 0.00;",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "tax_code_description",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `tax_code_description` VARCHAR(255) NOT NULL DEFAULT 'Standard tax code';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "tax_code_job_type",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `tax_code_job_type` VARCHAR(30) NOT NULL DEFAULT 'Primary';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "primary_tax_code",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `primary_tax_code` VARCHAR(10) NOT NULL DEFAULT 'M';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "secondary_tax_code",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `secondary_tax_code` VARCHAR(10) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "special_tax_code",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `special_tax_code` VARCHAR(10) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "special_tax_code_rate",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `special_tax_code_rate` VARCHAR(20) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "stc_letter_received",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `stc_letter_received` VARCHAR(30) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "stc_expiry_date",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `stc_expiry_date` VARCHAR(30) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "student_loan_existence",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `student_loan_existence` VARCHAR(10) NOT NULL DEFAULT 'Yes';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "student_loan_sdr_ird_letter_existence",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `student_loan_sdr_ird_letter_existence` VARCHAR(50) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "student_loan_special_deduction_rate",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `student_loan_special_deduction_rate` VARCHAR(20) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "student_loan_repayment_exemption_existence",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `student_loan_repayment_exemption_existence` VARCHAR(30) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "student_loan_repayment_exemption_reason",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `student_loan_repayment_exemption_reason` VARCHAR(255) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "student_loan_repayment_exemption_expiry_date",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `student_loan_repayment_exemption_expiry_date` VARCHAR(30) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "student_loan_repayment_threshold",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `student_loan_repayment_threshold` VARCHAR(20) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "child_support_status",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `child_support_status` VARCHAR(50) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "child_support_standard_deduction",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `child_support_standard_deduction` VARCHAR(50) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "child_support_voluntary_deduction",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `child_support_voluntary_deduction` VARCHAR(50) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "child_support_protected_net_earnings",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `child_support_protected_net_earnings` VARCHAR(50) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "child_support_ird_notice_of_deduction_existence",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `child_support_ird_notice_of_deduction_existence` VARCHAR(50) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "child_support_ird_notice_of_deduction_amount",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `child_support_ird_notice_of_deduction_amount` VARCHAR(50) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "child_support_ird_notice_of_deduction_effective_date",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `child_support_ird_notice_of_deduction_effective_date` VARCHAR(30) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "child_support_ird_notice_of_deduction_expiry_date",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `child_support_ird_notice_of_deduction_expiry_date` VARCHAR(30) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "child_support_ird_notice_of_deduction_reference_number",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `child_support_ird_notice_of_deduction_reference_number` VARCHAR(100) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "employee_extras",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `employee_extras` VARCHAR(100) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "esct_tax_rate",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `esct_tax_rate` VARCHAR(20) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "esct_income_band",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `esct_income_band` VARCHAR(100) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "student_loan_rate",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `student_loan_rate` DECIMAL(12,2) NOT NULL DEFAULT 12.00;",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "kiwi_saver_employer_contribution_rate",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `kiwi_saver_employer_contribution_rate` DECIMAL(12,2) NOT NULL DEFAULT 3.00;",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "acc_levy_rate",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `acc_levy_rate` DECIMAL(12,2) NOT NULL DEFAULT 1.75;",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "employee_tax_information",
                "employee_type",
                "ALTER TABLE `employee_tax_information` ADD COLUMN `employee_type` VARCHAR(30) NOT NULL DEFAULT 'Salary';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "leave_requests",
                "handled_by_username",
                "ALTER TABLE `leave_requests` ADD COLUMN `handled_by_username` VARCHAR(50) NOT NULL DEFAULT '';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "grievance_requests",
                "created_by_role",
                "ALTER TABLE `grievance_requests` ADD COLUMN `created_by_role` VARCHAR(50) NOT NULL DEFAULT 'Employee';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "grievance_requests",
                "notification_group",
                "ALTER TABLE `grievance_requests` ADD COLUMN `notification_group` VARCHAR(50) NOT NULL DEFAULT 'Employee';",
                cancellationToken);

            await EnsureColumnExistsAsync(
                db,
                logger,
                "grievance_requests",
                "handled_by_username",
                "ALTER TABLE `grievance_requests` ADD COLUMN `handled_by_username` VARCHAR(50) NOT NULL DEFAULT '';",
                cancellationToken);
        }

        private static async Task EnsureColumnExistsAsync(
            ApplicationDbContext db,
            ILogger logger,
            string tableName,
            string columnName,
            string addColumnSql,
            CancellationToken cancellationToken)
        {
            if (await ColumnExistsAsync(db, tableName, columnName, cancellationToken))
            {
                return;
            }

            logger.LogWarning(
                "MySQL compatibility update: adding missing column {ColumnName} to {TableName}.",
                columnName,
                tableName);

            await db.Database.ExecuteSqlRawAsync(addColumnSql, cancellationToken);
        }

        private static async Task<bool> ColumnExistsAsync(
            ApplicationDbContext db,
            string tableName,
            string columnName,
            CancellationToken cancellationToken)
        {
            var connection = db.Database.GetDbConnection();
            var closeWhenDone = connection.State != System.Data.ConnectionState.Open;
            if (closeWhenDone)
            {
                await connection.OpenAsync(cancellationToken);
            }

            try
            {
                await using var command = connection.CreateCommand();
                command.CommandText =
                    "SELECT COUNT(*) " +
                    "FROM information_schema.columns " +
                    "WHERE table_schema = DATABASE() AND table_name = @tableName AND column_name = @columnName;";

                var tableParameter = command.CreateParameter();
                tableParameter.ParameterName = "@tableName";
                tableParameter.Value = tableName;
                command.Parameters.Add(tableParameter);

                var columnParameter = command.CreateParameter();
                columnParameter.ParameterName = "@columnName";
                columnParameter.Value = columnName;
                command.Parameters.Add(columnParameter);

                var result = await command.ExecuteScalarAsync(cancellationToken);
                return result is not null && Convert.ToInt32(result) > 0;
            }
            finally
            {
                if (closeWhenDone)
                {
                    await connection.CloseAsync();
                }
            }
        }

        private static EmployeeEntity BuildEmployeeEntity(string username, IReadOnlyDictionary<string, string> details)
        {
            return new EmployeeEntity
            {
                Username = username.Trim(),
                FirstName = EmployeeRecordStore.TryGetValue(details, "First Name"),
                LastName = EmployeeRecordStore.TryGetValue(details, "Last Name"),
                BusinessRole = EmployeeRecordStore.TryGetValue(details, "Business Role"),
                JobRole = EmployeeRecordStore.TryGetValue(details, "Job Role"),
                AccountStatus = string.IsNullOrWhiteSpace(EmployeeRecordStore.TryGetValue(details, "Account Status"))
                    ? "Active"
                    : EmployeeRecordStore.TryGetValue(details, "Account Status"),
                Email = EmployeeRecordStore.TryGetValue(details, "Email Address"),
                Phone = EmployeeRecordStore.TryGetValue(details, "Phone Number"),
                UpdatedOnUtc = DateTime.UtcNow
            };
        }

        private static EmployeeAccountDetailsEntity BuildAccountDetailsEntity(string username, IReadOnlyDictionary<string, string> details)
        {
            return new EmployeeAccountDetailsEntity
            {
                Username = username.Trim(),
                EmergencyContactEncrypted = EmployeeRecordStore.TryGetValue(details, "Emergency Contact"),
                DiagnosedHealthConditionsEncrypted = EmployeeRecordStore.TryGetValue(details, "Diagnosed Health Conditions"),
                AllergiesEncrypted = EmployeeRecordStore.TryGetValue(details, "Allergies"),
                ApprovedEmergencyStepsEncrypted = EmployeeRecordStore.TryGetValue(details, "Approved Emergency Steps and Medicines"),
                PriorTrainingEncrypted = EmployeeRecordStore.TryGetValue(details, "Prior Training and Qualifications"),
                FirstAidCertificationEncrypted = EmployeeRecordStore.TryGetValue(details, "First Aid Certification"),
                GroupName = EmployeeRecordStore.TryGetValue(details, "Business Role").Trim().ToLowerInvariant(),
                RecordSnapshotJson = JsonSerializer.Serialize(details, SnapshotJsonOptions),
                UpdatedOnUtc = DateTime.UtcNow
            };
        }

        private static EmployeeTaxInformationEntity BuildTaxEntity(TaxInformationEditViewModel model, string username)
        {
            var primaryTaxCode = model.PrimaryTaxCode?.Trim() ?? string.Empty;
            var secondaryTaxCode = model.SecondaryTaxCode?.Trim() ?? string.Empty;
            var effectiveTaxCode = !string.IsNullOrWhiteSpace(primaryTaxCode) ? primaryTaxCode : secondaryTaxCode;
            return new EmployeeTaxInformationEntity
            {
                Username = username,
                IrdNumber = model.IRDNumber?.Trim() ?? string.Empty,
                IrdConfirmationStatus = string.IsNullOrWhiteSpace(model.IRDNumber) ? "Unconfirmed" : "Confirmed",
                PayFrequency = model.PayFrequency?.Trim() ?? "Fortnightly",
                PayeTableVersion = model.PAYETableVersion?.Trim() ?? DateTime.UtcNow.Year.ToString(),
                ExtraPayeVoluntaryDeduction = decimal.TryParse(model.ExtraPayeVoluntaryDeduction, out var extraPayeVoluntaryDeduction) ? extraPayeVoluntaryDeduction : 0m,
                TaxCode = string.IsNullOrWhiteSpace(effectiveTaxCode) ? "M" : effectiveTaxCode,
                TaxCodeDescription = string.IsNullOrWhiteSpace(effectiveTaxCode) ? "Standard tax code" : $"{effectiveTaxCode} tax code",
                TaxCodeJobType = model.TaxCodeJobType?.Trim() ?? "Primary",
                PrimaryTaxCode = primaryTaxCode,
                SecondaryTaxCode = secondaryTaxCode,
                SpecialTaxCode = model.SpecialTaxCode?.Trim() ?? string.Empty,
                SpecialTaxCodeRate = model.STCRate?.Trim() ?? string.Empty,
                STCLetterReceived = model.STCLetterReceived?.Trim() ?? string.Empty,
                STCExpiryDate = model.STCExpiryDate?.Trim() ?? string.Empty,
                KiwiSaverOptInOutStatus = model.KiwiSaverOptInOutStatus?.Trim() ?? "opt-in",
                KiwiSaverEmployeeContributionRate = model.KiwiSaverEmployeeContributionRate?.Trim() ?? "3%",
                KiwiSaverOptOutDate = model.KiwiSaverOptOutDate?.Trim() ?? string.Empty,
                StudentLoanStatus = string.Equals(model.StudentLoanExistence?.Trim(), "Yes", StringComparison.OrdinalIgnoreCase)
                    ? "In repayment"
                    : "Not in repayment",
                StudentLoanExistence = model.StudentLoanExistence?.Trim() ?? "Yes",
                StudentLoanSDRIRDLetterExistence = model.StudentLoanSDRIRDLetterExistence?.Trim() ?? string.Empty,
                StudentLoanSpecialDeductionRate = model.StudentLoanSpecialDeductionRate?.Trim() ?? string.Empty,
                StudentLoanRepaymentExemptionExistence = model.StudentLoanRepaymentExemptionExistence?.Trim() ?? string.Empty,
                StudentLoanRepaymentExemptionReason = model.StudentLoanRepaymentExemptionReason?.Trim() ?? string.Empty,
                StudentLoanRepaymentExemptionExpiryDate = model.StudentLoanRepaymentExemptionExpiryDate?.Trim() ?? string.Empty,
                StudentLoanRepaymentThreshold = model.StudentLoanRepaymentThreshold?.Trim() ?? string.Empty,
                ChildSupportStatus = model.ChildSupportStatus?.Trim() ?? string.Empty,
                ChildSupportStandardDeduction = model.ChildSupportStandardDeduction?.Trim() ?? string.Empty,
                ChildSupportVoluntaryDeduction = model.ChildSupportVoluntaryDeduction?.Trim() ?? string.Empty,
                ChildSupportProtectedNetEarnings = model.ChildSupportProtectedNetEarnings?.Trim() ?? string.Empty,
                ChildSupportIRDNoticeOfDeductionExistence = model.ChildSupportIRDNoticeOfDeductionExistence?.Trim() ?? string.Empty,
                ChildSupportIRDNoticeOfDeductionAmount = model.ChildSupportIRDNoticeOfDeductionAmount?.Trim() ?? string.Empty,
                ChildSupportIRDNoticeOfDeductionEffectiveDate = model.ChildSupportIRDNoticeOfDeductionEffectiveDate?.Trim() ?? string.Empty,
                ChildSupportIRDNoticeOfDeductionExpiryDate = model.ChildSupportIRDNoticeOfDeductionExpiryDate?.Trim() ?? string.Empty,
                ChildSupportIRDNoticeOfDeductionReferenceNumber = model.ChildSupportIRDNoticeOfDeductionReferenceNumber?.Trim() ?? string.Empty,
                EmployeeExtras = model.EmployeeExtras?.Trim() ?? string.Empty,
                ESCTTaxRate = model.ESCTTaxRate?.Trim() ?? string.Empty,
                ESCTIncomeBand = model.ESCTIncomeBand?.Trim() ?? string.Empty,
                StudentLoanRate = 12m,
                KiwiSaverEmployerContributionRate = 3m,
                ACCLevyRate = 1.75m,
                EmployeeType = "Salary",
                UpdatedOnUtc = DateTime.UtcNow
            };
        }

        private static LeaveEntitlementEntity BuildLeaveEntitlementEntity(LeaveEntitlement entitlement)
        {
            return new LeaveEntitlementEntity
            {
                Username = entitlement.Username,
                BusinessRole = entitlement.BusinessRole,
                JobRole = entitlement.JobRole,
                SalaryPackageName = entitlement.SalaryPackageName,
                SalaryPackageDisplayName = entitlement.SalaryPackageDisplayName,
                ApproximateAnnualSalary = entitlement.ApproximateAnnualSalary,
                KiwiSaverEmployerContribution = entitlement.KiwiSaverEmployerContribution,
                AnnualLeaveLawfulDays = entitlement.AnnualLeaveLawfulDays,
                AnnualLeavePackageExtraDays = entitlement.AnnualLeavePackageExtraDays,
                AnnualLeaveDays = entitlement.AnnualLeaveDays,
                AnnualLeaveTakenDays = entitlement.AnnualLeaveTakenDays,
                AnnualLeaveScheduledDays = entitlement.AnnualLeaveScheduledDays,
                SickLeaveDays = entitlement.SickLeaveDays,
                SickLeaveLawfulDays = entitlement.SickLeaveLawfulDays,
                SickLeavePackageExtraDays = entitlement.SickLeavePackageExtraDays,
                SickLeaveTakenDays = entitlement.SickLeaveTakenDays,
                SickLeaveScheduledDays = entitlement.SickLeaveScheduledDays,
                PublicHolidayDays = entitlement.PublicHolidayDays,
                PublicHolidayLawfulDays = entitlement.PublicHolidayLawfulDays,
                PublicHolidayPackageExtraDays = entitlement.PublicHolidayPackageExtraDays,
                SpecialLeaveDays = entitlement.SpecialLeaveDays,
                SpecialLeaveLawfulDays = entitlement.SpecialLeaveLawfulDays,
                SpecialLeavePackageExtraDays = entitlement.SpecialLeavePackageExtraDays,
                SpecialLeaveTakenDays = entitlement.SpecialLeaveTakenDays,
                SpecialLeaveScheduledDays = entitlement.SpecialLeaveScheduledDays,
                ParentalLeaveWeeks = entitlement.ParentalLeaveWeeks,
                ParentalLeaveLawfulWeeks = entitlement.ParentalLeaveLawfulWeeks,
                ParentalLeavePackageExtraWeeks = entitlement.ParentalLeavePackageExtraWeeks,
                ParentalLeaveTakenWeeks = entitlement.ParentalLeaveTakenWeeks,
                ParentalLeaveScheduledWeeks = entitlement.ParentalLeaveScheduledWeeks,
                SuperannuationContribution = entitlement.SuperannuationContribution,
                HealthInsuranceContribution = entitlement.HealthInsuranceContribution,
                ProfessionalDevelopmentContribution = entitlement.ProfessionalDevelopmentContribution,
                EmployeeAssistanceProgramContribution = entitlement.EmployeeAssistanceProgramContribution,
                RemainingAnnualLeaveDays = entitlement.RemainingAnnualLeaveDays,
                RemainingSickLeaveDays = entitlement.RemainingSickLeaveDays,
                RemainingSpecialLeaveDays = entitlement.RemainingSpecialLeaveDays,
                RemainingParentalLeaveWeeks = entitlement.RemainingParentalLeaveWeeks,
                UpdatedOnUtc = DateTime.UtcNow
            };
        }

        private static LeaveRequestEntity BuildLeaveRequestEntity(LeaveRequest request)
        {
            return new LeaveRequestEntity
            {
                Id = request.Id,
                SubmittedByUsername = request.SubmittedByUsername,
                SubmittedForUsername = request.SubmittedForUsername,
                EmployeeName = request.EmployeeName,
                LeaveType = request.LeaveType,
                StartDate = request.StartDate,
                EndDate = request.EndDate,
                ReasonEncrypted = request.Reason,
                Status = request.Status,
                DecisionReasonEncrypted = request.DecisionReason,
                RequestedOnUtc = request.RequestedOn == default ? DateTime.UtcNow : request.RequestedOn,
                UpdatedOnUtc = request.UpdatedOn,
                HandledByUsername = request.HandledByUsername
            };
        }

        private static GrievanceRequestEntity BuildGrievanceRequestEntity(GrievanceRequest request)
        {
            return new GrievanceRequestEntity
            {
                Id = request.Id,
                SubmittedByUsername = request.SubmittedByUsername,
                SubmittedForUsername = request.SubmittedForUsername,
                EmployeeName = request.EmployeeName,
                Subject = request.Subject,
                DescriptionEncrypted = request.Description,
                Category = request.Category,
                Severity = request.Severity,
                Status = request.Status,
                OutcomeEncrypted = request.Outcome,
                DecisionReasonEncrypted = request.DecisionReason,
                SubmittedOnUtc = request.SubmittedOn == default ? DateTime.UtcNow : request.SubmittedOn,
                UpdatedOnUtc = request.UpdatedOn,
                CreatedByRole = request.CreatedByRole,
                NotificationGroup = request.NotificationGroup,
                HandledByUsername = request.HandledByUsername
            };
        }

        private static PayslipEntity BuildPayslipEntity(PayslipRecord record)
        {
            return new PayslipEntity
            {
                Id = record.Id,
                Username = record.Username,
                EmployeeName = record.EmployeeName,
                SalaryPackageName = record.SalaryPackageName,
                SalaryPackageDisplayName = record.SalaryPackageDisplayName,
                IrdNumber = record.IRDNumber,
                PayPeriod = record.PayPeriod,
                PayFrequency = record.PayFrequency,
                BasePay = record.BasePay,
                OvertimePay = record.OvertimePay,
                BonusPay = record.BonusPay,
                AllowancePay = record.AllowancePay,
                PreTaxDeductions = record.PreTaxDeductions,
                GrossPay = record.GrossPay,
                TaxableIncome = record.TaxableIncome,
                Paye = record.PAYE,
                StudentLoan = record.StudentLoan,
                KiwiSaverEmployeeContribution = record.KiwiSaverEmployeeContribution,
                KiwiSaverEmployerContribution = record.KiwiSaverEmployerContribution,
                Esct = record.ESCT,
                AccLevy = record.ACCLevy,
                PostTaxDeductions = record.PostTaxDeductions,
                NetPay = record.NetPay,
                AnnualLeaveLawfulDays = record.AnnualLeaveLawfulDays,
                AnnualLeavePackageExtraDays = record.AnnualLeavePackageExtraDays,
                AnnualLeaveTakenDays = record.AnnualLeaveTakenDays,
                AnnualLeaveScheduledDays = record.AnnualLeaveScheduledDays,
                AnnualLeaveRemainingDays = record.AnnualLeaveRemainingDays,
                SickLeaveLawfulDays = record.SickLeaveLawfulDays,
                SickLeavePackageExtraDays = record.SickLeavePackageExtraDays,
                SickLeaveTakenDays = record.SickLeaveTakenDays,
                SickLeaveScheduledDays = record.SickLeaveScheduledDays,
                SickLeaveRemainingDays = record.SickLeaveRemainingDays,
                SpecialLeaveLawfulDays = record.SpecialLeaveLawfulDays,
                SpecialLeavePackageExtraDays = record.SpecialLeavePackageExtraDays,
                SpecialLeaveTakenDays = record.SpecialLeaveTakenDays,
                SpecialLeaveScheduledDays = record.SpecialLeaveScheduledDays,
                SpecialLeaveRemainingDays = record.SpecialLeaveRemainingDays,
                ParentalLeaveLawfulWeeks = record.ParentalLeaveLawfulWeeks,
                ParentalLeavePackageExtraWeeks = record.ParentalLeavePackageExtraWeeks,
                ParentalLeaveRemainingWeeks = record.ParentalLeaveRemainingWeeks,
                PublicHolidayDays = record.PublicHolidayDays,
                PublicHolidayPackageExtraDays = record.PublicHolidayPackageExtraDays,
                CreatedOnUtc = record.CreatedOn == default ? DateTime.UtcNow : record.CreatedOn
            };
        }

        private static bool HasKnownEmployeeReferences(
            string? submittedByUsername,
            string? submittedForUsername,
            IReadOnlySet<string> knownEmployeeUsernames)
        {
            return !string.IsNullOrWhiteSpace(submittedByUsername)
                && !string.IsNullOrWhiteSpace(submittedForUsername)
                && knownEmployeeUsernames.Contains(submittedByUsername.Trim())
                && knownEmployeeUsernames.Contains(submittedForUsername.Trim());
        }
    }
}
