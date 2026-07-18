@echo off
REM Cattura un vero ciclo _PS3 (sleep) -> _PS0 (resume) per HSPI.
REM Il disable/enable in Gestione Dispositivi (capture_runtime.cmd) NON basta:
REM abbiamo verificato che chiama _PS0 ma con FLAG=0 (ramo "no-op", 46us,
REM niente Sleep(300ms)/GPIO). Serve un vero Modern Standby (S0ix) perche'
REM il power engine porti davvero il device a _PS3 (FLAG=3) prima di
REM richiamare _PS0 con il ramo "vero" (M010 5B=1, Sleep 300ms, M010 103=1).
REM
REM Esegui COME AMMINISTRATORE.

setlocal
set DIR=%USERPROFILE%\Desktop\SL4A_capture
set OUT=%DIR%\touch_sleep_resume.etl

wpr -cancel >nul 2>&1
echo === Avvio trace (profilo touch_boot.wprp, sopravvive al Modern Standby) ===
wpr -start "%DIR%\touch_boot.wprp" -filemode
if errorlevel 1 (
  echo ERRORE avvio wpr. Prova:  wpr -cancel   e rilancia.
  exit /b 1
)

echo.
echo ============================================================
echo  TRACE ATTIVO. Fai ORA queste cose (nell'ordine):
echo.
echo   1^) Menu Start -^> icona Accensione -^> SOSPENDI
echo      (NON chiudere il coperchio: usa proprio il menu Sospendi,
echo       cosi' siamo sicuri che sia un vero Modern Standby S0ix).
echo.
echo   2^) Aspetta che lo schermo si spenga del tutto (5-10 secondi).
echo.
echo   3^) RISVEGLIA il PC (premi un tasto o il pulsante di accensione).
echo      Fai login se richiesto.
echo.
echo   4^) ASPETTA 15 secondi dopo il login (il touch deve ri-
echo      stabilizzarsi), poi torna in questa finestra.
echo.
echo   5^) Premi un tasto qui per FERMARE la cattura.
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
