@echo off
@cd /d D:\ugame_c\SimpleScriptSystem\sss
@echo CD is "%CD%"

@rmdir /S /Q obj > nul 2>&1
@rmdir /S /Q native-activity\obj > nul 2>&1
@rmdir /S /Q native-activity\libs > nul 2>&1
@rmdir /S /Q native-activity\bin > nul 2>&1

@del /S /Q python-2.2.2-android\*.exe > nul 2>&1
@del /S /Q python-2.2.2-android\*.a > nul 2>&1
@del /S /Q python-2.2.2-android\Modules\*.o > nul 2>&1
@del /S /Q python-2.2.2-android\Objects\*.o > nul 2>&1
@del /S /Q python-2.2.2-android\Parser\*.o > nul 2>&1
@del /S /Q python-2.2.2-android\Python\*.o > nul 2>&1

@pause
@echo on