#pragma once

#include "resource.h"
#include "ZFileHash.h"

using namespace std;

class ZMD5Checker : public FileHash::IHashProgressListener
{
public:
	static ZMD5Checker & getInstance();
	~ZMD5Checker(){}
	inline void setHwnd(HWND hWnd) { m_hWnd = hWnd; }
	void GetMD5SUMFile();
	void ShowMsg(const string & strMsg);
	void Execute();

	static int CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);
	static DWORD WINAPI ThreadFunc(LPVOID lpParam);

	void SetInitInputFileName(const string & strFileName)
	{
		m_strInitArgFileName = strFileName;
	}

	void OnProgress(int progressPercentage);		// percetage �� 100% �� 10000 �̴�.
	void RunThread();

	HINSTANCE hInstance;

private:

	bool ZMD5Checker::CreateShortCut(string strLnkTarget, string strLnkName, UINT SpecialFolder, string strLnkDescription, string strIconLocation, UINT IconIndex);
	void MakeSendToShortcut();
	void DeleteSendToShortcut();

	/// �������� Ÿ��Ʋ�� �����Ѵ�.
	void SetWindowTitle();

	void OnExit();
	bool CheckInvalidDataInput();
	void CheckSameStringMD5SUM();
	void StartFromInitFilename();	// �����Ҷ� ���� ���ڷ� �ٷ� �����Ѵ�.
	ZMD5Checker();
	void SetStop();
	HWND m_hWnd;
	FileHash::ZFileHash m_fileHash;

	string m_strInitArgFileName;			// ���� ���ڷ� ���ϸ��� �־������� ��Ƴ���.

	bool m_bMD5Summing;						// md5sum ���ΰ�.

	void EnableStartButton();	// Start ��ư�� Ȱ��ȭ, Stop ��ư�� ��Ȱ��ȭ
	void EnableStopButton();	// Stop ��ư�� Ȱ��ȭ, Start ��ư�� ��Ȱ��ȭ
};

#define MD5_OBJ		ZMD5Checker::getInstance()