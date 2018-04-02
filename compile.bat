:: This automates the compiling of this app, requires MinGW g++ on the PATH variable
:: It also compiles the icon resource file in this directory, and adds it with the icon to the app
:: icon credit to xenatt: http://xenatt.deviantart.com/
@ECHO off

windres -O coff src/icon.rc -o icon.res
g++ -std=c++11 -o bin/ascew.exe src/main.cpp src/ascewapi.cpp src/config.cpp src/console.cpp icon.res
del icon.res
pause
