@echo off
rd /s /q ".\prod"
md ".\prod"
md ".\prod\img"
xcopy /y /d ".\.gitignore" ".\prod"
xcopy /y /d ".\LICENSE" ".\prod"
xcopy /y /d ".\README.md" ".\prod"
xcopy /y /d ".\img\*" ".\prod\img"

xcopy /y /d ".\WinString_DLL\Release\WinString_DLL.lib" ".\prod"
xcopy /y /d ".\WinString_DLL\Release\WinString_DLL.dll" ".\prod"
xcopy /y /d ".\WinString_DLL\WinString_DLL.h" ".\prod"
