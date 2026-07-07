@echo off
REM Export the boot trace ETL to text/CSV for analysis on Linux.
REM Run as Administrator after 'wpr -boottrace -stopboot %SystemDrive%\touch_boot.etl'.

set ETL=%SystemDrive%\touch_boot.etl

if not exist "%ETL%" (
  echo ERRORE: %ETL% non trovato. Hai eseguito:
  echo   wpr -boottrace -stopboot %%SystemDrive%%\touch_boot.etl
  exit /b 1
)

echo Esporto %ETL% in CSV...
tracerpt "%ETL%" -o "%SystemDrive%\touch_boot.csv" -of CSV -y

echo.
echo Esporto anche il summary/dumpfile leggibile...
tracerpt "%ETL%" -o "%SystemDrive%\touch_boot_summary.txt" -report "%SystemDrive%\touch_boot_report.html" -f html -y 2>nul

echo.
echo Fatto. Manda questi file:
echo   %SystemDrive%\touch_boot.csv
echo   %SystemDrive%\touch_boot.etl   (se il CSV perde dettagli, l'ETL ha tutto)
dir "%SystemDrive%\touch_boot.*"
