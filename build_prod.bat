@echo off
rd /s /q ".\prod_TMP"
md ".\prod_TMP"

xcopy /y /d ".\WinString_DLL\Release\WinString_DLL.lib" ".\prod_TMP"
xcopy /y /d ".\WinString_DLL\Release\WinString_DLL.dll" ".\prod_TMP"
xcopy /y /d ".\WinString_DLL\WinString_DLL.h" ".\prod_TMP"
