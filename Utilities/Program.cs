using Microsoft.EntityFrameworkCore;
using NZFTC_EMS.Data;
using NZFTC_EMS.Services;
using NZFTC_EMS.Utilities;
using System;

var builder = WebApplication.CreateBuilder(args);

var connectionString = builder.Configuration.GetConnectionString("DefaultConnection");
var mysqlPassword = builder.Configuration["MYSQL_PASSWORD"] ?? Environment.GetEnvironmentVariable("MYSQL_PASSWORD");
if (string.IsNullOrWhiteSpace(connectionString))
{
    throw new InvalidOperationException("Connection string 'DefaultConnection' is not configured.");
}

if (!ConnectionStringHasPassword(connectionString))
{
    if (string.IsNullOrWhiteSpace(mysqlPassword))
    {
        throw new InvalidOperationException(
            "MySQL startup requires the MYSQL_PASSWORD environment variable because the configured DefaultConnection does not include a password. " +
            "Set MYSQL_PASSWORD before running the web app, or add the password through local development configuration.");
    }

    connectionString = AppendMySqlPassword(connectionString, mysqlPassword);
}

builder.Services.AddDbContext<ApplicationDbContext>(options =>
    options.UseMySql(
        connectionString,
        ServerVersion.AutoDetect(connectionString)
    ));

// Add session services
builder.Services.AddDistributedMemoryCache();
builder.Services.AddSession(options =>
{
    options.IdleTimeout = TimeSpan.FromMinutes(15);
    options.Cookie.HttpOnly = true;
    options.Cookie.IsEssential = true;
});

// Add authentication service
builder.Services.AddScoped<IAuthenticationService, AuthenticationService>();
builder.Services.AddScoped<IPublicHolidayCalendarService, PublicHolidayCalendarService>();
builder.Services.AddScoped<MySqlRepository>();
builder.Services.AddScoped<EmployeeRecordStore>();
builder.Services.AddScoped<EmployeeAccountRecordService>();
builder.Services.AddScoped<PasswordResetRequestService>();
builder.Services.AddScoped<GrievanceRequestService>();
builder.Services.AddScoped<LeaveRequestService>();
builder.Services.AddScoped<PayrollService>();
builder.Services.AddScoped<SessionAuditService>();

// Add services to the container.
builder.Services.AddControllersWithViews();

var app = builder.Build();

await MySqlDatabaseInitializer.InitializeAsync(app.Services);

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Privacy/Error");
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
    app.UseHttpsRedirection();
}

app.UseRouting();

// Add session middleware BEFORE authorization
app.UseSession();

app.Use(async (context, next) =>
{
    if (!string.IsNullOrWhiteSpace(context.Session.GetString("Username")))
    {
        var sessionAuditService = context.RequestServices.GetRequiredService<SessionAuditService>();
        await sessionAuditService.TouchSessionAsync(context.Session.Id);
    }

    await next();
});

app.UseAuthorization();

app.MapStaticAssets();

app.MapControllerRoute(
    name: "default",
    pattern: "{controller=Login}/{action=Login}/{id?}")
    .WithStaticAssets();

// Launch browser on application start (Development environment only)
if (app.Environment.IsDevelopment())
{
    var lifetime = app.Services.GetRequiredService<IHostApplicationLifetime>();
    lifetime.ApplicationStarted.Register(() =>
    {
        Task.Run(() =>
        {
            try
            {
                System.Threading.Thread.Sleep(1000); // Wait 1 second for server to fully start
                
                var defaultBrowser = BrowserLauncher.GetDefaultBrowser();
                var installedBrowsers = BrowserLauncher.GetInstalledBrowsers();
                
                if (installedBrowsers.Count > 0 || defaultBrowser != null)
                {
                    string url = "http://localhost:5186";
                    BrowserLauncher.LaunchBrowser(url, defaultBrowser);
                    Console.WriteLine($"Launching application in {(defaultBrowser?.Name ?? "default")} browser: {url}");
                }
                else
                {
                    Console.WriteLine("Warning: No supported browsers found. Please navigate manually to http://localhost:5186");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Note: Could not auto-launch browser: {ex.Message}. Please navigate to http://localhost:5186 manually.");
            }
        });
    });
}

app.Run();

static bool ConnectionStringHasPassword(string connectionString)
{
    if (string.IsNullOrWhiteSpace(connectionString))
    {
        return false;
    }

    var segments = connectionString.Split(';', StringSplitOptions.RemoveEmptyEntries | StringSplitOptions.TrimEntries);
    foreach (var segment in segments)
    {
        if (segment.StartsWith("password=", StringComparison.OrdinalIgnoreCase) ||
            segment.StartsWith("pwd=", StringComparison.OrdinalIgnoreCase))
        {
            return true;
        }
    }

    return false;
}

static string AppendMySqlPassword(string connectionString, string password)
{
    const string passwordKey = "password=";
    var separator = connectionString.EndsWith(';') ? string.Empty : ";";
    if (connectionString.Contains(passwordKey, StringComparison.OrdinalIgnoreCase))
    {
        return connectionString;
    }

    return $"{connectionString}{separator}password={password}";
}
