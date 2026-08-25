using System;

namespace NZFTC_EMS.Models
{
    public class PayrollTaxInformation
    {
        public string Username { get; set; } = string.Empty;
        public string EmployeeName { get; set; } = string.Empty;
        public string IRDNumber { get; set; } = string.Empty;
        public string IRDConfirmationStatus { get; set; } = "Unconfirmed";
        public string PayFrequency { get; set; } = "Fortnightly";
        public string PAYETableVersion { get; set; } = "2025";
        public decimal ExtraPayeVoluntaryDeduction { get; set; }
        public string TaxCode { get; set; } = "M";
        public string TaxCodeDescription { get; set; } = "Standard tax code";
        public string TaxCodeJobType { get; set; } = "Primary";
        public string PrimaryTaxCode { get; set; } = "M";
        public string SecondaryTaxCode { get; set; } = string.Empty;
        public string SpecialTaxCode { get; set; } = string.Empty;
        public string SpecialTaxCodeRate { get; set; } = string.Empty;
        public string KiwiSaverOptInOutStatus { get; set; } = "opt-in";
        public string KiwiSaverEmployeeContributionRate { get; set; } = "3%";
        public string KiwiSaverOptOutDate { get; set; } = string.Empty;
        public string StudentLoanStatus { get; set; } = "Not in repayment";
        public string StudentLoanExistence { get; set; } = "Yes";
        public string StudentLoanSDRIRDLetterExistence { get; set; } = string.Empty;
        public string StudentLoanSpecialDeductionRate { get; set; } = string.Empty;
        public string StudentLoanRepaymentExemptionExistence { get; set; } = string.Empty;
        public string StudentLoanRepaymentExemptionReason { get; set; } = string.Empty;
        public string StudentLoanRepaymentExemptionExpiryDate { get; set; } = string.Empty;
        public string StudentLoanRepaymentThreshold { get; set; } = string.Empty;
        public string EmployeeExtras { get; set; } = string.Empty;
        public string ESCTTaxRate { get; set; } = string.Empty;
        public string ESCTIncomeBand { get; set; } = string.Empty;
        public decimal StudentLoanRate { get; set; } = 12m;
        public decimal KiwiSaverEmployerContributionRate { get; set; } = 3m;
        public decimal ACCLevyRate { get; set; } = 1.75m;
        public string EmployeeType { get; set; } = "Salary";
        public bool IsConfigured => !string.IsNullOrWhiteSpace(IRDNumber);
    }

    public class TaxInformationEditViewModel
    {
        public string Username { get; set; } = string.Empty;
        public string EmployeeName { get; set; } = string.Empty;
        public string IRDNumber { get; set; } = string.Empty;

        public string? PayFrequency { get; set; }
        public string? PAYETableVersion { get; set; }
        public string? ExtraPayeVoluntaryDeduction { get; set; }
        public string? TaxCodeJobType { get; set; }
        public string? PrimaryTaxCode { get; set; }
        public string? SecondaryTaxCode { get; set; }
        public string? SpecialTaxCode { get; set; }
        public string? STCRate { get; set; }
        public string? STCLetterReceived { get; set; }
        public string? STCExpiryDate { get; set; }
        public string? StudentLoanExistence { get; set; }
        public string? StudentLoanSDRIRDLetterExistence { get; set; }
        public string? StudentLoanSpecialDeductionRate { get; set; }
        public string? StudentLoanRepaymentExemptionExistence { get; set; }
        public string? StudentLoanRepaymentExemptionReason { get; set; }
        public string? StudentLoanRepaymentExemptionExpiryDate { get; set; }
        public string? StudentLoanRepaymentThreshold { get; set; }
        public string? ChildSupportStatus { get; set; }
        public string? ChildSupportStandardDeduction { get; set; }
        public string? ChildSupportVoluntaryDeduction { get; set; }
        public string? ChildSupportProtectedNetEarnings { get; set; }
        public string? ChildSupportIRDNoticeOfDeductionExistence { get; set; }
        public string? ChildSupportIRDNoticeOfDeductionAmount { get; set; }
        public string? ChildSupportIRDNoticeOfDeductionEffectiveDate { get; set; }
        public string? ChildSupportIRDNoticeOfDeductionExpiryDate { get; set; }
        public string? ChildSupportIRDNoticeOfDeductionReferenceNumber { get; set; }
        public string? KiwiSaverOptInOutStatus { get; set; }
        public string? KiwiSaverEmployeeContributionRate { get; set; }
        public string? KiwiSaverOptOutDate { get; set; }
        public string? EmployeeExtras { get; set; }
        public string? ESCTTaxRate { get; set; }
        public string? ESCTIncomeBand { get; set; }

        public bool ShowStudentLoanOptions { get; set; }
        public bool SpecialTaxCodeActive { get; set; }
        public bool StudentLoanActive { get; set; }
        public bool ChildSupportActive { get; set; }
        public bool KiwiSaverActive { get; set; }
        public bool KiwiSaverOptedOut { get; set; }
        public bool ESCTActive { get; set; }
    }

    public class PayslipRecord
    {
        public int Id { get; set; }
        public string Username { get; set; } = string.Empty;
        public string EmployeeName { get; set; } = string.Empty;
        public string SalaryPackageName { get; set; } = string.Empty;
        public string SalaryPackageDisplayName { get; set; } = string.Empty;
        public string IRDNumber { get; set; } = string.Empty;
        public string PayPeriod { get; set; } = string.Empty;
        public string PayFrequency { get; set; } = "Fortnightly";
        public decimal BasePay { get; set; }
        public decimal OvertimePay { get; set; }
        public decimal BonusPay { get; set; }
        public decimal AllowancePay { get; set; }
        public decimal PreTaxDeductions { get; set; }
        public decimal GrossPay { get; set; }
        public decimal TaxableIncome { get; set; }
        public decimal PAYE { get; set; }
        public decimal StudentLoan { get; set; }
        public decimal KiwiSaverEmployeeContribution { get; set; }
        public decimal KiwiSaverEmployerContribution { get; set; }
        public decimal KiwiSaver => KiwiSaverEmployeeContribution;
        public decimal ESCT { get; set; }
        public decimal ACCLevy { get; set; }
        public decimal PostTaxDeductions { get; set; }
        public decimal NetPay { get; set; }
        public int AnnualLeaveLawfulDays { get; set; }
        public int AnnualLeavePackageExtraDays { get; set; }
        public int AnnualLeaveTakenDays { get; set; }
        public int AnnualLeaveScheduledDays { get; set; }
        public int AnnualLeaveRemainingDays { get; set; }
        public int SickLeaveLawfulDays { get; set; }
        public int SickLeavePackageExtraDays { get; set; }
        public int SickLeaveTakenDays { get; set; }
        public int SickLeaveScheduledDays { get; set; }
        public int SickLeaveRemainingDays { get; set; }
        public int SpecialLeaveLawfulDays { get; set; }
        public int SpecialLeavePackageExtraDays { get; set; }
        public int SpecialLeaveTakenDays { get; set; }
        public int SpecialLeaveScheduledDays { get; set; }
        public int SpecialLeaveRemainingDays { get; set; }
        public int ParentalLeaveLawfulWeeks { get; set; }
        public int ParentalLeavePackageExtraWeeks { get; set; }
        public int ParentalLeaveRemainingWeeks { get; set; }
        public int PublicHolidayDays { get; set; }
        public int PublicHolidayPackageExtraDays { get; set; }
        public DateTime CreatedOn { get; set; } = DateTime.UtcNow;
    }
}
