@echo off
REM 04_find_extn_driver.cmd showed ACPI\MSHW0231\A is bound to service=hidspi,
REM inf=hidspi_km.inf -- i.e. the SAME hidspi.sys we already decompiled, not a
REM mystery binary. But pnputil /enum-drivers separately listed a REAL published
REM driver package "surfacedigitizerhidspiextnpackage.inf" (provider: Surface)
REM that is NOT currently bound to this device instance. This checks what
REM hardware ID(s) that INF actually targets -- a different digitizer generation?
REM the pen instead of the touch? or genuinely this device but losing driver
REM ranking to the in-box hidspi_km.inf? Read-only, just locates+dumps the file.
REM
REM Run as Administrator: 05_check_extn_inf.cmd > extn_inf_info.txt

echo ===== Locating the actual .inf in the DriverStore =====
dir /s /b "C:\Windows\System32\DriverStore\FileRepository\surfacedigitizerhidspiextnpackage.inf_amd64_*\surfacedigitizerhidspiextnpackage.inf"

echo.
echo ===== Full contents =====
for /f "delims=" %%F in ('dir /s /b "C:\Windows\System32\DriverStore\FileRepository\surfacedigitizerhidspiextnpackage.inf_amd64_*\surfacedigitizerhidspiextnpackage.inf" 2^>nul') do type "%%F"

echo.
echo ===== Same directory listing (in case there's a companion .sys/.dll) =====
for /f "delims=" %%F in ('dir /s /b /a:d "C:\Windows\System32\DriverStore\FileRepository\surfacedigitizerhidspiextnpackage.inf_amd64_*" 2^>nul') do dir "%%F"
