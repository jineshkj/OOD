

@echo off

cls
echo Going to start Server process
pause
@echo on
start Debug\Server.exe

@echo off
echo Started the service
pause

cls
echo Contents of Downloads/ directory
dir Downloads /p
pause
echo Now going execute Tester application with 2 processes
pause
@echo on
Tester\bin\Debug\Tester.exe 2
@echo off
pause

cls
echo Going to start GUI for testing
pause
@echo on
start /WAIT GuiTester\bin\Debug\GuiTester.exe
@echo off
pause

cls
echo Going to kill the server process.
pause

taskkill /IM Server.exe
