@echo off
set /p v="Build version: "

g++ ./simulation/main/main.cpp -o ./simulation/builds/build_%v%