-- NZFTC EMS MySQL prep schema
-- Sensitive text fields should be encrypted in the application before storage.

CREATE DATABASE IF NOT EXISTS `NZFTC_EMS`
  CHARACTER SET utf8mb4
  COLLATE utf8mb4_0900_ai_ci;

USE `NZFTC_EMS`;

CREATE TABLE IF NOT EXISTS `employees` (
  `username` VARCHAR(50) NOT NULL,
  `first_name` VARCHAR(100) NOT NULL,
  `last_name` VARCHAR(100) NOT NULL,
  `business_role` VARCHAR(100) NOT NULL DEFAULT '',
  `job_role` VARCHAR(100) NOT NULL DEFAULT '',
  `account_status` VARCHAR(30) NOT NULL DEFAULT 'Active',
  `email` VARCHAR(255) NOT NULL DEFAULT '',
  `phone` VARCHAR(50) NOT NULL DEFAULT '',
  `created_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  `updated_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) ON UPDATE CURRENT_TIMESTAMP(6),
  PRIMARY KEY (`username`),
  KEY `ix_employees_business_role` (`business_role`),
  KEY `ix_employees_job_role` (`job_role`),
  KEY `ix_employees_account_status` (`account_status`)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `employee_account_details` (
  `username` VARCHAR(50) NOT NULL,
  `emergency_contact_encrypted` TEXT NOT NULL,
  `diagnosed_health_conditions_encrypted` TEXT NOT NULL,
  `allergies_encrypted` TEXT NOT NULL,
  `approved_emergency_steps_encrypted` TEXT NOT NULL,
  `prior_training_encrypted` TEXT NOT NULL,
  `first_aid_certification_encrypted` TEXT NOT NULL,
  `group_name` VARCHAR(50) NOT NULL DEFAULT '',
  `record_snapshot_json` LONGTEXT NOT NULL,
  `created_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  `updated_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) ON UPDATE CURRENT_TIMESTAMP(6),
  PRIMARY KEY (`username`),
  CONSTRAINT `fk_employee_account_details_employee`
    FOREIGN KEY (`username`) REFERENCES `employees` (`username`)
    ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `employee_tax_information` (
  `username` VARCHAR(50) NOT NULL,
  `ird_number` VARCHAR(20) NOT NULL,
  `ird_confirmation_status` VARCHAR(30) NOT NULL DEFAULT 'Unconfirmed',
  `pay_frequency` VARCHAR(30) NOT NULL DEFAULT 'Fortnightly',
  `paye_table_version` VARCHAR(20) NOT NULL DEFAULT '2025',
  `extra_paye_voluntary_deduction` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `tax_code` VARCHAR(10) NOT NULL DEFAULT 'M',
  `tax_code_description` VARCHAR(255) NOT NULL DEFAULT 'Standard tax code',
  `tax_code_job_type` VARCHAR(30) NOT NULL DEFAULT 'Primary',
  `primary_tax_code` VARCHAR(10) NOT NULL DEFAULT 'M',
  `secondary_tax_code` VARCHAR(10) NOT NULL DEFAULT '',
  `special_tax_code` VARCHAR(10) NOT NULL DEFAULT '',
  `special_tax_code_rate` VARCHAR(20) NOT NULL DEFAULT '',
  `stc_letter_received` VARCHAR(30) NOT NULL DEFAULT '',
  `stc_expiry_date` VARCHAR(30) NOT NULL DEFAULT '',
  `kiwi_saver_opt_in_out_status` VARCHAR(20) NOT NULL DEFAULT 'opt-in',
  `kiwi_saver_employee_contribution_rate` VARCHAR(20) NOT NULL DEFAULT '3%',
  `kiwi_saver_opt_out_date` VARCHAR(30) NOT NULL DEFAULT '',
  `student_loan_status` VARCHAR(50) NOT NULL DEFAULT 'Not in repayment',
  `student_loan_existence` VARCHAR(10) NOT NULL DEFAULT 'Yes',
  `student_loan_sdr_ird_letter_existence` VARCHAR(50) NOT NULL DEFAULT '',
  `student_loan_special_deduction_rate` VARCHAR(20) NOT NULL DEFAULT '',
  `student_loan_repayment_exemption_existence` VARCHAR(30) NOT NULL DEFAULT '',
  `student_loan_repayment_exemption_reason` VARCHAR(255) NOT NULL DEFAULT '',
  `student_loan_repayment_exemption_expiry_date` VARCHAR(30) NOT NULL DEFAULT '',
  `student_loan_repayment_threshold` VARCHAR(20) NOT NULL DEFAULT '',
  `child_support_status` VARCHAR(50) NOT NULL DEFAULT '',
  `child_support_standard_deduction` VARCHAR(50) NOT NULL DEFAULT '',
  `child_support_voluntary_deduction` VARCHAR(50) NOT NULL DEFAULT '',
  `child_support_protected_net_earnings` VARCHAR(50) NOT NULL DEFAULT '',
  `child_support_ird_notice_of_deduction_existence` VARCHAR(50) NOT NULL DEFAULT '',
  `child_support_ird_notice_of_deduction_amount` VARCHAR(50) NOT NULL DEFAULT '',
  `child_support_ird_notice_of_deduction_effective_date` VARCHAR(30) NOT NULL DEFAULT '',
  `child_support_ird_notice_of_deduction_expiry_date` VARCHAR(30) NOT NULL DEFAULT '',
  `child_support_ird_notice_of_deduction_reference_number` VARCHAR(100) NOT NULL DEFAULT '',
  `employee_extras` VARCHAR(100) NOT NULL DEFAULT '',
  `esct_tax_rate` VARCHAR(20) NOT NULL DEFAULT '',
  `esct_income_band` VARCHAR(100) NOT NULL DEFAULT '',
  `student_loan_rate` DECIMAL(12,2) NOT NULL DEFAULT 12.00,
  `kiwi_saver_employer_contribution_rate` DECIMAL(12,2) NOT NULL DEFAULT 3.00,
  `acc_levy_rate` DECIMAL(12,2) NOT NULL DEFAULT 1.75,
  `employee_type` VARCHAR(30) NOT NULL DEFAULT 'Salary',
  `created_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  `updated_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) ON UPDATE CURRENT_TIMESTAMP(6),
  PRIMARY KEY (`username`),
  UNIQUE KEY `ux_employee_tax_information_ird_number` (`ird_number`),
  KEY `ix_employee_tax_information_tax_code` (`tax_code`),
  KEY `ix_employee_tax_information_student_loan_status` (`student_loan_status`),
  CONSTRAINT `fk_employee_tax_information_employee`
    FOREIGN KEY (`username`) REFERENCES `employees` (`username`)
    ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `leave_entitlements` (
  `username` VARCHAR(50) NOT NULL,
  `business_role` VARCHAR(100) NOT NULL DEFAULT '',
  `job_role` VARCHAR(100) NOT NULL DEFAULT '',
  `salary_package_name` VARCHAR(150) NOT NULL DEFAULT '',
  `salary_package_display_name` VARCHAR(150) NOT NULL DEFAULT '',
  `approximate_annual_salary` INT NOT NULL DEFAULT 0,
  `kiwi_saver_employer_contribution` INT NOT NULL DEFAULT 0,
  `annual_leave_lawful_days` INT NOT NULL DEFAULT 0,
  `annual_leave_package_extra_days` INT NOT NULL DEFAULT 0,
  `annual_leave_days` INT NOT NULL DEFAULT 0,
  `annual_leave_taken_days` INT NOT NULL DEFAULT 0,
  `annual_leave_scheduled_days` INT NOT NULL DEFAULT 0,
  `sick_leave_days` INT NOT NULL DEFAULT 0,
  `sick_leave_lawful_days` INT NOT NULL DEFAULT 0,
  `sick_leave_package_extra_days` INT NOT NULL DEFAULT 0,
  `sick_leave_taken_days` INT NOT NULL DEFAULT 0,
  `sick_leave_scheduled_days` INT NOT NULL DEFAULT 0,
  `public_holiday_days` INT NOT NULL DEFAULT 0,
  `public_holiday_lawful_days` INT NOT NULL DEFAULT 0,
  `public_holiday_package_extra_days` INT NOT NULL DEFAULT 0,
  `special_leave_days` INT NOT NULL DEFAULT 0,
  `special_leave_lawful_days` INT NOT NULL DEFAULT 0,
  `special_leave_package_extra_days` INT NOT NULL DEFAULT 0,
  `special_leave_taken_days` INT NOT NULL DEFAULT 0,
  `special_leave_scheduled_days` INT NOT NULL DEFAULT 0,
  `parental_leave_weeks` INT NOT NULL DEFAULT 0,
  `parental_leave_lawful_weeks` INT NOT NULL DEFAULT 0,
  `parental_leave_package_extra_weeks` INT NOT NULL DEFAULT 0,
  `parental_leave_taken_weeks` INT NOT NULL DEFAULT 0,
  `parental_leave_scheduled_weeks` INT NOT NULL DEFAULT 0,
  `superannuation_contribution` INT NOT NULL DEFAULT 0,
  `health_insurance_contribution` INT NOT NULL DEFAULT 0,
  `professional_development_contribution` INT NOT NULL DEFAULT 0,
  `employee_assistance_program_contribution` INT NOT NULL DEFAULT 0,
  `remaining_annual_leave_days` INT NOT NULL DEFAULT 0,
  `remaining_sick_leave_days` INT NOT NULL DEFAULT 0,
  `remaining_special_leave_days` INT NOT NULL DEFAULT 0,
  `remaining_parental_leave_weeks` INT NOT NULL DEFAULT 0,
  `created_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  `updated_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6) ON UPDATE CURRENT_TIMESTAMP(6),
  PRIMARY KEY (`username`),
  CONSTRAINT `fk_leave_entitlements_employee`
    FOREIGN KEY (`username`) REFERENCES `employees` (`username`)
    ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `leave_requests` (
  `id` BIGINT NOT NULL AUTO_INCREMENT,
  `submitted_by_username` VARCHAR(50) NOT NULL,
  `submitted_for_username` VARCHAR(50) NOT NULL,
  `employee_name` VARCHAR(200) NOT NULL DEFAULT '',
  `leave_type` VARCHAR(100) NOT NULL DEFAULT '',
  `start_date` DATE NOT NULL,
  `end_date` DATE NOT NULL,
  `reason_encrypted` TEXT NOT NULL,
  `status` VARCHAR(50) NOT NULL DEFAULT 'Open',
  `decision_reason_encrypted` TEXT NOT NULL,
  `requested_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  `updated_on_utc` DATETIME(6) NULL DEFAULT NULL,
  `handled_by_username` VARCHAR(50) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  KEY `ix_leave_requests_submitted_for_status` (`submitted_for_username`, `status`),
  KEY `ix_leave_requests_status_requested_on` (`status`, `requested_on_utc`),
  KEY `ix_leave_requests_dates` (`start_date`, `end_date`),
  CONSTRAINT `fk_leave_requests_submitter`
    FOREIGN KEY (`submitted_by_username`) REFERENCES `employees` (`username`)
    ON DELETE RESTRICT,
  CONSTRAINT `fk_leave_requests_employee`
    FOREIGN KEY (`submitted_for_username`) REFERENCES `employees` (`username`)
    ON DELETE RESTRICT
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `grievance_requests` (
  `id` BIGINT NOT NULL AUTO_INCREMENT,
  `submitted_by_username` VARCHAR(50) NOT NULL,
  `submitted_for_username` VARCHAR(50) NOT NULL,
  `employee_name` VARCHAR(200) NOT NULL DEFAULT '',
  `subject` VARCHAR(255) NOT NULL DEFAULT '',
  `description_encrypted` LONGTEXT NOT NULL,
  `category` VARCHAR(100) NOT NULL DEFAULT 'General',
  `severity` VARCHAR(50) NOT NULL DEFAULT 'Medium',
  `status` VARCHAR(50) NOT NULL DEFAULT 'Open',
  `outcome_encrypted` LONGTEXT NOT NULL,
  `decision_reason_encrypted` LONGTEXT NOT NULL,
  `submitted_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  `updated_on_utc` DATETIME(6) NULL DEFAULT NULL,
  `created_by_role` VARCHAR(50) NOT NULL DEFAULT 'Employee',
  `notification_group` VARCHAR(50) NOT NULL DEFAULT 'Employee',
  `handled_by_username` VARCHAR(50) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  KEY `ix_grievance_requests_employee_status` (`submitted_for_username`, `status`),
  KEY `ix_grievance_requests_status_submitted_on` (`status`, `submitted_on_utc`),
  CONSTRAINT `fk_grievance_requests_submitter`
    FOREIGN KEY (`submitted_by_username`) REFERENCES `employees` (`username`)
    ON DELETE RESTRICT,
  CONSTRAINT `fk_grievance_requests_employee`
    FOREIGN KEY (`submitted_for_username`) REFERENCES `employees` (`username`)
    ON DELETE RESTRICT
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `payslips` (
  `id` BIGINT NOT NULL AUTO_INCREMENT,
  `username` VARCHAR(50) NOT NULL,
  `employee_name` VARCHAR(200) NOT NULL DEFAULT '',
  `salary_package_name` VARCHAR(150) NOT NULL DEFAULT '',
  `salary_package_display_name` VARCHAR(150) NOT NULL DEFAULT '',
  `ird_number` VARCHAR(20) NOT NULL DEFAULT '',
  `pay_period` VARCHAR(100) NOT NULL DEFAULT '',
  `pay_frequency` VARCHAR(30) NOT NULL DEFAULT 'Fortnightly',
  `base_pay` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `overtime_pay` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `bonus_pay` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `allowance_pay` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `pre_tax_deductions` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `gross_pay` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `taxable_income` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `paye` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `student_loan` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `kiwi_saver_employee_contribution` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `kiwi_saver_employer_contribution` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `esct` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `acc_levy` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `post_tax_deductions` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `net_pay` DECIMAL(12,2) NOT NULL DEFAULT 0.00,
  `annual_leave_lawful_days` INT NOT NULL DEFAULT 0,
  `annual_leave_package_extra_days` INT NOT NULL DEFAULT 0,
  `annual_leave_taken_days` INT NOT NULL DEFAULT 0,
  `annual_leave_scheduled_days` INT NOT NULL DEFAULT 0,
  `annual_leave_remaining_days` INT NOT NULL DEFAULT 0,
  `sick_leave_lawful_days` INT NOT NULL DEFAULT 0,
  `sick_leave_package_extra_days` INT NOT NULL DEFAULT 0,
  `sick_leave_taken_days` INT NOT NULL DEFAULT 0,
  `sick_leave_scheduled_days` INT NOT NULL DEFAULT 0,
  `sick_leave_remaining_days` INT NOT NULL DEFAULT 0,
  `special_leave_lawful_days` INT NOT NULL DEFAULT 0,
  `special_leave_package_extra_days` INT NOT NULL DEFAULT 0,
  `special_leave_taken_days` INT NOT NULL DEFAULT 0,
  `special_leave_scheduled_days` INT NOT NULL DEFAULT 0,
  `special_leave_remaining_days` INT NOT NULL DEFAULT 0,
  `parental_leave_lawful_weeks` INT NOT NULL DEFAULT 0,
  `parental_leave_package_extra_weeks` INT NOT NULL DEFAULT 0,
  `parental_leave_remaining_weeks` INT NOT NULL DEFAULT 0,
  `public_holiday_days` INT NOT NULL DEFAULT 0,
  `public_holiday_package_extra_days` INT NOT NULL DEFAULT 0,
  `created_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  PRIMARY KEY (`id`),
  KEY `ix_payslips_username_created_on` (`username`, `created_on_utc`),
  KEY `ix_payslips_ird_number` (`ird_number`),
  CONSTRAINT `fk_payslips_employee`
    FOREIGN KEY (`username`) REFERENCES `employees` (`username`)
    ON DELETE RESTRICT
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `session_records` (
  `session_id` VARCHAR(128) NOT NULL,
  `username` VARCHAR(50) NOT NULL,
  `account_type` VARCHAR(50) NOT NULL DEFAULT '',
  `access_profile_json` JSON NOT NULL,
  `created_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  `last_seen_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  `expires_on_utc` DATETIME(6) NOT NULL,
  `is_active` TINYINT(1) NOT NULL DEFAULT 1,
  PRIMARY KEY (`session_id`),
  KEY `ix_session_records_username` (`username`),
  KEY `ix_session_records_expires_on` (`expires_on_utc`),
  CONSTRAINT `fk_session_records_employee`
    FOREIGN KEY (`username`) REFERENCES `employees` (`username`)
    ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS `audit_events` (
  `id` BIGINT NOT NULL AUTO_INCREMENT,
  `entity_type` VARCHAR(100) NOT NULL,
  `entity_key` VARCHAR(100) NOT NULL,
  `action_type` VARCHAR(50) NOT NULL,
  `actor_username` VARCHAR(50) NOT NULL DEFAULT '',
  `details_json` JSON NOT NULL,
  `occurred_on_utc` DATETIME(6) NOT NULL DEFAULT CURRENT_TIMESTAMP(6),
  PRIMARY KEY (`id`),
  KEY `ix_audit_events_entity` (`entity_type`, `entity_key`),
  KEY `ix_audit_events_actor` (`actor_username`, `occurred_on_utc`)
) ENGINE=InnoDB;
