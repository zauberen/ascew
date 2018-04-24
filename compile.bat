:: This automates the compiling of this app, requires MINGW to be in the default install path.
:: MinGW/bin needs to be on the windows path for this to work still
:: It also compiles the icon resource file in this directory, and adds it with the icon to the app
:: icon credit to xenatt: http://xenatt.deviantart.com/
@ECHO off

C:/mingw/bin/windres -O coff src/icon.rc -o icon.res
C:/mingw/bin/g++ -std=c++11 -o bin/ascew.exe src/main.cpp src/ascewapi.cpp src/config.cpp src/console.cpp icon.res
del icon.res
pause
