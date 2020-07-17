#pragma once
#ifndef _LWS_WINSTRING_AFX_H
#define _LWS_WINSTRING_AFX_H

	#define UNUSED_VAR(x) ((void)(x))			// 通知编译器：变量x未被引用 

	#include <stdlib.h>
	#include <string.h>		// strlen, strcpy, strcat
	#include <wchar.h>		// wchar_t
	#include <windows.h>

	#include <iostream>		// support new (std::nothrow), std::bad_alloc
	#include <new>

	#include <stdarg.h>		// 支持可变参数
	#include <climits>		// 支持类型最大值

	//导入CRT库支持内存泄露测试
	#ifndef _CRTDBG_MAP_ALLOC
		#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>
		#ifdef _DEBUG
			#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
			// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
			// allocations to be of _CLIENT_BLOCK type
		#else
			#define DBG_NEW new
		#endif
	#endif


	// Code Page from https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers			
	#define TYPEID_WIDE_CHARACTER				1
	#define TYPEID_ASCII						0
	#define TYPEID_UTF8							65001
	#define TYPEID_UTF16_LittleEndian			1200	// Unicode - code page 1200	(it is the default utf16 in Windows except Windows NT 3.51 & Windows NT 4.0)
	#define TYPEID_UTF16_BigEndian				1201	//
	#define TYPEID_ISO_8859_1					28591	// 1 bytes
	#define TYPEID_GB18030						54936	// Windows XP and later: GB18030 Simplified Chinese (4 byte); Chinese Simplified (GB18030)
	#define TYPEID_GB2312						936		// ANSI/OEM 编码，即英文及英文符号采用ASCII单字节编码，中文和中文符号用GB2312双字节编码

	// 空间限制
	#define DWORD_BYTESNUM						2		// DWORD 占用2个字节
	#define WINSTRING_BYTES_MAX					1000	// WinString 最多含有的字节 
	#define WINSTRING_WCHAR_MAX					500		// WinString 最多含有的WChar数量

	// 额外的类型定义
	#define TYPEID_INT							100
	#define TYPEID_CHAR							200
	#define TYPEID_STR_BYTES					300
	#define TYPEID_STR_WCHAR					400
	#define TYPEID_WINSTRING_PTR				500

#endif