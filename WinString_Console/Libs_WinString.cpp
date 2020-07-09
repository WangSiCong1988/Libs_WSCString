// Libs_WinString.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <io.h>
#include <direct.h>
#include<fstream>
#include "string.h"
#include "windows.h"

#include "./libs/WinString.h"

#define CURPATH  "./Dist/"
#define CURPATH_FOLDER "./Dist"



void PathCheckAndCreate();
void WriteData2UTF8File(const char * fileName, char * data);	
void WriteData2UTF8File(const char * fileName, wchar_t * data);
void ShowCharArray(char * str);

int main(int argc, char* argv[])
{

	//printf("Hello World!\n");
	//printf("%s\n", NULL);
	//printf("unsigned short is %d bytes, unsigned long is %d bytes, wchar_t is %d bytes\n", sizeof(unsigned short), sizeof(unsigned long), sizeof(wchar_t));


	//char str[] = "四个中文";
    //printf( "字符串长度：%d，字节数：%d\n", strlen( str ), sizeof( str ) );
 
    //wchar_t wstr[] = L"中文";
 
    //printf( "宽字符串长度（wcslen）：%d，字节数：%d\n", wcslen( wstr ), sizeof( wstr ) );


	//char * lpUTF8Str = NULL;
	// calculate the lenght of utf8 space we need
	// dwFlags = 0 -> or UTF-8 or code page 54936(GB18030, starting with Windows Vista)
	// cbMultiByte = -1 -> The process includes '\0\0'
	// cchWideChar = 0 -> we just calculate the space we need
	//int txtLen = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	//printf("The space we need is %d\n", txtLen);

	
	// Test Case 1
	printf("------------------\n");
	printf("Test Case 1\n");
	// Create 3 strings
	WinString * ws11 = WinString::Create(); 
	WinString * ws12 = WinString::Create("Wang SiCong is handsome!");
	WinString * ws13 = WinString::Create("<html><head><meta charset=\"UTF-8\"></head>中文</html>");
	if(ws11 == NULL || ws12 == NULL || ws13 == NULL ){
		printf("Test Case 1 Strings' generation fails! OOM happens\n");
	}else{
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
		
	}
	// Release the space
	if(ws11){
		delete ws11;
	}
	if(ws12){
		delete ws12;
	}
	if(ws13){
		delete ws13;
	}
	
	printf("------------------\n\n");
	
	printf("------------------\n");
	printf("Test Case 2\n");
	WinString * ws21 = WinString::Create("Test Case2: Append 测试");
	printf("ws21 original content is: \"%s\"\n", ws21->GetBytes());
	ws21->Append(",这是追加的内容", WinString::TYPE_ASCII);
	printf("After appending, ws21 content is: \"%s\"\n", ws21->GetBytes());
	if(ws21){
		delete ws21;
	}
	printf("------------------\n\n");

	printf("------------------\n");
	printf("Test Case 3\n");
	WinString * ws31 = WinString::Create("Test Case3: Append 测试");
	printf("ws31 is: \"%s\"\n", ws31->GetBytes());
	DWORD dw31 = 16;
	ws31->Append(dw31);
	printf("ws31 (add a DWORD): \"%s\"\n", ws31->GetBytes());
	if(ws31){
		delete ws31;
	}
	printf("------------------\n");

	printf("\n");
	printf("\n");
	printf("\n");
	return 0;
}





//-----------------------------------
// Support Functions
//-----------------------------------
void PathCheckAndCreate(){
	if(_access(CURPATH, 0) != 0){
		_mkdir(CURPATH_FOLDER); 
	}
}
char * FilePathCreate(const char * fileName){
	char * curpath = CURPATH;
	char * filepath = new char[strlen(curpath) + strlen(fileName) + 1];
	strcpy(filepath, curpath);
	strcat(filepath, fileName);
	return filepath;
}
void FilePathDelete(char * filepath){
	delete[] filepath;
}

void WriteData2UTF8File(const char * fileName, char * data){
	PathCheckAndCreate();

	char * filepath = FilePathCreate(fileName);
	FILE * file = fopen(filepath, "w+");

	// Set to UTF8
	char header[3] = {(char)0xEF, (char)0xBB, (char)0xBF};
	fwrite(header, sizeof(char), 3, file);

	fwrite( data, sizeof(char), strlen(data) + 1, file);
	fclose(file);

	FilePathDelete(filepath);
}
void WriteData2UTF8File(const char * fileName, wchar_t * data){
	PathCheckAndCreate();

	FILE * file = fopen(fileName, "w+");

	// Set to UTF8
	char header[3] = {(char)0xEF, (char)0xBB, (char)0xBF};
	fwrite(header, sizeof(char), 3, file);

	fwrite( data, sizeof(wchar_t), wcslen(data) + 1, file);
	fclose(file);
}
void ShowCharArray(char * str){
	printf("The string is\n");
	for(int i = 0; i < strlen(str); i++){
		printf("%c: %d\n", str[i], str[i]);
	}
}