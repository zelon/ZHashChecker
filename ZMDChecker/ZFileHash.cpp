#include "stdafx.h"
#include ".\zfilehash.h"
#include "md5class.h"

using namespace std;

using namespace FileHash;

ZFileHash::ZFileHash(void)
{
	m_bGoOn = true;
}

ZFileHash::~ZFileHash(void)
{
}


string ZFileHash::GetHashStringFromFile(const string & filename, IHashProgressListener * pProgressListener)
{
	string strRet;

	try
	{
		strRet = MDFile (filename.c_str(), pProgressListener);
	}
	catch ( FileHashException ex )
	{
		string msg = ex.GetExceptionMessage();
		//MessageBox(NULL, "Can't hash the file.", "md5Checker", MB_OK);
		MessageBox(NULL, msg.c_str(), "md5checker", MB_OK);
	}

	return strRet;
}

string ZFileHash::MDFile(const char *filename, IHashProgressListener * pProgressListener)
{
	unsigned __int64 filesize = 0;

	HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, 0);
	if ( hFile == INVALID_HANDLE_VALUE )
	{
		CloseHandle(hFile);
		_ASSERTE(!"Can't open file");
		throw FileHashException("������ �� �� �����ϴ�.");
	}
	DWORD highSize = 0;
	DWORD lowSize = GetFileSize(hFile, &highSize);

	filesize = (__int64)highSize << 32 | lowSize;

	CloseHandle(hFile);

	string ret;

	FILE *file;
	//  MD_CTX context;
	size_t len;
	unsigned char buffer[1024], digest[16];

	if ((file = fopen (filename, "rb")) == NULL)
	{
		throw FileHashException("������ �� �� �����ϴ�. ���ϸ��� �߸��Ǿ��ų�, �ٸ� ���α׷��� ���� ���� �� �ֽ��ϴ�.");
	}
	else
	{
		CMD5 md5;
		// MDInit (&context);

		if ( len = 0 )
		{
			int k =0;
		}

		__int64 lenTotal = 0;
		while (len = fread (buffer, (size_t)1, (size_t)1024, file))
		{
			if ( m_bGoOn == false )
			{
				throw HashStopException("����ڿ� ���� �ߴܵǾ����ϴ�.");
			}
			lenTotal += len;
			
			/// ���⼭ �ѹ� insertString �� ������ callback function �� ȣ���ؼ� �����Ȳ�� �� �� �ְ� �Ѵ�.
			if ( pProgressListener != NULL )
			{
				pProgressListener->OnProgress( (int)(lenTotal * 10000 / filesize));
			}
			md5.InsertString( buffer, (int)len );
		}
		md5.GetHashResult( digest );
		fclose (file);

		ret = MDPrint(digest);
	}
	return ret;
}

string ZFileHash::MDPrint(unsigned char *digest )
{
	unsigned int i;
	char buffer[5] = {0};

	string ret;

	for (i = 0; i < 16; i++)
	{
		memset(buffer, 0, sizeof(buffer));
		sprintf (buffer, "%02x", digest[i]);
		ret += buffer;
	}
	return ret;
}
