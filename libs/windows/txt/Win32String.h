#pragma once
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <iostream>		// Two libs for "new (std::nothrow)"
#include <new>

/*** Predefined Values ***/
#define Win32String_TYPE_ANSCII								0
#define Win32String_TYPE_WIDE_CHARACTER						1
#define Win32String_TYPE_UTF8								2
#define Win32String_TYPE_UTF16_LittleEndian					3	// Unicode - code page 1200


// This STRING class is designed to transfer Unicode into UTF-8
// Chinese Character in ASCII is actually coded in GB2312
class Win32String{
private:
	int type;

	// Pointers to storage different coding
	// They are only released in the deconstruction method
	wchar_t * lpWChar;			// WideChar Type (It is for any character in any language but only used in the memory)
	char * lpAsciiStr;			// Ascii Code
	char * lpUTF8Str;			// UTF-8 code

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Construction
	Win32String(){
		this->lpAsciiStr = NULL;
		this->lpWChar = NULL;
		this->lpUTF8Str = NULL;
		this->type = Win32String_TYPE_ANSCII;	// The default type is ASCII
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Init
	//
	// Init: Empty String
	bool Init(){
		this->lpAsciiStr = new (std::nothrow) char[1];
		// Fail for OOM
		if(this->lpAsciiStr == NULL){
			return false;
		}
		// No OOM
		this->lpAsciiStr[0] = '\0';
		this->type = Win32String_TYPE_ANSCII;	// The default type is ASCII
		return true;
	}
	// Init: An Initial Default string (depending which code you select)
	bool Init(const char * str){
		// Empty Input, so an empty string
		if(str == NULL){
			return Init();
		}
		// Has Input
		this->lpAsciiStr = new (std::nothrow) char[strlen(str) + 1];	// 1 more space for '\0'
		// Fail for OOM
		if(this->lpAsciiStr == NULL){
			return false;
		}
		// No OOM
		strcpy(this->lpAsciiStr, str);
		this->type = Win32String_TYPE_ANSCII;
		return true;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
public:
	/*** Coding Types ***/
	const static int TYPE_ANSCII;
	const static int TYPE_WIDE_CHARACTER;
	const static int TYPE_UTF8;
	const static int TYPE_UTF16_LittleEndian;
	const static int TYPE_UNICODE_PAGE1200;
	

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Deconstruct
	~Win32String(){
		if(this->lpAsciiStr != NULL){
			delete[] this->lpAsciiStr;
		}
		if(this->lpWChar != NULL){
			delete[] this->lpWChar;
		}
		if(this->lpUTF8Str != NULL){
			delete[] this->lpUTF8Str;
		}
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Create
	//
	// Create: An empty string
	static Win32String * Create(){
		Win32String * lpWin32Str = new (std::nothrow) Win32String();
		// OOM
		if(lpWin32Str == NULL){
			return NULL;
		}
		// Init fails
		if(!lpWin32Str->Init()){
			delete lpWin32Str;
			return NULL;
		}
		// All good
		return lpWin32Str; 
	}
	// Create: An Initial ASCII String
	static Win32String * Create(const char * str){
		Win32String * lpWin32Str = new (std::nothrow) Win32String();
		// OOM
		if(lpWin32Str == NULL){
			return NULL;
		}
		// Init fails
		if(!lpWin32Str->Init(str)){
			delete lpWin32Str;
			return NULL;
		}
		// All good
		return lpWin32Str;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Type
	//
	// Type: Is WideChar
	bool TypeIsWideChar(){
		return this->type == Win32String_TYPE_WIDE_CHARACTER;
	}
	// Type: Is ASCII
	bool TypeIsASCII(){
		return this->type == Win32String_TYPE_ANSCII;
	}
	// Type: Is UTF8
	bool TypeIsUTF8(){
		return this->type == Win32String_TYPE_UTF8;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Parse
	// Parse: to UTF8
	bool Parse2UTF8(){
		// No transfer if it is
		if(this->TypeIsUTF8()){
			return true;
		}
		// Transfer - variales defination
		int iUTF8StrSpaceLen = 0;		// This length must incldue '\0'
		bool bTransRes = false;			// Default is false
		// Space clean
		if(this->lpUTF8Str != NULL){
			delete[] this->lpUTF8Str;
			this->lpUTF8Str = NULL;
		}
		// Transfer - based on the type
		switch(this->type){
		case Win32String_TYPE_ANSCII:
			iUTF8StrSpaceLen = strlen(this->lpAsciiStr) + 1;
			this->lpUTF8Str = new (std::nothrow) char[iUTF8StrSpaceLen];
			if(!this->lpUTF8Str){
				break;				// OOM
			}
			strcpy(this->lpUTF8Str, this->lpAsciiStr);	// ASCII and UTF-8 are the same
			
			// We succeed here
			bTransRes = true;
			break;
		case Win32String_TYPE_WIDE_CHARACTER:
			iUTF8StrSpaceLen = WideCharToMultiByte(CP_UTF8, 0, this->lpWChar, -1, NULL, 0, NULL, NULL);
			if(iUTF8StrSpaceLen == 0){
				break;			// Transfer 2 UTF8 (Space Calculation) fails
			}
			this->lpUTF8Str = new (std::nothrow) char[iUTF8StrSpaceLen];
			if(!this->lpUTF8Str){
				break;			// OOM
			}
			if(WideCharToMultiByte(CP_UTF8, 0, this->lpWChar, -1, this->lpUTF8Str, iUTF8StrSpaceLen, NULL, NULL) == 0){
				break;			// Tranfer 2 UTF8 fails
			}
			// We succeed here
			bTransRes = true;
			break;
		}
		// If succeed, set type to UTF8
		if(bTransRes){
			this->type = Win32String_TYPE_UTF8;
		}
		// Return
		return bTransRes;
	}
	// Parse: to UTF8
	// @lpUTF8: the pointer from the outside to storage data
	bool Parse2UTF8(char * & lpUTF8){
		bool bParseRes = this->Parse2UTF8();
		if(bParseRes){
			lpUTF8 = this->lpUTF8Str;
		}
		return bParseRes;
	}
	// Parse: to WideChar
	bool Parse2WideChar(){
		// No transfer if it is
		if(this->TypeIsWideChar()){
			return true;
		}

		int iWCStrSpaceLen = 0;			// This length must include '\0\0'
		bool bTransRes = false;			// Default is false
		// Space clean
		if(this->lpWChar != NULL){
			delete[] this->lpWChar;
			this->lpWChar = NULL;
		}
		
		// Transfer - based on the type
		switch(this->type){
		case Win32String_TYPE_ANSCII:
			iWCStrSpaceLen = iWCStrSpaceLen = MultiByteToWideChar(CP_ACP, 0, this->lpAsciiStr, -1, NULL, 0);
			if(!iWCStrSpaceLen){
				break;				//  Space calculation fails
			}
			this->lpWChar = new (std::nothrow) wchar_t[iWCStrSpaceLen];
			if(!this->lpWChar){
				break;				// OOM
			}
			if(!MultiByteToWideChar(CP_ACP, 0, this->lpAsciiStr, -1, this->lpWChar, iWCStrSpaceLen)){
				break;				// Tranfer 2 WideChar fails
			}
			// We succeed here
			bTransRes = true;
			break;
		case Win32String_TYPE_UTF8:
			break;
		}
		
		return bTransRes;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
};


//------------------------------------------------------------------------------------------
// Init static Variables
//------------------------------------------------------------------------------------------
const int Win32String::TYPE_ANSCII = Win32String_TYPE_ANSCII;
const int Win32String::TYPE_WIDE_CHARACTER = Win32String_TYPE_WIDE_CHARACTER	;
const int Win32String::TYPE_UTF8 = Win32String_TYPE_UTF8;
const int Win32String::TYPE_UTF16_LittleEndian = Win32String_TYPE_UTF16_LittleEndian;
const int Win32String::TYPE_UNICODE_PAGE1200 = Win32String_TYPE_UTF16_LittleEndian;
