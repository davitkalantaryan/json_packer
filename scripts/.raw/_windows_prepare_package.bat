::
:: File:			_windows_prepare_package.bat
:: Created on:		2021 Aug 07
:: Autor:			Davit Kalantaryan (davit.kalantaryan@gmail.com)
::
:: Purpose:	
::
:: Argumets: 
::
:: Notice: call this from developers command prompt
::

@echo off
setlocal EnableDelayedExpansion enableextensions

set "Platform=%1"
set "Configuration=%2"

:: these are defination
set exeNameBase=monitor_test
set exeName=%exeNameBase%.exe

rem here we have qt root defination
if not defined QTDIR (
	if not defined qtRoot (
	        set "qtRoot=C:\Qt\5.15.2"
	)
	if /i "%Platform%" == "x64" (
	    set "QTDIR=!qtRoot!\msvc2019_64"
	) else (
	    set "QTDIR=!qtRoot!\msvc2019"
	)
)
echo QTDIR is !QTDIR!


if /i "%Configuration%" == "Debug" (
    set "libNameExt=d"
) else (
    set "libNameExt="
)

set deployArch=deploy\win_%Platform%\%Configuration%
set sysAndArch=sys\win_%Platform%\%Configuration%

:: calculating current directory
set  scriptDirectory=%~dp0
set  currentDirectory=%cd%
cd /D "%scriptDirectory%..\.."
set "repositoryRoot=%cd%\"
echo "repositoryRoot="%repositoryRoot%

set directoryToDeploy=%repositoryRoot%%deployArch%\%exeNameBase%_windows
set openSslDllDir=%repositoryRoot%%sysAndArch%\dll
set exePath=%repositoryRoot%%sysAndArch%\bin\%exeName%

if exist "%directoryToDeploy%" rmdir /s /q "%directoryToDeploy%"
call "%QTDIR%\bin\qtenv2.bat"

echo "%directoryToDeploy%"%directoryToDeploy%

rem if not exist "%directoryToDeploy%" mkdir "%directoryToDeploy%"
mkdir "%directoryToDeploy%"

rem exit /b 0

echo "!!!!!!!! directoryToDeploy="%directoryToDeploy%
call windeployqt --dir "%directoryToDeploy%" --plugindir "%directoryToDeploy%\plugins" "%exePath%"
if not "%ERRORLEVEL%"=="0" (exit /b %ERRORLEVEL%)
if exist "%directoryToDeploy%\vc_redist.%Platform%.exe" del "%directoryToDeploy%\vc_redist.%Platform%.exe"

copy /Y "%exePath%" "%directoryToDeploy%\".

call msbuild "%repositoryRoot%workspaces\monitor_all_vs\monitor_all.vcxproj" /t:Rebuild /p:Configuration=%Configuration% /p:Platform=%Platform% -m:2
if not "%ERRORLEVEL%"=="0" (exit /b %ERRORLEVEL%)
copy /Y "%repositoryRoot%sys\win_%Platform%\%Configuration%\util"\*.dll "%directoryToDeploy%"\.

copy /Y "%repositoryRoot%dlls\win_%Platform%"\*.dll "%directoryToDeploy%"\.

if exist "!directoryToDeploy!.zip" del /f /q "!directoryToDeploy!.zip"
powershell Compress-Archive -Path "!directoryToDeploy!"  -DestinationPath "!directoryToDeploy!.zip"

cd %currentDirectory%

ENDLOCAL

::ENDLOCAL & (
::	set additionalPathUsedForDoocs=%additionalPath%
::	set MATLABPATH=%scriptDirectory%%sysAndArch%\mbin;%MATLABPATH%
::	set PYTHONPATH=%scriptDirectory%%sysAndArch%\pbin;%PYTHONPATH%
::	if not defined ENSHOST (
::		set ENSHOST=%defaultEnsHost%
::	)
::)
