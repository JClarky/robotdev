if not defined in_subprocess (cmd /k set in_subprocess=y ^& %0 %*) & exit )

@echo off
set /p v="Build version: "

g++ ./simulation/main/main.cpp ./simulation/main/simulation_interface.cpp -o ./simulation/builds/build_%v% -L C:/DEV/vcpkg/installed/x86-windows/include/json