

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
echo Going to list the contents of Downloads/ directory
pause
dir Downloads /p
pause

cls
del Downloads\a.txt
echo Going to check if Downloads\a.txt file exists
pause
dir Downloads\a.txt
pause
echo Now going to transfer a.txt from current directory
pause
@echo on
Debug\Client.exe 1 1 PutFile a.txt
@echo off
pause
echo Going to check to see if Downloads\a.txt exists
pause
dir Downloads\a.txt
pause

cls
echo Going to search for the string "hello world" using Client.exe using 1 thread
pause
@echo on
Debug\Client.exe 1 1 SearchString "hello world"
@echo off
pause
echo Going to search for the string "hello world" using Client.exe using 2 threads
pause
@echo on
Debug\Client.exe 1 2 SearchString "hello world"
@echo off
pause
echo Going to search for the string "hello crazy world" using Client.exe using 2 threads with 2 iterations
pause
@echo on
Debug\Client.exe 2 2 SearchString "hello crazy world"
@echo off
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
