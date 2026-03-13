@echo off
setlocal
set "ROOT=%~dp0..\.."
node "%ROOT%\scripts\cli\bat_run.mjs" %*
set "BAT_RUN_EXIT=%ERRORLEVEL%"
endlocal & exit /b %BAT_RUN_EXIT%
