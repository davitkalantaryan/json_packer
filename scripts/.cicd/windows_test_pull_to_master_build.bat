

@echo off
setlocal EnableDelayedExpansion

rem some definations
set "Platform=x64"
set "Configuration=Debug"
set "ActionConfirm=build"



set scriptDirectory=%~dp0
cd /D %scriptDirectory%..\..
set "repositoryRoot=%cd%\"

rem cd "%repositoryRoot%prj\tests\googletest_mult"
rem msbuild -t:restore -p:RestorePackagesConfig=true
rem if not "!ERRORLEVEL!"=="0" (exit /b !ERRORLEVEL!)
rem
rem rem cd "%repositoryRoot%workspaces\cpputils_vs"
rem call msbuild "%repositoryRoot%workspaces\cpputils_vs\cpputils.sln" /t:%ActionConfirm% /p:Configuration=%Configuration% /p:Platform=%Platform% -m:2
rem if not "!ERRORLEVEL!"=="0" (exit /b !ERRORLEVEL!)

rem do we need to compile makefile projects, then init env and do
rem if not "!VSCMD_ARG_HOST_ARCH!"=="!Platform!" (
rem 	echo "calling VsDevCmd in the !scriptDirectory!!scriptName!"
rem 	if /i "!Platform!"=="x64" (
rem 		set "PlatformTarget=amd64"
rem 	) else (
rem 		set "PlatformTarget=!Platform!"
rem 	)
rem 	call VsDevCmd -arch=!PlatformTarget!
rem ) else (
rem 	echo "VsDevCmd already set to !Platform!"
rem )


exit /b 0

endlocal
