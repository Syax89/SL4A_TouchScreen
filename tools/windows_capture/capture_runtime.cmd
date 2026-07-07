@echo off
REM Cattura RUNTIME (niente riavvio) del re-init del touch + tutto lo stack
REM ACPI/SAM/Panel/SPB. Molto piu' affidabile del boot trace per i provider
REM kernel-mode. Forza il re-init disabilitando/riabilitando il device ACPI
REM del touch (ACPI\MSHW0231): questo fa ri-eseguire _PS0/_INI + il pacchetto
REM di estensione "Surface Digitizer HidSpi Extn Package".
REM
REM Esegui COME AMMINISTRATORE.
REM NOTA: il touch sparisce per qualche secondo e torna. Se non tornasse,
REM un riavvio lo ripristina (nessun danno permanente).

setlocal
set DIR=%USERPROFILE%\Desktop\SL4A_capture
set OUT=%DIR%\touch_runtime.etl

echo === Avvio trace runtime (profilo touch_boot.wprp) ===
wpr -cancel >nul 2>&1
wpr -start "%DIR%\touch_boot.wprp" -filemode
if errorlevel 1 (
  echo ERRORE avvio wpr. Riprova dopo:  wpr -cancel
  exit /b 1
)

echo.
echo === Trigger re-init del touch (disable + enable ACPI\MSHW0231) ===
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$d = Get-PnpDevice -InstanceId 'ACPI\MSHW0231*' -ErrorAction SilentlyContinue;" ^
  "if(-not $d){ Write-Host 'ATTENZIONE: ACPI\MSHW0231 non trovato, provo per FriendlyName'; $d = Get-PnpDevice -FriendlyName '*Digitizer HidSpi*' -ErrorAction SilentlyContinue }" ^
  "foreach($x in $d){ Write-Host ('DISABLE '+$x.InstanceId); Disable-PnpDevice -InstanceId $x.InstanceId -Confirm:$false -ErrorAction SilentlyContinue };" ^
  "Start-Sleep -Seconds 3;" ^
  "foreach($x in $d){ Write-Host ('ENABLE  '+$x.InstanceId); Enable-PnpDevice -InstanceId $x.InstanceId -Confirm:$false -ErrorAction SilentlyContinue };"

echo.
echo === Aspetto 10s per la re-enumerazione completa ===
timeout /t 10 /nobreak >nul

echo === Stop trace ===
wpr -stop "%OUT%"

echo.
if exist "%OUT%" (
  echo FATTO. Copia questo file su Linux (o mandamelo):
  echo    %OUT%
  for %%A in ("%OUT%") do echo    dimensione: %%~zA byte
) else (
  echo ERRORE: %OUT% non creato.
)
endlocal
