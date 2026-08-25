using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.Json;
using NZFTC_EMS.Models;

namespace NZFTC_EMS.Services
{
    public class PayrollService
    {
        private static readonly List<PayslipRecord> Payslips = new();
        private static readonly object PayslipArchiveLock = new();
        private static readonly JsonSerializerOptions PayslipArchiveJsonOptions = new()
        {
            WriteIndented = true
        };
        private static bool PayslipArchiveLoaded;
        private readonly LeaveRequestService _leaveRequestService;

        public PayrollService(LeaveRequestService leaveRequestService)
        {
            _leaveRequestService = leaveRequestService;
            EnsurePayslipArchiveLoaded();
        }

        public TaxInformationEditViewModel GetEditableTaxInformationForUser(string username)
        {
            var targetUsername = string.IsNullOrWhiteSpace(username) ? string.Empty : username.Trim();
            if (string.IsNullOrWhiteSpace(targetUsername))
            {
                return new TaxInformationEditViewModel();
            }

            var employeeName = ResolveEmployeeDisplayName(targetUsername) ?? string.Empty;
            var irdNumber = TryResolveIrdNumberForUsername(targetUsername);
            var model = new TaxInformationEditViewModel
            {
                Username = targetUsername,
                EmployeeName = employeeName,
                IRDNumber = irdNumber ?? string.Empty
            };

            if (string.IsNullOrWhiteSpace(irdNumber))
            {
                return model;
            }

            foreach (var filePath in EnumerateIrdFiles())
            {
                if (!string.Equals(Path.GetFileNameWithoutExtension(filePath), irdNumber, StringComparison.OrdinalIgnoreCase))
                {
                    continue;
                }

                var parsed = ParseKeyValueFile(filePath);
                model.PayFrequency = GetFieldValue(parsed, "PAYE Frequency", "Pay Frequency");
                model.PAYETableVersion = GetFieldValue(parsed, "PAYE Table Version");
                model.ExtraPayeVoluntaryDeduction = GetFieldValue(parsed, "Extra PAYE Voluntary Deduction");
                model.TaxCodeJobType = GetFieldValue(parsed, "Primary or Secondary Employment");
                model.PrimaryTaxCode = GetFieldValue(parsed, "Primary Tax Code");
                model.SecondaryTaxCode = GetFieldValue(parsed, "Secondary Tax Code");
                model.SpecialTaxCode = GetFieldValue(parsed, "Special Tax Code");
                model.STCRate = GetFieldValue(parsed, "STC (Special Tax Code) Rate", "STC Rate");
                model.STCLetterReceived = GetFieldValue(parsed, "STC Letter Received");
                model.STCExpiryDate = GetFieldValue(parsed, "STC Expiry Date");
                model.StudentLoanExistence = GetFieldValue(parsed, "Student Loan Existence");
                model.StudentLoanSDRIRDLetterExistence = GetFieldValue(parsed, "Student Loan SDR IRD Letter Existence", "Student Loan Special Deduction Rate Letter Received");
                model.StudentLoanSpecialDeductionRate = GetFieldValue(parsed, "Student Loan Special Deduction Rate");
                model.StudentLoanRepaymentExemptionExistence = GetFieldValue(parsed, "Student Loan Repayment Exemption Existence");
                model.StudentLoanRepaymentExemptionReason = GetFieldValue(parsed, "Student Loan Repayment Exemption Reason");
                model.StudentLoanRepaymentExemptionExpiryDate = GetFieldValue(parsed, "Student Loan Repayment Exemption Expiry Date");
                model.StudentLoanRepaymentThreshold = GetFieldValue(parsed, "Student Loan Repayment Threshold");
                model.ChildSupportStatus = GetFieldValue(parsed, "Child Support Status");
                model.ChildSupportStandardDeduction = GetFieldValue(parsed, "Child Support Standard Deduction");
                model.ChildSupportVoluntaryDeduction = GetFieldValue(parsed, "Child Support Voluntary Deduction");
                model.ChildSupportProtectedNetEarnings = GetFieldValue(parsed, "Child Support Protected Net Earnings");
                model.ChildSupportIRDNoticeOfDeductionExistence = GetFieldValue(parsed, "Child Support IRD Notice of Deduction Existence");
                model.ChildSupportIRDNoticeOfDeductionAmount = GetFieldValue(parsed, "Child Support IRD Notice of Deduction Amount");
                model.ChildSupportIRDNoticeOfDeductionEffectiveDate = GetFieldValue(parsed, "Child Support IRD Notice of Deduction Effective Date");
                model.ChildSupportIRDNoticeOfDeductionExpiryDate = GetFieldValue(parsed, "Child Support IRD Notice of Deduction Expiry Date");
                model.ChildSupportIRDNoticeOfDeductionReferenceNumber = GetFieldValue(parsed, "Child Support IRD Notice of Deduction Reference Number");
                model.KiwiSaverOptInOutStatus = GetFieldValue(parsed, "KiwiSaver Status");
                model.KiwiSaverEmployeeContributionRate = GetFieldValue(parsed, "KiwiSaver Employee Contribution Rate");
                model.KiwiSaverOptOutDate = GetFieldValue(parsed, "KiwiSaver Opt Out Date");
                model.EmployeeExtras = GetFieldValue(parsed, "Employee Extras");
                model.ESCTTaxRate = GetFieldValue(parsed, "Employee Extra Rate", "ESCT Tax Rate");
                model.ESCTIncomeBand = GetFieldValue(parsed, "Employee Extra Type(s)", "ESCT Income Band");

                model.ShowStudentLoanOptions = !string.Equals(model.PrimaryTaxCode?.Trim(), "ME", StringComparison.OrdinalIgnoreCase);
                model.SpecialTaxCodeActive = IsActiveValue(model.SpecialTaxCode);
                model.StudentLoanActive = IsYesValue(model.StudentLoanExistence);
                model.ChildSupportActive = IsActiveValue(model.ChildSupportStatus);
                model.KiwiSaverActive = IsOptInValue(model.KiwiSaverOptInOutStatus);
                model.KiwiSaverOptedOut = IsOptOutValue(model.KiwiSaverOptInOutStatus);
                model.ESCTActive = IsActiveValue(model.EmployeeExtras);
                break;
            }

            return model;
        }

        public PayrollTaxInformation GetTaxInformationForUser(string username)
        {
            var targetUsername = string.IsNullOrWhiteSpace(username) ? string.Empty : username.Trim();
            if (string.IsNullOrWhiteSpace(targetUsername))
            {
                return new PayrollTaxInformation();
            }

            var employeeName = ResolveEmployeeDisplayName(targetUsername) ?? string.Empty;
            var irdNumber = TryResolveIrdNumberForUsername(targetUsername);
            var employerDefaults = GetEmployerDefaults();
            var payeInfo = new PayrollTaxInformation
            {
                Username = targetUsername,
                EmployeeName = employeeName,
                IRDNumber = irdNumber ?? string.Empty,
                IRDConfirmationStatus = "Unconfirmed",
                PayFrequency = "Fortnightly",
                PAYETableVersion = DateTime.UtcNow.Year.ToString(CultureInfo.InvariantCulture),
                ExtraPayeVoluntaryDeduction = 0m,
                TaxCode = "M",
                TaxCodeDescription = "Standard tax code",
                TaxCodeJobType = "Primary",
                PrimaryTaxCode = "M",
                SecondaryTaxCode = string.Empty,
                SpecialTaxCode = string.Empty,
                SpecialTaxCodeRate = string.Empty,
                KiwiSaverOptInOutStatus = "opt-in",
                KiwiSaverEmployeeContributionRate = "3%",
                KiwiSaverOptOutDate = string.Empty,
                StudentLoanStatus = "Not in repayment",
                StudentLoanExistence = "Yes",
                StudentLoanSDRIRDLetterExistence = string.Empty,
                StudentLoanSpecialDeductionRate = string.Empty,
                StudentLoanRepaymentExemptionExistence = string.Empty,
                StudentLoanRepaymentExemptionReason = string.Empty,
                StudentLoanRepaymentExemptionExpiryDate = string.Empty,
                StudentLoanRepaymentThreshold = employerDefaults.StudentLoanThreshold.ToString(CultureInfo.InvariantCulture),
                EmployeeExtras = string.Empty,
                ESCTTaxRate = string.Empty,
                ESCTIncomeBand = string.Empty,
                StudentLoanRate = employerDefaults.StudentLoanRate,
                KiwiSaverEmployerContributionRate = employerDefaults.KiwiSaverEmployerContributionRate,
                ACCLevyRate = employerDefaults.ACCLevyRate,
                EmployeeType = "Salary"
            };

            if (string.IsNullOrWhiteSpace(irdNumber))
            {
                return payeInfo;
            }

            foreach (var filePath in EnumerateIrdFiles())
            {
                if (!string.Equals(Path.GetFileNameWithoutExtension(filePath), irdNumber, StringComparison.OrdinalIgnoreCase))
                {
                    continue;
                }

                var parsed = ParseKeyValueFile(filePath);
                if (parsed.TryGetValue("IRD Confirmation Status", out var confirmationStatus) &&
                    !string.IsNullOrWhiteSpace(confirmationStatus))
                {
                    payeInfo.IRDConfirmationStatus = confirmationStatus.Trim();
                }
                else if (parsed.TryGetValue("IRD Confirmation Status:", out var confirmationStatusWithColon) &&
                         !string.IsNullOrWhiteSpace(confirmationStatusWithColon))
                {
                    payeInfo.IRDConfirmationStatus = confirmationStatusWithColon.Trim();
                }

                if (TryGetField(parsed, out var payFrequency, "PAYE Frequency", "Pay Frequency"))
                {
                    payeInfo.PayFrequency = payFrequency;
                }

                if (TryGetField(parsed, out var tableVersion, "PAYE Table Version"))
                {
                    payeInfo.PAYETableVersion = tableVersion;
                }

                if (TryGetField(parsed, out var voluntaryDeduction, "Extra PAYE Voluntary Deduction") &&
                    decimal.TryParse(voluntaryDeduction.Trim(), NumberStyles.Number, CultureInfo.InvariantCulture, out var parsedDeduction))
                {
                    payeInfo.ExtraPayeVoluntaryDeduction = parsedDeduction;
                }

                if (TryGetField(parsed, out var taxCodeJobType, "Primary or Secondary Employment"))
                {
                    payeInfo.TaxCodeJobType = taxCodeJobType;
                }

                if (TryGetField(parsed, out var taxCode, "Primary Tax Code"))
                {
                    payeInfo.TaxCode = taxCode;
                    payeInfo.PrimaryTaxCode = taxCode;
                }

                if (TryGetField(parsed, out var secondaryTaxCode, "Secondary Tax Code"))
                {
                    payeInfo.SecondaryTaxCode = secondaryTaxCode;
                }

                if (TryGetField(parsed, out var specialTaxCode, "Special Tax Code"))
                {
                    payeInfo.SpecialTaxCode = specialTaxCode;
                }

                if (TryGetField(parsed, out var specialTaxCodeRate, "STC (Special Tax Code) Rate", "STC Rate"))
                {
                    payeInfo.SpecialTaxCodeRate = specialTaxCodeRate;
                }

                if (TryGetField(parsed, out var taxCodeDescription, "Tax Code Description"))
                {
                    payeInfo.TaxCodeDescription = taxCodeDescription;
                }

                if (TryGetField(parsed, out var kiwiSaverStatus, "KiwiSaver Status", "KiwiSaver Opt In Out Status"))
                {
                    payeInfo.KiwiSaverOptInOutStatus = kiwiSaverStatus;
                }

                if (TryGetField(parsed, out var kiwiSaverEmployeeRate, "KiwiSaver Employee Contribution Rate"))
                {
                    payeInfo.KiwiSaverEmployeeContributionRate = kiwiSaverEmployeeRate;
                }

                if (TryGetField(parsed, out var kiwiSaverOptOutDate, "KiwiSaver Opt Out Date"))
                {
                    payeInfo.KiwiSaverOptOutDate = kiwiSaverOptOutDate;
                }

                if (TryGetField(parsed, out var studentLoanStatus, "Student Loan Existence", "Student Loan Status"))
                {
                    payeInfo.StudentLoanStatus = studentLoanStatus;
                }

                if (TryGetField(parsed, out var studentLoanExistence, "Student Loan Existence"))
                {
                    payeInfo.StudentLoanExistence = studentLoanExistence;
                }

                if (TryGetField(parsed, out var studentLoanSdr, "Student Loan SDR IRD Letter Existence", "Student Loan Special Deduction Rate Letter Received"))
                {
                    payeInfo.StudentLoanSDRIRDLetterExistence = studentLoanSdr;
                }

                if (TryGetField(parsed, out var studentLoanSpecialRate, "Student Loan Special Deduction Rate"))
                {
                    payeInfo.StudentLoanSpecialDeductionRate = studentLoanSpecialRate;
                }

                if (TryGetField(parsed, out var studentLoanExemption, "Student Loan Repayment Exemption Existence"))
                {
                    payeInfo.StudentLoanRepaymentExemptionExistence = studentLoanExemption;
                }

                if (TryGetField(parsed, out var studentLoanExemptionReason, "Student Loan Repayment Exemption Reason"))
                {
                    payeInfo.StudentLoanRepaymentExemptionReason = studentLoanExemptionReason;
                }

                if (TryGetField(parsed, out var studentLoanExemptionExpiry, "Student Loan Repayment Exemption Expiry Date"))
                {
                    payeInfo.StudentLoanRepaymentExemptionExpiryDate = studentLoanExemptionExpiry;
                }

                if (TryGetField(parsed, out var studentLoanThreshold, "Student Loan Repayment Threshold"))
                {
                    payeInfo.StudentLoanRepaymentThreshold = studentLoanThreshold;
                }

                if (TryGetField(parsed, out var employeeExtras, "Employee Extras"))
                {
                    payeInfo.EmployeeExtras = employeeExtras;
                }

                if (TryGetField(parsed, out var esctTaxRate, "Employee Extra Rate", "ESCT Tax Rate"))
                {
                    payeInfo.ESCTTaxRate = esctTaxRate;
                }

                if (TryGetField(parsed, out var esctIncomeBand, "Employee Extra Type(s)", "ESCT Income Band"))
                {
                    payeInfo.ESCTIncomeBand = esctIncomeBand;
                }

                if (TryGetField(parsed, out var kiwiSaverRate, "KiwiSaver Employer Contribution Rate") &&
                    decimal.TryParse(kiwiSaverRate.Trim(), NumberStyles.Number, CultureInfo.InvariantCulture, out var parsedKiwiSaverRate))
                {
                    payeInfo.KiwiSaverEmployerContributionRate = parsedKiwiSaverRate;
                }

                if (TryGetField(parsed, out var accLevyRate, "ACC Levy Rate") &&
                    decimal.TryParse(accLevyRate.Trim(), NumberStyles.Number, CultureInfo.InvariantCulture, out var parsedAccLevyRate))
                {
                    payeInfo.ACCLevyRate = parsedAccLevyRate;
                }

                if (TryGetField(parsed, out var employeeType, "Employee Type"))
                {
                    payeInfo.EmployeeType = employeeType;
                }

                payeInfo.TaxCode = ResolveTaxCodeForPayslip(payeInfo);
                payeInfo.TaxCodeDescription = DescribeTaxCode(payeInfo);
                break;
            }

            return payeInfo;
        }

        public IReadOnlyList<PayslipRecord> GetPayslipsForUser(string username)
        {
            EnsurePayslipArchiveLoaded();
            var targetUsername = string.IsNullOrWhiteSpace(username) ? string.Empty : username.Trim();
            if (string.IsNullOrWhiteSpace(targetUsername))
            {
                return Array.Empty<PayslipRecord>();
            }

            return Payslips
                .Where(p => string.Equals(p.Username, targetUsername, StringComparison.OrdinalIgnoreCase))
                .OrderByDescending(p => p.CreatedOn)
                .ToList();
        }

        public PayslipRecord CreatePayslip(
            string submittedByUsername,
            string submittedForUsername,
            decimal basePay,
            decimal overtimePay,
            decimal bonusPay,
            decimal allowancePay,
            decimal preTaxDeductions,
            decimal postTaxDeductions,
            string? payPeriod,
            string? payFrequency)
        {
            EnsurePayslipArchiveLoaded();
            var targetUsername = string.IsNullOrWhiteSpace(submittedForUsername) ? submittedByUsername : submittedForUsername;
            var targetTaxInfo = GetTaxInformationForUser(targetUsername);
            var targetLeaveInfo = _leaveRequestService.GetEntitlementForUser(targetUsername);
            var employeeName = ResolveEmployeeDisplayName(targetUsername) ?? string.Empty;
            var irdNumber = string.IsNullOrWhiteSpace(targetTaxInfo.IRDNumber)
                ? TryResolveIrdNumberForUsername(targetUsername) ?? string.Empty
                : targetTaxInfo.IRDNumber;

            var normalizedBasePay = NormalizeMoney(basePay);
            var normalizedOvertimePay = NormalizeMoney(overtimePay);
            var normalizedBonusPay = NormalizeMoney(bonusPay);
            var normalizedAllowancePay = NormalizeMoney(allowancePay);
            var normalizedPreTaxDeductions = NormalizeMoney(preTaxDeductions);
            var normalizedPostTaxDeductions = NormalizeMoney(postTaxDeductions);
            var grossPay = RoundMoney(normalizedBasePay + normalizedOvertimePay + normalizedBonusPay + normalizedAllowancePay);
            var taxableIncome = Math.Max(0m, grossPay - normalizedPreTaxDeductions);
            var payFrequencyValue = string.IsNullOrWhiteSpace(payFrequency) ? targetTaxInfo.PayFrequency : payFrequency;
            var paye = CalculatePaye(taxableIncome, targetTaxInfo, payFrequencyValue);
            var studentLoan = CalculateStudentLoan(taxableIncome, targetTaxInfo, payFrequencyValue);
            var kiwiSaverEmployeeContribution = CalculateKiwiSaverEmployeeContribution(taxableIncome, targetTaxInfo);
            var kiwiSaverEmployerContribution = CalculateKiwiSaverEmployerContribution(taxableIncome, targetTaxInfo, targetLeaveInfo);
            var esct = CalculateEsct(taxableIncome, targetTaxInfo, targetLeaveInfo, kiwiSaverEmployerContribution);
            var payDate = DateTime.UtcNow;
            var accLevy = CalculateAccLevy(targetUsername, taxableIncome, targetTaxInfo, payDate);
            var netPay = RoundMoney(taxableIncome - paye - studentLoan - kiwiSaverEmployeeContribution - accLevy - normalizedPostTaxDeductions);

            var record = new PayslipRecord
            {
                Id = Payslips.Count == 0 ? 1 : Payslips.Max(p => p.Id) + 1,
                Username = targetUsername,
                EmployeeName = employeeName,
                SalaryPackageName = targetLeaveInfo.SalaryPackageName,
                SalaryPackageDisplayName = targetLeaveInfo.SalaryPackageDisplayName,
                IRDNumber = irdNumber,
                PayPeriod = string.IsNullOrWhiteSpace(payPeriod) ? "Current period" : payPeriod.Trim(),
                PayFrequency = string.IsNullOrWhiteSpace(payFrequency) ? targetTaxInfo.PayFrequency : payFrequency.Trim(),
                BasePay = normalizedBasePay,
                OvertimePay = normalizedOvertimePay,
                BonusPay = normalizedBonusPay,
                AllowancePay = normalizedAllowancePay,
                PreTaxDeductions = normalizedPreTaxDeductions,
                GrossPay = grossPay,
                TaxableIncome = taxableIncome,
                PAYE = paye,
                StudentLoan = studentLoan,
                KiwiSaverEmployeeContribution = kiwiSaverEmployeeContribution,
                KiwiSaverEmployerContribution = kiwiSaverEmployerContribution,
                ESCT = esct,
                ACCLevy = accLevy,
                PostTaxDeductions = normalizedPostTaxDeductions,
                NetPay = netPay,
                AnnualLeaveLawfulDays = targetLeaveInfo.AnnualLeaveLawfulDays,
                AnnualLeavePackageExtraDays = targetLeaveInfo.AnnualLeavePackageExtraDays,
                AnnualLeaveTakenDays = targetLeaveInfo.AnnualLeaveTakenDays,
                AnnualLeaveScheduledDays = targetLeaveInfo.AnnualLeaveScheduledDays,
                AnnualLeaveRemainingDays = targetLeaveInfo.RemainingAnnualLeaveDays,
                SickLeaveLawfulDays = targetLeaveInfo.SickLeaveLawfulDays,
                SickLeavePackageExtraDays = targetLeaveInfo.SickLeavePackageExtraDays,
                SickLeaveTakenDays = targetLeaveInfo.SickLeaveTakenDays,
                SickLeaveScheduledDays = targetLeaveInfo.SickLeaveScheduledDays,
                SickLeaveRemainingDays = targetLeaveInfo.RemainingSickLeaveDays,
                SpecialLeaveLawfulDays = targetLeaveInfo.SpecialLeaveLawfulDays,
                SpecialLeavePackageExtraDays = targetLeaveInfo.SpecialLeavePackageExtraDays,
                SpecialLeaveTakenDays = targetLeaveInfo.SpecialLeaveTakenDays,
                SpecialLeaveScheduledDays = targetLeaveInfo.SpecialLeaveScheduledDays,
                SpecialLeaveRemainingDays = targetLeaveInfo.RemainingSpecialLeaveDays,
                ParentalLeaveLawfulWeeks = targetLeaveInfo.ParentalLeaveLawfulWeeks,
                ParentalLeavePackageExtraWeeks = targetLeaveInfo.ParentalLeavePackageExtraWeeks,
                ParentalLeaveRemainingWeeks = targetLeaveInfo.RemainingParentalLeaveWeeks,
                PublicHolidayDays = targetLeaveInfo.PublicHolidayDays,
                PublicHolidayPackageExtraDays = targetLeaveInfo.PublicHolidayPackageExtraDays,
                CreatedOn = DateTime.UtcNow
            };

            lock (PayslipArchiveLock)
            {
                Payslips.Add(record);
                SavePayslipArchive();
                SavePayslipExport(record);
            }

            return record;
        }

        public PayslipRecord? GetPayslipById(int payslipId)
        {
            EnsurePayslipArchiveLoaded();
            return Payslips.FirstOrDefault(p => p.Id == payslipId);
        }

        public string? GetPayslipExportText(int payslipId)
        {
            var record = GetPayslipById(payslipId);
            return record == null ? null : BuildPayslipExportText(record);
        }

        public bool UpdateTaxInformation(string username, TaxInformationEditViewModel updatedInformation)
        {
            var targetUsername = string.IsNullOrWhiteSpace(updatedInformation.Username) ? username : updatedInformation.Username;
            if (string.IsNullOrWhiteSpace(targetUsername))
            {
                return false;
            }

            var irdNumber = TryResolveIrdNumberForUsername(targetUsername);
            if (string.IsNullOrWhiteSpace(irdNumber))
            {
                return false;
            }

            var filePath = EnumerateIrdFiles()
                .FirstOrDefault(path => string.Equals(Path.GetFileNameWithoutExtension(path), irdNumber, StringComparison.OrdinalIgnoreCase));
            if (string.IsNullOrWhiteSpace(filePath) || !File.Exists(filePath))
            {
                return false;
            }

            var lines = File.ReadAllLines(filePath).ToList();
            UpsertField(lines, "PAYE Frequency", updatedInformation.PayFrequency);
            UpsertField(lines, "PAYE Table Version", updatedInformation.PAYETableVersion);
            UpsertField(lines, "Extra PAYE Voluntary Deduction", updatedInformation.ExtraPayeVoluntaryDeduction);

            if (!string.IsNullOrWhiteSpace(updatedInformation.TaxCodeJobType))
            {
                UpsertField(lines, "Primary or Secondary Employment", updatedInformation.TaxCodeJobType);
            }
            if (!string.IsNullOrWhiteSpace(updatedInformation.PrimaryTaxCode))
            {
                UpsertField(lines, "Primary Tax Code", updatedInformation.PrimaryTaxCode);
                RemoveField(lines, "Secondary Tax Code");
                UpsertField(lines, "Primary or Secondary Employment", "Primary");
            }
            if (!string.IsNullOrWhiteSpace(updatedInformation.SecondaryTaxCode))
            {
                UpsertField(lines, "Secondary Tax Code", updatedInformation.SecondaryTaxCode);
                RemoveField(lines, "Primary Tax Code");
                UpsertField(lines, "Primary or Secondary Employment", "Secondary");
            }
            UpsertField(lines, "Special Tax Code", updatedInformation.SpecialTaxCode);
            UpsertField(lines, "STC (Special Tax Code) Rate", updatedInformation.STCRate);
            UpsertField(lines, "STC Letter Received", updatedInformation.STCLetterReceived);
            UpsertField(lines, "STC Expiry Date", updatedInformation.STCExpiryDate);
            UpsertField(lines, "Student Loan Existence", updatedInformation.StudentLoanExistence);
            UpsertField(lines, "Student Loan SDR IRD Letter Existence", updatedInformation.StudentLoanSDRIRDLetterExistence);
            UpsertField(lines, "Student Loan Special Deduction Rate", updatedInformation.StudentLoanSpecialDeductionRate);
            UpsertField(lines, "Student Loan Repayment Exemption Existence", updatedInformation.StudentLoanRepaymentExemptionExistence);
            UpsertField(lines, "Student Loan Repayment Exemption Reason", updatedInformation.StudentLoanRepaymentExemptionReason);
            UpsertField(lines, "Student Loan Repayment Exemption Expiry Date", updatedInformation.StudentLoanRepaymentExemptionExpiryDate);
            UpsertField(lines, "Student Loan Repayment Threshold", updatedInformation.StudentLoanRepaymentThreshold);
            UpsertField(lines, "Child Support Status", updatedInformation.ChildSupportStatus);
            UpsertField(lines, "Child Support Standard Deduction", updatedInformation.ChildSupportStandardDeduction);
            UpsertField(lines, "Child Support Voluntary Deduction", updatedInformation.ChildSupportVoluntaryDeduction);
            UpsertField(lines, "Child Support Protected Net Earnings", updatedInformation.ChildSupportProtectedNetEarnings);
            UpsertField(lines, "Child Support IRD Notice of Deduction Existence", updatedInformation.ChildSupportIRDNoticeOfDeductionExistence);
            UpsertField(lines, "Child Support IRD Notice of Deduction Amount", updatedInformation.ChildSupportIRDNoticeOfDeductionAmount);
            UpsertField(lines, "Child Support IRD Notice of Deduction Effective Date", updatedInformation.ChildSupportIRDNoticeOfDeductionEffectiveDate);
            UpsertField(lines, "Child Support IRD Notice of Deduction Expiry Date", updatedInformation.ChildSupportIRDNoticeOfDeductionExpiryDate);
            UpsertField(lines, "Child Support IRD Notice of Deduction Reference Number", updatedInformation.ChildSupportIRDNoticeOfDeductionReferenceNumber);
            UpsertField(lines, "KiwiSaver Status", updatedInformation.KiwiSaverOptInOutStatus);
            UpsertField(lines, "KiwiSaver Employee Contribution Rate", updatedInformation.KiwiSaverEmployeeContributionRate);
            UpsertField(lines, "KiwiSaver Opt Out Date", updatedInformation.KiwiSaverOptOutDate);
            UpsertField(lines, "Employee Extras", updatedInformation.EmployeeExtras);
            UpsertField(lines, "Employee Extra Rate", updatedInformation.ESCTTaxRate);
            UpsertField(lines, "Employee Extra Type(s)", updatedInformation.ESCTIncomeBand);

            File.WriteAllLines(filePath, lines);
            return true;
        }

        public bool UpdateTaxInformation(string username, PayrollTaxInformation updatedInformation)
        {
            var request = new TaxInformationEditViewModel
            {
                Username = updatedInformation.Username,
                PayFrequency = updatedInformation.PayFrequency,
                PAYETableVersion = updatedInformation.PAYETableVersion,
                ExtraPayeVoluntaryDeduction = updatedInformation.ExtraPayeVoluntaryDeduction.ToString(CultureInfo.InvariantCulture),
                PrimaryTaxCode = updatedInformation.TaxCode,
                StudentLoanExistence = updatedInformation.StudentLoanStatus,
                KiwiSaverOptInOutStatus = updatedInformation.KiwiSaverOptInOutStatus,
                KiwiSaverEmployeeContributionRate = updatedInformation.KiwiSaverEmployeeContributionRate,
                KiwiSaverOptOutDate = updatedInformation.KiwiSaverOptOutDate,
                EmployeeExtras = updatedInformation.EmployeeExtras,
                ESCTTaxRate = updatedInformation.ESCTTaxRate,
                ESCTIncomeBand = updatedInformation.ESCTIncomeBand
            };

            return UpdateTaxInformation(username, request);
        }

        private static string? TryResolveIrdNumberForUsername(string username)
        {
            var usernameMappings = ResolveIrdUsernameFile();
            if (string.IsNullOrWhiteSpace(usernameMappings) || !File.Exists(usernameMappings))
            {
                return null;
            }

            foreach (var rawLine in File.ReadLines(usernameMappings))
            {
                var trimmed = rawLine.Trim();
                if (string.IsNullOrWhiteSpace(trimmed) || !trimmed.Contains(':'))
                {
                    continue;
                }

                var parts = trimmed.Split(':', 2, StringSplitOptions.TrimEntries | StringSplitOptions.RemoveEmptyEntries);
                if (parts.Length != 2)
                {
                    continue;
                }

                var recordIrdNumber = parts[0].Trim();
                var mappedUsername = parts[1].Trim();
                if (string.Equals(mappedUsername, username, StringComparison.OrdinalIgnoreCase))
                {
                    return recordIrdNumber;
                }
            }

            return null;
        }

        private static string? ResolveEmployeeDisplayName(string username)
        {
            var root = ResolveEmployeeRecordRoot();
            if (string.IsNullOrWhiteSpace(root) || !Directory.Exists(root))
            {
                return null;
            }

            foreach (var filePath in Directory.EnumerateFiles(root, "*_Employee_Record.txt", SearchOption.AllDirectories))
            {
                var parsed = ParseKeyValueFile(filePath);
                if (!parsed.TryGetValue("Username", out var recordUsername) || 
                    !string.Equals(recordUsername.Trim(), username.Trim(), StringComparison.OrdinalIgnoreCase))
                {
                    continue;
                }

                var firstName = parsed.TryGetValue("First Name", out var first) ? first.Trim() : string.Empty;
                var lastName = parsed.TryGetValue("Last Name", out var last) ? last.Trim() : string.Empty;
                if (string.IsNullOrWhiteSpace(firstName) && string.IsNullOrWhiteSpace(lastName))
                {
                    return string.Empty;
                }

                return string.Join(" ", new[] { firstName, lastName }.Where(value => !string.IsNullOrWhiteSpace(value))).Trim();
            }

            return null;
        }

        private static void EnsurePayslipArchiveLoaded()
        {
            lock (PayslipArchiveLock)
            {
                if (PayslipArchiveLoaded)
                {
                    return;
                }

                Payslips.Clear();
                var archivePath = ResolvePayslipArchiveIndexPath();
                if (File.Exists(archivePath))
                {
                    var json = File.ReadAllText(archivePath, Encoding.UTF8);
                    var archivedPayslips = JsonSerializer.Deserialize<List<PayslipRecord>>(json, PayslipArchiveJsonOptions);
                    if (archivedPayslips != null)
                    {
                        Payslips.AddRange(archivedPayslips.OrderBy(p => p.CreatedOn).ThenBy(p => p.Id));
                    }
                }

                PayslipArchiveLoaded = true;
            }
        }

        private static void SavePayslipArchive()
        {
            var archiveDirectory = ResolvePayslipArchiveDirectory();
            Directory.CreateDirectory(archiveDirectory);
            var archivePath = ResolvePayslipArchiveIndexPath();
            var orderedPayslips = Payslips.OrderBy(p => p.CreatedOn).ThenBy(p => p.Id).ToList();
            var json = JsonSerializer.Serialize(orderedPayslips, PayslipArchiveJsonOptions);
            File.WriteAllText(archivePath, json, Encoding.UTF8);
        }

        private static void SavePayslipExport(PayslipRecord record)
        {
            var exportDirectory = ResolvePayslipArchiveDirectory();
            Directory.CreateDirectory(exportDirectory);
            var exportPath = Path.Combine(exportDirectory, BuildPayslipExportFileName(record));
            File.WriteAllText(exportPath, BuildPayslipExportText(record), Encoding.UTF8);
        }

        private static string BuildPayslipExportFileName(PayslipRecord record)
        {
            var username = SanitizeFileComponent(record.Username);
            var generatedOn = record.CreatedOn == default
                ? DateTime.UtcNow
                : record.CreatedOn.ToUniversalTime();
            return $"Payslip_{record.Id:000000}_{username}_{generatedOn:yyyyMMddHHmmss}.txt";
        }

        private static string BuildPayslipExportText(PayslipRecord record)
        {
            var builder = new StringBuilder();
            builder.AppendLine("NZFTC EMS Payslip");
            builder.AppendLine($"Payslip ID: {record.Id}");
            builder.AppendLine($"Generated On: {record.CreatedOn:yyyy-MM-dd HH:mm:ss} UTC");
            builder.AppendLine($"Employee: {record.EmployeeName}");
            builder.AppendLine($"Username: {record.Username}");
            builder.AppendLine($"IRD Number: {record.IRDNumber}");
            builder.AppendLine($"Package: {record.SalaryPackageDisplayName}");
            builder.AppendLine($"Pay Period: {record.PayPeriod}");
            builder.AppendLine($"Pay Frequency: {record.PayFrequency}");
            builder.AppendLine();
            builder.AppendLine("Earnings");
            builder.AppendLine($"Base Pay: {FormatMoney(record.BasePay)}");
            builder.AppendLine($"Overtime Pay: {FormatMoney(record.OvertimePay)}");
            builder.AppendLine($"Bonus Pay: {FormatMoney(record.BonusPay)}");
            builder.AppendLine($"Allowances: {FormatMoney(record.AllowancePay)}");
            builder.AppendLine($"Gross Pay: {FormatMoney(record.GrossPay)}");
            builder.AppendLine($"Pre-tax Deductions: {FormatMoney(record.PreTaxDeductions)}");
            builder.AppendLine($"Taxable Income: {FormatMoney(record.TaxableIncome)}");
            builder.AppendLine();
            builder.AppendLine("Deductions");
            builder.AppendLine($"PAYE: {FormatMoney(record.PAYE)}");
            builder.AppendLine($"Student Loan: {FormatMoney(record.StudentLoan)}");
            builder.AppendLine($"KiwiSaver Employee: {FormatMoney(record.KiwiSaverEmployeeContribution)}");
            builder.AppendLine($"KiwiSaver Employer: {FormatMoney(record.KiwiSaverEmployerContribution)}");
            builder.AppendLine($"ESCT: {FormatMoney(record.ESCT)}");
            builder.AppendLine($"ACC Levy: {FormatMoney(record.ACCLevy)}");
            builder.AppendLine($"Post-tax Deductions: {FormatMoney(record.PostTaxDeductions)}");
            builder.AppendLine($"Net Pay: {FormatMoney(record.NetPay)}");
            builder.AppendLine();
            builder.AppendLine("Leave Snapshot");
            builder.AppendLine($"Annual Leave Remaining: {record.AnnualLeaveRemainingDays} days");
            builder.AppendLine($"Sick Leave Remaining: {record.SickLeaveRemainingDays} days");
            builder.AppendLine($"Special Leave Remaining: {record.SpecialLeaveRemainingDays} days");
            builder.AppendLine($"Parental Leave Remaining: {record.ParentalLeaveRemainingWeeks} weeks");
            builder.AppendLine($"Public Holidays: {record.PublicHolidayDays} days");
            return builder.ToString();
        }

        private static string FormatMoney(decimal value)
        {
            return "$" + value.ToString("0.00", CultureInfo.InvariantCulture);
        }

        private static string ResolvePayslipArchiveDirectory()
        {
            var root = ResolveMainSystemRoot();
            if (string.IsNullOrWhiteSpace(root))
            {
                return Path.Combine(Directory.GetCurrentDirectory(), "App_Data", "Payslips");
            }

            return Path.Combine(root, "Employee Management", "Employee_Records", "Payslips");
        }

        private static string ResolvePayslipArchiveIndexPath()
        {
            return Path.Combine(ResolvePayslipArchiveDirectory(), "NZFTC_Web_Payslips.json");
        }

        private static string SanitizeFileComponent(string value)
        {
            if (string.IsNullOrWhiteSpace(value))
            {
                return "unknown";
            }

            var invalidChars = Path.GetInvalidFileNameChars();
            var sanitized = new string(value.Select(ch => invalidChars.Contains(ch) ? '_' : ch).ToArray());
            return string.IsNullOrWhiteSpace(sanitized) ? "unknown" : sanitized.Trim();
        }

        private static IEnumerable<string> EnumerateIrdFiles()
        {
            var irdDirectory = ResolveIrdDirectory();
            if (string.IsNullOrWhiteSpace(irdDirectory) || !Directory.Exists(irdDirectory))
            {
                return Enumerable.Empty<string>();
            }

            return Directory.EnumerateFiles(irdDirectory, "*.txt", SearchOption.TopDirectoryOnly)
                .Where(path => !string.Equals(Path.GetFileName(path), "IRD_Username.txt", StringComparison.OrdinalIgnoreCase));
        }

        private static string ResolveIrdDirectory()
        {
            var root = ResolveEmployeeRecordRoot();
            if (string.IsNullOrWhiteSpace(root))
            {
                return string.Empty;
            }

            return Path.Combine(root, "IRD");
        }

        private static string ResolveIrdUsernameFile()
        {
            var irdDirectory = ResolveIrdDirectory();
            return string.IsNullOrWhiteSpace(irdDirectory) ? string.Empty : Path.Combine(irdDirectory, "IRD_Username.txt");
        }

        private static string ResolveEmployeeRecordRoot()
        {
            var root = ResolveMainSystemRoot();
            if (string.IsNullOrWhiteSpace(root))
            {
                return string.Empty;
            }

            return Path.Combine(root, "Employee Management", "Employee_Records");
        }

        private static string ResolveMainSystemRoot()
        {
            var candidates = new[]
            {
                Path.Combine(Directory.GetCurrentDirectory(), "main", "Main_System"),
                Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "main", "Main_System")),
                Path.GetFullPath(Path.Combine(AppContext.BaseDirectory, "..", "..", "..", "..", "..", "main", "Main_System"))
            };

            return candidates.FirstOrDefault(Directory.Exists) ?? string.Empty;
        }

        private static IReadOnlyDictionary<string, string> ParseKeyValueFile(string filePath)
        {
            var parsed = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
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

        private static void UpsertField(ICollection<string> lines, string fieldName, string? value)
        {
            if (string.IsNullOrWhiteSpace(value))
            {
                return;
            }

            var normalizedValue = value.Trim();
            var updatedLine = $"{fieldName}: {normalizedValue}";
            var index = -1;
            var prefix = fieldName + ":";
            var currentIndex = 0;
            foreach (var line in lines)
            {
                if (line.TrimStart().StartsWith(prefix, StringComparison.OrdinalIgnoreCase))
                {
                    index = currentIndex;
                    break;
                }

                currentIndex++;
            }

            if (index >= 0 && lines is List<string> list)
            {
                list[index] = updatedLine;
                return;
            }

            if (lines is List<string> appendable)
            {
                appendable.Add(updatedLine);
            }
        }

        private static void RemoveField(ICollection<string> lines, string fieldName)
        {
            var prefix = fieldName + ":";
            if (lines is List<string> list)
            {
                list.RemoveAll(line => line.TrimStart().StartsWith(prefix, StringComparison.OrdinalIgnoreCase));
                return;
            }

            var toRemove = lines.Where(line => line.TrimStart().StartsWith(prefix, StringComparison.OrdinalIgnoreCase)).ToList();
            foreach (var line in toRemove)
            {
                lines.Remove(line);
            }
        }

        private static string? GetFieldValue(IReadOnlyDictionary<string, string> parsed, params string[] keys)
        {
            foreach (var key in keys)
            {
                if (parsed.TryGetValue(key, out var value) && !string.IsNullOrWhiteSpace(value))
                {
                    return value.Trim();
                }
            }

            return null;
        }

        private static bool TryGetField(IReadOnlyDictionary<string, string> parsed, out string value, params string[] keys)
        {
            value = string.Empty;
            var fieldValue = GetFieldValue(parsed, keys);
            if (string.IsNullOrWhiteSpace(fieldValue))
            {
                return false;
            }

            value = fieldValue;
            return true;
        }

        private static bool IsActiveValue(string? value)
        {
            if (string.IsNullOrWhiteSpace(value))
            {
                return false;
            }

            return value.Trim().Equals("active", StringComparison.OrdinalIgnoreCase) ||
                   value.Trim().Equals("yes", StringComparison.OrdinalIgnoreCase) ||
                   value.Trim().Equals("confirmed", StringComparison.OrdinalIgnoreCase) ||
                   value.Trim().Equals("opt-in", StringComparison.OrdinalIgnoreCase);
        }

        private static bool IsYesValue(string? value)
        {
            return !string.IsNullOrWhiteSpace(value) &&
                   value.Trim().Equals("yes", StringComparison.OrdinalIgnoreCase);
        }

        private static bool IsOptInValue(string? value)
        {
            return !string.IsNullOrWhiteSpace(value) &&
                   value.Trim().Equals("opt-in", StringComparison.OrdinalIgnoreCase);
        }

        private static bool IsOptOutValue(string? value)
        {
            return !string.IsNullOrWhiteSpace(value) &&
                   value.Trim().Equals("opt-out", StringComparison.OrdinalIgnoreCase);
        }

        private static EmployerDefaults GetEmployerDefaults()
        {
            var defaults = new EmployerDefaults(3m, 1.75m, 142283m, 24128m, 12m);
            var employerDetailsPath = ResolveEmployerDetailsFile();
            if (string.IsNullOrWhiteSpace(employerDetailsPath) || !File.Exists(employerDetailsPath))
            {
                return defaults;
            }

            var parsed = ParseKeyValueFile(employerDetailsPath);
            return defaults with
            {
                KiwiSaverEmployerContributionRate = TryParsePercent(parsed, "KiwiSaver Employer Contribution Rate", defaults.KiwiSaverEmployerContributionRate),
                ACCLevyRate = TryParsePercent(parsed, "ACC Levy Rate", defaults.ACCLevyRate),
                ACCLevyMaximum = TryParseCurrency(parsed, "ACC Levy Maximum", defaults.ACCLevyMaximum),
                StudentLoanThreshold = TryParseCurrency(parsed, "Student Loan Repayment Threshold", defaults.StudentLoanThreshold),
                StudentLoanRate = TryParsePercent(parsed, "Student Loan Repayment Rate Default", defaults.StudentLoanRate)
            };
        }

        private static decimal TryParsePercent(IReadOnlyDictionary<string, string> parsed, string key, decimal fallback)
        {
            if (!parsed.TryGetValue(key, out var rawValue) || string.IsNullOrWhiteSpace(rawValue))
            {
                return fallback;
            }

            var cleaned = rawValue.Replace("(Default)", string.Empty, StringComparison.OrdinalIgnoreCase)
                .Replace("%", string.Empty, StringComparison.OrdinalIgnoreCase)
                .Trim();
            return decimal.TryParse(cleaned, NumberStyles.Number, CultureInfo.InvariantCulture, out var value) ? value : fallback;
        }

        private static decimal TryParseCurrency(IReadOnlyDictionary<string, string> parsed, string key, decimal fallback)
        {
            if (!parsed.TryGetValue(key, out var rawValue) || string.IsNullOrWhiteSpace(rawValue))
            {
                return fallback;
            }

            var cleaned = rawValue.Replace("$", string.Empty, StringComparison.OrdinalIgnoreCase)
                .Replace(",", string.Empty, StringComparison.OrdinalIgnoreCase)
                .Trim();
            return decimal.TryParse(cleaned, NumberStyles.Number, CultureInfo.InvariantCulture, out var value) ? value : fallback;
        }

        private static string ResolveEmployerDetailsFile()
        {
            var root = ResolveMainSystemRoot();
            if (string.IsNullOrWhiteSpace(root))
            {
                return string.Empty;
            }

            return Path.Combine(root, "Employer_Details.txt");
        }

        private static decimal CalculatePaye(decimal taxableIncome, PayrollTaxInformation taxInfo, string payFrequency)
        {
            var resolvedTaxCode = ResolveTaxCodeForPayslip(taxInfo);
            if (TryGetFlatTaxRate(resolvedTaxCode, out var flatRate))
            {
                return RoundMoney(taxableIncome * flatRate);
            }

            if (IsSpecialTaxCode(taxInfo) && TryParsePercent(taxInfo.SpecialTaxCodeRate, out var specialRate))
            {
                return RoundMoney(taxableIncome * specialRate);
            }

            var normalizedVersion = string.IsNullOrWhiteSpace(taxInfo.PAYETableVersion) ? "2025" : taxInfo.PAYETableVersion.Trim();
            var periodsPerYear = GetPeriodsPerYear(payFrequency);
            var annualIncome = taxableIncome * periodsPerYear;

            if (annualIncome <= 0m)
            {
                return 0m;
            }

            var brackets = normalizedVersion.StartsWith("2024", StringComparison.OrdinalIgnoreCase)
                ? new[] { (0m, 0m, 0.105m), (15600m, 15600m, 0.175m), (53500m, 53500m, 0.30m), (78100m, 78100m, 0.33m), (180000m, 180000m, 0.39m) }
                : new[] { (0m, 0m, 0.105m), (15600m, 15600m, 0.175m), (53500m, 53500m, 0.30m), (78100m, 78100m, 0.33m), (180000m, 180000m, 0.39m) };

            decimal total = 0m;
            decimal previousLimit = 0m;

            foreach (var (threshold, _, rate) in brackets)
            {
                if (threshold == 0m)
                {
                    previousLimit = 0m;
                    continue;
                }

                var taxableAtThisBand = Math.Min(annualIncome, threshold) - previousLimit;
                if (taxableAtThisBand > 0m)
                {
                    total += taxableAtThisBand * rate;
                }

                previousLimit = threshold;
            }

            if (annualIncome > 180000m)
            {
                total += (annualIncome - 180000m) * 0.39m;
            }

            return RoundMoney(total / periodsPerYear);
        }

            private static decimal CalculateStudentLoan(decimal taxableIncome, PayrollTaxInformation taxInfo, string payFrequency)
        {
                if (!ShouldDeductStudentLoan(taxInfo))
                {
                    return 0m;
                }

                var defaults = GetEmployerDefaults();
                var threshold = TryParseCurrencyValue(taxInfo.StudentLoanRepaymentThreshold, defaults.StudentLoanThreshold);
                var ratePercent = TryParsePercentValue(taxInfo.StudentLoanSpecialDeductionRate, defaults.StudentLoanRate);
                var rate = ratePercent / 100m;
                var periodsPerYear = GetPeriodsPerYear(payFrequency);
                var annualIncome = taxableIncome * periodsPerYear;
                if (annualIncome <= threshold)
                {
                    return 0m;
                }

                return RoundMoney(((annualIncome - threshold) * rate) / periodsPerYear);
            }

            private static decimal CalculateKiwiSaverEmployeeContribution(decimal taxableIncome, PayrollTaxInformation taxInfo)
            {
                if (taxInfo == null || taxableIncome <= 0m || !IsOptInValue(taxInfo.KiwiSaverOptInOutStatus))
                {
                    return 0m;
                }

                var ratePercent = TryParsePercentValue(taxInfo.KiwiSaverEmployeeContributionRate, 3m);
                return RoundMoney(taxableIncome * (ratePercent / 100m));
            }

            private static decimal CalculateKiwiSaverEmployerContribution(decimal taxableIncome, PayrollTaxInformation taxInfo, LeaveEntitlement leaveEntitlement)
            {
                if (taxInfo == null || taxableIncome <= 0m || IsOptOutValue(taxInfo.KiwiSaverOptInOutStatus))
                {
                    return 0m;
                }

                var leaveRate = leaveEntitlement?.KiwiSaverEmployerContribution > 0
                    ? leaveEntitlement.KiwiSaverEmployerContribution
                    : 0;
                var employerRate = leaveRate > 0
                    ? leaveRate
                    : Math.Max(0m, taxInfo.KiwiSaverEmployerContributionRate);

                return RoundMoney(taxableIncome * (employerRate / 100m));
            }

            private static decimal CalculateEsct(decimal taxableIncome, PayrollTaxInformation taxInfo, LeaveEntitlement leaveEntitlement, decimal kiwiSaverEmployerContribution)
            {
                if (taxInfo == null || taxableIncome <= 0m)
                {
                    return 0m;
                }

                var superannuationContributionRate = leaveEntitlement?.SuperannuationContribution > 0
                    ? leaveEntitlement.SuperannuationContribution
                    : 0;
                var superannuationContribution = superannuationContributionRate > 0
                    ? RoundMoney(taxableIncome * (superannuationContributionRate / 100m))
                    : 0m;

                var taxableContributionBase = kiwiSaverEmployerContribution + superannuationContribution;
                if (taxableContributionBase <= 0m)
                {
                    return 0m;
                }

                var esctRatePercent = TryParsePercentValue(taxInfo.ESCTTaxRate, 0m);
                if (esctRatePercent <= 0m)
                {
                    return 0m;
                }

                return RoundMoney(taxableContributionBase * (esctRatePercent / 100m));
            }

            private static decimal CalculateAccLevy(string username, decimal taxableIncome, PayrollTaxInformation taxInfo, DateTime payDateUtc)
            {
                if (taxInfo == null || taxableIncome <= 0m)
                {
                    return 0m;
                }

                var defaults = GetEmployerDefaults();
                var ratePercent = Math.Max(0m, taxInfo.ACCLevyRate);
                var annualEarningsCap = defaults.ACCLevyMaximum;
                var taxYearStart = GetNewZealandTaxYearStart(payDateUtc);

                var priorPayslips = Payslips
                    .Where(p => string.Equals(p.Username, username, StringComparison.OrdinalIgnoreCase)
                        && p.CreatedOn >= taxYearStart
                        && p.CreatedOn < payDateUtc)
                    .ToList();

                var priorLiableEarnings = priorPayslips.Sum(p => p.TaxableIncome);
                var priorAccLevy = priorPayslips.Sum(p => p.ACCLevy);
                var currentYtdLiableEarnings = Math.Max(0m, priorLiableEarnings + taxableIncome);
                var ytdCappedEarnings = Math.Min(currentYtdLiableEarnings, annualEarningsCap);
                var ytdLiability = RoundMoney(ytdCappedEarnings * (ratePercent / 100m));
                var currentLevy = Math.Max(0m, ytdLiability - priorAccLevy);
                return RoundMoney(currentLevy);
            }

            private static DateTime GetNewZealandTaxYearStart(DateTime dateUtc)
            {
                var date = dateUtc.Date;
                var year = date.Month >= 4 ? date.Year : date.Year - 1;
                return new DateTime(year, 4, 1, 0, 0, 0, DateTimeKind.Utc);
            }

            private static decimal GetPeriodsPerYear(string payFrequency)
            {
                return payFrequency.Trim().ToLowerInvariant() switch
                {
                    "weekly" => 52m,
                    "monthly" => 12m,
                    _ => 26m
                };
            }

            private static decimal NormalizeMoney(decimal value)
            {
                    return RoundMoney(Math.Max(0m, value));
            }

            private static decimal RoundMoney(decimal value)
            {
                    return Math.Round(value, 2, MidpointRounding.AwayFromZero);
            }

            private static bool TryParsePercent(string? value, out decimal percent)
            {
                percent = 0m;
                if (string.IsNullOrWhiteSpace(value))
                {
                    return false;
                }

                var cleaned = value.Replace("%", string.Empty, StringComparison.OrdinalIgnoreCase).Trim();
                return decimal.TryParse(cleaned, NumberStyles.Number, CultureInfo.InvariantCulture, out percent);
            }

            private static decimal TryParsePercentValue(string? value, decimal fallback)
            {
                return TryParsePercent(value, out var percent) ? percent : fallback;
            }

            private static decimal TryParseCurrencyValue(string? value, decimal fallback)
            {
                if (string.IsNullOrWhiteSpace(value))
                {
                    return fallback;
                }

                var cleaned = value.Replace("$", string.Empty, StringComparison.OrdinalIgnoreCase)
                    .Replace(",", string.Empty, StringComparison.OrdinalIgnoreCase)
                    .Trim();

                return decimal.TryParse(cleaned, NumberStyles.Number, CultureInfo.InvariantCulture, out var parsed)
                    ? parsed
                    : fallback;
            }

            private static bool ShouldDeductStudentLoan(PayrollTaxInformation taxInfo)
            {
                if (taxInfo == null)
                {
                    return false;
                }

                var status = taxInfo.StudentLoanStatus?.Trim() ?? string.Empty;
                var active = status.Equals("yes", StringComparison.OrdinalIgnoreCase)
                    || status.Equals("active", StringComparison.OrdinalIgnoreCase)
                    || status.Equals("in repayment", StringComparison.OrdinalIgnoreCase)
                    || status.Equals("repayment", StringComparison.OrdinalIgnoreCase)
                    || status.Equals("required", StringComparison.OrdinalIgnoreCase);

                if (!active)
                {
                    return false;
                }

                var exemptionFlag = taxInfo.StudentLoanRepaymentExemptionExistence?.Trim() ?? string.Empty;
                if (exemptionFlag.Equals("yes", StringComparison.OrdinalIgnoreCase)
                    || exemptionFlag.Equals("active", StringComparison.OrdinalIgnoreCase))
                {
                    if (DateTime.TryParse(taxInfo.StudentLoanRepaymentExemptionExpiryDate, CultureInfo.InvariantCulture, DateTimeStyles.AssumeLocal, out var expiry)
                        && expiry.Date >= DateTime.Today)
                    {
                        return false;
                    }
                }

                return true;
            }

            private static bool IsSpecialTaxCode(PayrollTaxInformation taxInfo)
            {
                var code = NormalizeTaxCode(taxInfo.SpecialTaxCode);
                return !string.IsNullOrWhiteSpace(code) && code.StartsWith("ST", StringComparison.OrdinalIgnoreCase);
            }

            private static string ResolveTaxCodeForPayslip(PayrollTaxInformation taxInfo)
            {
                if (taxInfo == null)
                {
                    return string.Empty;
                }

                if (IsSpecialTaxCode(taxInfo))
                {
                    return NormalizeTaxCode(taxInfo.SpecialTaxCode);
                }

                var jobType = taxInfo.TaxCodeJobType?.Trim() ?? string.Empty;
                if (jobType.Equals("Secondary", StringComparison.OrdinalIgnoreCase) && !string.IsNullOrWhiteSpace(taxInfo.SecondaryTaxCode))
                {
                    return NormalizeTaxCode(taxInfo.SecondaryTaxCode);
                }

                if (!string.IsNullOrWhiteSpace(taxInfo.PrimaryTaxCode))
                {
                    return NormalizeTaxCode(taxInfo.PrimaryTaxCode);
                }

                return NormalizeTaxCode(taxInfo.TaxCode);
            }

            private static string DescribeTaxCode(PayrollTaxInformation taxInfo)
            {
                var code = ResolveTaxCodeForPayslip(taxInfo);
                if (string.IsNullOrWhiteSpace(code))
                {
                    return "Unspecified tax code";
                }

                if (IsSpecialTaxCode(taxInfo) && TryParsePercent(taxInfo.SpecialTaxCodeRate, out var specialRate))
                {
                    return $"Special tax code at {specialRate:0.##}%";
                }

                return code switch
                {
                    "M" => "Primary employment",
                    "ME" => "Primary employment with student loan",
                    "SB" => "Secondary employment (10.5%)",
                    "S" => "Secondary employment (17.5%)",
                    "SH" => "Secondary employment (30%)",
                    "ST" => "Secondary employment (33%)",
                    "SA" => "Secondary employment (39%)",
                    _ => $"Tax code {code}"
                };
            }

            private static bool TryGetFlatTaxRate(string taxCode, out decimal rate)
            {
                rate = 0m;
                var normalized = NormalizeTaxCode(taxCode);
                if (string.IsNullOrWhiteSpace(normalized))
                {
                    return false;
                }

                rate = normalized switch
                {
                    "SB" => 0.105m,
                    "S" => 0.175m,
                    "SH" => 0.30m,
                    "ST" => 0.33m,
                    "SA" => 0.39m,
                    _ => 0m
                };

                return rate > 0m;
            }

            private static string NormalizeTaxCode(string? taxCode)
            {
                if (string.IsNullOrWhiteSpace(taxCode))
                {
                    return string.Empty;
                }

                return new string(taxCode.Where(char.IsLetterOrDigit).ToArray()).ToUpperInvariant();
            }

            private sealed record EmployerDefaults(
                decimal KiwiSaverEmployerContributionRate,
                decimal ACCLevyRate,
                decimal ACCLevyMaximum,
                decimal StudentLoanThreshold,
                decimal StudentLoanRate);
        }
}
