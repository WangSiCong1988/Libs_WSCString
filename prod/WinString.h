#pragma once
#ifndef _LWS_WINSTRING_H
#define _LWS_WINSTRING_H

	// 定义class的形式
	#ifdef _LWS_WINSTRING_DEV
		// 开发模式，不作为dll导出
		#define _LWS_WINSTRING_DLL_API
	#else
		// DLL 导出模式，如果定义 WINSTRING_DLL_EXPORTS，则为导出，否则则为导入
		#ifdef WINSTRING_DLL_EXPORTS
			#define _LWS_WINSTRING_DLL_API __declspec(dllexport)
		#else
			#define _LWS_WINSTRING_DLL_API __declspec(dllimport)
		#endif
	#endif // !WINSTRING_DEV

	

	// public 方法采用_stdcall方便其他语言调用
	#define PUBLIC_METHOD_CALL_TYPE _stdcall 			

	/*** Class Definition ***/
	class  _LWS_WINSTRING_DLL_API WinString {
	private:
		static UINT defaultType;
		UINT type;
		size_t bytesNum;
		size_t wcharNum;

		// Pointers to storage different coding
		// They are only released in the deconstruction method
		wchar_t * lpWChar;			// WideChar Type (It is for any character in any language but only used in the memory)
		char * lpBytes;				// Bytes of the string (based on what code you choose)

		/* Construction & Deconstruction */
		WinString();
		~WinString();
		/* Init */
		bool _Init();					// Init an empty string, set it to ASCII
		bool _Init(const char * str);	// Init a string from given bytes (thoses bytes are supposed to be the same as the default type)
		bool _Init(int count, ...);
	public:
		/*** Data Type ***/
		const static UINT DATA_TYPE_INT;
		const static UINT DATA_TYPE_CHAR;
		const static UINT DATA_TYPE_STR_BYTES;
		const static UINT DATA_TYPE_STR_WCHAR;
		const static UINT DATA_TYPE_WCHAR_PTR;
		const static UINT DATA_TYPE_WCHAR_ARRAY;
		const static UINT DATA_TYPE_WINSTRING_PTR;

		/*** Coding Types ***/
		const static UINT STR_TYPE_ASCII;
		const static UINT STR_TYPE_WIDE_CHARACTER;
		const static UINT STR_TYPE_UTF8;
		const static UINT STR_TYPE_UTF16_LittleEndian;	// utf16, little endian
		const static UINT STR_TYPE_UNICODE_PAGE1200;	// utf16, little endian
		const static UINT STR_TYPE_UNICODE;				// utf16, little endian

		/* SetDefaultType
		 * @param type: the code type in your source files
		 */
		static bool PUBLIC_METHOD_CALL_TYPE SetDefaultType(const UINT type);

		/* Create */
		static WinString * PUBLIC_METHOD_CALL_TYPE Create();					// Create: An empty default string
		static WinString * PUBLIC_METHOD_CALL_TYPE Create(const char * str);	// Create: An initial default string
		static WinString * PUBLIC_METHOD_CALL_TYPE Create(int count, ...);		// Create: An initial default string from various inputs (various types)

		/* Destroy */
		static void PUBLIC_METHOD_CALL_TYPE Destroy(WinString * lp);

		/**
		 * Add extra bytes inside
		 * @param bytes: the bytes to be appended
		 * @param type: the extra bytes' type. If it does not fit the type of old bytes, return false
		 */
		bool PUBLIC_METHOD_CALL_TYPE Append(const char * bytes, UINT type);
		/**
		* Add extra bytes inside
		* @param dw: a DWORD will be added and it will be shown in Hex digits
		*/
		bool PUBLIC_METHOD_CALL_TYPE Append(DWORD dw);

		/**
		 * Get the bytes
		 */
		char * PUBLIC_METHOD_CALL_TYPE GetBytes();

		/* Type */
		bool PUBLIC_METHOD_CALL_TYPE TypeIsWideChar();		// Type: Is WideChar
		bool PUBLIC_METHOD_CALL_TYPE TypeIsASCII();		// Type: Is ASCII
		bool PUBLIC_METHOD_CALL_TYPE TypeIsUTF8();			// Type: Is UTF8

		/**
		 * Parse to UTF8
		 * @return: NULL, fail; not NULL, succeed
		 */
		char * PUBLIC_METHOD_CALL_TYPE Parse2UTF8();

		/**
		 * Parse to WideChar
		 */
		wchar_t * PUBLIC_METHOD_CALL_TYPE Parse2WideChar();

		/**
		 * Parse to ASCII
		 */
		char * PUBLIC_METHOD_CALL_TYPE Parse2ASCII();
	};

#endif