namespace NZFTC_EMS.Utilities;

public static class MainSystemAuthMessages
{
    public const string DisplayError = "Error: ";
    public const string DisplayInfo = "Information: ";

    public const string InvalidCredentials = "Invalid credentials. Please try again.";
    public const string PasswordMissingOrInvalid = "Password is missing or invalid.";
    public const string UsernameInvalid = "Invalid username.";
    public const string InvalidInputTryAgain = "Invalid input. Please try again.";
    public const string LoginFailed = "Login failed.";
    public const string LogoutFailed = "Logout failed.";
    public const string LoggedOutMessage = "you have been logged out";

    public static string AsError(string message) => $"{DisplayError}{message}";

    public static string AsInfo(string message) => $"{DisplayInfo}{message}";
}
