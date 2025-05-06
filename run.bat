@echo off
echo.
echo ------------------------------
echo         LOAD PROGRAM
echo ------------------------------
process
g++ -o testimage imagetransfernew.cpp lodepng.cpp

if %errorlevel% neq 0 (
    echo  Compilation failed for maze_converter!
    exit /b
)

echo Running mazedata...
testimage

if %errorlevel% neq 0 (
    echo  maze_converter execution failed!
    exit /b
)

echo.
echo ------------------------------
echo  Compiling testing...
echo ------------------------------
g++ -o astarnewvcl astar.cpp

if %errorlevel% neq 0 (
    echo  Compilation failed for testing!
    exit /b
)

echo.
echo ------------------------------
echo  Solving maze...
echo ------------------------------
astarnewvcl testcase1.txt

if %errorlevel% neq 0 (
    echo  Maze solving failed!
    exit /b
)

echo Maze solved successfully!

@REM echo.
@REM echo ------------------------------
@REM echo  Compiling process...
@REM echo ------------------------------
@REM g++ -o process process.cpp

if %errorlevel% neq 0 (
    echo  Compilation failed for process!
    exit /b
)


echo.
echo ------------------------------
echo  FINISH!
echo ------------------------------
pause
