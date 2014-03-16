@echo off

cls
echo Running Executive.exe without any arguments will display the usage
pause

@echo on
Debug\Executive.exe
@echo off
pause

cls
echo Running Executive.exe with 2 test files and with /s and without /d option
pause
@echo on
Debug\Executive.exe Test *.cpp *.h /S /l3
@echo off
pause

cls
echo Running Executive.exe with 2 test files and with /s and /d option
pause
@echo on
Debug\Executive.exe Test *.cpp *.h /S /d /l3
@echo off
pause

cls
echo GET READY FOR THE BIG GAME.
echo RUNNING EXECUTIVE.EXE WITH /S and /d OPTIONS  and *.CPP and *.H as patterns
pause
@echo on
Debug\Executive.exe . *.cpp *.h /S /d /l5
@echo off
pause

cls
@echo HOPE YOU HAD A GOOD TIME !! GOOD BYE :)
