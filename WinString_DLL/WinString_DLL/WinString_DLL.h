// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� WINSTRING_DLL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// WINSTRING_DLL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef WINSTRING_DLL_EXPORTS
#define WINSTRING_DLL_API __declspec(dllexport)
#else
#define WINSTRING_DLL_API __declspec(dllimport)
#endif

// �����Ǵ� WinString_DLL.dll ������
class WINSTRING_DLL_API CWinString_DLL {
public:
	CWinString_DLL(void);
	// TODO:  �ڴ�������ķ�����
};

extern WINSTRING_DLL_API int nWinString_DLL;

WINSTRING_DLL_API int fnWinString_DLL(void);
