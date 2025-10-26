REM testing set for and /a
@echo off
echo input files:
dir /B *.ccsds *.dds
set /a cnt=1
for  %%G in (*.exe) do ( call :sub1 %%G )
choice /C:123456789 /M "select one number"
if %ERRORLEVEL% EQU 1 countDDSpackets.exe 
if %ERRORLEVEL% EQU 2 countpackets.exe
if %ERRORLEVEL% EQU 3 dds2ccsds.exe 
if %ERRORLEVEL% EQU 4 readagilepackets.exe 
if %ERRORLEVEL% EQU 5 readDDSpackets.exe
if %ERRORLEVEL% EQU 6 readpackets.exe
if %ERRORLEVEL% EQU 7 writeagilepackets.exe
if %ERRORLEVEL% EQU 8 writeDDSpackets.exe
if %ERRORLEVEL% EQU 9 writepackets.exe
goto :eof
:sub1
echo %cnt% %1
set /a cnt = %cnt% + 1
goto :eof
