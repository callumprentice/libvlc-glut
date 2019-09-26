@rem make c:\vlc_builds\vlc-3.0.8
@rem or 
@rem make .\vlc\vlc-3.0.8

@set BUILD_DIR=build64
@set VLC_DIR=%~f1
@if [%VLC_DIR%]==[] (
    @echo.
    @echo *** ERROR: You must specify a VLC directory to build against ***
    @goto End
)
@if exist %BUILD_DIR% (
    @echo.
    @echo *** ERROR: The build directory '%BUILD_DIR%' exists - remove before running this script ***
    @goto End
)     
@echo Building in '%BUILD_DIR%'' and using VLC from '%VLC_DIR%'
@pushd .
@set CMAKE_GENERATOR="Visual Studio 15 2017 Win64"
@rem if exist build64 rmdir /s /q build64
@mkdir %BUILD_DIR%
@cd %BUILD_DIR%
@cmake -G %CMAKE_GENERATOR% .. -DVLC_DIR=%VLC_DIR%
@if errorlevel 0 start libvlc.sln
@popd
:End