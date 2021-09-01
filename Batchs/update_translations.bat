@echo off

echo [93mUpdating translations...[0m
echo.

setlocal

call set_envvars.bat

set SOLUTION_DIR=%~dp0\..\

pushd ..\QSampleDLL\translations
for /r %%F in (*.ts) do (
	%QT_BASE_DIR%\x64\bin\lupdate -no-recursive -no-obsolete %SOLUTION_DIR%\QSampleDLL -ts %%F
	if errorlevel 1 (
		if not "%1"=="nopause" pause
		exit /b 1
	)
)

python %SOLUTION_DIR%/Batchs/crlf.py *.ts -w -t

popd

if not "%1"=="nopause" pause
exit /b 0
