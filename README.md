# Libs_WinString
My personal string header file working in Windows C/C++ (only support English & Chinese)

[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg?style=flat-square)](https://github.com/996icu/996.ICU/blob/master/LICENSE) [![HitCount](http://hits.dwyl.com/WangSiCong1988/Libs_Win32String.svg)](http://hits.dwyl.com/WangSiCong1988/Libs_Win32String)

## 支持的编码
* `WIDE_CHARACTER` 仅在内存中使用。**<font color="red">注意：这个格式vc/c++中直接使用的格式不同</font>**<br>
	* VC/C++ wchar_t实现方式<br>
	vc/c++中的英文字符仍然采用ASCII编码方式。可以设想，其他国家程序员利用vc/c++编写程序输入本国字符时，vc/c++则会采用该国的字符编码方式来处理这些字符。<br>
	以两字节为单位存放字符，即如果一个字符码为两字节，则在内存中占四字节，字符码为一字节，就占两字节。例如，字符串“中国abc”就存为如下方式:<br>
	```c++
	wchar_t str = "中国abc";
	```
	![wchar_t 空间分配规则](/img/WideCharSet.bmp)<br>

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
![GB18030字符集编码规则](/img/GB18030字符集.webp)<br>
* `GB2312`

### Windows Code Page Identifiers
[Windows编码ID](https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers)

## 源代码修改注意事项
### 内存泄漏测试方式
[使用 CRT 库查找内存泄漏](https://docs.microsoft.com/zh-cn/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019)<br>
CRT库的建议会导致new (str::nothrow)无法被编译器识别，所以编写时new必须采用try catch方式检测失败（设置异常函数的方法被抛弃：因为异常函数编写时可能再发生异常，导致逻辑复杂）

### try catch 异常捕捉采用引用捕捉
当catch捕捉到异常时，如果采用按值传递，会把异常对象拷贝一次，才专递过来（按引用就没有这个拷贝过程）。异常处理可能工作在内存极度紧张的情况，因此采用按引用传递，减少内存消耗

License
---

[Anti-996 License](LICENSE)

[![Badge](https://img.shields.io/badge/link-996.icu-%23FF4D5B.svg?style=flat-square)](https://996.icu/#/en_US) [![Slack](https://img.shields.io/badge/slack-996icu-green.svg?style=flat-square)](https://join.slack.com/t/996icu/shared_invite/enQtNjI0MjEzMTUxNDI0LTkyMGViNmJiZjYwOWVlNzQ3NmQ4NTQyMDRiZTNmOWFkMzYxZWNmZGI0NDA4MWIwOGVhOThhMzc3NGQyMDBhZDc)

 - The purpose of this license is to prevent anti-labour-law companies from using the software or codes under the license, and force those companies to weigh their way of working
 - See a [full list of projects](awesomelist/README.md) under Anti-996 License

 - This draft is adapted from the MIT license. For a more detailed explanation, please see [Wiki](https://github.com/kattgu7/996-License-Draft/wiki). This license is designed to be compatible with all major open source licenses.
 - For law professionals or anyone who is willing to contribute to future version directly, please go to [Anti-996-License-1.0](https://github.com/kattgu7/996-License-Draft). Thank you.
