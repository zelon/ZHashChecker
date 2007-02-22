
#pragma once

#include <string>

/// �ؽø� �����ϴ� Ŭ������ �������̽�
class IHahFunction
{
public:
	/// �ʱ�ȭ
	virtual int init() = 0;

	/// ������ ������Ʈ�Ѵ�.
	virtual int insertData( const unsigned char * pData, size_t dataLen) = 0;

	/// ��� �迭�� ���´�.
	virtual int getResult( unsigned char * pResult, size_t resultLen ) = 0;

	/// ������� ũ�⸦ ���´�.
	virtual size_t getResultSize() = 0;
};