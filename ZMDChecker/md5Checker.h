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

	void OnProgress(int progressPercentage);		// percetage 의 100% 는 10000 이다.
	void RunThread();

	HINSTANCE hInstance;

private:

	bool ZMD5Checker::CreateShortCut(string strLnkTarget, string strLnkName, UINT SpecialFolder, string strLnkDescription, string strIconLocation, UINT IconIndex);
	void MakeSendToShortcut();
	void DeleteSendToShortcut();

	/// 윈도우의 타이틀을 설정한다.
	void SetWindowTitle();

	void OnExit();
	bool CheckInvalidDataInput();
	void CheckSameStringMD5SUM();
	void StartFromInitFilename();	// 시작할때 받은 인자로 바로 시작한다.
	ZMD5Checker();
	void SetStop();
	HWND m_hWnd;
	FileHash::ZFileHash m_fileHash;

	string m_strInitArgFileName;			// 실행 인자로 파일명이 주어졌을때 담아놓음.

	bool m_bMD5Summing;						// md5sum 중인가.

	void EnableStartButton();	// Start 버튼을 활성화, Stop 버튼을 비활성화
	void EnableStopButton();	// Stop 버튼을 활성화, Start 버튼을 비활성화
};

#define MD5_OBJ		ZMD5Checker::getInstance()