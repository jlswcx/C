@echo off


for /R %%i in (*.c) do (
 echo %%~ti
 set k=1324
 echo %k%
) 


pause