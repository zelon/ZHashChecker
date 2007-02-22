#pragma once

#ifndef MD
#define MD 5
#endif

#include "md5class.h"

#define TEST_BLOCK_LEN 1000
#define TEST_BLOCK_COUNT 1000
#define MD_CTX MD5_CTX
#define MDInit MD5Init
#define MDUpdate MD5Update
#define MDFinal MD5Final

enum eHashType
{
	eHashType_MD5SUM,
	eHashType_SHA1,
};


using namespace std;

namespace FileHash
{
	class HashStopException
	{
	public:
		HashStopException(const string & strMsg)
		{
			m_strMsg = strMsg;
		}

		const string & getMessage() const { return m_strMsg; }

	private:
		string m_strMsg;
	};

	class IHashProgressListener
	{
	public:
		IHashProgressListener(){}
		virtual ~IHashProgressListener() {}

		virtual void OnProgress(int progressPercentage) = 0;
	};

	class FileHashException
	{
	public:
		FileHashException(const string & msg)
		{
			m_strMsg = msg;
			
		}
		~FileHashException() {}
		const string & GetExceptionMessage() const { return m_strMsg; }

	private:
		string m_strMsg;
	};

	/// 이 클래스는 static method 만을 이용한다.
	class ZFileHash
	{
	public:
		ZFileHash(void);
		~ZFileHash(void);

		string GetHashStringFromFile(const string & filename, const eHashType hashType, IHashProgressListener * pProgressListener);
		void setGoOn(bool bGoOn) { m_bGoOn = bGoOn; }

	private:
		string HashFile(const char * szFilename, const eHashType hashType, IHashProgressListener * pProgressListener);
		std::string HexPrint(unsigned char *digest, size_t len );

		IHashProgressListener * m_pProgressListener;
		bool m_bGoOn;
	};
}
