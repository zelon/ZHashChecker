// MakeBigTempFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	int dataLen = 1024 * 1024;
	char * data = new char[dataLen];

	FILE * pFile = fopen("Temp.tmp", "w");

	if ( pFile == NULL )
	{
		cout << "Can't file open" << endl;
		return -1;
	}

	unsigned __int64 iWantSize = 1024 * 1024 * (__int64)5000;	// 5GB
	unsigned __int64 iTotal = 0;
	bool bGoOn = true;

	while ( bGoOn )
	{
		if ( dataLen != fwrite(data, sizeof(char), dataLen, pFile))
		{
			cout << "fwrite fail" << endl;
			break;
		}

		cout << "One block written" << endl;

		iTotal += dataLen;

		if ( iTotal >= iWantSize )
		{
			cout << "The file is made" << endl;
			break;
		}
	}

	fclose(pFile);

	return 0;
}

