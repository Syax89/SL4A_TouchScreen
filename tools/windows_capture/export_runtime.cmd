@echo off
REM Export the runtime centroid trace after capture_runtime.cmd.
REM Run as Administrator from the Windows SL4A_capture directory.

setlocal
set DIR=%USERPROFILE%\Desktop\SL4A_capture
set ETL=%DIR%\touch_runtime.etl

if not exist "%ETL%" (
  echo ERRORE: %ETL% non trovato. Esegui prima capture_runtime.cmd.
  exit /b 1
)

echo Esporto gli eventi TraceLogging e HID in CSV...
tracerpt "%ETL%" -o "%DIR%\touch_runtime.csv" -of CSV -y

if errorlevel 1 (
  echo ERRORE: tracerpt non ha potuto esportare il trace.
  exit /b 1
)

echo.
echo FATTO. Conserva entrambi i file per l'analisi:
echo   %ETL%
echo   %DIR%\touch_runtime.csv
endlocal
