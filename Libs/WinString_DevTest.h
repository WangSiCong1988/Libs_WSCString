#pragma once
#ifndef _LWS_WINSTRING_DEV_TEST_H
	#define _LWS_WINSTRING_DEV_TEST_H

	#define _LWS_WINSTRING_DEV		// 开启开发者模式

	#include <stdio.h>
	#include <stdlib.h>
	#include <io.h>					// support _access()
	#include <direct.h>				// support _mkdir()
	#include <string.h>				// strlen, strcat, strcpy

	#define CURPATH  "./Dist/"
	#define CURPATH_FOLDER "./Dist"

	// Memory check
	#define _CRTDBG_MAP_ALLOC
		#include <crtdbg.h>
		#ifdef _DEBUG
			#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
			// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
			// allocations to be of _CLIENT_BLOCK type
		#else
			#define DBG_NEW new
	#endif

	//-----------------------------------
	// Support Functions
	//-----------------------------------
	void PathCheckAndCreate() {
		if (_access(CURPATH, 0) != 0) {
			_mkdir(CURPATH_FOLDER);
		}
	}
	char * FilePathCreate(const char * fileName) {
		char * curpath = CURPATH;
		char * filepath = new char[strlen(curpath) + strlen(fileName) + 1];
		strcpy_s(filepath, strlen(curpath) + strlen(fileName) + 1, curpath);
		strcat_s(filepath, strlen(curpath) + strlen(fileName) + 1, fileName);
		return filepath;
	}
	void FilePathDelete(char * filepath) {
		delete[] filepath;
	}

	void WriteData2UTF8File(const char * fileName, char * data) {
		PathCheckAndCreate();

		char * filepath = FilePathCreate(fileName);
		FILE * file = NULL;
		fopen_s(&file, filepath, "w+");

		// Set to UTF8
		char header[3] = { (char)0xEF, (char)0xBB, (char)0xBF };
		fwrite(header, sizeof(char), 3, file);

		fwrite(data, sizeof(char), strlen(data) + 1, file);
		fclose(file);

		FilePathDelete(filepath);
	}
	void WriteData2UTF8File(const char * fileName, wchar_t * data) {
		PathCheckAndCreate();

		FILE * file = NULL;
		fopen_s(&file, fileName, "w+");

		// Set to UTF8
		char header[3] = { (char)0xEF, (char)0xBB, (char)0xBF };
		fwrite(header, sizeof(char), 3, file);

		fwrite(data, sizeof(wchar_t), wcslen(data) + 1, file);
		fclose(file);
	}
	void ShowCharArray(char * str) {
		printf("The string is\n");
		for (size_t i = 0; i < strlen(str); i++) {
			printf("%c: %d\n", str[i], str[i]);
		}
	}

	/**
	 * Test
	 */
	void WinStringTest() {
		// 使用CRT检测内存泄漏
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		// Test Case 1
		printf("------------------\n");
		printf("Test Case 1\n");
		// Create 3 strings
		WinString * ws11 = WinString::Create();
		WinString * ws12 = WinString::Create("Wang SiCong is handsome!");
		WinString * ws13 = WinString::Create("<html><head><meta charset=\"UTF-8\"></head>中文</html>");
		// Type test
		printf("ws11 type is ASCII? %s\n", ws11->TypeIsASCII() ? "True" : "False");
		printf("ws12 type is ASCII? %s\n", ws12->TypeIsASCII() ? "True" : "False");
		printf("ws13 type is ASCII? %s\n", ws13->TypeIsASCII() ? "True" : "False");
		
		char * ws12_utf8 = NULL;
		ws12_utf8 = ws12->Parse2UTF8();
		printf("ws12 parses to utf8 and it has a length of %d\n", strlen(ws12_utf8));
		printf("ws12(utf8) is \"%s\"\n", ws12_utf8);
		
		char * ws13_utf8 = NULL;
		ws13_utf8 = ws13->Parse2UTF8();
		printf("ws13 parses to utf8 and it has a length of %d\nws13(utf8) is \"%s\"\n", strlen(ws13_utf8), ws13_utf8);
		WriteData2UTF8File("Test_Case_1_ch2utf8.html", ws13_utf8);
		
		ws13_utf8 = ws13->Parse2UTF8();
		printf("Parse2UTF8()方法二次调用成功\n");
		//ShowCharArray(ws13_utf8);
		// Release the space
		WinString::Destroy(ws11);
		WinString::Destroy(ws12);
		WinString::Destroy(ws13);

		printf("------------------\n\n");

		printf("Type: %d\n", WinString::STR_TYPE_UNICODE);
		
		printf("------------------\n");
		printf("Test Case 2\n");
		WinString * ws21 = WinString::Create("Test Case2: Append 测试");
		printf("ws21 original content is: \"%s\"\n", ws21->GetBytes());
		ws21->Append(",这是追加的内容", WinString::STR_TYPE_ASCII);
		printf("After appending, ws21 content is: \"%s\"\n", ws21->GetBytes());
		WinString::Destroy(ws21);
		printf("------------------\n\n");


		//----------------------------------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------------------------------
		// Test cases 3
		//----------------------------------------------------------------------------------------------------------------------------
		const char * tc3FileName_utf8 = "TestCase3_utf8.txt";
		WinString * ws31 = NULL;
		DWORD dw31;
		wchar_t * ws31_wchar_t = NULL;
		char * ws31_utf8 = NULL;
		char * ws31_ascii = NULL;
		//----------------------------------------------------------------------------------------------------------------------------
		printf("------------------\n");
		printf("Test Case 3\n");
		ws31 = WinString::Create("Test Case3: Append 测试");
		printf("ws31 is: \"%s\"\n", ws31->GetBytes());
		dw31 = 16;
		ws31->Append(dw31);
		printf("ws31 (add a DWORD): \"%s\"\n", ws31->GetBytes());

		setlocale(LC_CTYPE, "chs");
		//setlocale(LC_CTYPE, "chinese_taiwan.950");
		//setlocale(LC_CTYPE, "chinese-traditional");
		ws31_wchar_t = ws31->Parse2WideChar();
		printf("1st time: ws31 transfer to wide char is \"%ls\"\n", ws31_wchar_t);
		ws31_wchar_t = NULL;
		ws31_wchar_t = ws31->Parse2WideChar();
		printf("2nd time: ws31 transfer to wide char\"%ls\"\n", ws31_wchar_t);
		setlocale(LC_CTYPE, "C");

		// 2 utf8
		ws31_utf8 = ws31->Parse2UTF8();
		printf("ws31 transers to utf8 is  \"%s\"\n", ws31_utf8);
		// utf8 chars written into the file
		WriteData2UTF8File(tc3FileName_utf8, ws31_utf8);
		printf("ws31 transers to utf8's result is saved in %s\n", tc3FileName_utf8);


		// 2 ascii
		ws31_ascii = NULL;
		ws31_ascii = ws31->Parse2ASCII();
		printf("1st: ws31 transers to ASCII is \"%s\"\n", ws31_ascii);
		ws31_ascii = NULL;
		ws31_ascii = ws31->Parse2ASCII();
		printf("2st: ws31 transers to ASCII is \"%s\"\n", ws31_ascii);

		//----------------------------------------------------------------------------------------------------------------------------
		WinString::Destroy(ws31);
		printf("------------------\n");
		//----------------------------------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------------------------------
		printf("\n");
		printf("\n");
		printf("\n");
	}
#endif