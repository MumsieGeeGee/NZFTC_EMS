using System;

namespace NZFTC_EMS.Models
{
    public class LeaveRequest
    {
        public int Id { get; set; }
        public string SubmittedByUsername { get; set; } = string.Empty;
        public string SubmittedForUsername { get; set; } = string.Empty;
        public string EmployeeName { get; set; } = string.Empty;
        public string LeaveType { get; set; } = string.Empty;
        public DateTime StartDate { get; set; }
        public DateTime EndDate { get; set; }
        public string Reason { get; set; } = string.Empty;
        public string Status { get; set; } = string.Empty;
        public string DecisionReason { get; set; } = string.Empty;
        public DateTime RequestedOn { get; set; }
        public DateTime? UpdatedOn { get; set; }
        public string HandledByUsername { get; set; } = string.Empty;

        public int TotalDays => Math.Max(1, (EndDate.Date - StartDate.Date).Days + 1);
    }

    public class LeaveEntitlement
    {
        public string Username { get; set; } = string.Empty;
        public string BusinessRole { get; set; } = string.Empty;
        public string JobRole { get; set; } = string.Empty;
        public string SalaryPackageName { get; set; } = string.Empty;
        public string SalaryPackageDisplayName { get; set; } = string.Empty;
        public string ResolutionMessage { get; set; } = string.Empty;
        public bool HasResolvedPackage { get; set; }
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
    }
}
