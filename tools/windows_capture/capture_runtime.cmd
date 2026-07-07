@echo off
REM Cattura RUNTIME affidabile (niente riavvio). Il wpr registra; TU generi
REM l'attivita' da catturare con due gesti che funzionano di sicuro.
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
echo   1^) TRASCINA UN DITO sullo schermo touch per ~5 secondi
echo      (serve a confermare che la cattura SPB funziona).
echo.
echo   2^) Apri GESTIONE DISPOSITIVI (tasto Win+X -^> Gestione dispositivi).
echo      In "Human Interface Device" / "Dispositivi interfaccia utente"
echo      trova una voce tipo:
echo         - "Surface Touch Screen Device"   oppure
echo         - "Surface Digitizer HidSpi Extn Package"
echo      Tasto destro -^> DISABILITA. Aspetta 3 secondi.
echo      Tasto destro -^> ABILITA.
echo      (Lo schermo touch smette e riparte: e' normale.)
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
