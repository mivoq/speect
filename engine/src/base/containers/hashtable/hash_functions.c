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
/* Hash functions.                                                                  */
/*                                                                                  */
/* Derived from the work by Bob Jenkins.                                            */
/* http://www.burtleburtle.net/bob/, May 2006, Public Domain.                       */
/* Changed the function names and data types.                                       */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/containers/hashtable/hash_functions.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/* from http://www.burtleburtle.net/bob/
--------------------------------------------------------------------
 This works on all machines.  To be useful, it requires
 -- that the key be an array of uint32_t's, and
 -- that the length be the number of uint32_t's in the key

 The function hashword() is identical to hashlittle() on little-endian
 machines, and identical to hashbig() on big-endian machines,
 except that the length has to be measured in uint32_ts rather than in
 bytes.  hashlittle() is more complicated than hashword() only because
 hashlittle() has to dance around fitting the key bytes into registers.
--------------------------------------------------------------------
*/
S_LOCAL uint32 s_hashword(const uint32 *k, size_t length, uint32 initval)
{
	uint32 a;
	uint32 b;
	uint32 c;

	/* Set up the internal state */
	a = b = c = 0xdeadbeef + (((uint32)length)<<2) + initval;

	/*------------------------------------------------- handle most of the key */
	while (length > 3)
	{
		a += k[0];
		b += k[1];
		c += k[2];
		S_MIX(a,b,c);
		length -= 3;
		k += 3;
	}

	/*------------------------------------------- handle the last 3 uint32's */
	switch(length)                     /* all the case statements fall through */
	{
	case 3 : c+=k[2];
	case 2 : b+=k[1];
	case 1 : a+=k[0];
		S_FINAL(a,b,c);
	case 0:     /* case 0: nothing left to add */
		break;
	}
	/*------------------------------------------------------ report the result */
	return c;
}


/*  from http://www.burtleburtle.net/bob/
--------------------------------------------------------------------
hashword2() -- same as hashword(), but take two seeds and return two
32-bit values.  pc and pb must both be nonnull, and *pc and *pb must
both be initialized with seeds.  If you pass in (*pb)==0, the output
(*pc) will be the same as the return value from hashword().
--------------------------------------------------------------------
*/
S_LOCAL void s_hashword2(const uint32 *k, size_t length, uint32 *pc, uint32 *pb)
{
	uint32 a;
	uint32 b;
	uint32 c;

	/* Set up the internal state */
	a = b = c = 0xdeadbeef + ((uint32)(length<<2)) + *pc;
	c += *pb;

	/*------------------------------------------------- handle most of the key */
	while (length > 3)
	{
		a += k[0];
		b += k[1];
		c += k[2];
		S_MIX(a,b,c);
		length -= 3;
		k += 3;
	}

	/*------------------------------------------- handle the last 3 uint32_t's */
	switch(length)                     /* all the case statements fall through */
	{
	case 3 : c+=k[2];
	case 2 : b+=k[1];
	case 1 : a+=k[0];
	S_FINAL(a,b,c);
	case 0:     /* case 0: nothing left to add */
	break;
	}
	/*------------------------------------------------------ report the result */
	*pc=c; *pb=b;
}


/* from http://www.burtleburtle.net/bob/
-------------------------------------------------------------------------------
hashlittle() -- hash a variable-length key into a 32-bit value
  k       : the key (the unaligned variable-length array of bytes)
  length  : the length of the key, counting by bytes
  initval : can be any 4-byte value
Returns a 32-bit value.  Every bit of the key affects every bit of
the return value.  Two keys differing by one or two bits will have
totally different hash values.

The best hash table sizes are powers of 2.  There is no need to do
mod a prime (mod is sooo slow!).  If you need less than 32 bits,
use a bitmask.  For example, if you need only 10 bits, do
  h = (h & hashmask(10));
In which case, the hash table should have hashsize(10) elements.

If you are hashing n strings (uint8_t **)k, do it like this:
  for (i=0, h=0; i<n; ++i) h = hashlittle( k[i], len[i], h);

By Bob Jenkins, 2006.  bob_jenkins@burtleburtle.net.  You may use this
code any way you wish, private, educational, or commercial.  It's free.

Use for hash table lookup, or anything where one collision in 2^^32 is
acceptable.  Do NOT use for cryptographic purposes.
-------------------------------------------------------------------------------
*/
S_LOCAL uint32 s_hashlittle(const void *key, size_t length, uint32 initval)
{
        /* internal state */
	uint32 a;
	uint32 b;
	uint32 c;


	union
	{
		const void *ptr;
		size_t i;
	} u;     /* needed for Mac Powerbook G4 */

	/* Set up the internal state */
	a = b = c = 0xdeadbeef + ((uint32)length) + initval;

	u.ptr = key;
	if (S_HASH_LITTLE_ENDIAN && ((u.i & 0x3) == 0))
	{
		const uint32 *k = (const uint32 *)key;         /* read 32-bit chunks */
		const uint8  *k8;

		/*------ all but last block: aligned reads and affect 32 bits of (a,b,c) */
		while (length > 12)
		{
			a += k[0];
			b += k[1];
			c += k[2];
			S_MIX(a,b,c);
			length -= 12;
			k += 3;
		}

		/*----------------------------- handle the last (probably partial) block */
		/*
		 * "k[2]&0xffffff" actually reads beyond the end of the string, but
		 * then masks off the part it's not allowed to read.  Because the
		 * string is aligned, the masked-off tail is in the same word as the
		 * rest of the string.  Every machine with memory protection I've seen
		 * does it on word boundaries, so is OK with this.  But VALGRIND will
		 * still catch it and complain.  The masking trick does make the hash
		 * noticably faster for short strings (like English words).
		 */

                /* make valgrind happy */

		k8 = (const uint8 *)k;
		switch(length)
		{
		case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
		case 11: c+=((uint32)k8[10])<<16;  /* fall through */
		case 10: c+=((uint32)k8[9])<<8;    /* fall through */
		case 9 : c+=k8[8];                 /* fall through */
		case 8 : b+=k[1]; a+=k[0]; break;
		case 7 : b+=((uint32)k8[6])<<16;   /* fall through */
		case 6 : b+=((uint32)k8[5])<<8;    /* fall through */
		case 5 : b+=k8[4];                 /* fall through */
		case 4 : a+=k[0]; break;
		case 3 : a+=((uint32)k8[2])<<16;   /* fall through */
		case 2 : a+=((uint32)k8[1])<<8;    /* fall through */
		case 1 : a+=k8[0]; break;
		case 0 : return c;
		}

	}
	else if (S_HASH_LITTLE_ENDIAN && ((u.i & 0x1) == 0))
	{
		const uint16 *k = (const uint16*)key;         /* read 16-bit chunks */
		const uint8  *k8;

		/*--------------- all but last block: aligned reads and different mixing */
		while (length > 12)
		{
			a += k[0] + (((uint32)k[1])<<16);
			b += k[2] + (((uint32)k[3])<<16);
			c += k[4] + (((uint32)k[5])<<16);
			S_MIX(a,b,c);
			length -= 12;
			k += 6;
		}

		/*----------------------------- handle the last (probably partial) block */
		k8 = (const uint8 *)k;
		switch(length)
		{
		case 12: c+=k[4]+(((uint32)k[5])<<16);
			b+=k[2]+(((uint32)k[3])<<16);
			a+=k[0]+(((uint32)k[1])<<16);
			break;
		case 11: c+=((uint32)k8[10])<<16;     /* fall through */
		case 10: c+=k[4];
			b+=k[2]+(((uint32)k[3])<<16);
			a+=k[0]+(((uint32)k[1])<<16);
			break;
		case 9 : c+=k8[8];                      /* fall through */
		case 8 : b+=k[2]+(((uint32)k[3])<<16);
			a+=k[0]+(((uint32)k[1])<<16);
			break;
		case 7 : b+=((uint32)k8[6])<<16;      /* fall through */
		case 6 : b+=k[2];
			a+=k[0]+(((uint32)k[1])<<16);
			break;
		case 5 : b+=k8[4];                      /* fall through */
		case 4 : a+=k[0]+(((uint32)k[1])<<16);
			break;
		case 3 : a+=((uint32)k8[2])<<16;      /* fall through */
		case 2 : a+=k[0];
			break;
		case 1 : a+=k8[0];
			break;
		case 0 : return c;                     /* zero length requires no mixing */
		}

	}
	else
	{                        /* need to read the key one byte at a time */
		const uint8 *k = (const uint8 *)key;

		/*--------------- all but the last block: affect some 32 bits of (a,b,c) */
		while (length > 12)
		{
			a += k[0];
			a += ((uint32)k[1])<<8;
			a += ((uint32)k[2])<<16;
			a += ((uint32)k[3])<<24;
			b += k[4];
			b += ((uint32)k[5])<<8;
			b += ((uint32)k[6])<<16;
			b += ((uint32)k[7])<<24;
			c += k[8];
			c += ((uint32)k[9])<<8;
			c += ((uint32)k[10])<<16;
			c += ((uint32)k[11])<<24;
			S_MIX(a,b,c);
			length -= 12;
			k += 12;
		}

		/*-------------------------------- last block: affect all 32 bits of (c) */
		switch(length)                   /* all the case statements fall through */
		{
		case 12: c+=((uint32)k[11])<<24;
		case 11: c+=((uint32)k[10])<<16;
		case 10: c+=((uint32)k[9])<<8;
		case 9 : c+=k[8];
		case 8 : b+=((uint32)k[7])<<24;
		case 7 : b+=((uint32)k[6])<<16;
		case 6 : b+=((uint32)k[5])<<8;
		case 5 : b+=k[4];
		case 4 : a+=((uint32)k[3])<<24;
		case 3 : a+=((uint32)k[2])<<16;
		case 2 : a+=((uint32)k[1])<<8;
		case 1 : a+=k[0];
			break;
		case 0 : return c;
		}
	}

	S_FINAL(a,b,c);
	return c;
}


/* from http://www.burtleburtle.net/bob/
 *
 * hashlittle2: return 2 32-bit hash values
 *
 * This is identical to hashlittle(), except it returns two 32-bit hash
 * values instead of just one.  This is good enough for hash table
 * lookup with 2^^64 buckets, or if you want a second hash if you're not
 * happy with the first, or if you want a probably-unique 64-bit ID for
 * the key.  *pc is better mixed than *pb, so use *pc first.  If you want
 * a 64-bit value do something like "*pc + (((uint64_t)*pb)<<32)".
 */
S_LOCAL void s_hashlittle2(const uint32 *key, size_t length, uint32 *pc, uint32 *pb)
{
	/* internal state */
	uint32 a;
	uint32 b;
	uint32 c;


	union
	{
		const void *ptr;
		size_t i;
	} u;     /* needed for Mac Powerbook G4 */


	/* Set up the internal state */
	a = b = c = 0xdeadbeef + ((uint32)length) + *pc;
	c += *pb;

	u.ptr = key;
	if (S_HASH_LITTLE_ENDIAN && ((u.i & 0x3) == 0))
	{
		const uint32 *k = (const uint32 *)key;         /* read 32-bit chunks */
		const uint8  *k8;

		/*------ all but last block: aligned reads and affect 32 bits of (a,b,c) */
		while (length > 12)
		{
			a += k[0];
			b += k[1];
			c += k[2];
			S_MIX(a,b,c);
			length -= 12;
			k += 3;
		}

		/*----------------------------- handle the last (probably partial) block */
		/*
		 * "k[2]&0xffffff" actually reads beyond the end of the string, but
		 * then masks off the part it's not allowed to read.  Because the
		 * string is aligned, the masked-off tail is in the same word as the
		 * rest of the string.  Every machine with memory protection I've seen
		 * does it on word boundaries, so is OK with this.  But VALGRIND will
		 * still catch it and complain.  The masking trick does make the hash
		 * noticably faster for short strings (like English words).
		 */

                /* make valgrind happy */
		k8 = (const uint8 *)k;
		switch(length)
		{
		case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
		case 11: c+=((uint32)k8[10])<<16;  /* fall through */
		case 10: c+=((uint32)k8[9])<<8;    /* fall through */
		case 9 : c+=k8[8];                   /* fall through */
		case 8 : b+=k[1]; a+=k[0]; break;
		case 7 : b+=((uint32)k8[6])<<16;   /* fall through */
		case 6 : b+=((uint32)k8[5])<<8;    /* fall through */
		case 5 : b+=k8[4];                   /* fall through */
		case 4 : a+=k[0]; break;
		case 3 : a+=((uint32)k8[2])<<16;   /* fall through */
		case 2 : a+=((uint32)k8[1])<<8;    /* fall through */
		case 1 : a+=k8[0]; break;
		case 0 : *pc=c; *pb=b; return;  /* zero length strings require no mixing */
		}
	}
	else if (S_HASH_LITTLE_ENDIAN && ((u.i & 0x1) == 0))
	{
		const uint16 *k = (const uint16*)key;         /* read 16-bit chunks */
		const uint8  *k8;

		/*--------------- all but last block: aligned reads and different mixing */
		while (length > 12)
		{
			a += k[0] + (((uint32)k[1])<<16);
			b += k[2] + (((uint32)k[3])<<16);
			c += k[4] + (((uint32)k[5])<<16);
			S_MIX(a,b,c);
			length -= 12;
			k += 6;
		}

		/*----------------------------- handle the last (probably partial) block */
		k8 = (const uint8 *)k;
		switch(length)
		{
		case 12: c+=k[4]+(((uint32)k[5])<<16);
			b+=k[2]+(((uint32)k[3])<<16);
			a+=k[0]+(((uint32)k[1])<<16);
			break;
		case 11: c+=((uint32)k8[10])<<16;     /* fall through */
		case 10: c+=k[4];
			b+=k[2]+(((uint32)k[3])<<16);
			a+=k[0]+(((uint32)k[1])<<16);
			break;
		case 9 : c+=k8[8];                      /* fall through */
		case 8 : b+=k[2]+(((uint32)k[3])<<16);
			a+=k[0]+(((uint32)k[1])<<16);
			break;
		case 7 : b+=((uint32)k8[6])<<16;      /* fall through */
		case 6 : b+=k[2];
			a+=k[0]+(((uint32)k[1])<<16);
			break;
		case 5 : b+=k8[4];                      /* fall through */
		case 4 : a+=k[0]+(((uint32)k[1])<<16);
			break;
		case 3 : a+=((uint32)k8[2])<<16;      /* fall through */
		case 2 : a+=k[0];
			break;
		case 1 : a+=k8[0];
			break;
		case 0 : *pc=c; *pb=b; return;  /* zero length strings require no mixing */
		}

	} else
	{                        /* need to read the key one byte at a time */
		const uint8 *k = (const uint8 *)key;

		/*--------------- all but the last block: affect some 32 bits of (a,b,c) */
		while (length > 12)
		{
			a += k[0];
			a += ((uint32)k[1])<<8;
			a += ((uint32)k[2])<<16;
			a += ((uint32)k[3])<<24;
			b += k[4];
			b += ((uint32)k[5])<<8;
			b += ((uint32)k[6])<<16;
			b += ((uint32)k[7])<<24;
			c += k[8];
			c += ((uint32)k[9])<<8;
			c += ((uint32)k[10])<<16;
			c += ((uint32)k[11])<<24;
			S_MIX(a,b,c);
			length -= 12;
			k += 12;
		}

		/*-------------------------------- last block: affect all 32 bits of (c) */
		switch(length)                   /* all the case statements fall through */
		{
		case 12: c+=((uint32)k[11])<<24;
		case 11: c+=((uint32)k[10])<<16;
		case 10: c+=((uint32)k[9])<<8;
		case 9 : c+=k[8];
		case 8 : b+=((uint32)k[7])<<24;
		case 7 : b+=((uint32)k[6])<<16;
		case 6 : b+=((uint32)k[5])<<8;
		case 5 : b+=k[4];
		case 4 : a+=((uint32)k[3])<<24;
		case 3 : a+=((uint32)k[2])<<16;
		case 2 : a+=((uint32)k[1])<<8;
		case 1 : a+=k[0];
			break;
		case 0 : *pc=c; *pb=b; return;  /* zero length strings require no mixing */
		}
	}

	S_FINAL(a,b,c);
	*pc=c; *pb=b;
}


/*
 * hashbig():
 * This is the same as hashword() on big-endian machines.  It is different
 * from hashlittle() on all machines.  hashbig() takes advantage of
 * big-endian byte ordering.
 */
S_LOCAL uint32 s_hashbig(const void *key, size_t length, uint32 initval)
{
	/* internal state */
	uint32 a;
	uint32 b;
	uint32 c;


	union
	{
		const void *ptr;
		size_t i;
	} u;     /* needed for Mac Powerbook G4, to cast key to (size_t) happily */


	/* Set up the internal state */
	a = b = c = 0xdeadbeef + ((uint32)length) + initval;

	u.ptr = key;
	if (S_HASH_BIG_ENDIAN && ((u.i & 0x3) == 0))
	{
		const uint32 *k = (const uint32 *)key;         /* read 32-bit chunks */
		const uint8  *k8;

		/*------ all but last block: aligned reads and affect 32 bits of (a,b,c) */
		while (length > 12)
		{
			a += k[0];
			b += k[1];
			c += k[2];
			S_MIX(a,b,c);
			length -= 12;
			k += 3;
		}

		/*----------------------------- handle the last (probably partial) block */
		/*
		 * "k[2]<<8" actually reads beyond the end of the string, but
		 * then shifts out the part it's not allowed to read.  Because the
		 * string is aligned, the illegal read is in the same word as the
		 * rest of the string.  Every machine with memory protection I've seen
		 * does it on word boundaries, so is OK with this.  But VALGRIND will
		 * still catch it and complain.  The masking trick does make the hash
		 * noticably faster for short strings (like English words).
		 */

		/* make valgrind happy */
		k8 = (const uint8 *)k;
		switch(length)                   /* all the case statements fall through */
		{
		case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
		case 11: c+=((uint32)k8[10])<<8;  /* fall through */
		case 10: c+=((uint32)k8[9])<<16;  /* fall through */
		case 9 : c+=((uint32)k8[8])<<24;  /* fall through */
		case 8 : b+=k[1]; a+=k[0]; break;
		case 7 : b+=((uint32)k8[6])<<8;   /* fall through */
		case 6 : b+=((uint32)k8[5])<<16;  /* fall through */
		case 5 : b+=((uint32)k8[4])<<24;  /* fall through */
		case 4 : a+=k[0]; break;
		case 3 : a+=((uint32)k8[2])<<8;   /* fall through */
		case 2 : a+=((uint32)k8[1])<<16;  /* fall through */
		case 1 : a+=((uint32)k8[0])<<24; break;
		case 0 : return c;
		}

	}
	else
	{                        /* need to read the key one byte at a time */
		const uint8 *k = (const uint8 *)key;

		/*--------------- all but the last block: affect some 32 bits of (a,b,c) */
		while (length > 12)
		{
			a += ((uint32)k[0])<<24;
			a += ((uint32)k[1])<<16;
			a += ((uint32)k[2])<<8;
			a += ((uint32)k[3]);
			b += ((uint32)k[4])<<24;
			b += ((uint32)k[5])<<16;
			b += ((uint32)k[6])<<8;
			b += ((uint32)k[7]);
			c += ((uint32)k[8])<<24;
			c += ((uint32)k[9])<<16;
			c += ((uint32)k[10])<<8;
			c += ((uint32)k[11]);
			S_MIX(a,b,c);
			length -= 12;
			k += 12;
		}

		/*-------------------------------- last block: affect all 32 bits of (c) */
		switch(length)                   /* all the case statements fall through */
		{
		case 12: c+=k[11];
		case 11: c+=((uint32)k[10])<<8;
		case 10: c+=((uint32)k[9])<<16;
		case 9 : c+=((uint32)k[8])<<24;
		case 8 : b+=k[7];
		case 7 : b+=((uint32)k[6])<<8;
		case 6 : b+=((uint32)k[5])<<16;
		case 5 : b+=((uint32)k[4])<<24;
		case 4 : a+=k[3];
		case 3 : a+=((uint32)k[2])<<8;
		case 2 : a+=((uint32)k[1])<<16;
		case 1 : a+=((uint32)k[0])<<24;
			break;
		case 0 : return c;
		}
	}

	S_FINAL(a,b,c);
	return c;
}


/*
 * hashbig2():
 * Same as hashbig(), but take two seeds and return two
 * 32-bit values.  pc and pb must both be nonnull, and *pc and *pb must
 * both be initialized with seeds.  If you pass in (*pb)==0, the output
 * (*pc) will be the same as the return value from hashword().
 */
S_LOCAL void s_hashbig2(const uint32 *key, size_t length, uint32 *pc, uint32 *pb)
{
	/* internal state */
	uint32 a;
	uint32 b;
	uint32 c;


	union
	{
		const void *ptr;
		size_t i;
	} u;     /* needed for Mac Powerbook G4, to cast key to (size_t) happily */


	/* Set up the internal state */
	a = b = c = 0xdeadbeef + ((uint32)length) + *pc;
	c += *pb;


	u.ptr = key;
	if (S_HASH_BIG_ENDIAN && ((u.i & 0x3) == 0))
	{
		const uint32 *k = (const uint32 *)key;         /* read 32-bit chunks */
		const uint8  *k8;

		/*------ all but last block: aligned reads and affect 32 bits of (a,b,c) */
		while (length > 12)
		{
			a += k[0];
			b += k[1];
			c += k[2];
			S_MIX(a,b,c);
			length -= 12;
			k += 3;
		}

		/*----------------------------- handle the last (probably partial) block */
		/*
		 * "k[2]<<8" actually reads beyond the end of the string, but
		 * then shifts out the part it's not allowed to read.  Because the
		 * string is aligned, the illegal read is in the same word as the
		 * rest of the string.  Every machine with memory protection I've seen
		 * does it on word boundaries, so is OK with this.  But VALGRIND will
		 * still catch it and complain.  The masking trick does make the hash
		 * noticably faster for short strings (like English words).
		 */

		/* make valgrind happy */
		k8 = (const uint8 *)k;
		switch(length)                   /* all the case statements fall through */
		{
		case 12: c+=k[2]; b+=k[1]; a+=k[0]; break;
		case 11: c+=((uint32)k8[10])<<8;  /* fall through */
		case 10: c+=((uint32)k8[9])<<16;  /* fall through */
		case 9 : c+=((uint32)k8[8])<<24;  /* fall through */
		case 8 : b+=k[1]; a+=k[0]; break;
		case 7 : b+=((uint32)k8[6])<<8;   /* fall through */
		case 6 : b+=((uint32)k8[5])<<16;  /* fall through */
		case 5 : b+=((uint32)k8[4])<<24;  /* fall through */
		case 4 : a+=k[0]; break;
		case 3 : a+=((uint32)k8[2])<<8;   /* fall through */
		case 2 : a+=((uint32)k8[1])<<16;  /* fall through */
		case 1 : a+=((uint32)k8[0])<<24; break;
		case 0 : *pc=c; *pb=b; return;  /* zero length strings require no mixing */
		}

	}
	else
	{                        /* need to read the key one byte at a time */
		const uint8 *k = (const uint8 *)key;

		/*--------------- all but the last block: affect some 32 bits of (a,b,c) */
		while (length > 12)
		{
			a += ((uint32)k[0])<<24;
			a += ((uint32)k[1])<<16;
			a += ((uint32)k[2])<<8;
			a += ((uint32)k[3]);
			b += ((uint32)k[4])<<24;
			b += ((uint32)k[5])<<16;
			b += ((uint32)k[6])<<8;
			b += ((uint32)k[7]);
			c += ((uint32)k[8])<<24;
			c += ((uint32)k[9])<<16;
			c += ((uint32)k[10])<<8;
			c += ((uint32)k[11]);
			S_MIX(a,b,c);
			length -= 12;
			k += 12;
		}

		/*-------------------------------- last block: affect all 32 bits of (c) */
		switch(length)                   /* all the case statements fall through */
		{
		case 12: c+=k[11];
		case 11: c+=((uint32)k[10])<<8;
		case 10: c+=((uint32)k[9])<<16;
		case 9 : c+=((uint32)k[8])<<24;
		case 8 : b+=k[7];
		case 7 : b+=((uint32)k[6])<<8;
		case 6 : b+=((uint32)k[5])<<16;
		case 5 : b+=((uint32)k[4])<<24;
		case 4 : a+=k[3];
		case 3 : a+=((uint32)k[2])<<8;
		case 2 : a+=((uint32)k[1])<<16;
		case 1 : a+=((uint32)k[0])<<24;
			break;
		case 0 : *pc=c; *pb=b; return;  /* zero length strings require no mixing */
		}
	}

	S_FINAL(a,b,c);
	*pc=c; *pb=b;
}
