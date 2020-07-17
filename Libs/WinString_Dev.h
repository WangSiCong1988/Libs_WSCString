#pragma once
#ifndef _LWS_WINSTRING_DEV_H
	#define _LWS_WINSTRING_DEV_H
	//-------------------------------------------------------------------------------
	// 私有方法
	//-------------------------------------------------------------------------------
	/**
	 * Construction
	 */
	WinString::WinString() {
		this->lpWChar = NULL;
		this->lpBytes = NULL;
		this->type = WinString::defaultType;	// The default type

		this->bytesNum = 0;
		this->wcharNum = 0;
	}

	/**
	 * Deconstruct
	 */
	WinString::~WinString() {
		if (this->lpWChar != NULL) {
			delete[] this->lpWChar;
		}
		if (this->lpBytes != NULL) {
			delete[] this->lpBytes;
		}
	}
	/**
	 * Init
	 * @description: init an empty string, set it to the default type
	 */
	bool WinString::_Init() {
		bool res = false;
		char * lpTMP_Bytes = NULL;
		wchar_t * lpTMP_Wchar = NULL;
		size_t bytesNumTMP = 0;
		size_t wcharNumTMP = 0;

		// 处理
		try {
			if (this->type == TYPEID_WIDE_CHARACTER) {
				// 空宽字符串
				wcharNumTMP = 1;
				lpTMP_Wchar = DBG_NEW wchar_t[wcharNumTMP];
				lpTMP_Wchar[0] = L'\0';
				res = true;
			}
			else {
				// 空的某种编码字符串
				bytesNumTMP = _STR_EOSNum(this->type);
				lpTMP_Bytes = DBG_NEW char[bytesNumTMP];
				_STR_BytesClr(lpTMP_Bytes, bytesNumTMP);
				res = true;
			}
		}
		catch (std::bad_alloc & e) {
			UNUSED_VAR(e);
		}
		
		// If succeed
		if (res) {
			// lp
			this->lpBytes = lpTMP_Bytes;
			lpTMP_Bytes = NULL;
			this->lpWChar = lpTMP_Wchar;
			lpTMP_Wchar = NULL;
			// num
			this->bytesNum = bytesNumTMP;
			this->wcharNum = wcharNumTMP;
		}
		// Return
		return res;
	}
	/**
	* Init
	* @description: init a string from given bytes (thoses bytes are supposed to be the same as the default type)
	*/
	bool WinString::_Init(const char * str) {
		bool res = false;
		size_t bytesNumTMP = 0;
		char * lpBytesTMP = NULL;

		// Process
		try {
			if (this->type != TYPEID_WIDE_CHARACTER) {			// Widechar fails
				if (!str) {
					lpBytesTMP = DBG_NEW char[1];
					lpBytesTMP[0] = '\0';
					bytesNumTMP = 1;
					res = true;

				}
				else {
					bytesNumTMP = _STR_BytesNum(str, this->type);	// Space calculation (we can't get 0 here because NULL is handled before)
					// Not too long
					if (bytesNumTMP != -1) {
						// Succeed!
						lpBytesTMP = DBG_NEW char[bytesNumTMP];
						_STR_BytesCpy(lpBytesTMP, str, bytesNumTMP);
						res = true;

					}
				}
			}
		}
		catch (std::bad_alloc& e) {
			// Do nothing
			UNUSED_VAR(e);
		}
		
		// If succeed
		if (res) {
			this->lpBytes = lpBytesTMP;				// bytes
			lpBytesTMP = NULL;
			this->bytesNum = bytesNumTMP;			// num
		}
		// Return
		return res;
	}

	//------------------------------------------------------------------------------------------
	//  公有静态变量
	//------------------------------------------------------------------------------------------
	UINT WinString::defaultType = TYPEID_ASCII;									// The default type is ASCII

	const UINT WinString::STR_TYPE_ASCII = TYPEID_ASCII;
	const UINT WinString::STR_TYPE_WIDE_CHARACTER = TYPEID_WIDE_CHARACTER;
	const UINT WinString::STR_TYPE_UTF8 = TYPEID_UTF8;
	const UINT WinString::STR_TYPE_UTF16_LittleEndian = TYPEID_UTF16_LittleEndian;
	const UINT WinString::STR_TYPE_UNICODE_PAGE1200 = TYPEID_UTF16_LittleEndian;
	const UINT WinString::STR_TYPE_UNICODE = TYPEID_UTF16_LittleEndian;



	//-------------------------------------------------------------------------------
	// 公有方法
	//-------------------------------------------------------------------------------
	/**
	* SetDefaultType
	* @param type: the code type in your source files
	*/
	bool PUBLIC_METHOD_CALL_TYPE WinString::SetDefaultType(const UINT type) {
		// 如果类型是支持的则更改，否则返回false
		if (type == TYPEID_ASCII ||
			type == TYPEID_WIDE_CHARACTER ||
			type == TYPEID_UTF8 ||
			type == TYPEID_UTF16_LittleEndian ||
			type == TYPEID_UTF16_BigEndian ||
			type == TYPEID_ISO_8859_1 ||
			type == TYPEID_GB18030 ||
			type == TYPEID_GB2312
			) {
			WinString::defaultType = type;
			return true;
		}
		return false;
	}



	//------------------------------------------------------------------------------
	// Create
	//
	// Create: An empty ASCII string
	WinString * PUBLIC_METHOD_CALL_TYPE WinString::Create() {
		bool isAllGood = false;								// 默认失败
		WinString * lpWin32Str = NULL;
		// 处理	
		try {
			lpWin32Str = DBG_NEW WinString;
			if (lpWin32Str->_Init()) {
				isAllGood = true;
			}
		}
		catch (std::bad_alloc & e) {
			UNUSED_VAR(e);
		}
		// If fail, memory release
		if (!isAllGood) {
			delete lpWin32Str;
			lpWin32Str = NULL;
		}
		// Return
		return lpWin32Str;
	}
	// Create: An Default String
	WinString * PUBLIC_METHOD_CALL_TYPE WinString::Create(const char * str) {
		bool isAllGood = false;
		WinString * lpWin32Str = NULL;

		// 处理
		try {
			lpWin32Str = DBG_NEW WinString();
			if (lpWin32Str->_Init(str)) {
				// Succeed!
				isAllGood = true;
			}
		}
		catch (std::bad_alloc& e) {
			UNUSED_VAR(e);
		}
		// If fail, release space
		if (!isAllGood) {
			delete lpWin32Str;
			lpWin32Str = NULL;
		}
		// All good
		return lpWin32Str;
	}

	/**
	 * Destroy an instance
	 */
	void PUBLIC_METHOD_CALL_TYPE WinString::Destroy(WinString * lp) {
		if (lp) {
			delete lp;
		}
	}

	/**
	 * Add extra bytes inside
	 * @param bytes: the bytes to be appended
	 * @param type: the extra bytes' type. If it does not fit the type of old bytes, return false
	 */
	bool PUBLIC_METHOD_CALL_TYPE WinString::Append(const char * bytes, UINT type) {
		bool isAllGood = false;
		size_t curNeededBytesNum;						// 之前残存的Byte需要的字节数
		size_t extraBytesNum;							// 插入的字节数
		char * lpBytesTMP = NULL;

		// Refuse the different type coding
		try{
			if (type == this->type) {
				if (!bytes) {
					// Succeed for the empty str!
					isAllGood = true;
				}
				else {
					// 计算空间，这个时候this->lpBytes至少会有1个byte的空间。this->lpBytes为NULL只有采用wchar_t字符串初始化才会出现，但是会被前面的类型判断拒绝掉
					curNeededBytesNum = _STR_BytesNum(this->lpBytes, type) - _STR_EOSNum(type);	// Not include EOS (end of string)
					extraBytesNum = _STR_BytesNum(bytes, type);
					// Not an overlong str
					if (extraBytesNum != -1) {
						// 加上新的数据，总字节数也不超上限才追加
						if (curNeededBytesNum + extraBytesNum <= WINSTRING_BYTES_MAX) {
							lpBytesTMP = DBG_NEW char[curNeededBytesNum + extraBytesNum];
							_STR_BytesCpy(lpBytesTMP, this->lpBytes, curNeededBytesNum);
							_STR_BytesCat(lpBytesTMP, bytes, curNeededBytesNum, extraBytesNum);
							// Free old space & link it to the new one
							delete[] this->lpBytes;				// 释放旧空间（这里不需要检查指针是否为空，因为if条件保证这里必然有数据）
							this->lpBytes = lpBytesTMP;
							lpBytesTMP = NULL;
						}
					}
				}
			}
		}
		catch (std::bad_alloc& e) {
			// 空间分配失败，无任何操作
			UNUSED_VAR(e);
		}
		// 返回
		return isAllGood;
	}
	/**
	* Add extra bytes inside
	* @param dw: a DWORD will be added and it will be shown in Hex digits
	*/
	bool PUBLIC_METHOD_CALL_TYPE WinString::Append(DWORD dw) {
		int dw2StrBytesNum = 2 + DWORD_BYTESNUM * 2 + 1;	// DWORD 转换到字符串 "0x%04x"，需要2个字节放"0x"，4个字节显示DWORD数据，一个字节放结束符号
		char * lpASCIIBytes = NULL;
		wchar_t * wcExtra = NULL;
		wchar_t * lpWCharNew = NULL;
		char * bytesExtra = NULL;
		char * lpBytesNew = NULL;
		size_t oldNeededWCharLen = 0;
		size_t extraWcharLen = 0;
		size_t oldNeededBytesLen = 0;
		size_t extraBytesLen = 0;
		bool res = false;		// 返回的操作结果
		
		// 追加操作
		try {
			lpASCIIBytes = DBG_NEW char[dw2StrBytesNum];							// Apply space for ASCII bytes
			sprintf_s(lpASCIIBytes, dw2StrBytesNum, "0x%04x", dw);					// Change DWORD to ASCII bytes
			wcExtra = _PARSE_Bytes2WChar(lpASCIIBytes, TYPEID_ASCII);				// Change ASCII bytes to wide char
			// 转换wchar_t成功才继续
			if (wcExtra) {
				// 根据当前类型处理
				if (this->type == TYPEID_WIDE_CHARACTER) {
					// 我们需要wchar_t类型
					oldNeededWCharLen = _STR_WCharsNum(this->lpWChar) - _STR_EOSNum(this->type);
					extraWcharLen = _STR_WCharsNum(wcExtra);						// 如果这里返回-1，说明宽字符上限过低
					// 加上新的数据，总宽字符数也不超上限才追加
					if (oldNeededWCharLen + extraWcharLen <= WINSTRING_WCHAR_MAX) {
						lpWCharNew = DBG_NEW wchar_t[oldNeededWCharLen + extraWcharLen];
						// Copy
						_STR_WCharsCpy(lpWCharNew, this->lpWChar, oldNeededWCharLen);
						_STR_WCharsCat(lpWCharNew, wcExtra, oldNeededWCharLen, extraWcharLen);
						// Release old space & link it to the new one
						delete[] this->lpWChar;
						this->lpWChar = lpWCharNew;
						lpWCharNew = NULL;
						// Set success
						res = true;
					}
				}
				else {
					// Other coding
					bytesExtra = _PARSE_WChar2Bytes(wcExtra, this->type);
					if (bytesExtra) {
						oldNeededBytesLen = _STR_BytesNum(this->lpBytes, this->type) - _STR_EOSNum(this->type);
						extraBytesLen = _STR_BytesNum(bytesExtra, this->type);		// 如果这里返回-1，说明宽字符上限过低
						// 加上新的数据，总字节数也不超上限才追加
						if (oldNeededBytesLen + extraBytesLen <= WINSTRING_BYTES_MAX) {
							lpBytesNew = DBG_NEW char[oldNeededBytesLen + extraBytesLen];
							// Copy
							_STR_BytesCpy(lpBytesNew, this->lpBytes, oldNeededBytesLen);
							_STR_BytesCat(lpBytesNew, bytesExtra, oldNeededBytesLen, extraBytesLen);
							// Release space
							delete[] this->lpBytes;
							this->lpBytes = lpBytesNew;
							lpBytesNew = NULL;
							// Set success
							res = true;
						}
					}
				}
			}
		}
		catch(std::bad_alloc& e){
			// 空间分配失败，无任何操作
			UNUSED_VAR(e);
		}
		// Release temporal space
		if (lpASCIIBytes)
			delete[] lpASCIIBytes;
		if (wcExtra)
			delete[] wcExtra;
		if (lpWCharNew)
			delete[] lpWCharNew;
		if (bytesExtra)
			delete[] bytesExtra;
		if (lpBytesNew)
			delete[] lpBytesNew;
		// Return
		return res;
	}

	/**
	* Get the bytes
	*/
	char * PUBLIC_METHOD_CALL_TYPE WinString::GetBytes() {
		return this->lpBytes;
	}

	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Type
	//
	// Type: Is WideChar
	bool PUBLIC_METHOD_CALL_TYPE WinString::TypeIsWideChar() {
		return this->type == TYPEID_WIDE_CHARACTER;
	}
	// Type: Is ASCII
	bool PUBLIC_METHOD_CALL_TYPE WinString::TypeIsASCII() {
		return this->type == TYPEID_ASCII;
	}
	// Type: Is UTF8
	bool PUBLIC_METHOD_CALL_TYPE WinString::TypeIsUTF8() {
		return this->type == TYPEID_UTF8;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	/**
	* Parse to UTF8
	* @return: NULL, fail; not NULL, succeed
	*/
	char * PUBLIC_METHOD_CALL_TYPE WinString::Parse2UTF8() {
		wchar_t * lpWCTemp = NULL;
		char * lpBytesTemp = NULL;
		char * utf8Bytes = NULL;			// Default is NULL

		// 转换
		if (this->TypeIsUTF8()) {
			// If it is already utf8, just return those bytes
			utf8Bytes = this->lpBytes;
		}
		else {
			// 非utf8类型，需要做转换
			if (this->type == TYPEID_WIDE_CHARACTER) {
				// 从wchar_t转到utf8
				lpBytesTemp = _PARSE_WChar2Bytes(this->lpWChar, TYPEID_UTF8);
				// Succeed!	
				if (lpBytesTemp) {
					if (this->lpBytes)								// Free old space
						delete[] this->lpBytes;
					this->lpBytes = lpBytesTemp;					// Link bytes pointer to the new one
					lpBytesTemp = NULL;
					// Return pointer has value now
					utf8Bytes = this->lpBytes;				
				}
			}
			else {
				// 从其他编码转到utf8 (此时this->lpBytes至少含有字符串结束符号，具体几个字节取决于类型)
				lpWCTemp = _PARSE_Bytes2WChar(this->lpBytes, this->type);
				if (lpWCTemp) {
					lpBytesTemp = _PARSE_WChar2Bytes(lpWCTemp, TYPEID_UTF8);
					// Succeed! 
					if (lpBytesTemp) {
						// Free old space & link it to the new one	
						delete[] this->lpBytes;
						this->lpBytes = lpBytesTemp;
						lpBytesTemp = NULL;
						// Return pointer has value now
						utf8Bytes = this->lpBytes;			
					}
				}
			}
		}
		// If succeed, set type to UTF8
		if (utf8Bytes) {
			this->type = TYPEID_UTF8;
		}
		// Release Temperal Variables
		if (lpWCTemp)
			delete[] lpWCTemp;
		if (lpBytesTemp)
			delete[] lpBytesTemp;
		// Return
		return utf8Bytes;
	}

	/**
	* Parse to WideChar
	*/
	wchar_t * PUBLIC_METHOD_CALL_TYPE WinString::Parse2WideChar() {
		// 只有非wchar_t类型才转换
		if (!this->TypeIsWideChar()) {
			// 如果this->lpWChar非空，先释放空间
			if (this->lpWChar) {
				delete[] this->lpWChar;
				this->lpWChar = NULL;
			}
			// Try to change to wide char
			this->lpWChar = _PARSE_Bytes2WChar(this->lpBytes, this->type);
		}
		// If succeed, set type to WideChar
		if (this->lpWChar)
			this->type = TYPEID_WIDE_CHARACTER;
		// Return
		return this->lpWChar;
	}

	/**
	* Parse to ASCII
	*/
	char * PUBLIC_METHOD_CALL_TYPE WinString::Parse2ASCII() {
		char * res = NULL;															// The return object
		// Temporal variables
		wchar_t * lpWCTMP = NULL;
		char * lpBytesTMP = NULL;

		// Set the return object
		if (this->TypeIsASCII()) {
			// No transfer if it is
			res = this->lpBytes;
		}
		else {
			if (this->type == TYPEID_WIDE_CHARACTER) {
				// From wide char to ASCII 
				lpBytesTMP = _PARSE_WChar2Bytes(this->lpWChar, TYPEID_ASCII);
				if (lpBytesTMP) {
					if (this->lpBytes) {												// Old space may be NULL
						delete[] this->lpBytes;
					}
					this->lpBytes = lpBytesTMP;
					res = lpBytesTMP;
					lpBytesTMP = NULL;
				}
			}
			else {
				// Begin with other coding bytes (此时this->lpBytes至少有一个字符串结束符号，具体多长取决于类型)
				lpWCTMP = _PARSE_Bytes2WChar(this->lpBytes, this->type);
				if (lpWCTMP) {
					lpBytesTMP = _PARSE_WChar2Bytes(lpWCTMP, TYPEID_ASCII);
					if (lpBytesTMP) {
						delete[] this->lpBytes;										// old space mustn't be NULL
						this->lpBytes = lpBytesTMP;
						res = lpBytesTMP;
						lpBytesTMP = NULL;
					}
				}
			}
		}

		// If success, set type to ASCII
		if (res)
			this->type = TYPEID_ASCII;
		// Temporal space is released
		if (lpWCTMP)
			delete[] lpWCTMP;
		if (lpBytesTMP)
			delete[] lpBytesTMP;

		// Return
		return res;
	}
#endif // !_LWS_WINSTRING_DEV_H