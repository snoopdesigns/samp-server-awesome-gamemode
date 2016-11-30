@echo off
echo ==========================
echo Building samp-server-awesome-gamemode
echo Using GIT base: %1
echo Using SAMP Server base: %2
echo ==========================
rmdir /s /q %1\target
if not exist %1\target mkdir %1\target
mkdir %1\target\gamemodes
mkdir %1\target\scriptfiles
%2\pawno\pawncc.exe -w200 -w217 -v0 -o%1\target\gamemodes\awesome-freeroam.amx %1\gamemodes\awesome-freeroam.pwn -; -(
xcopy /s /q /Y %1\scriptfiles %1\target\scriptfiles
xcopy /s /q /Y %1\target %2\