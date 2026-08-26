param(
    [string]$RootPath = (Split-Path -Path $PSScriptRoot -Parent),
    [switch]$IncludeReviewFindings,
    [switch]$IncludeScriptFiles,
    [switch]$FailOnFindings
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path -LiteralPath $RootPath)) {
    throw "RootPath does not exist: $RootPath"
}

$resolvedRoot = (Resolve-Path -LiteralPath $RootPath).Path
$targetFiles = Get-ChildItem -Path $resolvedRoot -Recurse -File -Include *.cs,*.cshtml,*.ps1 |
    Where-Object {
        $_.FullName -notmatch '\\bin\\' -and
        $_.FullName -notmatch '\\obj\\' -and
        $_.FullName -notmatch '\\\.git\\' -and
        ($IncludeScriptFiles -or $_.FullName -notmatch '\\Scripts\\')
    }

$rules = @(
    @{
        Id = "explicit-placeholder"
        Severity = "high"
        Description = "Explicit placeholder wording"
        AppliesTo = { param($file) $true }
        Pattern = 'Next Integration|coming soon|not yet completed|view-only here until'
        Ignore = 'button\.disabled\s*=|:disabled|\[disabled\]'
    },
    @{
        Id = "todo-marker"
        Severity = "high"
        Description = "TODO/FIXME/HACK marker"
        AppliesTo = { param($file) $true }
        Pattern = '\b(TODO|FIXME|HACK)\b'
        Ignore = $null
    },
    @{
        Id = "not-implemented"
        Severity = "high"
        Description = "Thrown not-implemented path"
        AppliesTo = { param($file) $true }
        Pattern = 'NotImplementedException'
        Ignore = $null
    }
)

if ($IncludeReviewFindings) {
    $rules += @{
        Id = "disabled-mutable-action"
        Severity = "review"
        Description = "Disabled action inside mutable workflow view"
        AppliesTo = {
            param($file)
            (($file.FullName -match '\\Views\\') -and ($file.Name -match '^(Create_|Delete_|Edit_)')) -or
            ($file.Name -eq 'Secondary_Authentication.cshtml')
        }
        Pattern = '<button[^>]*disabled'
        Ignore = 'Account Already Active'
    }
}

$findings = New-Object System.Collections.Generic.List[object]

function Add-Finding {
    param(
        [string]$Severity,
        [string]$Rule,
        [string]$Description,
        [string]$File,
        [int]$Line,
        [string]$Text
    )

    $findings.Add([pscustomobject]@{
        Severity = $Severity
        Rule = $Rule
        Description = $Description
        File = $File
        Line = $Line
        Text = $Text.Trim()
    })
}

foreach ($file in $targetFiles) {
    $relativePath = $file.FullName.Substring($resolvedRoot.Length).TrimStart('\')

    foreach ($rule in $rules) {
        if (-not (& $rule.AppliesTo $file)) {
            continue
        }

        $matches = Select-String -Path $file.FullName -Pattern $rule.Pattern -AllMatches
        foreach ($match in $matches) {
            if ($rule.Ignore -and $match.Line -match $rule.Ignore) {
                continue
            }

            Add-Finding -Severity $rule.Severity -Rule $rule.Id -Description $rule.Description -File $relativePath -Line $match.LineNumber -Text $match.Line
        }
    }

    $isMutableView = $file.FullName -match '\\Views\\' -and $file.Name -match '^(Create_|Delete_|Edit_)'
    if (-not $isMutableView) {
        continue
    }

    $content = Get-Content -LiteralPath $file.FullName
    $contentText = $content -join [Environment]::NewLine
    $containsForm = $contentText -match '<form\b'
    $hasPostForm = $contentText -match '<form\b[^>]*\bmethod\s*=\s*"post"'

    if ($containsForm -and -not $hasPostForm) {
        $firstFormLine = (Select-String -Path $file.FullName -Pattern '<form\b' | Select-Object -First 1)
        if ($firstFormLine) {
            Add-Finding -Severity "high" -Rule "mutable-form-without-post" -Description "Mutable workflow form does not post anywhere" -File $relativePath -Line $firstFormLine.LineNumber -Text $firstFormLine.Line
        }
    }

    if ($hasPostForm -and $contentText -notmatch '@Html\.AntiForgeryToken\(\)') {
        $firstPostFormLine = (Select-String -Path $file.FullName -Pattern '<form\b[^>]*\bmethod\s*=\s*"post"' | Select-Object -First 1)
        if ($firstPostFormLine) {
            Add-Finding -Severity "review" -Rule "post-form-without-antiforgery" -Description "POST form is missing an antiforgery token marker" -File $relativePath -Line $firstPostFormLine.LineNumber -Text $firstPostFormLine.Line
        }
    }

    $insideForm = $false
    for ($index = 0; $index -lt $content.Count; $index++) {
        $line = $content[$index]
        if ($line -match '<form\b') {
            $insideForm = $true
        }

        if ($insideForm -and $line -match '<(input|select|textarea)\b') {
            $isInteractiveField = $line -notmatch '\breadonly\b' -and
                $line -notmatch '\bdisabled\b' -and
                $line -notmatch '\btype\s*=\s*"button"' -and
                $line -notmatch '\btype\s*=\s*"submit"' -and
                $line -notmatch '\btype\s*=\s*"hidden"'

            if ($isInteractiveField -and $line -notmatch '\bname\s*=') {
                Add-Finding -Severity "high" -Rule "mutable-field-without-name" -Description "Interactive form field is missing a name attribute" -File $relativePath -Line ($index + 1) -Text $line
            }
        }

        if ($line -match '</form>') {
            $insideForm = $false
        }
    }
}

$orderedFindings = $findings |
    Sort-Object Severity, File, Line

if (-not $orderedFindings) {
    Write-Host "No placeholder or incomplete-surface findings found."
    exit 0
}

$orderedFindings |
    Format-Table Severity, Rule, File, Line, Text -AutoSize

$highCount = ($orderedFindings | Where-Object Severity -eq "high").Count
$reviewCount = ($orderedFindings | Where-Object Severity -eq "review").Count

Write-Host ""
Write-Host "Summary: $highCount high-signal finding(s), $reviewCount review finding(s)."

if ($FailOnFindings -and $orderedFindings.Count -gt 0) {
    exit 1
}

exit 0
