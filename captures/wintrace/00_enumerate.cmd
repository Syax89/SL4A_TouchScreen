@echo off
REM Enumerate ETW providers likely involved in gating the touchscreen.
REM Run as Administrator:  00_enumerate.cmd > providers.txt   then send providers.txt back.

echo ===== SURFACE / AGGREGATOR / SAM providers =====
logman query providers | findstr /I "Surface Aggregator SAM Serial-Hub SerialHub"

echo.
echo ===== ACPI providers =====
logman query providers | findstr /I "Acpi"

echo.
echo ===== POWER providers =====
logman query providers | findstr /I "Power"

echo.
echo ===== PnP / Device providers =====
logman query providers | findstr /I "Pnp Device-Management Kernel-PnP"

echo.
echo ===== HID / SPB / GPIO providers (per completezza) =====
logman query providers | findstr /I "HID SPB GPIO"

echo.
echo ===== Il touchscreen e i device SAM/companion in ACPI =====
REM Mostra i device e il loro stato (utile per capire ordine/dipendenze)
powershell -NoProfile -Command "Get-PnpDevice | Where-Object { $_.InstanceId -match 'MSHW0231|MSHW0084|SURFACE|SAM|SPI' } | Select-Object Status,Class,FriendlyName,InstanceId | Format-Table -AutoSize"
