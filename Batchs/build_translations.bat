@echo off

echo [93mBuilding translations...[0m
echo.

setlocal

call set_envvars.bat

pushd ..\QSampleDLL\translations

rem Check for unfinished translations
findstr /spin /c:"type=\"unfinished\"" /A:0C *.ts
if not errorlevel 1 (
	if not "%1"=="nopause" pause
	exit /b 1
)

rem Create .qm files
for /r %%F in (*.ts) do (
	%QT_BASE_DIR%\x64\bin\lrelease -compress %%F -qm %%~nF.qm
	if errorlevel 1 (
		if not "%1"=="nopause" pause
		exit /b 1
	)
)
popd

if not "%1"=="nopause" pause
exit /b 0
