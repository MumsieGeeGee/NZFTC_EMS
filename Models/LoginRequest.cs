using System.ComponentModel.DataAnnotations;
using NZFTC_EMS.Utilities;

namespace NZFTC_EMS.Models
{
    public class LoginRequest
    {
        [Required(ErrorMessage = MainSystemAuthMessages.UsernameInvalid)]
        [StringLength(50, ErrorMessage = MainSystemAuthMessages.UsernameInvalid)]
        public string Username { get; set; } = string.Empty;

        [Required(ErrorMessage = MainSystemAuthMessages.PasswordMissingOrInvalid)]
        [StringLength(255)]
        [DataType(DataType.Password)]
        public string Password { get; set; } = string.Empty;
    }
}
