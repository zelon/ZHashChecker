#include "stdafx.h"
#include ".\zfilehash.h"
#include "md5class.h"
#include "IHashFunction.h"
#include "sha-1.h"

using namespace std;

using namespace FileHash;

ZFileHash::ZFileHash(void)
{
	m_bGoOn = true;
}

ZFileHash::~ZFileHash(void)
{
}


string ZFileHash::GetHashStringFromFile(const string & filename, const eHashType hashType, IHashProgressListener * pProgressListener)
{
	string strRet;

	try
	{
		strRet = HashFile (filename.c_str(), hashType, pProgressListener);
	}
	catch ( FileHashException ex )
	{
		string msg = ex.GetExceptionMessage();
		//MessageBox(NULL, "Can't hash the file.", "md5Checker", MB_OK);
		MessageBox(NULL, msg.c_str(), "md5checker", MB_OK);
	}

	return strRet;
}

string ZFileHash::HashFile(const char * szFilename, const eHashType hashType, IHashProgressListener * pProgressListener)
{
	unsigned __int64 filesize = 0;

	HANDLE hFile = CreateFile(szFilename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, 0);
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
	unsigned char buffer[1024], digest[20];

	if ((file = fopen (szFilename, "rb")) == NULL)
	{
		throw FileHashException("������ �� �� �����ϴ�. ���ϸ��� �߸��Ǿ��ų�, �ٸ� ���α׷��� ���� ���� �� �ֽ��ϴ�.");
	}
	else
	{
		CMD5 md5Hash;
		SHA1 sha1Hash;

		IHahFunction * pFunc;

		if ( hashType == eHashType_MD5SUM )
		{
			pFunc = &md5Hash;
		}
		else
		{
			pFunc = &sha1Hash;
		}


		pFunc->init();

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
			pFunc->insertData( buffer, (int)len );
		}

		memset(digest, 0, sizeof(digest));

		pFunc->getResult( digest, sizeof(digest));
		fclose (file);

		size_t len = pFunc->getResultSize();

		ret = HexPrint(digest, len);
	}
	return ret;
}

std::string ZFileHash::HexPrint(unsigned char *digest, size_t len )
{
	size_t i;
	char buffer[5] = {0};

	std::string ret;

	for (i = 0; i < len; i++)
	{
		memset(buffer, 0, sizeof(buffer));
		sprintf (buffer, "%02x", digest[i]);
		ret += buffer;
	}
	return ret;
}


