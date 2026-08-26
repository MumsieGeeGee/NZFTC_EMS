using System;

namespace NZFTC_EMS.Data.Entities
{
    public class EmployeeEntity
    {
        public string Username { get; set; } = string.Empty;
        public string FirstName { get; set; } = string.Empty;
        public string LastName { get; set; } = string.Empty;
        public string BusinessRole { get; set; } = string.Empty;
        public string JobRole { get; set; } = string.Empty;
        public string AccountStatus { get; set; } = "Active";
        public string Email { get; set; } = string.Empty;
        public string Phone { get; set; } = string.Empty;
        public DateTime CreatedOnUtc { get; set; } = DateTime.UtcNow;
        public DateTime UpdatedOnUtc { get; set; } = DateTime.UtcNow;
    }

    public class EmployeeAccountDetailsEntity
    {
        public string Username { get; set; } = string.Empty;
        public string EmergencyContactEncrypted { get; set; } = string.Empty;
        public string DiagnosedHealthConditionsEncrypted { get; set; } = string.Empty;
        public string AllergiesEncrypted { get; set; } = string.Empty;
        public string ApprovedEmergencyStepsEncrypted { get; set; } = string.Empty;
        public string PriorTrainingEncrypted { get; set; } = string.Empty;
        public string FirstAidCertificationEncrypted { get; set; } = string.Empty;
        public string GroupName { get; set; } = string.Empty;
        public string RecordSnapshotJson { get; set; } = string.Empty;
        public DateTime CreatedOnUtc { get; set; } = DateTime.UtcNow;
        public DateTime UpdatedOnUtc { get; set; } = DateTime.UtcNow;
    }

    public class EmployeeTaxInformationEntity
    {
        public string Username { get; set; } = string.Empty;
        public string IrdNumber { get; set; } = string.Empty;
        public string IrdConfirmationStatus { get; set; } = "Unconfirmed";
        public string PayFrequency { get; set; } = "Fortnightly";
        public string PayeTableVersion { get; set; } = "2025";
        public decimal ExtraPayeVoluntaryDeduction { get; set; }
        public string TaxCode { get; set; } = "M";
        public string TaxCodeDescription { get; set; } = "Standard tax code";
        public string TaxCodeJobType { get; set; } = "Primary";
        public string PrimaryTaxCode { get; set; } = "M";
        public string SecondaryTaxCode { get; set; } = string.Empty;
        public string SpecialTaxCode { get; set; } = string.Empty;
        public string SpecialTaxCodeRate { get; set; } = string.Empty;
        public string STCLetterReceived { get; set; } = string.Empty;
        public string STCExpiryDate { get; set; } = string.Empty;
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
        public string ChildSupportStatus { get; set; } = string.Empty;
        public string ChildSupportStandardDeduction { get; set; } = string.Empty;
        public string ChildSupportVoluntaryDeduction { get; set; } = string.Empty;
        public string ChildSupportProtectedNetEarnings { get; set; } = string.Empty;
        public string ChildSupportIRDNoticeOfDeductionExistence { get; set; } = string.Empty;
        public string ChildSupportIRDNoticeOfDeductionAmount { get; set; } = string.Empty;
        public string ChildSupportIRDNoticeOfDeductionEffectiveDate { get; set; } = string.Empty;
        public string ChildSupportIRDNoticeOfDeductionExpiryDate { get; set; } = string.Empty;
        public string ChildSupportIRDNoticeOfDeductionReferenceNumber { get; set; } = string.Empty;
        public string EmployeeExtras { get; set; } = string.Empty;
        public string ESCTTaxRate { get; set; } = string.Empty;
        public string ESCTIncomeBand { get; set; } = string.Empty;
        public decimal StudentLoanRate { get; set; }
        public decimal KiwiSaverEmployerContributionRate { get; set; }
        public decimal ACCLevyRate { get; set; }
        public string EmployeeType { get; set; } = "Salary";
        public DateTime CreatedOnUtc { get; set; } = DateTime.UtcNow;
        public DateTime UpdatedOnUtc { get; set; } = DateTime.UtcNow;
    }

    public class LeaveEntitlementEntity
    {
        public string Username { get; set; } = string.Empty;
        public string BusinessRole { get; set; } = string.Empty;
        public string JobRole { get; set; } = string.Empty;
        public string SalaryPackageName { get; set; } = string.Empty;
        public string SalaryPackageDisplayName { get; set; } = string.Empty;
        public int ApproximateAnnualSalary { get; set; }
        public int KiwiSaverEmployerContribution { get; set; }
        public int AnnualLeaveLawfulDays { get; set; }
        public int AnnualLeavePackageExtraDays { get; set; }
        public int AnnualLeaveDays { get; set; }
        public int AnnualLeaveTakenDays { get; set; }
        public int AnnualLeaveScheduledDays { get; set; }
        public int SickLeaveDays { get; set; }
        public int SickLeaveLawfulDays { get; set; }
        public int SickLeavePackageExtraDays { get; set; }
        public int SickLeaveTakenDays { get; set; }
        public int SickLeaveScheduledDays { get; set; }
        public int PublicHolidayDays { get; set; }
        public int PublicHolidayLawfulDays { get; set; }
        public int PublicHolidayPackageExtraDays { get; set; }
        public int SpecialLeaveDays { get; set; }
        public int SpecialLeaveLawfulDays { get; set; }
        public int SpecialLeavePackageExtraDays { get; set; }
        public int SpecialLeaveTakenDays { get; set; }
        public int SpecialLeaveScheduledDays { get; set; }
        public int ParentalLeaveWeeks { get; set; }
        public int ParentalLeaveLawfulWeeks { get; set; }
        public int ParentalLeavePackageExtraWeeks { get; set; }
        public int ParentalLeaveTakenWeeks { get; set; }
        public int ParentalLeaveScheduledWeeks { get; set; }
        public int SuperannuationContribution { get; set; }
        public int HealthInsuranceContribution { get; set; }
        public int ProfessionalDevelopmentContribution { get; set; }
        public int EmployeeAssistanceProgramContribution { get; set; }
        public int RemainingAnnualLeaveDays { get; set; }
        public int RemainingSickLeaveDays { get; set; }
        public int RemainingSpecialLeaveDays { get; set; }
        public int RemainingParentalLeaveWeeks { get; set; }
        public DateTime CreatedOnUtc { get; set; } = DateTime.UtcNow;
        public DateTime UpdatedOnUtc { get; set; } = DateTime.UtcNow;
    }

    public class LeaveRequestEntity
    {
        public long Id { get; set; }
        public string SubmittedByUsername { get; set; } = string.Empty;
        public string SubmittedForUsername { get; set; } = string.Empty;
        public string EmployeeName { get; set; } = string.Empty;
        public string LeaveType { get; set; } = string.Empty;
        public DateTime StartDate { get; set; }
        public DateTime EndDate { get; set; }
        public string ReasonEncrypted { get; set; } = string.Empty;
        public string Status { get; set; } = string.Empty;
        public string DecisionReasonEncrypted { get; set; } = string.Empty;
        public DateTime RequestedOnUtc { get; set; } = DateTime.UtcNow;
        public DateTime? UpdatedOnUtc { get; set; }
        public string HandledByUsername { get; set; } = string.Empty;
    }

    public class GrievanceRequestEntity
    {
        public long Id { get; set; }
        public string SubmittedByUsername { get; set; } = string.Empty;
        public string SubmittedForUsername { get; set; } = string.Empty;
        public string EmployeeName { get; set; } = string.Empty;
        public string Subject { get; set; } = string.Empty;
        public string DescriptionEncrypted { get; set; } = string.Empty;
        public string Category { get; set; } = "General";
        public string Severity { get; set; } = "Medium";
        public string Status { get; set; } = "Open";
        public string OutcomeEncrypted { get; set; } = string.Empty;
        public string DecisionReasonEncrypted { get; set; } = string.Empty;
        public DateTime SubmittedOnUtc { get; set; } = DateTime.UtcNow;
        public DateTime? UpdatedOnUtc { get; set; }
        public string CreatedByRole { get; set; } = "Employee";
        public string NotificationGroup { get; set; } = "Employee";
        public string HandledByUsername { get; set; } = string.Empty;
    }

    public class PayslipEntity
    {
        public long Id { get; set; }
        public string Username { get; set; } = string.Empty;
        public string EmployeeName { get; set; } = string.Empty;
        public string SalaryPackageName { get; set; } = string.Empty;
        public string SalaryPackageDisplayName { get; set; } = string.Empty;
        public string IrdNumber { get; set; } = string.Empty;
        public string PayPeriod { get; set; } = string.Empty;
        public string PayFrequency { get; set; } = "Fortnightly";
        public decimal BasePay { get; set; }
        public decimal OvertimePay { get; set; }
        public decimal BonusPay { get; set; }
        public decimal AllowancePay { get; set; }
        public decimal PreTaxDeductions { get; set; }
        public decimal GrossPay { get; set; }
        public decimal TaxableIncome { get; set; }
        public decimal Paye { get; set; }
        public decimal StudentLoan { get; set; }
        public decimal KiwiSaverEmployeeContribution { get; set; }
        public decimal KiwiSaverEmployerContribution { get; set; }
        public decimal Esct { get; set; }
        public decimal AccLevy { get; set; }
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
        public DateTime CreatedOnUtc { get; set; } = DateTime.UtcNow;
    }

    public class SessionRecordEntity
    {
        public string SessionId { get; set; } = string.Empty;
        public string Username { get; set; } = string.Empty;
        public string AccountType { get; set; } = string.Empty;
        public string AccessProfileJson { get; set; } = string.Empty;
        public DateTime CreatedOnUtc { get; set; } = DateTime.UtcNow;
        public DateTime LastSeenOnUtc { get; set; } = DateTime.UtcNow;
        public DateTime ExpiresOnUtc { get; set; }
        public bool IsActive { get; set; } = true;
    }

    public class AuditEventEntity
    {
        public long Id { get; set; }
        public string EntityType { get; set; } = string.Empty;
        public string EntityKey { get; set; } = string.Empty;
        public string ActionType { get; set; } = string.Empty;
        public string ActorUsername { get; set; } = string.Empty;
        public string DetailsJson { get; set; } = string.Empty;
        public DateTime OccurredOnUtc { get; set; } = DateTime.UtcNow;
    }
}
