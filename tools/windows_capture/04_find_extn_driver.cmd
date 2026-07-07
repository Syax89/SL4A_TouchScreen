@echo off
REM Identify + copy the driver bound to the "Surface Digtizer HidSpi Extn Package" raw PDO.
REM
REM WHY: Get-PnpDevice enumeration (00_enumerate.cmd -> providers.txt) showed the touch
REM device tree is NOT flat:
REM
REM   ACPI\MSHW0231            <- base FDO, SPI-HID transport (this is what hidspi.sys/
REM                                amdspi.sys, already decompiled, bind to)
REM     ACPI\MSHW0231\A        <- separate RAW PDO, FriendlyName "Surface Digtizer HidSpi
REM                                Extn Package" (Class=HIDClass) <-- NEVER decompiled
REM       HID\MSHW0231&COL01..08  <- the HID collections (touch/pen/etc, split by HIDCLASS)
REM
REM The ETW provider list (00_enumerate.cmd output) separately shows a real provider
REM "Microsoft-Surface-SurfaceHidMiniDriver" {2FEA7205-B0B1-41CA-8609-5A1D16F3132F} --
REM likely emitted by whatever .sys is bound to ACPI\MSHW0231\A. This is a DIFFERENT
REM binary from hidspi.sys/amdspi.sys and has never been pulled or decompiled. It is the
REM most plausible place for a Surface-specific extra init/handshake sequence that our
REM Linux port (bare SPI-HID transport only) does not replicate.
REM
REM Run as Administrator:  04_find_extn_driver.cmd > extn_driver_info.txt
REM Then copy extn_driver_info.txt AND whatever .sys path(s) it prints (from
REM C:\Windows\System32\drivers\) into Desktop\windrivers\ and send both back.

echo ===== PnP device: ACPI\MSHW0231\A =====
powershell -NoProfile -Command "Get-PnpDevice -InstanceId 'ACPI\MSHW0231\A' | Format-List *"

echo.
echo ===== Driver service + INF bound to it =====
powershell -NoProfile -Command "Get-PnpDeviceProperty -InstanceId 'ACPI\MSHW0231\A' -KeyName DEVPKEY_Device_Service,DEVPKEY_Device_DriverInfPath,DEVPKEY_Device_Driver | Format-Table KeyName,Data -AutoSize"

echo.
echo ===== Same, for every MSHW0231 child (COL01-08 + \A), for comparison =====
powershell -NoProfile -Command "Get-PnpDevice | Where-Object { $_.InstanceId -match 'MSHW0231' } | ForEach-Object { $id = $_.InstanceId; Write-Host ('--- ' + $id + ' ---'); Get-PnpDeviceProperty -InstanceId $id -KeyName DEVPKEY_Device_Service | Select-Object Data }"

echo.
echo ===== driverquery: match any service name found above =====
driverquery /v /fo list | findstr /I "HidSpi HidMini Digitizer Extn"

echo.
echo ===== pnputil full driver package list (look for a Surface digitizer / HidSpi ext package) =====
pnputil /enum-drivers | findstr /I /C:"Published Name" /C:"Original Name" /C:"Provider" /C:"Class Name" /C:"digitizer" /C:"hidspi" /C:"surface"
