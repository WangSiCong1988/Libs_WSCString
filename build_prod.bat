@echo off
rd /s /q ".\prod"
md ".\prod"

xcopy /y /d ".\WinString_DLL\Release\WinString_DLL.lib" ".\prod"
xcopy /y /d ".\WinString_DLL\Release\WinString_DLL.dll" ".\prod"
xcopy /y /d ".\WinString_DLL\WinString_DLL.h" ".\prod"
