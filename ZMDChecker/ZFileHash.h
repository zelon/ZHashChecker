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

	/// �� Ŭ������ static method ���� �̿��Ѵ�.
	class ZFileHash
	{
	public:
		ZFileHash(void);
		~ZFileHash(void);

		string GetHashStringFromFile(const string & filename, IHashProgressListener * pProgressListener);
		void setGoOn(bool bGoOn) { m_bGoOn = bGoOn; }

	private:
		string MDFile(const char *filename, IHashProgressListener * pProgressListener);
		string MDPrint(unsigned char *digest );

		IHashProgressListener * m_pProgressListener;
		bool m_bGoOn;
	};
}
