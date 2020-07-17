#pragma once
#ifndef _LWS_WINSTRING_DEV_H
	#define _LWS_WINSTRING_DEV_H
	//-------------------------------------------------------------------------------
	// ˽�з���
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

		// ����
		try {
			if (this->type == TYPEID_WIDE_CHARACTER) {
				// �տ��ַ���
				wcharNumTMP = 1;
				lpTMP_Wchar = DBG_NEW wchar_t[wcharNumTMP];
				lpTMP_Wchar[0] = L'\0';
				res = true;
			}
			else {
				// �յ�ĳ�ֱ����ַ���
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
	//  ���о�̬����
	//------------------------------------------------------------------------------------------
	UINT WinString::defaultType = TYPEID_ASCII;									// The default type is ASCII

	const UINT WinString::STR_TYPE_ASCII = TYPEID_ASCII;
	const UINT WinString::STR_TYPE_WIDE_CHARACTER = TYPEID_WIDE_CHARACTER;
	const UINT WinString::STR_TYPE_UTF8 = TYPEID_UTF8;
	const UINT WinString::STR_TYPE_UTF16_LittleEndian = TYPEID_UTF16_LittleEndian;
	const UINT WinString::STR_TYPE_UNICODE_PAGE1200 = TYPEID_UTF16_LittleEndian;
	const UINT WinString::STR_TYPE_UNICODE = TYPEID_UTF16_LittleEndian;



	//-------------------------------------------------------------------------------
	// ���з���
	//-------------------------------------------------------------------------------
	/**
	* SetDefaultType
	* @param type: the code type in your source files
	*/
	bool PUBLIC_METHOD_CALL_TYPE WinString::SetDefaultType(const UINT type) {
		// ���������֧�ֵ�����ģ����򷵻�false
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
		bool isAllGood = false;								// Ĭ��ʧ��
		WinString * lpWin32Str = NULL;
		// ����	
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

		// ����
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
		size_t curNeededBytesNum;						// ֮ǰ�д��Byte��Ҫ���ֽ���
		size_t extraBytesNum;							// ������ֽ���
		char * lpBytesTMP = NULL;

		// Refuse the different type coding
		try{
			if (type == this->type) {
				if (!bytes) {
					// Succeed for the empty str!
					isAllGood = true;
				}
				else {
					// ����ռ䣬���ʱ��this->lpBytes���ٻ���1��byte�Ŀռ䡣this->lpBytesΪNULLֻ�в���wchar_t�ַ�����ʼ���Ż���֣����ǻᱻǰ��������жϾܾ���
					curNeededBytesNum = _STR_BytesNum(this->lpBytes, type) - _STR_EOSNum(type);	// Not include EOS (end of string)
					extraBytesNum = _STR_BytesNum(bytes, type);
					// Not an overlong str
					if (extraBytesNum != -1) {
						// �����µ����ݣ����ֽ���Ҳ�������޲�׷��
						if (curNeededBytesNum + extraBytesNum <= WINSTRING_BYTES_MAX) {
							lpBytesTMP = DBG_NEW char[curNeededBytesNum + extraBytesNum];
							_STR_BytesCpy(lpBytesTMP, this->lpBytes, curNeededBytesNum);
							_STR_BytesCat(lpBytesTMP, bytes, curNeededBytesNum, extraBytesNum);
							// Free old space & link it to the new one
							delete[] this->lpBytes;				// �ͷžɿռ䣨���ﲻ��Ҫ���ָ���Ƿ�Ϊ�գ���Ϊif������֤�����Ȼ�����ݣ�
							this->lpBytes = lpBytesTMP;
							lpBytesTMP = NULL;
						}
					}
				}
			}
		}
		catch (std::bad_alloc& e) {
			// �ռ����ʧ�ܣ����κβ���
			UNUSED_VAR(e);
		}
		// ����
		return isAllGood;
	}
	/**
	* Add extra bytes inside
	* @param dw: a DWORD will be added and it will be shown in Hex digits
	*/
	bool PUBLIC_METHOD_CALL_TYPE WinString::Append(DWORD dw) {
		int dw2StrBytesNum = 2 + DWORD_BYTESNUM * 2 + 1;	// DWORD ת�����ַ��� "0x%04x"����Ҫ2���ֽڷ�"0x"��4���ֽ���ʾDWORD���ݣ�һ���ֽڷŽ�������
		char * lpASCIIBytes = NULL;
		wchar_t * wcExtra = NULL;
		wchar_t * lpWCharNew = NULL;
		char * bytesExtra = NULL;
		char * lpBytesNew = NULL;
		size_t oldNeededWCharLen = 0;
		size_t extraWcharLen = 0;
		size_t oldNeededBytesLen = 0;
		size_t extraBytesLen = 0;
		bool res = false;		// ���صĲ������
		
		// ׷�Ӳ���
		try {
			lpASCIIBytes = DBG_NEW char[dw2StrBytesNum];							// Apply space for ASCII bytes
			sprintf_s(lpASCIIBytes, dw2StrBytesNum, "0x%04x", dw);					// Change DWORD to ASCII bytes
			wcExtra = _PARSE_Bytes2WChar(lpASCIIBytes, TYPEID_ASCII);				// Change ASCII bytes to wide char
			// ת��wchar_t�ɹ��ż���
			if (wcExtra) {
				// ���ݵ�ǰ���ʹ���
				if (this->type == TYPEID_WIDE_CHARACTER) {
					// ������Ҫwchar_t����
					oldNeededWCharLen = _STR_WCharsNum(this->lpWChar) - _STR_EOSNum(this->type);
					extraWcharLen = _STR_WCharsNum(wcExtra);						// ������ﷵ��-1��˵�����ַ����޹���
					// �����µ����ݣ��ܿ��ַ���Ҳ�������޲�׷��
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
						extraBytesLen = _STR_BytesNum(bytesExtra, this->type);		// ������ﷵ��-1��˵�����ַ����޹���
						// �����µ����ݣ����ֽ���Ҳ�������޲�׷��
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
			// �ռ����ʧ�ܣ����κβ���
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

		// ת��
		if (this->TypeIsUTF8()) {
			// If it is already utf8, just return those bytes
			utf8Bytes = this->lpBytes;
		}
		else {
			// ��utf8���ͣ���Ҫ��ת��
			if (this->type == TYPEID_WIDE_CHARACTER) {
				// ��wchar_tת��utf8
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
				// ����������ת��utf8 (��ʱthis->lpBytes���ٺ����ַ����������ţ����弸���ֽ�ȡ��������)
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
		// ֻ�з�wchar_t���Ͳ�ת��
		if (!this->TypeIsWideChar()) {
			// ���this->lpWChar�ǿգ����ͷſռ�
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
				// Begin with other coding bytes (��ʱthis->lpBytes������һ���ַ����������ţ�����೤ȡ��������)
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