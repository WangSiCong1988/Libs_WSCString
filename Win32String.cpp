// WSCString.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<fstream>
#include "string.h"
#include "windows.h"

#include "./libs/windows/txt/Win32String.h"

void TestWChar(const wchar_t * msg){
	printf("%ls\n", msg);
	//printf("strlen(msg) = %d\n", strlen(msg));
	printf("wcslen(msg) = %d\n", wcslen(msg));
}

void WriteData2File(const char * fileName, char * data);	
void WriteData2File(const char * fileName, wchar_t * data);
void ShowCharArray(char * str);

int main(int argc, char* argv[])
{

	printf("Hello World!\n");
	printf("%s\n", NULL);
	printf("unsigned short is %d bytes, unsigned long is %d bytes, wchar_t is %d bytes\n", sizeof(unsigned short), sizeof(unsigned long), sizeof(wchar_t));

	wchar_t * ws = L"中文abc";
	printf("%ls has a size of %d\n", ws, sizeof(ws));


	TestWChar(ws);


	char str[] = "四个中文";
    printf( "字符串长度：%d，字节数：%d\n", strlen( str ), sizeof( str ) );
 
    wchar_t wstr[] = L"中文";
 
    printf( "宽字符串长度（wcslen）：%d，字节数：%d\n", wcslen( wstr ), sizeof( wstr ) );


	char * lpUTF8Str = NULL;
	// calculate the lenght of utf8 space we need
	// dwFlags = 0 -> or UTF-8 or code page 54936(GB18030, starting with Windows Vista)
	// cbMultiByte = -1 -> The process includes '\0\0'
	// cchWideChar = 0 -> we just calculate the space we need
	int txtLen = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	printf("The space we need is %d\n", txtLen);

	
	// Test Case 1
	printf("------------------\n");
	printf("------------------\n");
	printf("Test Case 1\n");
	// Create 3 strings
	Win32String * ws11 = Win32String::Create(); 
	Win32String * ws12 = Win32String::Create("Wang SiCong is handsome!");
	Win32String * ws13 = Win32String::Create("<html><head><meta charset=\"UTF-8\"></head>中文</html>");
	if(ws11 == NULL || ws12 == NULL || ws13 == NULL ){
		printf("Test Case 1 Strings' generation fails! OOM happens\n");
	}else{
		// Type test
		printf("ws11 type is ASCII? %s\n", ws11->TypeIsASCII() ? "True" : "False");
		printf("ws12 type is ASCII? %s\n", ws12->TypeIsASCII() ? "True" : "False");
		printf("ws13 type is WideChar? %s\n", ws13->TypeIsWideChar() ? "True" : "False");

		char * ws12_utf8 = NULL;
		ws12->Parse2UTF8(ws12_utf8);
		printf("ws12 parses to utf8 and it has a length of %d\nws12(utf8) is \"%s\"\n", strlen(ws12_utf8), ws12_utf8);

		char * ws13_utf8 = NULL;
		ws13->Parse2UTF8(ws13_utf8);
		printf("ws13 parses to utf8 and it has a length of %d\nws13(utf8) is \"%s\"\n", strlen(ws13_utf8), ws13_utf8);
		WriteData2File("Test_Case_1_ch2utf8.txt", ws13_utf8);
		//ShowCharArray(ws13_utf8);
	}

	
	// Release the space
	if(ws11 != NULL){
		delete ws11;
	}
	if(ws12 != NULL){
		delete ws12;
	}
	if(ws13 != NULL){
		delete ws13;
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
void WriteData2File(const char * fileName, char * data){
	FILE * file = fopen(fileName, "w+");

	// Set to UTF8
	char header[3] = {(char)0xEF, (char)0xBB, (char)0xBF};
	fwrite(header, sizeof(char), 3, file);

	fwrite( data, sizeof(char), strlen(data) + 1, file);
	fclose(file);
}
void WriteData2File(const char * fileName, wchar_t * data){
	FILE * file = fopen(fileName, "w+,ccs=UTF-8");
	fwrite( data, sizeof(wchar_t), wcslen(data) + 1, file);
	fclose(file);
}
void ShowCharArray(char * str){
	printf("The string is\n");
	for(int i = 0; i < strlen(str); i++){
		printf("%c: %d\n", str[i], str[i]);
	}
}