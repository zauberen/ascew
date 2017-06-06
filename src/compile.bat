:: This automates the compiling of this app, requires MinGW g++ on the PATH variable
:: It also compiles the icon resource file in this directory, and adds it with the icon to the app
:: icon credit to xenatt: http://xenatt.deviantart.com/
@ECHO off

windres windres icon.rc -O coff -o icon.res
g++ -o ascew.exe main.cpp config.cpp icon.res
pause