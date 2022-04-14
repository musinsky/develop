@echo off
rem Jan Musinsky <mucha@sunhe.jinr.ru>
rem 10-Oct-2005

set libcern=packlib.lib kernlib.lib mathlib.lib pawlib.lib graflib.lib geant321.lib grafX11.lib

rem without parameter or bad file name
if %1. == . goto exit
if not exist %1 goto exit

rem remove (if exist) previous exe and create new exe file
rem see: http://www.computerhope.com/call.htm
set exename=%~n1.exe

if exist %exename% del %exename%
df /nologo %1 %libcern%

if exist %exename% goto exe
echo.
echo Problem with compilation !!!
echo.
pause
goto exit

:exe
echo.
echo Compilation done, launching %exename% (or Ctrl-C)
echo.
pause
%exename%
echo.
echo %exename% done
echo.
pause

:exit
