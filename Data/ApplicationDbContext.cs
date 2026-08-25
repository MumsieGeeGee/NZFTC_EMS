using Microsoft.EntityFrameworkCore;
using NZFTC_EMS.Data.Entities;

namespace NZFTC_EMS.Data
{
    public class ApplicationDbContext : DbContext
    {
        public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options)
            : base(options)
        {
        }

        public DbSet<EmployeeEntity> Employees => Set<EmployeeEntity>();
        public DbSet<EmployeeAccountDetailsEntity> EmployeeAccountDetails => Set<EmployeeAccountDetailsEntity>();
        public DbSet<EmployeeTaxInformationEntity> EmployeeTaxInformation => Set<EmployeeTaxInformationEntity>();
        public DbSet<LeaveEntitlementEntity> LeaveEntitlements => Set<LeaveEntitlementEntity>();
        public DbSet<LeaveRequestEntity> LeaveRequests => Set<LeaveRequestEntity>();
        public DbSet<GrievanceRequestEntity> GrievanceRequests => Set<GrievanceRequestEntity>();
        public DbSet<PayslipEntity> Payslips => Set<PayslipEntity>();
        public DbSet<SessionRecordEntity> SessionRecords => Set<SessionRecordEntity>();
        public DbSet<AuditEventEntity> AuditEvents => Set<AuditEventEntity>();

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<EmployeeEntity>(entity =>
            {
                entity.ToTable("employees");
                entity.HasKey(x => x.Username);
            });

            modelBuilder.Entity<EmployeeAccountDetailsEntity>(entity =>
            {
                entity.ToTable("employee_account_details");
                entity.HasKey(x => x.Username);
                entity.HasOne<EmployeeEntity>()
                    .WithOne()
                    .HasForeignKey<EmployeeAccountDetailsEntity>(x => x.Username)
                    .OnDelete(DeleteBehavior.Cascade);
            });

            modelBuilder.Entity<EmployeeTaxInformationEntity>(entity =>
            {
                entity.ToTable("employee_tax_information");
                entity.HasKey(x => x.Username);
                entity.HasIndex(x => x.IrdNumber).IsUnique();
                entity.HasOne<EmployeeEntity>()
                    .WithOne()
                    .HasForeignKey<EmployeeTaxInformationEntity>(x => x.Username)
                    .OnDelete(DeleteBehavior.Cascade);
            });

            modelBuilder.Entity<LeaveEntitlementEntity>(entity =>
            {
                entity.ToTable("leave_entitlements");
                entity.HasKey(x => x.Username);
                entity.HasOne<EmployeeEntity>()
                    .WithOne()
                    .HasForeignKey<LeaveEntitlementEntity>(x => x.Username)
                    .OnDelete(DeleteBehavior.Cascade);
            });

            modelBuilder.Entity<LeaveRequestEntity>(entity =>
            {
                entity.ToTable("leave_requests");
                entity.HasKey(x => x.Id);
                entity.HasIndex(x => new { x.SubmittedForUsername, x.Status });
                entity.HasIndex(x => new { x.Status, x.RequestedOnUtc });
                entity.HasIndex(x => new { x.StartDate, x.EndDate });
                entity.HasOne<EmployeeEntity>()
                    .WithMany()
                    .HasForeignKey(x => x.SubmittedByUsername)
                    .OnDelete(DeleteBehavior.Restrict);
                entity.HasOne<EmployeeEntity>()
                    .WithMany()
                    .HasForeignKey(x => x.SubmittedForUsername)
                    .OnDelete(DeleteBehavior.Restrict);
            });

            modelBuilder.Entity<GrievanceRequestEntity>(entity =>
            {
                entity.ToTable("grievance_requests");
                entity.HasKey(x => x.Id);
                entity.HasIndex(x => new { x.SubmittedForUsername, x.Status });
                entity.HasIndex(x => new { x.Status, x.SubmittedOnUtc });
                entity.HasOne<EmployeeEntity>()
                    .WithMany()
                    .HasForeignKey(x => x.SubmittedByUsername)
                    .OnDelete(DeleteBehavior.Restrict);
                entity.HasOne<EmployeeEntity>()
                    .WithMany()
                    .HasForeignKey(x => x.SubmittedForUsername)
                    .OnDelete(DeleteBehavior.Restrict);
            });

            modelBuilder.Entity<PayslipEntity>(entity =>
            {
                entity.ToTable("payslips");
                entity.HasKey(x => x.Id);
                entity.HasIndex(x => new { x.Username, x.CreatedOnUtc });
                entity.HasIndex(x => x.IrdNumber);
                entity.HasOne<EmployeeEntity>()
                    .WithMany()
                    .HasForeignKey(x => x.Username)
                    .OnDelete(DeleteBehavior.Restrict);
            });

            modelBuilder.Entity<SessionRecordEntity>(entity =>
            {
                entity.ToTable("session_records");
                entity.HasKey(x => x.SessionId);
                entity.Property(x => x.AccessProfileJson).HasColumnType("json");
                entity.HasIndex(x => x.Username);
                entity.HasIndex(x => x.ExpiresOnUtc);
                entity.HasOne<EmployeeEntity>()
                    .WithMany()
                    .HasForeignKey(x => x.Username)
                    .OnDelete(DeleteBehavior.Cascade);
            });

            modelBuilder.Entity<AuditEventEntity>(entity =>
            {
                entity.ToTable("audit_events");
                entity.HasKey(x => x.Id);
                entity.Property(x => x.DetailsJson).HasColumnType("json");
                entity.HasIndex(x => new { x.EntityType, x.EntityKey });
                entity.HasIndex(x => new { x.ActorUsername, x.OccurredOnUtc });
            });
        }
    }
}
