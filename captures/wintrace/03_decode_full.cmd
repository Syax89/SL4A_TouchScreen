@echo off
REM Decodifica COMPLETA dell'ETL gia' catturato (NIENTE riavvio).
REM tracerpt in CSV/XML scarta i provider "manifest" (SPB, ACPI, Surface/SAM):
REM xperf (o netsh) li decodifica tutti. Serve a capire se i dati ci sono
REM davvero nell'ETL o se il boot trace non li ha proprio catturati.

setlocal
set DIR=%USERPROFILE%\Desktop\SL4A_capture
set ETL=%DIR%\touch_boot.etl
if not exist "%ETL%" set ETL=%SystemDrive%\touch_boot.etl
if not exist "%ETL%" (
  echo ERRORE: touch_boot.etl non trovato ne' sul Desktop\SL4A_capture ne' su C:\
  exit /b 1
)
set OUT=%DIR%\touch_boot_full.txt

echo ETL: %ETL%
echo.

where xperf >nul 2>&1
if %errorlevel%==0 (
  echo [1/1] Decodifica COMPLETA con xperf dumper...
  xperf -i "%ETL%" -o "%OUT%" -a dumper
) else (
  echo xperf non trovato. Provo con netsh trace convert...
  netsh trace convert input="%ETL%" output="%OUT%" dump=txt overwrite=yes
)

echo.
if exist "%OUT%" (
  echo FATTO. Manda questo file:
  echo    %OUT%
  for %%A in ("%OUT%") do echo    dimensione: %%~zA byte
  echo.
  echo Anteprima provider trovati:
  findstr /I "SPB Acpi GPIO Surface Serial SMF MSHW0231 RESET" "%OUT%" | find /c /v "" > "%TEMP%\_cnt.txt"
  set /p N=<"%TEMP%\_cnt.txt"
  echo    righe che citano SPB/Acpi/Surface/MSHW0231: %N%
) else (
  echo ERRORE: decodifica non riuscita. Ne' xperf ne' netsh disponibili?
  echo Installa "Windows Performance Toolkit" (gia' presente se hai wpr/wpa)
  echo oppure apri touch_boot.etl in WPA (Windows Performance Analyzer).
)
endlocal
