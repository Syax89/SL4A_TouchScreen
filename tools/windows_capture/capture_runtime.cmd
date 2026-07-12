@echo off
REM Cattura runtime senza riavvio. Il profilo include Microsoft.Surface.TouchAndPen.Prod,
REM che emette TouchBlobCoMX/Y: mantieni ciascun tocco fermo nel punto richiesto.
REM Esegui COME AMMINISTRATORE.

setlocal
set DIR=%USERPROFILE%\Desktop\SL4A_capture
set OUT=%DIR%\touch_runtime.etl

wpr -cancel >nul 2>&1
echo === Avvio trace (profilo touch_boot.wprp) ===
wpr -start "%DIR%\touch_boot.wprp" -filemode
if errorlevel 1 (
  echo ERRORE: wpr non parte. Prova:  wpr -cancel   e rilancia.
  exit /b 1
)
echo.
echo ============================================================
echo  TRACE ATTIVO. Fai ORA queste cose (nell'ordine):
echo.
echo   1^) Tocca e mantieni fermo un dito per ~2 secondi in ognuno di questi punti:
echo      alto-sinistra, alto-centro, alto-destra,
echo      centro-sinistra, centro, centro-destra,
echo      basso-sinistra, basso-centro, basso-destra.
echo      Lascia ~1 secondo tra un punto e il successivo.
echo.
echo   2^) Ripeti la stessa griglia con due dita distanti, se possibile.
echo      Non disabilitare/riabilitare il dispositivo: qui servono solo centroidi runtime.
echo.
echo   3^) Torna qui e premi un tasto per FERMARE la cattura.
echo ============================================================
echo.
pause

echo === Stop trace ===
wpr -stop "%OUT%"
echo.
if exist "%OUT%" (
  echo FATTO. Copia questo file su Linux ( ~/Scrivania/SL4A_capture/ ):
  echo    %OUT%
  for %%A in ("%OUT%") do echo    dimensione: %%~zA byte
) else (
  echo ERRORE: %OUT% non creato.
)
endlocal
