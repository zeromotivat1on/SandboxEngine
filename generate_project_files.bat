@echo off
set "build_dir=build"

REM Check if the directory exists
if not exist "%build_dir%" (
    echo Directory "%build_dir%" does not exist. Creating it...
    mkdir "%build_dir%"
)

REM Navigate into the directory
cd "%build_dir%"

REM Run CMake with the specified configuration
cmake ..
