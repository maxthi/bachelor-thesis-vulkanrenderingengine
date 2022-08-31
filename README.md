A project I did during 2019/20.

# Disclaimer
This was the first time I came into contact with Vulkan. In hindsight, I would do things a bit different.
Compilation might need a bit of configuration beforehand.

# Possible errors
This file contains some of the errors that occured while working with the vre and shall serve as a quick look up in case the same error occurs again.

## QtWidgetsd.dll not found (or similar dll)
Error: The executable cannot find the the qt dlls.
Solution: Either copy the qt dlls manually next to the program or add the directoy of your qt installation to the path variable in windows.

## Could not find a package configuration file provided by "Qt5" with any of the following names: Qt5Config.cmake qt5-config.cmake
This means CMake can't automatically detect the cmake => Add your qt binaries to the system environment path variable. Example: C:\Qt\5.15.2\msvc2019_64\bin
RESTART Visual Studio afterwards, because visual studio keeps the path variables at startup.

## "could not find or load the Qt platform plugin "windows" in ""
Error: Multiple qt dlls available. This may happen if you have a path variable that references a directory with Qt Dlls (namely if MikTex is installed). Or
if you placed QtDlls next to the executable while having the path variable for qt added. 
https://forum.qt.io/topic/90293/could-not-find-or-load-the-qt-platform-plugin-windows-in/2
Solution: It has helped in this to remove the qt dlls from the executable directory if existent or remove paths that reference other qt dlls..

# Visual Studio
The following advices are helpful when developing with he visual studio environment.

## Enable Doxygen comments in VS
Go to Extras -> Options -> Text Editor -> C/C++ -> CodeStyle > General > Generated documentation comments style
and change them to Doxygen comments (///). This will enable automatic comment generation when typing ///.
