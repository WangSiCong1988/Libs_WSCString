
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WINSTRING_DLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WINSTRING_DLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef WINSTRING_DLL_EXPORTS
#define WINSTRING_DLL_API __declspec(dllexport)
#else
#define WINSTRING_DLL_API __declspec(dllimport)
#endif

// This class is exported from the WinString_DLL.dll


/**
 * Version: 0.1.0
 */
namespace WinString_0_1_0{	


	class WINSTRING_DLL_API WinString{
	private:
		//-------------------------------------------------------------------------------
		// Assitance Functions （所有辅助函数都是静态方法）
		//-------------------------------------------------------------------------------
		static void * _MemoryApply(size_t size, UINT type);
		static void * _MemoryApply(int size, UINT type);

		static size_t _STR_WCharsNum(const wchar_t * lpWChars);													// include '\0\0'
		static size_t _STR_WCharsNumWithoutEOS(const wchar_t * lpWChars);								
		static size_t _STR_BytesNum(const char * bytes, UINT type);												// include '\0', '\0\?'(? means any value) or '\0\0'
		static size_t _STR_BytesNumWithoutEOS(const char * bytes, UINT type);

		static void _STR_WCharsCpy(wchar_t * dest, const wchar_t * source, size_t len);							// @len: must include '\0\0'
		static void _STR_WCharsCat(wchar_t * dest, const wchar_t * source, size_t beginIdx, size_t len);		// @len: must include '\0\0'
		static void _STR_BytesCpy(char * dest, const char * source, size_t len);								// @len: must include '\0' or '\0\0'
		static void _STR_BytesCat(char * dest, const char * source, size_t beginIdx, size_t len);				// @len: must include '\0' or '\0\0'
		// 转换函数（辅助操作，防止失败时空间没有释放），返回非NULL成功
		static wchar_t * _PARSE_Bytes2WChar(const char * bytes, UINT type);										// @byte: must incldue '\0' or '\0\0',	@return: include '\0\0'
		static char * _PARSE_WChar2Bytes(const wchar_t * wchars, UINT type);									// @wchars: must include '\0\0',		@return: include '\0' or '\0\0'

		//-------------------------------------------------------------------------------
		// 私有属性
		//-------------------------------------------------------------------------------
		static UINT defaultType;
		UINT type;

		// Pointers to storage different coding
		// They are only released in the deconstruction method
		wchar_t * lpWChar;			// WideChar Type (It is for any character in any language but only used in the memory)
		char * lpBytes;				// Bytes of the string (based on what code you choose)

		//-------------------------------------------------------------------------------
		// 私有方法
		//-------------------------------------------------------------------------------
		/* Construction */
		WinString();
		/* Init */
		bool Init();					// Init an empty string, set it to ASCII
		bool Init(const char * str);	// Init a string from given bytes (thoses bytes are supposed to be the same as the default type)
	public:
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
		static bool SetDefaultType(const UINT type);

		/**
		 * Deconstruct
		 */
		~WinString();

		/* Create */
		static WinString * Create();					// Create: An empty ASCII string
		static WinString * Create(const char * str);	// Create: An Default String
		
		/**
		 * Add extra bytes inside
		 * @param bytes: the bytes to be appended
		 * @param type: the extra bytes' type. If it does not fit the type of old bytes, return false
		 */
		bool Append(const char * bytes, UINT type);
		/**
		 * Add extra bytes inside
		 * @param dw: a DWORD will be added and it will be shown in Hex digits
		 */
		bool Append(DWORD dw);

		/**
		 * Get the bytes
		 */
		char * GetBytes();
		
		/* Type */
		bool TypeIsWideChar();	// Type: Is WideChar
		bool TypeIsASCII();		// Type: Is ASCII
		bool TypeIsUTF8();		// Type: Is UTF8

		/**
		 * Parse to UTF8
		 * @return: NULL, fail; not NULL, succeed
		 */
		char * Parse2UTF8();

		/**
		 * Parse to WideChar
		 */
		wchar_t * Parse2WideChar();
	};
}



