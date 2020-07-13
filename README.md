# Libs_WinString
My personal string header file working in Windows C/C++ (only support English & Chinese)

[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg?style=flat-square)](https://github.com/996icu/996.ICU/blob/master/LICENSE) [![HitCount](http://hits.dwyl.com/WangSiCong1988/Libs_Win32String.svg)](http://hits.dwyl.com/WangSiCong1988/Libs_Win32String)

## ����
`WinString_DLL`Release�����ִ��`build_prod.bat`�������������ļ�

## ���÷�ʽ��DLL��ʽ��
* DLL �������ļ�<br>
	`WinString_DLL.h`����WinString��<br>
	`WinString_DLL.dll`<br>
	`WinString_DLL.lib`<br>
* DLL �������ӵ���Ŀ<br>
[Microsoft�Ƽ���ʽ��DLL �������ӵ���Ŀ](https://docs.microsoft.com/zh-cn/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=vs-2019)
* ������
	* `�޷��������ⲿ���� "__declspec(dllimport) public: wchar_t * __thiscall WinString::Parse2WideChar(void)" (__imp_?Parse2WideChar@WinString@@QAEPA_WXZ)`<br>
	��Configuration Properties-> C/C++-> Language�У�����Treat wchar_t as Built-in Type = No�����ѡYes����	
	* �ֶ��ͷ�WinStringָ����ʾ���ɷ���<br>
	��Ϊ����Create()��������Ŀռ�����DLL�Ķ�ջ�ж�������ĳ����У���˲��ܲ���delete�ͷš���ȷ��������ʹ��`WinString::Destroy(WinString * lp)`�����ͷ���DLL��ջ�еĿռ䡣����`~WinString()`��������Ϊ˽�У���������ͷš�
	```c++
	WinString * lpWS = WinString::Create();
	WinString::Destroy(lpWS);			// delete lpWS ��ʾ���ɷ��ʣ����������Ϊ��ǿ�Ʊ�д����DLL��ջ���ͷſռ�
	```
	* `_CrtIsValidHeapPointer����`<br>
	�ڴ�������ͷŲ���ͬһ��ջ�ϣ����[_CrtIsValidHeapPointer����Ľ������](https://blog.csdn.net/u014287775/article/details/76098363)

## ֧�ֵı���
* `WIDE_CHARACTER` �����ڴ���ʹ�á�**<font color="red">ע�⣺�����ʽvc/c++��ֱ��ʹ�õĸ�ʽ��ͬ</font>**<br>
	* VC/C++ wchar_tʵ�ַ�ʽ<br>
	vc/c++�е�Ӣ���ַ���Ȼ����ASCII���뷽ʽ���������룬�������ҳ���Ա����vc/c++��д�������뱾���ַ�ʱ��vc/c++�����øù����ַ����뷽ʽ��������Щ�ַ���<br>
	�����ֽ�Ϊ��λ����ַ��������һ���ַ���Ϊ���ֽڣ������ڴ���ռ���ֽڣ��ַ���Ϊһ�ֽڣ���ռ���ֽڡ����磬�ַ������й�abc���ʹ�Ϊ���·�ʽ:<br>
	```c++
	wchar_t str = "�й�abc";
	```
	![wchar_t �ռ�������][/img/wchar_t�ռ�������.bmp]<br>

	* WinString ʵ�ַ�ʽ<br>
	WinString�е�wchar_t���Ͳ���UTF-16��һ�������ַ�ֻռ��2���ֽڣ��������Ǽ򵥵�����ռ�汾ASCII���롣����`printf("%ls")`��ӡʱ�������ı�����ϣ����Ҫ�ƶ������б��뷽ʽ
	```
	setlocale(LC_CTYPE, "chinese-simplified");		// ����
	setlocale(LC_CTYPE, "chs");
	setlocale(LC_CTYPE, "chinese-traditional");		// ���壬������ɹ��������ָ������ķ���
	setlocale(LC_ALL, "chinese-hongkong");			// ���壬���
	setlocale(LC_CTYPE, "chinese_taiwan.950");		// ���壬̨��
	```
* `ASCII` �����İ�Windowsϵͳ�У�ASCIIҲ����֧�����ġ�Ӣ�ġ�Ӣ�ı������Բ���ASCII���룬�������Ĳ��ñ�ı���ʵ�֡���½��Windows����GB2312��̨���Windows����Big5
* `UTF8`
* `UTF16_LittleEndian`
* `UTF16_BigEndian`
* `ISO_8859_1`
* `GB18030`<br>
![GB18030�ַ����������][/img/GB18030�ַ����������.webp]<br>
* `GB2312`

### Windows Code Page Identifiers
[Windows����ID](https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers)

## Դ�����޸�ע������
* public methods must be _stdcall<br>

���ͨ��VC++��д��DLL�����������Ա�д�ĳ�����ã�Ӧ�������ĵ��÷�ʽ����Ϊ__stdcall��ʽ
* �ڴ�й©���Է�ʽ<br>
[ʹ�� CRT ������ڴ�й©](https://docs.microsoft.com/zh-cn/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019)<br>
CRT��Ľ���ᵼ��new (str::nothrow)�޷���������ʶ�����Ա�дʱnew�������try catch��ʽ���ʧ�ܣ������쳣�����ķ�������������Ϊ�쳣������дʱ�����ٷ����쳣�������߼����ӣ�
* try catch �쳣��׽�������ò�׽<br>
��catch��׽���쳣ʱ��������ð�ֵ���ݣ�����쳣���󿽱�һ�Σ���ר�ݹ����������þ�û������������̣����쳣������ܹ������ڴ漫�Ƚ��ŵ��������˲��ð����ô��ݣ������ڴ�����
* `WinString()`��`~WinString()`������Ϊ˽�У����ڴ��ͷ�ǿ�Ƶ���DLL�ṩ�ķ����������ڴ��ģ���ͷ�<br>
���[_CrtIsValidHeapPointer����Ľ������](https://blog.csdn.net/u014287775/article/details/76098363)

License
---

[Anti-996 License](LICENSE)

[![Badge](https://img.shields.io/badge/link-996.icu-%23FF4D5B.svg?style=flat-square)](https://996.icu/#/en_US) [![Slack](https://img.shields.io/badge/slack-996icu-green.svg?style=flat-square)](https://join.slack.com/t/996icu/shared_invite/enQtNjI0MjEzMTUxNDI0LTkyMGViNmJiZjYwOWVlNzQ3NmQ4NTQyMDRiZTNmOWFkMzYxZWNmZGI0NDA4MWIwOGVhOThhMzc3NGQyMDBhZDc)

 - The purpose of this license is to prevent anti-labour-law companies from using the software or codes under the license, and force those companies to weigh their way of working
 - See a [full list of projects](awesomelist/README.md) under Anti-996 License

 - This draft is adapted from the MIT license. For a more detailed explanation, please see [Wiki](https://github.com/kattgu7/996-License-Draft/wiki). This license is designed to be compatible with all major open source licenses.
 - For law professionals or anyone who is willing to contribute to future version directly, please go to [Anti-996-License-1.0](https://github.com/kattgu7/996-License-Draft). Thank you.