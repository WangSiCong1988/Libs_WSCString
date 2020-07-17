// WinString_DevConsole.cpp : 定义控制台应用程序的入口点。
//
#pragma once
#include "stdafx.h"

#define _LWS_WINSTRING_DEV		// Winstring.h 开启开发者模式

#include "WinString_Afx.h"
#include "WinString_SupFunc.h"
#include "WinString.h"
#include "WinString_Dev.h"

#include "WinString_DevTest.h"

int main()
{

	WinStringTest();
	return 0;
}


