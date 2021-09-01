@echo off

rem Directory where Qt installations are installed
set QT_BASE_DIR=%QT_DIR%\v%TOOLSET_VERSION%

rem Optional set of parameters for robocopy
set ROBOCOPY_PARAMETERS=/NFL /NDL /NJS /NC /NS /NP /FFT /W:5
