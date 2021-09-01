@echo off

setlocal

call set_envvars.bat

for %%P in (Win32, x64) do (
	if not errorlevel 8 robocopy %ROBOCOPY_PARAMETERS% %QT_BASE_DIR%\%%P\bin                  ..\%%P\Release\              Qt5Core.dll Qt5Gui.dll Qt5Widgets.dll Qt5WinExtras..dll
	if not errorlevel 8 robocopy %ROBOCOPY_PARAMETERS% %QT_BASE_DIR%\%%P\plugins\imageformats ..\%%P\Release\imageformats\ *.dll /XF *d.dll
	if not errorlevel 8 robocopy %ROBOCOPY_PARAMETERS% %QT_BASE_DIR%\%%P\plugins\platforms    ..\%%P\Release\platforms\    qwindows.dll
	if not errorlevel 8 robocopy %ROBOCOPY_PARAMETERS% %QT_BASE_DIR%\%%P\plugins\iconengines  ..\%%P\Release\iconengines\  qsvgicon.dll
	
	if not errorlevel 8 robocopy %ROBOCOPY_PARAMETERS% %QT_BASE_DIR%\%%P\bin                  ..\%%P\Debug\                Qt5Cored.dll Qt5Guid.dll Qt5Widgetsd.dll Qt5WinExtrasd.dll
	if not errorlevel 8 robocopy %ROBOCOPY_PARAMETERS% %QT_BASE_DIR%\%%P\plugins\imageformats ..\%%P\Debug\imageformats\   *d.dll
	if not errorlevel 8 robocopy %ROBOCOPY_PARAMETERS% %QT_BASE_DIR%\%%P\plugins\platforms    ..\%%P\Debug\platforms\      qwindowsd.dll
	if not errorlevel 8 robocopy %ROBOCOPY_PARAMETERS% %QT_BASE_DIR%\%%P\plugins\iconengines  ..\%%P\Debug\iconengines\    qsvgicond.dll
)

if not "%1"=="nopause" pause

if errorlevel 8 exit /b
exit /b 0
