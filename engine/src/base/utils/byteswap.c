/************************************************************************************/
/* Copyright (c) 2008-2011 The Department of Arts and Culture,                      */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                                                                                  */
/* Permission is hereby granted, free of charge, to any person obtaining a copy     */
/* of this software and associated documentation files (the "Software"), to deal    */
/* in the Software without restriction, including without limitation the rights     */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        */
/* copies of the Software, and to permit persons to whom the Software is            */
/* furnished to do so, subject to the following conditions:                         */
/* The above copyright notice and this permission notice shall be included in       */
/* all copies or substantial portions of the Software.                              */
/*                                                                                  */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN        */
/* THE SOFTWARE.                                                                    */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Byteswapping functions.                                                          */
/*                                                                                  */
/* Adapted from Agar toolkit http://libagar.org/ (1.3.2)                            */
/* Note that this is a derived work.                                                */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* libagar license                                                                  */
/*                                                                                  */
/* Copyright (c) 2002-2007 Hypertriton, Inc. <http://hypertriton.com/>              */
/* All rights reserved.                                                             */
/*                                                                                  */
/* Redistribution and use in source and binary forms, with or without               */
/* modification, are permitted provided that the following conditions               */
/* are met:                                                                         */
/* 1. Redistributions of source code must retain the above copyright                */
/*    notice, this list of conditions and the following disclaimer.                 */
/* 2. Redistributions in binary form must reproduce the above copyright             */
/*    notice, this list of conditions and the following disclaimer in the           */
/*    documentation and/or other materials provided with the distribution.          */
/*                                                                                  */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"      */
/* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE        */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE       */
/* ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR       */
/* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL           */
/* DAMAGES (INCLUDING BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR        */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER       */
/* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,    */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE        */
/* USE OF THIS SOFTWARE EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.          */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "base/errdbg/errdbg.h"
#include "base/utils/byteswap.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/*
 * Swap 16-bit
 */
#if defined(__GNUC__) && defined(__i386__) && !(__GNUC__ == 2 && __GNUC_MINOR__ == 95)
S_API uint16 s_swap16(uint16 x)
{
	__asm__("xchgb %b0,%h0" :
	        "=q" (x) :
			"0" (x));
	return (x);
}
#elif defined(__GNUC__) && defined(__x86_64__)
S_API uint16 s_swap16(uint16 x)
{
	__asm__("xchgb %b0,%h0" :
	        "=Q" (x) :
			"0" (x));
	return (x);
}
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
S_API uint16 s_swap16(uint16 x)
{
	uint16 rv;
	__asm__("rlwimi %0,%2,8,16,23" :
	        "=&r" (rv) :
			"0" (x >> 8), "r" (x));
	return (rv);
}
#else
S_API uint16 s_swap16(uint16 x)
{
	return ((x<<8)|(x>>8));
}
#endif


/*
 * Swap 32-bit
 */
#if defined(__GNUC__) && defined(__i386__)
S_API uint32 s_swap32(uint32 x)
{
	__asm__("bswap %0" :
	        "=r" (x) :
			"0" (x));
	return (x);
}
#elif defined(__GNUC__) && defined(__x86_64__)
S_API uint32 s_swap32(uint32 x)
{
	__asm__("bswapl %0" :
	        "=r" (x) :
			"0" (x));
	return (x);
}
#elif defined(__GNUC__) && (defined(__powerpc__) || defined(__ppc__))
S_API uint32 s_swap32(uint32 x)
{
	uint32 rv;
	__asm__("rlwimi %0,%2,24,16,23" :
	        "=&r" (rv) :
			"0" (x>>24), "r" (x));
	__asm__("rlwimi %0,%2,8,8,15" :
	        "=&r" (rv) :
			"0" (rv), "r" (x));
	__asm__("rlwimi %0,%2,24,0,7" :
	        "=&r" (rv) :
			"0" (rv), "r" (x));
	return (rv);
}
#else
S_API uint32 s_swap32(uint32 x)
{
	return ((x << 24) |
			((x << 8) & 0x00FF0000) | ((x >> 8) & 0x0000FF00) |
	        (x >> 24));
}
#endif


/*
 * Swap float
 */
S_API float s_swapflt(float v)
{
	union { uint32 i; float v; } u;

	u.v = v;
	u.i = s_swap32(u.i);
	return u.v;
}


/*
 * Swap double
 */
S_API double s_swapdbl(double v)
{
	union { uint8 data[8]; double v; } u_in;
	union { uint8 data[8]; double v; } u_out;
	int i;

	u_in.v = v;

	for (i = 0; i < 8; i++)
	{
		u_out.data[i] = u_in.data[7-i];
	}

	return u_out.v;
}


/*
 * Swap s_byte
 */
S_API s_byte *s_swap_bytes(s_byte *self, size_t length, s_erc *error)
{
	s_byte *data;
	uint i;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_swap_bytes",
				  "Argument \"self\" is NULL.");
		return NULL;
	}

	if (length == 0)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_swap_bytes",
				  "Argument \"length\" is 0.");
		return NULL;
	}

	data = S_MALLOC(s_byte, length);

	if (data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_swap_bytes",
				  "Failed to allocate memory for swapped bytes");
		return NULL;
	}

	for (i = 0; i < length; i++)
		data[i] = self[length - 1 - i];

	S_FREE(self);

	return data;
}


S_API uint16 *s_swap_bytes16(uint16 *self, size_t length, s_erc *error)
{
	uint i;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_swap_bytes16",
				  "Argument \"self\" is NULL.");
		return NULL;
	}

	if (length == 0)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_swap_bytes16",
				  "Argument \"length\" is 0.");
		return NULL;
	}

	for (i = 0; i < length; i++)
		self[i] = s_swap16(self[i]);

	return self;
}


S_API uint32 *s_swap_bytes32(uint32 *self, uint32 length, s_erc *error)
{
	uint i;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_swap_bytes32",
				  "Argument \"self\" is NULL.");
		return NULL;
	}

	if (length == 0)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_swap_bytes32",
				  "Argument \"length\" is 0.");
		return NULL;
	}

	for (i = 0; i < length; i++)
		self[i] = s_swap32(self[i]);

	return self;
}

