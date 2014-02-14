@echo off

cls
echo Running Executive.exe without any arguments will display the usage
pause

@echo on
Debug\Executive.exe
@echo off
pause

cls
echo Running Executive.exe with a single filename as pattern and /s and /b options
pause
@echo on
Debug\Executive.exe . Parser.cpp /s /b
@echo off
pause

cls
echo Running Executive.exe with a single filename as pattern and with /s and without /b option
pause
@echo on
Debug\Executive.exe . Parser.cpp /s
@echo off
pause

cls
echo GET READY FOR THE BIG GAME.
echo RUNNING EXECUTIVE.EXE WITH /S OPTION (WITHOUT /B OPTION) and *.CPP and *.H as patterns
pause
@echo on
Debug\Executive.exe . *.cpp *.h /s
@echo off
pause

cls
@echo HOPE YOU HAD A GOOD TIME !! GOOD BYE :)
