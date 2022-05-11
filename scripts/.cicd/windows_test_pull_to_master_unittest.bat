

@echo off
setlocal EnableDelayedExpansion

rem some definations
set "Platform=x64"
set "Configuration=Debug"
rem set "ActionConfirm=build"

set scriptDirectory=%~dp0
cd /D %scriptDirectory%..\..
set "repositoryRoot=%cd%\"

rem cd "%repositoryRoot%sys\win_%Platform%\%Configuration%\test"
rem .\unittest.exe

exit /b !ERRORLEVEL!

endlocal
