@echo off
set FoundVSInstance=0

rem Change to directory where batch file is located.  We'll restore this later with "popd"
pushd %~dp0

for /F "tokens=3 delims= " %%G in ('REG QUERY "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders" /v "Personal"') do (set docsdir=%%G)

set natvisfile=PopcornFX.natvis

rem No VS141COMNTOOLS exists for vs2017
if exist "%docsdir%\Visual Studio 2017" (
  set FoundVSInstance=1
  mkdir "%docsdir%\Visual Studio 2017\Visualizers"
  copy %natvisfile% "%docsdir%\Visual Studio 2017\Visualizers"
  echo Installed visualizers for Visual Studio 2017
)

if "%VS140COMNTOOLS%" neq "" (
  set FoundVSInstance=1
  mkdir "%docsdir%\Visual Studio 2015\Visualizers"
  copy %natvisfile% "%docsdir%\Visual Studio 2015\Visualizers"
  echo Installed visualizers for Visual Studio 2015
)

if "%VS120COMNTOOLS%" neq "" (
  set FoundVSInstance=1
  mkdir "%docsdir%\Visual Studio 2013\Visualizers"
  copy %natvisfile% "%docsdir%\Visual Studio 2013\Visualizers"
  echo Installed visualizers for Visual Studio 2013
)

if "%VS110COMNTOOLS%" neq "" (
  set FoundVSInstance=1
  mkdir "%docsdir%\Visual Studio 2012\Visualizers"
  copy %natvisfile% "%docsdir%\Visual Studio 2012\Visualizers"
  echo Installed visualizers for Visual Studio 2012
)

if "%VS100COMNTOOLS%" neq "" (
  set FoundVSInstance=1
  mkdir "%docsdir%\Visual Studio 2010\Visualizers"
  copy %natvisfile% "%docsdir%\Visual Studio 2010\Visualizers"
  echo Installed visualizers for Visual Studio 2010
)

if "%SCE_ORBIS_SDK_DIR%" neq "" (
  rem set FoundVSInstance=1
  mkdir "%docsdir%\SCE\orbis-debugger"
  copy %natvisfile% "%docsdir%\SCE\orbis-debugger"
  echo Installed visualizers for SCE orbis-debugger
)

if "%FoundVSInstance%" equ "0" (
  echo ERROR: Could not find a valid version of Visual Studio installed (2010, 2012, 2013, 2015)
)

popd
pause
exit
