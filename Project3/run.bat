

@echo off

cls
echo Delete *.txt from Downloads/ directory
pause
del /Q Downloads\*.txt

cls
echo Going to start File Transfer service
pause
@echo on
start Debug\ConsoleApplication2.exe

@echo off
echo Started the service
pause

cls
echo Contents of Downloads/ directory
dir Downloads
pause
echo Now going to transfer a.txt from current directory
pause
@echo on
Debug\ConsoleApplication1.exe a.txt
@echo off
pause
echo Contents of Downloads/ directory
dir Downloads
pause

cls
echo Contents of Downloads/ directory
dir Downloads
pause
echo Now going to transfer b.txt from current directory
pause
@echo on
Debug\ConsoleApplication1.exe b.txt
@echo off
pause
echo Contents of Downloads/ directory
dir Downloads
pause

taskkill /IM ConsoleApplication2.exe
