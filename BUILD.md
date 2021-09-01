# Building projects

## Required tools

- Visual Studio 2017 or higher (requires C++17), with Windows SDK 8.1 available.
- Qt 5.14 or higher (though not tested with Qt 6 yet). Requires 32 and 64 bits by default.
- Qt VS Tools 2.7.1 or higher.
- Python 3.7 or higher (used for some minor scripts).

## Environment

- Python interpreter must be in PATH.
- The script `set_envvars.bat` defines the basic environment variables. Their default values matches the build environment at STT, but can be modified for your own.

- `QT_BASE_DIR` defines the path to the base installation of Qt (inside there should be a `Win32` and `x64` installations).
- `ROBOCOPY_PARAMETERS` default parameters for robocopy.

At STT Systems the following variables are defined:

- `TOOLSET_VERSION`: current toolset version used. `141` for VS 2017, for example.
- `QT_DIR`: root for all the installations for a given Qt version. It contains a subdirectory for each platform toolset supported, and inside a directory for each platform. For example, two installations of Qt 5.14.2 (32 y 64 bits) for toolset 141: `C:\Qt\qt_5_14_2\v141\Win32` and `C:\Qt\qt_5_14_2\v141\x64`, then `QT_DIR=C:\Qt\qt_5_14_2`.

## Build

- Run `Batchs/prebuild.bat` to copy all the dependencies, DLLs from Qt and compile translations.
- If you have to update and build translations in the future, you can execute the `Batchs/update_translations.bat` and `Batchs/build_translations.bat` respectively.
- Open the solution and compile `MFCApplication` (it is the main project and it will compile all the dependencies).
