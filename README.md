# Libs_WinString
My personal string header file working in Windows C/C++ (only support English & Chinese)

[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg?style=flat-square)](https://github.com/996icu/996.ICU/blob/master/LICENSE) [![HitCount](http://hits.dwyl.com/WangSiCong1988/Libs_Win32String.svg)](http://hits.dwyl.com/WangSiCong1988/Libs_Win32String)

## 编译
`WinString_DLL`Release编译后，执行`build_prod.bat`生成生产环境文件

## 调用方式（DLL方式）
* DLL 导入库的文件<br>
	`WinString_DLL.h`导出WinString类<br>
	`WinString_DLL.dll`<br>
	`WinString_DLL.lib`<br>
* DLL 导入库添加到项目<br>
[Microsoft推荐方式：DLL 导入库添加到项目](https://docs.microsoft.com/zh-cn/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=vs-2019)
* 错误处理
	* `无法解析的外部符号 "__declspec(dllimport) public: wchar_t * __thiscall WinString::Parse2WideChar(void)" (__imp_?Parse2WideChar@WinString@@QAEPA_WXZ)`<br>
	在Configuration Properties-> C/C++-> Language中，设置Treat wchar_t as Built-in Type = No（如果选Yes报错）	
	* 手动释放WinString指针显示不可访问<br>
	因为调用Create()方法分配的空间是在DLL的堆栈中而不是你的程序中，因此不能采用delete释放。正确的做法是使用`WinString::Destroy(WinString * lp)`，来释放在DLL堆栈中的空间。所以`~WinString()`方法被设为私有，避免错误释放。
	```c++
	WinString * lpWS = WinString::Create();
	WinString::Destroy(lpWS);			// delete lpWS 显示不可访问，这个设置是为了强制编写者在DLL堆栈中释放空间
	```
	* `_CrtIsValidHeapPointer出错`<br>
	内存申请和释放不在同一堆栈上，详见[_CrtIsValidHeapPointer出错的解决方法](https://blog.csdn.net/u014287775/article/details/76098363)

## 支持的编码
* `WIDE_CHARACTER` 仅在内存中使用。**<font color="red">注意：这个格式vc/c++中直接使用的格式不同</font>**<br>
	* VC/C++ wchar_t实现方式<br>
	vc/c++中的英文字符仍然采用ASCII编码方式。可以设想，其他国家程序员利用vc/c++编写程序输入本国字符时，vc/c++则会采用该国的字符编码方式来处理这些字符。<br>
	以两字节为单位存放字符，即如果一个字符码为两字节，则在内存中占四字节，字符码为一字节，就占两字节。例如，字符串“中国abc”就存为如下方式:<br>
	```c++
	wchar_t str = "中国abc";
	```
	![wchar_t 空间分配规则](http://github.com/WangSiCong1988/Libs_WinString/blob/dev/img/wchar_t空间分配规则.bmp)<br>

	* WinString 实现方式<br>
	WinString中的wchar_t类型采用UTF-16（一个中文字符只占用2个字节），而不是简单的扩充空间版本ASCII编码。所以`printf("%ls")`打印时出现中文编码故障，因此要制定命令行编码方式
	```
	setlocale(LC_CTYPE, "chinese-simplified");		// 简体
	setlocale(LC_CTYPE, "chs");
	setlocale(LC_CTYPE, "chinese-traditional");		// 繁体，如果不成功，则采用指明区域的繁体
	setlocale(LC_ALL, "chinese-hongkong");			// 繁体，香港
	setlocale(LC_CTYPE, "chinese_taiwan.950");		// 繁体，台湾
	```
* `ASCII` 在中文版Windows系统中，ASCII也可以支持中文。英文、英文标点符号仍采用ASCII编码，但是中文采用别的编码实现。大陆版Windows采用GB2312，台湾版Windows采用Big5
* `UTF8`
* `UTF16_LittleEndian`
* `UTF16_BigEndian`
* `ISO_8859_1`
* `GB18030`<br>
![GB18030字符集编码规则](http://github.com/WangSiCong1988/Libs_WinString/blob/dev/img/GB18030字符集编码规则.webp)<br>
* `GB2312`

### Windows Code Page Identifiers
[Windows编码ID](https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers)

## 源代码修改注意事项
* public methods must be _stdcall<br>

如果通过VC++编写的DLL欲被其他语言编写的程序调用，应将函数的调用方式声明为__stdcall方式
* 内存泄漏测试方式<br>
[使用 CRT 库查找内存泄漏](https://docs.microsoft.com/zh-cn/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019)<br>
CRT库的建议会导致new (str::nothrow)无法被编译器识别，所以编写时new必须采用try catch方式检测失败（设置异常函数的方法被抛弃：因为异常函数编写时可能再发生异常，导致逻辑复杂）
* try catch 异常捕捉采用引用捕捉<br>
当catch捕捉到异常时，如果采用按值传递，会把异常对象拷贝一次，才专递过来（按引用就没有这个拷贝过程）。异常处理可能工作在内存极度紧张的情况，因此采用按引用传递，减少内存消耗
* `WinString()`和`~WinString()`必须设为私有，让内存释放强制调用DLL提供的方法，避免内存跨模块释放<br>
详见[_CrtIsValidHeapPointer出错的解决方法](https://blog.csdn.net/u014287775/article/details/76098363)

License
---

[Anti-996 License](LICENSE)

[![Badge](https://img.shields.io/badge/link-996.icu-%23FF4D5B.svg?style=flat-square)](https://996.icu/#/en_US) [![Slack](https://img.shields.io/badge/slack-996icu-green.svg?style=flat-square)](https://join.slack.com/t/996icu/shared_invite/enQtNjI0MjEzMTUxNDI0LTkyMGViNmJiZjYwOWVlNzQ3NmQ4NTQyMDRiZTNmOWFkMzYxZWNmZGI0NDA4MWIwOGVhOThhMzc3NGQyMDBhZDc)

 - The purpose of this license is to prevent anti-labour-law companies from using the software or codes under the license, and force those companies to weigh their way of working
 - See a [full list of projects](awesomelist/README.md) under Anti-996 License

 - This draft is adapted from the MIT license. For a more detailed explanation, please see [Wiki](https://github.com/kattgu7/996-License-Draft/wiki). This license is designed to be compatible with all major open source licenses.
 - For law professionals or anyone who is willing to contribute to future version directly, please go to [Anti-996-License-1.0](https://github.com/kattgu7/996-License-Draft). Thank you.
