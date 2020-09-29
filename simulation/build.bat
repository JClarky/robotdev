@echo off
set /p v="Build version: "

g++ ./main/main.cpp -o ./builds/build_%v%