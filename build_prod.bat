@echo off
rd /s /q ".\prod_TMP"
md ".\prod_TMP\x86"
md ".\prod_TMP\x64"

xcopy /y /d ".\WinString_DLL\Release\WinString_DLL.lib" ".\prod_TMP\x86"
xcopy /y /d ".\WinString_DLL\Release\WinString_DLL.dll" ".\prod_TMP\x86"
xcopy /y /d ".\WinString_DLL\x64\Release\WinString_DLL.lib" ".\prod_TMP\x64"
xcopy /y /d ".\WinString_DLL\x64\Release\WinString_DLL.dll" ".\prod_TMP\x64"
xcopy /y /d ".\Libs\WinString.h" ".\prod_TMP"
