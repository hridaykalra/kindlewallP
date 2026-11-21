@echo off
setlocal
cd /d "%~dp0\.."
where qmake >nul 2>nul
if errorlevel 1 (
  echo qmake was not found. Run this from the Qt 6 MSVC command prompt.
  exit /b 1
)
qmake kindlewall.pro CONFIG+=release -o Makefile
if errorlevel 1 exit /b %errorlevel%
nmake
if errorlevel 1 exit /b %errorlevel%
echo Build complete. Run windeployqt kindlewall.exe before distributing.
