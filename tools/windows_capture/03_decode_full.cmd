@echo off
REM Decodifica COMPLETA di un ETL gia' catturato (NIENTE riavvio).
REM tracerpt in CSV/XML scarta i provider "manifest" (SPB, ACPI, Surface/SAM):
REM xperf (o netsh) li decodifica tutti. Serve a capire se i dati ci sono
REM davvero nell'ETL o se la cattura non li ha proprio presi.
REM
REM Uso:
REM   03_decode_full.cmd                     -> decodifica touch_boot.etl (default)
REM   03_decode_full.cmd touch_sleep_resume.etl  -> decodifica quel file
REM   03_decode_full.cmd touch_runtime.etl

setlocal
set DIR=%USERPROFILE%\Desktop\SL4A_capture
set NAME=%~1
if "%NAME%"=="" set NAME=touch_boot.etl
set ETL=%DIR%\%NAME%
if not exist "%ETL%" set ETL=%SystemDrive%\%NAME%
if not exist "%ETL%" (
  echo ERRORE: %NAME% non trovato ne' su Desktop\SL4A_capture ne' su C:\
  echo Uso:  03_decode_full.cmd [nomefile.etl]
  exit /b 1
)
set OUT=%DIR%\%~n1_full.txt
if "%NAME%"=="touch_boot.etl" set OUT=%DIR%\touch_boot_full.txt

echo ETL: %ETL%
echo OUT: %OUT%
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
  findstr /I "SPB Acpi GPIO Surface Serial SMF MSHW0231 RESET AmlMethod" "%OUT%" | find /c /v "" > "%TEMP%\_cnt.txt"
  set /p N=<"%TEMP%\_cnt.txt"
  echo    righe che citano SPB/Acpi/Surface/MSHW0231/AmlMethod: %N%
) else (
  echo ERRORE: decodifica non riuscita. Ne' xperf ne' netsh disponibili?
  echo Installa "Windows Performance Toolkit" (gia' presente se hai wpr/wpa)
  echo oppure apri l'ETL in WPA (Windows Performance Analyzer).
)
endlocal
