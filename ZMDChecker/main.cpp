#include "stdafx.h"
#include "resource.h"
#include "md5Checker.h"

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance ,LPSTR lpszCmdParam,int nCmdShow)
{
	CoInitialize(NULL);

	if ( strlen(lpszCmdParam) > 0 )
	{
		// ���� ����ǥ�� �����ϰ� ������(����ȭ�鿡�� ������ ��)
		size_t iLen = strlen(lpszCmdParam);
		string strFileName;
		for ( unsigned int i=0; i<iLen; ++i)
		{
			if ( lpszCmdParam[i] == '\"')
			{
				continue;
			}
			strFileName.push_back(lpszCmdParam[i]);
		}


		MD5_OBJ.SetInitInputFileName(strFileName);
	}

	MD5_OBJ.hInstance = hInstance;

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDLG), NULL, ZMD5Checker::WndProc);

	CoUninitialize();
	return 0;
}
