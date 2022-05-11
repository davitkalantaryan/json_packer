::
:: File:			_windows_compile_gui.bat
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

set  scriptDirectory=%~dp0
set  currentDirectory=%cd%
cd /D "%scriptDirectory%..\.."
set repositoryRoot=%cd%\
echo "repositoryRoot="%repositoryRoot%

if /i "!VSCMD_ARG_TGT_ARCH!"=="!Platform!" (
	echo "VsDevCmd already set to !Platform!"
) else (
	echo "calling VsDevCmd in the !scriptDirectory!!scriptName!"
	if /i "!Platform!"=="x86" (
		set PlatformTarget=x86
	) else (
		set PlatformTarget=x86_!Platform!
	)
	rem call VsDevCmd -arch=!PlatformTarget!
	cd /D "%VCINSTALLDIR%Auxiliary\Build"
	call vcvarsall.bat !PlatformTarget!
	cd /D %repositoryRoot%
)

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

cd /D "%repositoryRoot%prj\tests\monitor_test_qt"
if exist ".qmake.stash" del ".qmake.stash"
rem exit /b 0
if /i "%Configuration%" == "Debug" (
        echo "Debug build ++++++++++++++++++++++++"
    call "%QTDIR%\bin\qmake" CONFIG-=release CONFIG+=debug
	set "MakefileExtension=Debug"
) else (
    call "%QTDIR%\bin\qmake" CONFIG+=release CONFIG-=debug MONITOR_NO_MEMORY_ANALYZE=1
	set "MakefileExtension=Release"
)
if not "!ERRORLEVEL!"=="0" (exit /b !ERRORLEVEL!)
call nmake /f Makefile.monitor_test.win_%Platform%.!MakefileExtension!
if not "!ERRORLEVEL!"=="0" (exit /b !ERRORLEVEL!)


exit /b %ERRORLEVEL%

endlocal

::ENDLOCAL & (
::	set additionalPathUsedForDoocs=%additionalPath%
::	set MATLABPATH=%scriptDirectory%%sysAndArch%\mbin;%MATLABPATH%
::	set PYTHONPATH=%scriptDirectory%%sysAndArch%\pbin;%PYTHONPATH%
::	if not defined ENSHOST (
::		set ENSHOST=%defaultEnsHost%
::	)
::)
