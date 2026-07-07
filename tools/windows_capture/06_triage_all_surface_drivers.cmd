@echo off
REM Thin wrapper: triage every published "Surface*" driver package (see the .ps1 in this
REM same folder for what it checks and why) without decompiling anything blindly.
REM
REM Run as Administrator:  06_triage_all_surface_drivers.cmd > triage_result.txt
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0\06_triage_all_surface_drivers.ps1"
