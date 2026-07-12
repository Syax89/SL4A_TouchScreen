# Run from an elevated 64-bit PowerShell after the touchscreen is active.
# Finds the user-mode host that loaded TouchPenProcessor0C19.dll and preserves
# enough evidence to reverse the callback boundary on Linux.

$ErrorActionPreference = 'Stop'
$outDir = Join-Path $env:USERPROFILE 'Desktop\SL4A_capture\touch_host'
$dllName = 'TouchPenProcessor0C19.dll'

New-Item -ItemType Directory -Force -Path $outDir | Out-Null
$matches = @()

Get-Process | ForEach-Object {
    $process = $_
    try {
        $modules = @($process.Modules)
        if ($modules.FileName -match [regex]::Escape($dllName)) {
            $matches += [pscustomobject]@{
                Id = $process.Id
                Name = $process.ProcessName
                Path = $process.Path
                Modules = $modules
            }
        }
    } catch {
        # Protected processes and cross-architecture processes may deny module access.
    }
}

$report = Join-Path $outDir 'touch_host_modules.txt'
if ($matches.Count -eq 0) {
    "No process module match for $dllName was accessible." | Set-Content $report
    "Run this from elevated 64-bit PowerShell after using the touchscreen." | Add-Content $report
    exit 1
}

foreach ($match in $matches) {
    "Process: $($match.Name) (PID $($match.Id))" | Add-Content $report
    "Path: $($match.Path)" | Add-Content $report
    "Command line:" | Add-Content $report
    (Get-CimInstance Win32_Process -Filter "ProcessId=$($match.Id)").CommandLine | Add-Content $report
    "Modules:" | Add-Content $report
    $match.Modules | ForEach-Object { $_.FileName } | Sort-Object | Add-Content $report
    "" | Add-Content $report

    if ($match.Path -and (Test-Path $match.Path)) {
        Copy-Item -Force $match.Path (Join-Path $outDir (Split-Path $match.Path -Leaf))
    }
}

Write-Host "Saved host evidence to $outDir"
