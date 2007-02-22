// md5Checker.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "md5Checker.h"
#include "resource.h"

#include <shellapi.h>

#include "CommonDefine.h"
using namespace std;




ZMD5Checker & ZMD5Checker::getInstance()
{
	static ZMD5Checker aInstance;
	return aInstance;
}

ZMD5Checker::ZMD5Checker()
: m_bMD5Summing(false)
{
	m_hashType = eHashType_MD5SUM;
}

void ZMD5Checker::SetWindowTitle()
{
	char szFilename[MAX_PATH];

	// 파일명을 받아온다.
	GetDlgItemText(m_hWnd, IDC_GOT_FILE, szFilename, MAX_PATH);

	char szTitle[MAX_PATH + 50];

	// 받아온 파일명을 가지고 세팅한다.
	if ( strlen(szFilename) == 0 )
	{
		sprintf(szTitle, "ZMD5Checker v%s :: http://www.wimy.com", g_strVersion.c_str());
	}
	else
	{
		char szFile[FILENAME_MAX] = { 0 };
		char szExt[FILENAME_MAX] = { 0 };
		_splitpath(szFilename, 0, 0, szFile, szExt);

		sprintf(szTitle, "%s%s - ZMD5Checker v%s :: http://www.wimy.com", szFile, szExt, g_strVersion.c_str());
	}

	SetWindowText(m_hWnd, szTitle);
}

void ZMD5Checker::StartFromInitFilename()
{
	// 파일명을 세팅한다.
	SetDlgItemText(m_hWnd, IDC_GOT_FILE, m_strInitArgFileName.c_str());

	// 윈도우 타이틀을 세팅한다.
	SetWindowTitle();

	// 시작 버튼을 누른다.(꼭 PostMessage() 를 해야 DeakLock 에 안걸린다.)
	PostMessage(m_hWnd, WM_COMMAND, IDC_BT_START, 0);
}



BOOL GetSpecialFolder(UINT SpecialFolder, string & strSpecialFolderString)
{
	HRESULT hr;
	// Find the Special Folder:
	// Allocate a pointer to an Item ID list
	LPITEMIDLIST pidl;

	// Get a pointer to an item ID list that
	// represents the path of a special folder
	hr = SHGetSpecialFolderLocation(NULL, SpecialFolder, &pidl);
	if(SUCCEEDED(hr))
	{	// Convert the item ID list's binary
		// representation into a file system path
		char szPath[_MAX_PATH];
		if(SHGetPathFromIDList(pidl, szPath))
		{
			// Allocate a pointer to an IMalloc interface
			LPMALLOC pMalloc;

			// Get the address of our task allocator's IMalloc interface
			hr = SHGetMalloc(&pMalloc);

			// Free the item ID list allocated by SHGetSpecialFolderLocation
			pMalloc->Free(pidl);

			// Free our task allocator
			pMalloc->Release();

			// Work with the special folder's path (contained in szPath)
			strSpecialFolderString = szPath;
			strSpecialFolderString += "\\";

			return TRUE;
		}
	}
	return FALSE;
}

// CreateLink - uses the Shell's IShellLink and IPersistFile interfaces 
//              to create and store a shortcut to the specified object. 
//
// Returns the result of calling the member functions of the interfaces. 
//
// Parameters:
// lpszPathObj  - address of a buffer containing the path of the object. 
// lpszPathLink - address of a buffer containing the path where the 
//                Shell link is to be stored. 
// lpszDesc     - address of a buffer containing the description of the 
//                Shell link. 

HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc) 
{ 
	HRESULT hres; 
	IShellLink* psl; 

	// Get a pointer to the IShellLink interface. 
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
		IID_IShellLink, (LPVOID*)&psl); 
	if (SUCCEEDED(hres)) 
	{ 
		IPersistFile* ppf; 

		// Set the path to the shortcut target and add the description. 
		psl->SetPath(lpszPathObj); 
		psl->SetDescription(lpszDesc); 

		// Query IShellLink for the IPersistFile interface for saving the 
		// shortcut in persistent storage. 
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 

		if (SUCCEEDED(hres)) 
		{ 
			WCHAR wsz[MAX_PATH]; 

			// Ensure that the string is Unicode. 
			MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH); 

			// TODO: Check return value from MultiByteWideChar to ensure 
			/*
			*					if(!IconLocation.IsEmpty())
			{
			hr = psl->SetIconLocation(IconLocation, IconIndex);	
			#ifdef _DEBUG
			if(FAILED(hr))
			TRACE("IconLocation not changed!\n");
			#endif
			}
			 */

				// Save the link by calling IPersistFile::Save. 
			hres = ppf->Save(wsz, TRUE); 
			ppf->Release(); 
		} 
		psl->Release(); 
	} 
	return hres; 
}

void ZMD5Checker::MakeSendToShortcut()
{
	TCHAR szTemp[MAX_PATH];
	GetModuleFileName(NULL, szTemp, MAX_PATH);

	string strPosition;
	GetSpecialFolder(CSIDL_SENDTO, strPosition);
	strPosition += "MD5SUM 얻기.lnk";
	HRESULT hRet = CreateLink(szTemp, strPosition.c_str(), "파일의 MD5SUM 을 구합니다.");

	if ( hRet == S_OK )
	{
		ShowMsg("탐색기의 보내기 메뉴에 바로가기를 생성했습니다.");
	}
	//CreateLink()
	//CShortcut aShorcut;
	//aShorcut.SetCmdArguments()
}

void ZMD5Checker::DeleteSendToShortcut()
{
	TCHAR szTemp[MAX_PATH];
	GetModuleFileName(NULL, szTemp, MAX_PATH);

	string strPosition;
	GetSpecialFolder(CSIDL_SENDTO, strPosition);
	strPosition += "MD5SUM 얻기.lnk";

	if ( DeleteFile(strPosition.c_str()) )
	{
		ShowMsg("탐색기의 보내기 메뉴의 바로가기를 삭제했습니다.");
	}
	else
	{
		ShowMsg("탐색기의 보내기 메뉴의 바로가기를 삭제하는데 실패했습니다.");
	}
}

void ZMD5Checker::OnExit()
{
	if ( MD5_OBJ.m_bMD5Summing )
	{
		INT_PTR ret = MessageBox(m_hWnd, "MD5Sum 이 진행 중입니다. 종료하시겠습니까?", "ZMDChecker", MB_YESNO);

		if ( ret == IDYES )
		{
			MD5_OBJ.SetStop();
			PostQuitMessage(0);
		}
	}
	else
	{
		MD5_OBJ.SetStop();
		PostQuitMessage(0);
	}
}

int CALLBACK ZMD5Checker::WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage)
	{
	case WM_INITDIALOG:
		// 타이틀바를 세팅한다.
		ZMD5Checker::getInstance().SetWindowTitle();

		MD5_OBJ.setHwnd(hWnd);

		SendMessage(GetDlgItem(hWnd, IDC_MD5SUM_PROGRESS), PBM_SETRANGE, 0, MAKELPARAM(0, 10000));

		// 아이콘을 입힌다.
		SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)(LoadIcon(ZMD5Checker::getInstance().hInstance, MAKEINTRESOURCE(IDI_BIG_ICON))));

		// 시작할 때 입력받은 값이 있다면 바로 시작한다.
		if ( MD5_OBJ.m_strInitArgFileName.size() > 0 )
		{
			MD5_OBJ.StartFromInitFilename();
		}

		/// 시작할 때 MD5Sum 을 기본으로 한다.
		//::SendMessage(GetDlgItem(hWnd, IDC_RADIO_MD5), BM_SETCHECK, 0, 0);
		CheckRadioButton(hWnd, IDC_RADIO_MD5, IDC_RADIO_SHA1SUM, IDC_RADIO_MD5);

		// 탐색기에서의 Drag&Drop 을 가능하게 한다.
		DragAcceptFiles(hWnd, TRUE);

		return TRUE;

	case WM_DROPFILES:///< 탐색기에서 드래그 앤 드랍으로 놓았을 때
		{
			HDROP hDrop = (HDROP)wParam;

			UINT iFileNum = 0;

			iFileNum = DragQueryFile(hDrop, 0xffffffff, 0, 0);

			if ( iFileNum <= 0 ) break;

			TCHAR szFileName[MAX_PATH] = { 0 };
			DragQueryFile(hDrop, 0, szFileName, MAX_PATH);

			SetWindowText(GetDlgItem(hWnd, IDC_GOT_FILE), szFileName);
		}
		break;

	case WM_COMMAND:
		{
			switch ( wParam )
			{
			case IDOK:
				MD5_OBJ.OnExit();
				break;

			case IDC_BROWSE:
				{
					MD5_OBJ.GetMD5SUMFile();
				}
				break;

			case IDC_BT_START:		// Start and compare 버튼을 눌렀을 때
				{
					MD5_OBJ.EnableStopButton();
					MD5_OBJ.RunThread();
				}
				break;

			case IDC_BT_STOP:		// Stop 버튼을 눌렀을 때
				{
					MD5_OBJ.EnableStartButton();
					MD5_OBJ.SetStop();
				}
				break;

			case IDC_BT_COMPARE:	// 두 개의 문자열이 같은지 비교한다.
				MD5_OBJ.CheckSameStringMD5SUM();
				break;

			case IDC_BT_ADD_SENDTO:	// 탐색기의 바로가기 메뉴에 넣는다.
				{
					MD5_OBJ.MakeSendToShortcut();
				}
				break;

			case IDC_BT_REMOVE_SENDTO:	// 탐색기의 바로가기 메뉴를 제거한다.
				{
					MD5_OBJ.DeleteSendToShortcut();
				}
				break;
			}
		}
		break;

	case WM_CLOSE:
		{
			MD5_OBJ.OnExit();
		}
		break;

	case WM_PAINT:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return FALSE;
}

void ZMD5Checker::EnableStartButton()	// Start 버튼을 활성화, Stop 버튼을 비활성화
{
	EnableWindow(GetDlgItem(m_hWnd, IDC_BT_START), TRUE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_BT_STOP), FALSE);

	m_bMD5Summing = false;
}

void ZMD5Checker::EnableStopButton()	// Stop 버튼을 활성화, Start 버튼을 비활성화
{
	EnableWindow(GetDlgItem(m_hWnd, IDC_BT_STOP), TRUE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_BT_START), FALSE);

	m_bMD5Summing = true;
}

void ZMD5Checker::GetMD5SUMFile()
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "FavoriteFiles(iso,bin,zip,gz)\0*.iso;*.bin;*.zip;*.gz\0CDImages(iso,bin)\0*.iso;*.bin\0All(*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn)==TRUE) 
	{
		/*
		hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
		0, (LPSECURITY_ATTRIBUTES) NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		(HANDLE) NULL);
		*/
		SetWindowText(GetDlgItem(m_hWnd, IDC_GOT_FILE), ofn.lpstrFile);

		// 윈도우 타이틀을 세팅한다.
		SetWindowTitle();
	}
}

bool ZMD5Checker::CheckInvalidDataInput()	// 입력값들이 제대로 있는지 체크한다.
{
	char szTemp[256];
	string strFileName;

	GetDlgItemText(m_hWnd, IDC_GOT_FILE, szTemp, 256);
	strFileName = szTemp;

	if ( strFileName.size() <= 0 )
	{
		ShowMsg("파일명이 올바르지 않습니다.");
		return false;
	}

	// 파일이 존재하는지 체크한다.
	if( (_access( strFileName.c_str(), 0 )) == -1 )
	{
		ShowMsg("존재하지 않는 파일입니다.");
		return false;
	}

	return true;
}

void ZMD5Checker::CheckSameStringMD5SUM()
{
	char szTemp[256];
	string strResult, strInput;

	// 입력값을 가져온다.
	GetDlgItemText(m_hWnd, IDC_TARGET_MD5SUM, szTemp, 256);
	strInput = szTemp;

	if ( strInput.size() <= 0 )
	{
		ShowMsg("비교할 값을 넣어주세요.");
		return;
	}

	if ( strInput.size() <= 31 )
	{
		ShowMsg("입력값의 길이가 짧습니다.");
		return;
	}

#pragma message("TODO : 입력값이 MD5SUM 형식인지 검사한다.")

	// 결과값을 가져온다.
	GetDlgItemText(m_hWnd, IDC_GOT_MD5SUM, szTemp, 256);
	strResult = szTemp;

	if ( m_bMD5Summing )
	{
		ShowMsg("MD5SUM 을 진행 중 입니다.");
		return;
	}

	if ( strResult.size() <= 0 )
	{
		ShowMsg("파일을 선택한 후 Start 를 눌러주세요.");
		return;
	}

	// 비교해서 결과를 보여준다.
	if ( strInput == strResult )
	{
		ShowMsg("같은 MD5SUM 값입니다.");
	}
	else
	{
		ShowMsg("서로 다른 MD5SUM 값입니다!!");
	}
}

void ZMD5Checker::Execute()
{
	// md5sum 을 체크한다.

	m_fileHash.setGoOn(true);

	try
	{
		char szFileName[FILENAME_MAX];
		GetDlgItemText(m_hWnd, IDC_GOT_FILE, szFileName, FILENAME_MAX);

		if ( SendMessage(GetDlgItem(m_hWnd, IDC_RADIO_MD5), BM_GETCHECK, 0, 0) == BST_CHECKED )
		{
			m_hashType = eHashType_MD5SUM;
			//MessageBox(m_hWnd, "md5", "md5", MB_OK);
		}
		else
		{
			m_hashType = eHashType_SHA1;
			//MessageBox(m_hWnd, "sha", "sha", MB_OK);
		}

		string strGetMD5SUM = m_fileHash.GetHashStringFromFile(szFileName, m_hashType, this);

		// 체크된 내용을 화면에 표시한다.
		SetDlgItemText(m_hWnd, IDC_GOT_MD5SUM, strGetMD5SUM.c_str());
	}
	catch ( FileHash::HashStopException & e )
	{
		MessageBox(m_hWnd, e.getMessage().c_str(), "MD5Checker", MB_OK);
	}
}

void ZMD5Checker::ShowMsg(const string & strMsg)
{
	MessageBox(m_hWnd, strMsg.c_str(), "MD5SUMChecker", MB_OK);
}

void ZMD5Checker::SetStop()
{
	m_fileHash.setGoOn(false);
	OnProgress(0);
}

void ZMD5Checker::OnProgress(int progressPercentage)
{
	// 프로그레스바를 진행시킨다.
	PostMessage(GetDlgItem(m_hWnd, IDC_MD5SUM_PROGRESS), PBM_SETPOS, (int)progressPercentage, 0);

	// 화면에 간단히 깜빡이는 것을 보여준다.
	static int iCallCount = 0;
	static char szBar[10] = { 0 };
	static DWORD dwLast = 0;

	if ( (GetTickCount() - dwLast) > 250 )
	{
		dwLast = GetTickCount();
		iCallCount = ( iCallCount + 1 ) % 4;

		switch ( iCallCount )
		{
		case 0:
			sprintf(szBar, "");
			break;

		case 1:
			sprintf(szBar, ">");
			break;

		case 2:
			sprintf(szBar, ">>");
			break;

		case 3:
			sprintf(szBar, ">>>");
			break;

		default:
			_ASSERTE(false);
		}
		SetDlgItemText(m_hWnd, IDC_PROCESSING, szBar);
	}

}

DWORD WINAPI ZMD5Checker::ThreadFunc(LPVOID lpParam)
{
	ZMD5Checker * pThis = (ZMD5Checker*)lpParam;

	pThis->Execute();

	MD5_OBJ.EnableStartButton();

	return 0;
}

void ZMD5Checker::RunThread()
{
	DWORD dwThreadID;
	HANDLE hThread = CreateThread(0, 0, ThreadFunc, this, 0, &dwThreadID);
	CloseHandle(hThread);
}