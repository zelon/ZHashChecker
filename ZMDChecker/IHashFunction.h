
#pragma once

#include <string>

/// 해시를 구현하는 클래스의 인터페이스
class IHahFunction
{
public:
	/// 초기화
	virtual int init() = 0;

	/// 내용을 업데이트한다.
	virtual int insertData( const unsigned char * pData, size_t dataLen) = 0;

	/// 결과 배열을 얻어온다.
	virtual int getResult( unsigned char * pResult, size_t resultLen ) = 0;

	/// 결과값의 크기를 얻어온다.
	virtual size_t getResultSize() = 0;
};