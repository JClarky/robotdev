@echo off
set /p v="Build version: "

g++ ./simulation/main/main.cpp ./simulation/main/simulation_interface.cpp -o ./simulation/builds/build_%v%