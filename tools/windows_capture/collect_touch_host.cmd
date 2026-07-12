@echo off
REM Run as Administrator from the Windows SL4A_capture directory.
REM Identifies the user-mode host that loaded TouchPenProcessor0C19.dll.

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0collect_touch_host.ps1"
if errorlevel 1 (
  echo Nessun host accessibile trovato. Usa il touch per qualche secondo e riprova.
  exit /b 1
)
