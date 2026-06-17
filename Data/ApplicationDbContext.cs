using Microsoft.EntityFrameworkCore;

namespace NZFTC_EMS.Data
{
    public class ApplicationDbContext : DbContext
    {
        public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options)
            : base(options)
        {
        }

        // DbSets will go here later
        // public DbSet<Employee> Employees { get; set; }
    }
}
