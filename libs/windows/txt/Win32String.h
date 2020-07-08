#pragma once
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <iostream>		// Two libs for "new (std::nothrow)"
#include <new>

/*** Predefined Values ***/
#define Win32String_TYPE_ASCII								0
#define Win32String_TYPE_WIDE_CHARACTER						1
#define Win32String_TYPE_UTF8								2
#define Win32String_TYPE_UTF16_LittleEndian					3	// Unicode - code page 1200


// This STRING class is designed to transfer Unicode into UTF-8
// Chinese Character in ASCII is actually coded in GB2312
class Win32String{
private:
	static int defaultType;
	int type;

	// Pointers to storage different coding
	// They are only released in the deconstruction method
	wchar_t * lpWChar;			// WideChar Type (It is for any character in any language but only used in the memory)
	char * lpBytes;				// Bytes of the string (based on what code you choose)

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Construction
	Win32String(){
		this->lpWChar = NULL;
		this->lpBytes = NULL;
		this->type = Win32String::defaultType;	// The default type
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Init
	//
	// Init: An empty ASCII string
	bool Init(){
		this->lpBytes = new (std::nothrow) char[1];
		// Fail for OOM
		if(!this->lpBytes){
			return false;
		}
		// No OOM
		this->lpBytes[0] = '\0';
		this->type = Win32String_TYPE_ASCII;	// The empty string is ALWAYS ASCII
		return true;
	}
	// Init: An Default string
	bool Init(const char * str){
		// Empty Input, so an empty string
		if(!str){
			return Init();
		}
		// Space Calculation
		size_t bytesNum = 0;
		switch(this->type){
		case Win32String_TYPE_ASCII:
			bytesNum = this->STR_ASCIIBytesNum(str);
			break;
		case Win32String_TYPE_UTF8:
			bytesNum = this->STR_UTF8BytesNum(str);
			break;
		case Win32String_TYPE_UTF16_LittleEndian:
			bytesNum = this->STR_UTF16BytesNum(str);
			break;
		}

		// Has Input
		this->lpBytes = new (std::nothrow) char[bytesNum];
		// Fail for OOM
		if(!this->lpBytes){
			return false;
		}
		// No OOM
		this->STR_BytesCpy(this->lpBytes, str, bytesNum);
		return true;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------
	// Assitance Functions
	//-------------------------------------------------------------------------------
	size_t STR_ASCIIBytesNum(const char * bytes);
	size_t STR_UTF8BytesNum(const char * bytes);
	size_t STR_UTF16BytesNum(const char * bytes);
	void STR_BytesCpy(char * dest, char * source, size_t len);
	void STR_BytesCpy(char * dest, const char * source, size_t len);
	void STR_BytesCat(char * dest, char * source, size_t beginIdx, size_t len);
	void STR_BytesCat(char * dest, const char * source, size_t beginIdx, size_t len);
public:
	/*** Coding Types ***/
	const static int TYPE_ASCII;
	const static int TYPE_WIDE_CHARACTER;
	const static int TYPE_UTF8;
	const static int TYPE_UTF16_LittleEndian;
	const static int TYPE_UNICODE_PAGE1200;
	
	//-----------------------------------------------------------------------------
	// SetDefaultType
	//-----------------------------------------------------------------------------
	static void SetDefaultType(const int type){
		if(	type == Win32String_TYPE_ASCII || 
			type == Win32String_TYPE_WIDE_CHARACTER || 
			type == Win32String_TYPE_UTF8 ||
			type == Win32String_TYPE_UTF16_LittleEndian){
			Win32String::defaultType = type;
		}
	}

	//-----------------------------------------------------------------------------
	// Deconstruct
	~Win32String(){
		if(this->lpWChar != NULL){
			delete[] this->lpWChar;
		}
		if(this->lpBytes != NULL){
			delete[] this->lpBytes;
		}
	}
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// Create
	//
	// Create: An empty ASCII string
	static Win32String * Create(){
		Win32String * lpWin32Str = new (std::nothrow) Win32String();
		// OOM
		if(!lpWin32Str){
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
	// Create: An Default String
	static Win32String * Create(const char * str){
		Win32String * lpWin32Str = new (std::nothrow) Win32String();
		// OOM
		if(!lpWin32Str){
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
	
	/**
	 * Add extra bytes inside
	 * @param bytes: the bytes to be appended
	 * @param type: the extra bytes' type. If it does not fit the type of old bytes, return false
	 */
	bool Append(const char * bytes, int type){
		// Refuse the different type coding
		if(type != this->type){
			return false;
		}
		// Empty Bytes alway true
		if(bytes == NULL){
			return true;
		}
		// Calculate the space
		size_t curNeededBytesNum = 0;		// no space for '\0' or '\0\0'
		size_t extraBytesNum = 0;
		switch(this->type){
		case Win32String_TYPE_ASCII:
			curNeededBytesNum = this->STR_ASCIIBytesNum(this->lpBytes) - 1;
			extraBytesNum = this->STR_ASCIIBytesNum(bytes);
			break;
		case Win32String_TYPE_UTF8:
			curNeededBytesNum = this->STR_UTF8BytesNum(this->lpBytes) - 1;
			extraBytesNum = this->STR_UTF8BytesNum(bytes);
			break;
		case Win32String_TYPE_UTF16_LittleEndian:
			curNeededBytesNum = this->STR_UTF16BytesNum(this->lpBytes) - 2;
			extraBytesNum = this->STR_UTF16BytesNum(bytes);
			break;
		}
		// Create new space
		char * lpBytesTMP = new (std::nothrow) char[curNeededBytesNum + extraBytesNum];
		if(!lpBytesTMP){
			return false;					// OOM
		}
		// Copy
		this->STR_BytesCpy(lpBytesTMP, this->lpBytes, curNeededBytesNum);
		this->STR_BytesCat(lpBytesTMP, bytes, curNeededBytesNum, extraBytesNum);
		// Free old space & link it to the new one
		delete[] this->lpBytes;
		this->lpBytes = lpBytesTMP;
		return true;
	}

	/**
	 * Get the bytes
	 */
	char * GetBytes(){
		return this->lpBytes;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Type
	//
	// Type: Is WideChar
	bool TypeIsWideChar(){
		return this->type == Win32String_TYPE_WIDE_CHARACTER;
	}
	// Type: Is ASCII
	bool TypeIsASCII(){
		return this->type == Win32String_TYPE_ASCII;
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
		wchar_t * lpWCTemp = NULL;
		char * lpBytesTemp = NULL;
		int iWCStrSpaceLen = 0;			// This length must include '\0\0'
		int iUTF8StrSpaceLen = 0;		// This length must incldue '\0'
		bool bTransRes = false;			// Default is false


		// Transfer - based on the type
		switch(this->type){
		case Win32String_TYPE_ASCII:
			// To Wide Char
			iWCStrSpaceLen = MultiByteToWideChar(CP_ACP, 0, this->lpBytes, -1, NULL, 0);
			if(!iWCStrSpaceLen){
				break;				// WideChar Space Calculation Fails
			}
			lpWCTemp = new (std::nothrow) wchar_t[iWCStrSpaceLen];
			if(!lpWCTemp){
				break;				// OOM
			}
			if(!MultiByteToWideChar(CP_ACP, 0, this->lpBytes, -1, lpWCTemp, iWCStrSpaceLen)){
				break;				// WideChar Transfer Fails
			}
			// To utf-8
			iUTF8StrSpaceLen = WideCharToMultiByte(CP_UTF8, 0, lpWCTemp, -1, NULL, 0, NULL, NULL);
			if(!iUTF8StrSpaceLen){
				break;				// UTF8 Space Calculation Fails
			}
			lpBytesTemp = new (std::nothrow) char[iUTF8StrSpaceLen];
			if(!lpBytesTemp){
				break;				// OOM
			}
			if(!WideCharToMultiByte(CP_UTF8, 0, lpWCTemp, -1, lpBytesTemp, iUTF8StrSpaceLen, NULL, NULL)){
				break;				// Tranfer 2 UTF8 fails
			}
			// We succeed here
			delete[] this->lpBytes;
			this->lpBytes = lpBytesTemp;
			lpBytesTemp = NULL;
			bTransRes = true;
			break;
		case Win32String_TYPE_WIDE_CHARACTER:
			iUTF8StrSpaceLen = WideCharToMultiByte(CP_UTF8, 0, this->lpWChar, -1, NULL, 0, NULL, NULL);
			if(!iUTF8StrSpaceLen){
				break;			// Transfer 2 UTF8 (Space Calculation) fails
			}
			// Free bytes space if it exists
			if(!this->lpBytes){
				delete[] this->lpBytes;
			}
			this->lpBytes = new (std::nothrow) char[iUTF8StrSpaceLen];
			if(!this->lpBytes){
				break;			// OOM
			}
			if(!WideCharToMultiByte(CP_UTF8, 0, this->lpWChar, -1, this->lpBytes, iUTF8StrSpaceLen, NULL, NULL)){
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
		// Release Temperal Variables
		if(!lpWCTemp){
			delete[] lpWCTemp;
		}
		// Return
		return bTransRes;
	}
	// Parse: to UTF8
	// @lpUTF8: the pointer from the outside to storage data
	bool Parse2UTF8(char * & lpUTF8){
		bool bParseRes = this->Parse2UTF8();
		if(bParseRes){
			lpUTF8 = this->lpBytes;
		}
		return bParseRes;
	}
	// Parse: to WideChar
	bool Parse2WideChar(){
		// No transfer if it is
		if(this->TypeIsWideChar()){
			return true;
		}

		// Variable Defination
		int iWCStrSpaceLen = 0;			// This length must include '\0\0'
		// WChar Space clean
		if(this->lpWChar != NULL){
			delete[] this->lpWChar;
			this->lpWChar = NULL;
		}
		// Chose the code page before transfer
		UINT uiCodePage = -1;
		switch(this->type){
		case Win32String_TYPE_ASCII:
			uiCodePage = CP_ACP;
			break;
		case Win32String_TYPE_UTF8:
			uiCodePage = CP_UTF8;
			break;
		}
		// Space Calculation
		iWCStrSpaceLen = iWCStrSpaceLen = MultiByteToWideChar(uiCodePage, 0, this->lpBytes, -1, NULL, 0);
		if(!iWCStrSpaceLen){
			return false;		//  Space calculation fails
		}
		this->lpWChar = new (std::nothrow) wchar_t[iWCStrSpaceLen];
		if(!this->lpWChar){
			return false;		// OOM
		}
		if(!MultiByteToWideChar(CP_ACP, 0, this->lpBytes, -1, this->lpWChar, iWCStrSpaceLen)){
			delete[] this->lpWChar;
			this->lpWChar = NULL;
			return false;		// Tranfer 2 WideChar fails
		}

		// We succeed here
		return true;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
};


//------------------------------------------------------------------------------------------
// Init static Variables
//------------------------------------------------------------------------------------------
int Win32String::defaultType = Win32String_TYPE_ASCII;									// The default type is ASCII

const int Win32String::TYPE_ASCII = Win32String_TYPE_ASCII;
const int Win32String::TYPE_WIDE_CHARACTER = Win32String_TYPE_WIDE_CHARACTER;
const int Win32String::TYPE_UTF8 = Win32String_TYPE_UTF8;
const int Win32String::TYPE_UTF16_LittleEndian = Win32String_TYPE_UTF16_LittleEndian;
const int Win32String::TYPE_UNICODE_PAGE1200 = Win32String_TYPE_UTF16_LittleEndian;

//------------------------------------------------------------------------------------------
// Assistance Functions
//------------------------------------------------------------------------------------------
size_t Win32String::STR_ASCIIBytesNum(const char * bytes){
	return strlen(bytes) + 1;
}
size_t Win32String::STR_UTF8BytesNum(const char * bytes){
	return strlen(bytes) + 1;
}
size_t Win32String::STR_UTF16BytesNum(const char * bytes){
	if(!bytes){
		return 0;
	}
	size_t len = 0;
	while(bytes[len] != '\0' && bytes[len + 1] != '\0'){
		len += 1;
	}
	return len + 2;	// The extra space for '\0\0'
}
void Win32String::STR_BytesCpy(char * dest, char * source, size_t len){
	size_t i;
	for(i = 0; i < len; i++){
		dest[i] = source[i];
	}
}
void Win32String::STR_BytesCpy(char * dest, const char * source, size_t len){
	Win32String::STR_BytesCpy(dest, (char * )source, len);
}
void Win32String::STR_BytesCat(char * dest, char * source, size_t beginIdx, size_t len){
	size_t i;
	for(i = 0; i < len; i++){
		dest[i + beginIdx] = source[i];
	}
}
void Win32String::STR_BytesCat(char * dest, const char * source, size_t beginIdx, size_t len){
	Win32String::STR_BytesCat(dest, (char *)source, beginIdx, len);
}