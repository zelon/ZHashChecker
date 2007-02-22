#pragma once

#include "IHashFunction.h"

namespace FileHash
{
	#ifndef PROTOTYPES
	#define PROTOTYPES 0
	#endif

		/* POINTER defines a generic pointer type */
		typedef unsigned char *POINTER;

		/* UINT2 defines a two byte word */
		typedef unsigned short UINT2;

		/* UINT4 defines a four byte word */
		typedef unsigned long UINT4;

		/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
		If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
		returns an empty list.
		*/
	#if PROTOTYPES
	#define PROTO_LIST(list) list
	#else
	#define PROTO_LIST(list) ()
	#endif

	/* MD5 context. */
	typedef struct {
		UINT4 state[4];                                   /* state (ABCD) */
		UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
		unsigned char buffer[64];                         /* input buffer */
	} MD5_CTX;

	//void MD5Init(MD5_CTX *);
	//void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
	//void MD5Final(unsigned char *, MD5_CTX *);

	#define S11 7
	#define S12 12
	#define S13 17
	#define S14 22
	#define S21 5
	#define S22 9
	#define S23 14
	#define S24 20
	#define S31 4
	#define S32 11
	#define S33 16
	#define S34 23
	#define S41 6
	#define S42 10
	#define S43 15
	#define S44 21

		static unsigned char PADDING[64] = {
			0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};

		/* F, G, H and I are basic MD5 functions.
		*/
	#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
	#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
	#define H(x, y, z) ((x) ^ (y) ^ (z))
	#define I(x, y, z) ((y) ^ ((x) | (~z)))

		/* ROTATE_LEFT rotates x left n bits.
		*/
	#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

		/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
		Rotation is separate from addition to prevent recomputation.
		*/
	#define FF(a, b, c, d, x, s, ac) { \
		(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
		(a) = ROTATE_LEFT ((a), (s)); \
		(a) += (b); \
	}
	#define GG(a, b, c, d, x, s, ac) { \
		(a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
		(a) = ROTATE_LEFT ((a), (s)); \
		(a) += (b); \
	}
	#define HH(a, b, c, d, x, s, ac) { \
		(a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
		(a) = ROTATE_LEFT ((a), (s)); \
		(a) += (b); \
	}
	#define II(a, b, c, d, x, s, ac) { \
		(a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
		(a) = ROTATE_LEFT ((a), (s)); \
		(a) += (b); \
	}


	class CMD5 : public IHahFunction
	{
	private:
		MD5_CTX _context;

	public:
		CMD5();
		~CMD5();

		virtual int init();
		virtual int insertData( const unsigned char *_data, size_t _dataLen );
		virtual int getResult( unsigned char * pResult, size_t resultLen);

		virtual size_t getResultSize() { return 16; }

	private:
		void MD5Final ( unsigned char *digest, MD5_CTX *context );
		void MD5Update ( MD5_CTX *context, const unsigned char *input, unsigned int inputLen );
		void Encode ( unsigned char *output, UINT4 *input, unsigned int len );
		void MD5_memset ( POINTER output, int value, unsigned int len );
		void MD5_memcpy ( POINTER output, POINTER input, unsigned int len );
		void Decode ( UINT4 *output, const unsigned char *input, unsigned int len );
		void MD5Transform ( UINT4 *state, const unsigned char *block );
		void MD5Init ( MD5_CTX *context );
	};
}
