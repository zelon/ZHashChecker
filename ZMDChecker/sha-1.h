/*
 -- this sha-1.h and sha-1.cpp is derived from http://www.faqs.org/rfcs/rfc3174.html
*/


/*
*  sha1.h
*
*  Description:
*      This is the header file for code which implements the Secure
*      Hashing Algorithm 1 as defined in FIPS PUB 180-1 published
*      April 17, 1995.
*
*      Many of the variable names in this code, especially the
*      single character names, were used because those were the names
*      used in the publication.
*
*      Please read the file sha1.c for more information.
*
*/

#pragma once

#include "IHashFunction.h"

#include <stdint.h>
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
//typedef int int_least16_t;
/*
* If you do not have the ISO standard stdint.h header file, then you
* must typdef the following:
*    name              meaning
*  uint32_t         unsigned 32 bit integer
*  uint8_t          unsigned 8 bit integer (i.e., unsigned char)
*  int_least16_t    integer of >= 16 bits
*
*/

#ifndef _SHA_enum_
#define _SHA_enum_
enum
{
	shaSuccess = 0,
	shaNull,            /* Null pointer parameter */
	shaInputTooLong,    /* input data too long */
	shaStateError       /* called Input after Result */
};
#endif
#define SHA1HashSize 20

/*
*  This structure will hold context information for the SHA-1
*  hashing operation
*/
typedef struct SHA1Context
{
	uint32_t Intermediate_Hash[SHA1HashSize/4]; /* Message Digest  */

	uint32_t Length_Low;            /* Message length in bits      */
	uint32_t Length_High;           /* Message length in bits      */

	/* Index into message block array   */
	int_least16_t Message_Block_Index;
	uint8_t Message_Block[64];      /* 512-bit message blocks      */

	int Computed;               /* Is the digest computed?         */
	int Corrupted;             /* Is the message digest corrupted? */
} SHA1Context;

/*
*  Function Prototypes
*/


class SHA1 : public IHahFunction
{
public:
	virtual int init();
	virtual int insertData( const unsigned char * pData, size_t dataLen)
	{
		return SHA1Input(pData, dataLen);
	}
	virtual int getResult( unsigned char * pResult, size_t resultLen)
	{
		if ( resultLen < SHA1HashSize )
		{
			_ASSERTE(false);
			return shaNull;
		}
		SHA1Result(pResult);
	}

	virtual size_t getResultSize() { return 20; }

private:
	SHA1Context m_sha1Context;

	int SHA1Input(const uint8_t *, unsigned int);
	int SHA1Result(uint8_t Message_Digest[SHA1HashSize]);
};