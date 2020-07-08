# Libs_WinString
My personal string header file working in Windows C/C++ (only support English & Chinese)

[![Badge](https://img.shields.io/badge/link-996.icu-%23FF4D5B.svg?style=flat-square)](https://996.icu/#/en_US) [![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg?style=flat-square)](https://github.com/996icu/996.ICU/blob/master/LICENSE) [![Slack](https://img.shields.io/badge/slack-996icu-green.svg?style=flat-square)](https://join.slack.com/t/996icu/shared_invite/enQtNjI0MjEzMTUxNDI0LTkyMGViNmJiZjYwOWVlNzQ3NmQ4NTQyMDRiZTNmOWFkMzYxZWNmZGI0NDA4MWIwOGVhOThhMzc3NGQyMDBhZDc) [![HitCount](http://hits.dwyl.io/996icu/996.ICU.svg)](http://hits.dwyl.io/996icu/996.ICU)

## 支持的编码
* WIDE_CHARACTER

仅在内存中使用
* ASCII
在中文版Windows系统中，ASCII也可以支持中文。英文、英文标点符号仍采用ASCII编码，但是中文采用别的编码实现。大陆版Windows采用GB2312，台湾版Windows采用Big5
* UTF8
* UTF16_LittleEndian
* UTF16_BigEndian
* ISO_8859_1
* GB18030
* GB2312

### Windows Code Page Identifiers
[Windows编码ID](https://docs.microsoft.com/en-us/windows/win32/intl/code-page-identifiers)

License
---

[Anti-996 License](LICENSE)

 - The purpose of this license is to prevent anti-labour-law companies from using the software or codes under the license, and force those companies to weigh their way of working
 - See a [full list of projects](awesomelist/README.md) under Anti-996 License

 - This draft is adapted from the MIT license. For a more detailed explanation, please see [Wiki](https://github.com/kattgu7/996-License-Draft/wiki). This license is designed to be compatible with all major open source licenses.
 - For law professionals or anyone who is willing to contribute to future version directly, please go to [Anti-996-License-1.0](https://github.com/kattgu7/996-License-Draft). Thank you.
