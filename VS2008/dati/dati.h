// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� DATI_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// DATI_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef DATI_EXPORTS
#define DATI_API __declspec(dllexport)
#else
#define DATI_API __declspec(dllimport)
#endif

// �����Ǵ� dati.dll ������
class DATI_API Cdati {
public:
	Cdati(void);
	// TODO: �ڴ�������ķ�����
};

extern DATI_API int ndati;

DATI_API int fndati(void);
