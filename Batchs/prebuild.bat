@echo off

for %%A in (Act_*.bat) do (
	call %%A nopause
	if errorlevel 8 exit /b
)

if not errorlevel 1 call update_translations.bat nopause
if not errorlevel 1 call build_translations.bat nopause

if errorlevel 1 exit /b
exit /b 0
