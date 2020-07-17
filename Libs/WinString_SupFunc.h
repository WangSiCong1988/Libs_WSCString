#pragma once
#ifndef _LWS_WINSTRING_SUPFUNC_H
#define _LWS_WINSTRING_SUPFUNC_H

#include "WinString_Afx.h"									// ���ͷ�ļ�

/*** �ַ��������������� ***/
/**
 * ����wchar_t�ַ�����wchar_t�ĸ���������'\0\0'��
 */
size_t _STR_WCharsNum(const wchar_t * lpWChars) {
	size_t len = 0;
	bool oom = false;
	// NULL 0
	if (lpWChars) {										
		while (lpWChars[len] != L'\0') {
			// ѭ��չ���Ż������ٷ�ָ֧��ִ�еĴ�����Ҳ�ͼ����˴�������֧Ԥ�����ʧ�ܵĴ�����90�����watcom c++�������ʹ��������·�����ڽ��µĴ������������Ż�Ч�������ԣ�
			++len;
			if (len > WINSTRING_WCHAR_MAX) {
				oom = true;
			}
			if (lpWChars[len] == L'\0')
				break;
			++len;
			if (len > WINSTRING_WCHAR_MAX) {
				oom = true;
			}
			if (lpWChars[len] == L'\0')
				break;
			++len;
			if (len > WINSTRING_WCHAR_MAX) {
				oom = true;
			}
			if (lpWChars[len] == L'\0')
				break;
			++len;
			if (len > WINSTRING_WCHAR_MAX) {
				oom = true;
			}
		}
		++len;	// ����'\0'�Ŀռ�
	}
	// if OOM, set len -1
	if (oom)
		len = -1;
	// Return
	return len;
}

/**
 * ����char�������ֽ�����������'\0'��
 */
size_t _STR_BytesNum(const char * bytes, UINT type) {
	size_t len = 0;
	bool oom = false;		// check whether the len is out of maximal accepted value
	if (bytes) {
		// �������ͼ����ֽ�����������Ͳ�֧���򷵻�0
		switch (type) {
		case TYPEID_ASCII:
		case TYPEID_UTF8:
		case TYPEID_ISO_8859_1:
		case TYPEID_GB2312:
			while (bytes[len] != '\0') {
				// ѭ��չ���Ż�
				++len;
				if (len > WINSTRING_BYTES_MAX) {
					oom = true;
					break;
				}
				if (bytes[len] == '\0')
					break;
				++len;
				if (len > WINSTRING_BYTES_MAX) {
					oom = true;
					break;
				}
				if (bytes[len] == '\0')
					break;
				++len;
				if (len > WINSTRING_BYTES_MAX) {
					oom = true;
					break;
				}
				if (bytes[len] == '\0')
					break;
				++len;
				if (len > WINSTRING_BYTES_MAX) {
					oom = true;
					break;
				}
			}
			++len;	// ����'\0'��Ҫ�Ŀռ�
			break;
		case TYPEID_UTF16_LittleEndian:
		case TYPEID_UTF16_BigEndian:
			while (bytes[len] != '\0' && bytes[len + 1] != '\0') {
				// ѭ��չ���Ż�
				len += 2;
				if (len > WINSTRING_BYTES_MAX) {
					oom = true;
					break;
				}
				if (bytes[len] != '\0' && bytes[len + 1] != '\0')
					break;
				len += 2;
				if (len > WINSTRING_BYTES_MAX) {
					oom = true;
					break;
				}
				if (bytes[len] != '\0' && bytes[len + 1] != '\0')
					break;
				len += 2;
				if (len > WINSTRING_BYTES_MAX) {
					oom = true;
					break;
				}
				if (bytes[len] != '\0' && bytes[len + 1] != '\0')
					break;
				len += 2;
				if (len > WINSTRING_BYTES_MAX) {
					oom = true;
					break;
				}
			}
			len += 2;	// The extra space for '\0\0'
			break;
		case TYPEID_GB18030:
			// ���ÿ4��bytes�е�һ��byteΪ0����Ϊ�ַ����������
			while (bytes[len] != '\0') {
				// ѭ��չ���Ż�
				len += 4;
				if (len > WINSTRING_BYTES_MAX) {
					oom = true;
					break;
				}
				if (bytes[len] == '\0')
					break;
				len += 4;
				if (len > WINSTRING_BYTES_MAX) {
					oom = true;
					break;
				}
				if (bytes[len] == '\0')
					break;
				len += 4;
				if (len > WINSTRING_BYTES_MAX) {
					oom = true;
					break;
				}
				if (bytes[len] == '\0')
					break;
				len += 4;
				if (len > WINSTRING_BYTES_MAX) {
					oom = true;
					break;
				}
			}
			len += 4;	// The extra space for '\0\0\0\0'
			break;
		}
	}
	// If oom, set len -1
	if (oom)
		len = -1;
	// Return
	return len;
}

/**
 * �����ַ����������ŵĿռ�
 */
size_t _STR_EOSNum(UINT type) {
	size_t len = 0;
	switch (type)
	{
	case TYPEID_WIDE_CHARACTER:
		len = 1;								// �ڴ���ֻռ��һ��wchar_t
		break;
	case TYPEID_ASCII:
	case TYPEID_UTF8:
	case TYPEID_ISO_8859_1:
	case TYPEID_GB2312:
		len = 1;
		break;
	case TYPEID_UTF16_LittleEndian:
	case TYPEID_UTF16_BigEndian:
		len = 2;
		break;
	case TYPEID_GB18030:
		len = 4;
		break;
	}
	return len;
}

/**
 * Copy wchar string
 */
void _STR_WCharsCpy(wchar_t * dest, const wchar_t * source, size_t len) {
	size_t i;
	if (source && len <= WINSTRING_WCHAR_MAX)
		for (i = 0; i < len; i++)
			dest[i] = source[i];
}

/**
 * Concatenate wchar string
 */
void _STR_WCharsCat(wchar_t * dest, const wchar_t * source, size_t beginIdx, size_t len) {
	size_t i;
	if (source && len <= WINSTRING_WCHAR_MAX)
		for (i = 0; i < len; i++)
			dest[i + beginIdx] = source[i];
}


void _STR_BytesCpy(char * dest, const char * source, size_t len) {
	size_t i;
	if (source && len <= WINSTRING_BYTES_MAX)
		for (i = 0; i < len; i++)
			dest[i] = source[i];
}

void _STR_BytesCat(char * dest, const char * source, size_t beginIdx, size_t len) {
	size_t i;
	if (source && len <= WINSTRING_BYTES_MAX)
		for (i = 0; i < len; i++)
			dest[i + beginIdx] = source[i];
}

void _STR_BytesClr(char * str, size_t len) {
	size_t i;
	if (str) {
		for (i = 0; i < len; i++) {
			str[i] = '\0';
		}
	}
}

wchar_t * _PARSE_Bytes2WChar(const char * bytes, UINT type) {
	wchar_t * res = NULL;
	wchar_t * lpWCTemp = NULL;
	int wcharNum = 0;								// include the one more space for '\0\0'
	if (bytes) {
		if (type == TYPEID_ASCII ||
			type == TYPEID_UTF8 ||
			type == TYPEID_UTF16_LittleEndian ||
			type == TYPEID_UTF16_BigEndian ||
			type == TYPEID_ISO_8859_1 ||
			type == TYPEID_GB18030 ||
			type == TYPEID_GB2312) {
			wcharNum = MultiByteToWideChar(type, 0, bytes, -1, NULL, 0);
			if (wcharNum) {
				try {
					lpWCTemp = (wchar_t *)DBG_NEW wchar_t[wcharNum];
					if (MultiByteToWideChar(type, 0, bytes, -1, lpWCTemp, wcharNum)) {
						// Succeed!
						res = lpWCTemp;
						lpWCTemp = NULL;
					}
				}
				catch (std::bad_alloc & e) { 
					// Do nothing 
					UNUSED_VAR(e);
				}
			}
		}
	}
	// Space release
	if (lpWCTemp)
		delete[] lpWCTemp;
	// Return
	return res;
}

char *_PARSE_WChar2Bytes(const wchar_t * wchars, UINT type) {
	char * res = NULL;
	char * lpBytesTMP = NULL;
	int bytesNum = 0;			// include the one more space for '\0', '\0\0' or other types
	// Transfer
	if (wchars) {
		if (type == TYPEID_ASCII ||
			type == TYPEID_UTF8 ||
			type == TYPEID_UTF16_LittleEndian ||
			type == TYPEID_UTF16_BigEndian ||
			type == TYPEID_ISO_8859_1 ||
			type == TYPEID_GB18030 ||
			type == TYPEID_GB2312) {
			bytesNum = WideCharToMultiByte(type, 0, wchars, -1, NULL, 0, NULL, NULL);
			if (bytesNum) {
				try{
					lpBytesTMP = new char[bytesNum];
					if (WideCharToMultiByte(type, 0, wchars, -1, lpBytesTMP, bytesNum, NULL, NULL)) {
						// Succeed!
						res = lpBytesTMP;
						lpBytesTMP = NULL;
					}
				}
				catch(std::bad_alloc & e){
					// Do nothing
					UNUSED_VAR(e);
				}
				
			}
		}
	}
	// Space release
	if (lpBytesTMP)
		delete[] lpBytesTMP;
	// Return
	return res;
}

#endif