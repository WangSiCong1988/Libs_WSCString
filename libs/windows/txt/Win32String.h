#pragma once
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include <iostream>		// Two libs for "new (std::nothrow)"
#include <new>

/*** Predefined Values ***/
// Code Page from https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
#define Win32String_TYPE_WIDE_CHARACTER						1
#define Win32String_TYPE_ASCII								0
#define Win32String_TYPE_UTF8								65001
#define Win32String_TYPE_UTF16_LittleEndian					1200	// Unicode - code page 1200	(it is the default utf16 in Windows except Windows NT 3.51 & Windows NT 4.0)
#define Win32String_TYPE_UTF16_BigEndian					1201	//
#define Win32String_TYPE_ISO_8859_1							28591
#define Win32String_TYPE_GB18030							54936	// Windows XP and later: GB18030 Simplified Chinese (4 byte); Chinese Simplified (GB18030)
#define Win32String_TYPE_GB2312								936		// ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)

#define Win32String_DWORD_BYTESNUM							2

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
			bytesNum = this->_STR_ASCIIBytesNum(str);
			break;
		case Win32String_TYPE_UTF8:
			bytesNum = this->_STR_UTF8BytesNum(str);
			break;
		case Win32String_TYPE_UTF16_LittleEndian:
			bytesNum = this->_STR_UTF16BytesNum(str);
			break;
		}

		// Has Input
		this->lpBytes = new (std::nothrow) char[bytesNum];
		// Fail for OOM
		if(!this->lpBytes){
			return false;
		}
		// No OOM
		this->_STR_BytesCpy(this->lpBytes, str, bytesNum);
		return true;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------
	// Assitance Functions
	//-------------------------------------------------------------------------------
	size_t _STR_ASCIIBytesNum(const char * bytes);
	size_t _STR_UTF8BytesNum(const char * bytes);
	size_t _STR_UTF16BytesNum(const char * bytes);
	void _STR_BytesCpy(char * dest, char * source, size_t len);
	void _STR_BytesCpy(char * dest, const char * source, size_t len);
	void _STR_BytesCat(char * dest, char * source, size_t beginIdx, size_t len);
	void _STR_BytesCat(char * dest, const char * source, size_t beginIdx, size_t len);
	wchar_t * _PARSE_Bytes2WChar(char * bytes, UINT type);
	wchar_t * _PARSE_Bytes2WChar(const char * bytes, UINT type);
	char * _PARSE_WChar2Bytes(wchar_t * wchars, UINT type);
	char * _PARSE_WChar2Bytes(const wchar_t * wchars, UINT type);
public:
	/*** Coding Types ***/
	const static int TYPE_ASCII;
	const static int TYPE_WIDE_CHARACTER;
	const static int TYPE_UTF8;
	const static int TYPE_UTF16_LittleEndian;	// utf16, little endian
	const static int TYPE_UNICODE_PAGE1200;		// utf16, little endian
	const static int TYPE_UNICODE;				// utf16, little endian
	
	/**
	 * SetDefaultType
	 * @param type: the code type in your source files 
	 */
	static void SetDefaultType(const int type){
		if(	type == Win32String_TYPE_ASCII || 
			type == Win32String_TYPE_WIDE_CHARACTER || 
			type == Win32String_TYPE_UTF8 ||
			type == Win32String_TYPE_UTF16_LittleEndian){
			Win32String::defaultType = type;
		}
	}

	/**
	 * Deconstruct
	 */
	~Win32String(){
		if(this->lpWChar != NULL){
			delete[] this->lpWChar;
		}
		if(this->lpBytes != NULL){
			delete[] this->lpBytes;
		}
	}

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
			curNeededBytesNum = this->_STR_ASCIIBytesNum(this->lpBytes) - 1;
			extraBytesNum = this->_STR_ASCIIBytesNum(bytes);
			break;
		case Win32String_TYPE_UTF8:
			curNeededBytesNum = this->_STR_UTF8BytesNum(this->lpBytes) - 1;
			extraBytesNum = this->_STR_UTF8BytesNum(bytes);
			break;
		case Win32String_TYPE_UTF16_LittleEndian:
			curNeededBytesNum = this->_STR_UTF16BytesNum(this->lpBytes) - 2;
			extraBytesNum = this->_STR_UTF16BytesNum(bytes);
			break;
		}
		// Create new space
		char * lpBytesTMP = new (std::nothrow) char[curNeededBytesNum + extraBytesNum];
		if(!lpBytesTMP){
			return false;					// OOM
		}
		// Copy
		this->_STR_BytesCpy(lpBytesTMP, this->lpBytes, curNeededBytesNum);
		this->_STR_BytesCat(lpBytesTMP, bytes, curNeededBytesNum, extraBytesNum);
		// Free old space & link it to the new one
		delete[] this->lpBytes;
		this->lpBytes = lpBytesTMP;
		return true;
	}
	/**
	 * Add extra bytes inside
	 * @param dw: a DWORD will be added and it will be shown in Hex digits
	 */
	bool Append(DWORD dw){
		// Change DWORD to ASCII bytes
		char * lpASCIIBytes = new char[2 + Win32String_DWORD_BYTESNUM*2 + 1];
		sprintf(lpASCIIBytes, "0x%04x", dw);
		
		// Change ASCII bytes to adapt to the current type
		
		if(this->type == Win32String_TYPE_WIDE_CHARACTER){
		}else{
		}

		return false;
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

	/**
	 * Parse to UTF8
	 */
	bool Parse2UTF8(){
		// No transfer if it is
		if(this->TypeIsUTF8()){
			return true;
		}
		// Transfer - variales defination
		wchar_t * lpWCTemp = NULL;
		char * lpBytesTemp = NULL;
		bool bTransRes = false;			// Default is false


		// Transfer - based on the type
		if(this->type == Win32String_TYPE_WIDE_CHARACTER){
			// Begin with Wide Chars
			lpBytesTemp = this->_PARSE_WChar2Bytes(this->lpWChar, Win32String_TYPE_UTF8);
			if(!lpBytesTemp){
				bTransRes = false;		// To utf8 fails
			}else{
				// Succeed! Free old space & link it to the new one	
				if(this->lpBytes){		
					delete[] this->lpBytes;
				}
				this->lpBytes = lpBytesTemp;
				lpBytesTemp = NULL;
				bTransRes = true;
			}
		}else{
			// Begin with other coding bytes
			lpWCTemp = this->_PARSE_Bytes2WChar(this->lpBytes, this->type);
			if(!lpWCTemp){
				bTransRes = false;		// To wide char fails
			}else{
				lpBytesTemp = this->_PARSE_WChar2Bytes(lpWCTemp, Win32String_TYPE_UTF8);
				if(!lpBytesTemp){
					bTransRes = false;	// To utf8 fails
				}else{
					// Succeed! 
					bTransRes = true;
					// Free old space & link it to the new one	
					delete[] this->lpBytes;
					this->lpBytes = lpBytesTemp;
					lpBytesTemp = NULL;
				}
			}	
		}
		// If succeed, set type to UTF8
		if(bTransRes){
			this->type = Win32String_TYPE_UTF8;
		}
		// Release Temperal Variables
		if(lpWCTemp){ delete[] lpWCTemp; }
		if(lpBytesTemp){ delete[] lpBytesTemp; }

		// Return
		return bTransRes;
	}
	/**
     * Parse to UTF8
	 * @param lpUTF8: the pointer from the outside to storage data
	 */
	bool Parse2UTF8(char * & lpUTF8){
		bool bParseRes = this->Parse2UTF8();
		if(bParseRes){
			lpUTF8 = this->lpBytes;
		}
		return bParseRes;
	}

	/**
	 * Parse to WideChar
	 */
	bool Parse2WideChar(){
		// No transfer if it is
		if(this->TypeIsWideChar()){
			return true;
		}

		// WChar Space clean
		if(this->lpWChar != NULL){
			delete[] this->lpWChar;
			this->lpWChar = NULL;
		}
		// Space Calculation
		this->lpWChar = this->_PARSE_Bytes2WChar(this->lpBytes, this->type);
		// If succeed, set type to WideChar
		if(this->lpWChar){ this->type = Win32String_TYPE_WIDE_CHARACTER; }
		// Return
		return this->lpWChar != NULL;
	}
	/**
	 * Parse to WideChar
	 * @param lpWChar: the pointer from the outside to storage data
	 */
	bool Parse2WideChar(wchar_t * lpWChar){
		bool res = this->Parse2WideChar();
		if(res){
			lpWChar = this->lpWChar;
		}
		return res;
	}
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
const int Win32String::TYPE_UNICODE = Win32String_TYPE_UTF16_LittleEndian;

//------------------------------------------------------------------------------------------
// Assistance Functions
//------------------------------------------------------------------------------------------
size_t Win32String::_STR_ASCIIBytesNum(const char * bytes){
	return strlen(bytes) + 1;
}
size_t Win32String::_STR_UTF8BytesNum(const char * bytes){
	return strlen(bytes) + 1;
}
size_t Win32String::_STR_UTF16BytesNum(const char * bytes){
	if(!bytes){
		return 0;
	}
	size_t len = 0;
	while(bytes[len] != '\0' && bytes[len + 1] != '\0'){
		len += 1;
	}
	return len + 2;	// The extra space for '\0\0'
}
void Win32String::_STR_BytesCpy(char * dest, char * source, size_t len){
	size_t i;
	for(i = 0; i < len; i++){
		dest[i] = source[i];
	}
}
void Win32String::_STR_BytesCpy(char * dest, const char * source, size_t len){
	Win32String::_STR_BytesCpy(dest, (char * )source, len);
}
void Win32String::_STR_BytesCat(char * dest, char * source, size_t beginIdx, size_t len){
	size_t i;
	for(i = 0; i < len; i++){
		dest[i + beginIdx] = source[i];
	}
}
void Win32String::_STR_BytesCat(char * dest, const char * source, size_t beginIdx, size_t len){
	Win32String::_STR_BytesCat(dest, (char *)source, beginIdx, len);
}
wchar_t * Win32String::_PARSE_Bytes2WChar(char * bytes, UINT type){
	// NULL for the empty pointer
	if(!bytes){
		return NULL;
	}		
	// NULL for not supported types
	if( type != Win32String_TYPE_ASCII &&
		type != Win32String_TYPE_UTF8 &&
		type != Win32String_TYPE_UTF16_LittleEndian &&
		type != Win32String_TYPE_UTF16_BigEndian &&
		type != Win32String_TYPE_ISO_8859_1 &&
		type != Win32String_TYPE_GB18030 &&
		type != Win32String_TYPE_GB2312){
		return NULL;
	}

	wchar_t * lpWCTemp = NULL;
	int wcharNum = 0;			// include the one more space for '\0\0'
	wcharNum = MultiByteToWideChar(type, 0, bytes, -1, NULL, 0);
	if(!wcharNum){
		return NULL;			// Can't calculate the space
	}
	lpWCTemp = new wchar_t[wcharNum];
	if(!lpWCTemp){
		return NULL;			// OOM
	}
	if(!MultiByteToWideChar(type, 0, bytes, -1, lpWCTemp, wcharNum)){
		delete[] lpWCTemp;		// Transfer fails
		return NULL;
	}
	// Success
	return lpWCTemp;
}
wchar_t * Win32String::_PARSE_Bytes2WChar(const char * bytes, UINT type){
	return Win32String::_PARSE_Bytes2WChar((char *)bytes, type);
}
char * Win32String::_PARSE_WChar2Bytes(wchar_t * wchars, UINT type){
	// NULL for the empty pointer
	if(!wchars){
		return NULL;
	}
	// NULL for not supported types
	if( type != Win32String_TYPE_ASCII &&
		type != Win32String_TYPE_UTF8 &&
		type != Win32String_TYPE_UTF16_LittleEndian &&
		type != Win32String_TYPE_UTF16_BigEndian &&
		type != Win32String_TYPE_ISO_8859_1 &&
		type != Win32String_TYPE_GB18030 &&
		type != Win32String_TYPE_GB2312){
		return NULL;
	}
	
	char * lpBytesTMP = NULL;
	int bytesNum = 0;			// include the one more space for '\0', '\0\0' or other types

	bytesNum = WideCharToMultiByte(type, 0, wchars, -1, NULL, 0, NULL, NULL);
	if(!bytesNum){
		return NULL;			// Space Calculation Fails
	}
	lpBytesTMP = new (std::nothrow) char[bytesNum];
	if(!lpBytesTMP){
		return NULL;			// OOM
	}
	if(!WideCharToMultiByte(type, 0, wchars, -1, lpBytesTMP, bytesNum, NULL, NULL)){
		delete[] lpBytesTMP;
		return NULL;			// Tranfer fails
	}
	// Success
	return lpBytesTMP;
}
char  * Win32String::_PARSE_WChar2Bytes(const wchar_t * wchars, UINT type){
	return Win32String::_PARSE_WChar2Bytes((wchar_t *)wchars, type);
}

