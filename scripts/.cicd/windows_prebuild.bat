@echo off
setlocal EnableDelayedExpansion


set scriptName=%0
set scriptDirectory=%~dp0
set currentDirectory=%cd%


cd /D "%scriptDirectory%..\.."
set "repositoryRoot=%cd%\"
echo "repositoryRoot="%repositoryRoot%

git submodule sync --recursive
git submodule update --init --recursive


rem rem compile google test
rem call :compileForPlatform  ARM
rem if not "!ERRORLEVEL!"=="0" (exit /b !ERRORLEVEL!)
rem call :compileForPlatform  ARM64
rem if not "!ERRORLEVEL!"=="0" (exit /b !ERRORLEVEL!)
rem call :compileForPlatform  x64
rem if not "!ERRORLEVEL!"=="0" (exit /b !ERRORLEVEL!)
rem call :compileForPlatform  Win32
rem if not "!ERRORLEVEL!"=="0" (exit /b !ERRORLEVEL!)

exit /b 0


:compileForPlatform
	set "PlatformTarget=%1"
	if "%1"=="Win32" ( set "PlatformTarget=x86" )
	cd "%repositoryRoot%contrib\googletest"
	cmake -G "Visual Studio 16 2019" -A %1 -H. -B../../build/googletest/win_!PlatformTarget! -Dgtest_force_shared_crt=1 --no-warn-unused-cli
	if not "!ERRORLEVEL!"=="0" (exit /b !ERRORLEVEL!)
	call :compileForPlatformAndConfiguration !PlatformTarget!  Release
	call :compileForPlatformAndConfiguration !PlatformTarget!  Debug

	exit /b 0

:compileForPlatformAndConfiguration
	cd "%repositoryRoot%build\googletest\win_%1"
	cmake --build . -- /p:Configuration=%2
	if not "!ERRORLEVEL!"=="0" (exit /b !ERRORLEVEL!)
	if not exist "%repositoryRoot%sys\win_%1\%2\lib" ( mkdir "%repositoryRoot%sys\win_%1\%2\lib" )
	xcopy lib\%2\*.lib "%repositoryRoot%sys\win_%1\%2\lib\." /Y
	exit /b 0

endlocal
::endlocal & (
::	if defined VsDevCmd_version_defined_here (
::		set VsDevCmd_version=!VsDevCmd_version!
::	)
::)
