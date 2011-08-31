/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* DATE    : 19 May 2008                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* UTF8 string functions.                                                           */
/*                                                                                  */
/* Adapted from Allegro http://alleg.sourceforge.net/  (4.9.7)                      */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/*  Copyright (c) 2008 the Allegro 5 Development Team                               */
/*                                                                                  */
/*  This software is provided 'as-is', without any express or implied warranty.     */
/*  In no event will the authors be held liable for any damages arising from the    */
/*  use of this software.                                                           */
/*                                                                                  */
/*  Permission is granted to anyone to use this software for any purpose,           */
/*  including commercial applications, and to alter it and redistribute it          */
/*  freely, subject to the following restrictions:                                  */
/*                                                                                  */
/*  1. The origin of this software must not be misrepresented; you must not         */
/*     claim that you wrote the original software. If you use this software         */
/*     in a product, an acknowledgment in the product documentation would be        */
/*     appreciated but is not required.                                             */
/*                                                                                  */
/*  2. Altered source versions must be plainly marked as such, and must not be      */
/*     misrepresented as being the original software.                               */
/*                                                                                  */
/*  3. This notice may not be removed or altered from any source distribution.      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "base/errdbg/errdbg_macros.h"
#include "base/strings/strings.h"
#include "base/strings/char.h"
#include "base/utils/alloc.h"
#include "base/strings/regexp.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_bool s_cforbidden(uchar octet);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/*
 *  Returns the size of the specified string in bytes, not including the
 *  trailing zero.
 */
S_API size_t s_strsize(const char *s, s_erc *error)
{
	const char *orig = s;
	const char *last;


	S_CLR_ERR(error);

	if (s == NULL)
		return 0;

	do
	{
		last = s;
	} while (s_getx((char**)&s, error) != 0);

	return ((long)last - (long)orig);
}


/*
 * Returns the size of len number of charachters of
 * the specified string in bytes.
 */
S_API size_t s_strnsize(const char *s, size_t n, s_erc *error)
{
	const char *orig = s;
	const char *last;
	uint count;


	S_CLR_ERR(error);

	if ((s == NULL) || (n <= 0))
		return 0;

	count = 0;

	do
	{
		last = s;
		count++;
	} while ((s_getx((char**)&s, error) != 0) && (count < n));

	return ((long)last - (long)orig);
}


/*
 *  Returns the size of the specified string in bytes, including the
 *  trailing zero.
 */
S_API size_t s_strzsize(const char *s, s_erc *error)
{
	const char *orig = s;


	S_CLR_ERR(error);

	if (s == NULL)
		return 0;

	do
	{
	} while (s_getx((char**)&s, error) != 0);

	return ((long)s - (long)orig);
}


/*
 *  Unicode-aware version of the ANSI strlen() function.
 */
S_API size_t s_strlen(const char *s, s_erc *error)
{
	size_t c = 0;


	S_CLR_ERR(error);

	if (s == NULL)
		return 0;

	while (s_getx((char**)&s, error) != 0)
		c++;

	return c;
}


/*
 *  Returns a newly allocated copy of the src string, which must later be
 *  freed by the caller.
 */
S_API char *s_strdup(const char *src, s_erc *error)
{
	char *s;
	size_t size;


	S_CLR_ERR(error);

	if (src == NULL)
		return NULL;

	size = s_strzsize(src, error);

	s = S_MALLOC(char, size);

	if (s == NULL)
		S_FTL_ERR(error, S_MEMERROR,
				  "s_strdup",
				  "Failed to allocate memory to new string");
	else
	{
		s_strzcpy(s, src, size, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_strdup",
					  "String copy error"))
		{
			S_FREE(s);
			return NULL;
		}
	}

	return s;
}


/*
 *  Enhanced Unicode-aware version of the ANSI strcpy() function
 *  that can handle the size (in bytes) of the destination string.
 */
S_API char *s_strzcpy(char *dest, const char *src, size_t size, s_erc *error)
{
	int pos = 0;
	uint32 c;
	int tmp;


	S_CLR_ERR(error);

	if ((dest == NULL)
	    || (src == NULL)
	    || (size <= 0))
		return NULL;

	tmp = size - s_cwidth(0, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_strzcpy",
				  "Character width error"))
		return NULL;

	if (tmp < 0)
		return NULL;

	while ((c = s_getx((char**)&src, error)) != 0)
	{
		tmp -= s_cwidth(c, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_strzcpy",
					  "Character width error"))
			return NULL;

		if (tmp < 0)
			break;

		pos += s_setc(dest + pos, c, error);
	}

	s_setc(dest + pos, 0, error);

	return dest;
}


/*
 *  Enhanced Unicode-aware version of the ANSI strncpy() function
 *  that can handle the size (in bytes) of the destination string.
 */
S_API char *s_strzncpy(char *dest, const char *src, size_t n, size_t size, s_erc *error)
{
	uint pos;
	uint len;
	s_bool ansi_oddness = FALSE;
	uint32 c;
	int tmp;


	S_CLR_ERR(error);

	if ((dest == NULL)
	    || (src == NULL)
	    || (size <= 0)
	    || (n <= 0))
		return NULL;

	/* detect raw s_strzncpy() call */
	if (size == INT_MAX)
		ansi_oddness = TRUE;

	tmp = size - s_cwidth(0, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_strzncpy",
				  "Character width error"))
		return NULL;

	if (tmp < 0)
		return NULL;

	/* copy at most n characters */
	len = 0;
	pos = 0;

	while (((c = s_getx((char**)&src, error)) != 0) && (len < n))
	{
		tmp -= s_cwidth(c, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_strzncpy",
					  "Character width error"))
			return NULL;

		if (tmp < 0)
			break;

		pos += s_setc(dest + pos, c, error);
		len++;
	}

	/* pad with NULL characters */
	while (len < n)
	{
		tmp -= s_cwidth(c, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_strzncpy",
					  "Character width error"))
			return NULL;

		if (tmp < 0)
			break;

		pos += s_setc(dest + pos, 0, error);
		len++;
	}

	/* ANSI C doesn't append the terminating NULL character */
	if (ansi_oddness != FALSE)
		s_setc(dest + pos, 0, error);

	return dest;
}


/*
 *  Enhanced Unicode-aware version of the ANSI strcat() function
 *  that can handle the size (in bytes) of the destination string.
 */
S_API char *s_strzcat(char *dest, const char *src, size_t size, s_erc *error)
{
	uint pos;
	uint32 c;
	int tmp;


	S_CLR_ERR(error);

	if ((dest == NULL)
	    || (src == NULL)
	    || (size <= 0))
		return NULL;

	pos = s_strsize(dest, error);
	tmp = size - (pos + s_cwidth(0, error));

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_strzcat",
				  "Character width error"))
		return NULL;

	if (tmp < 0)
		return NULL;

	while ((c = s_getx((char**)&src, error)) != 0)
	{
		tmp -= s_cwidth(c, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_strzcat",
					  "Character width error"))
			return NULL;

		if (tmp < 0)
			break;

		pos += s_setc(dest + pos, c, error);
	}

	s_setc(dest + pos, 0, error);

	return dest;
}


/*
 *  Enhanced Unicode-aware version of the ANSI strncat() function
 *  that can handle the size (in bytes) of the destination string.
 */
S_API char *s_strzncat(char *dest, const char *src, size_t n, size_t size, s_erc *error)
{
	uint pos;
	uint len;
	uint32 c;
	int tmp;


	S_CLR_ERR(error);

	if ((dest == NULL)
	    || (src == NULL)
	    || (size <= 0)
	    || (n <= 0))
		return NULL;

	pos = s_strsize(dest, error);
	tmp = size - (pos + s_cwidth(0, error));

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_strzncat",
				  "Character width error"))
		return NULL;

	len = 0;

	while (((c = s_getx((char**)&src, error)) != 0) && (len < n))
	{
		tmp -= s_cwidth(c, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_strzncat",
					  "Character width error"))
			return NULL;

		if ( tmp < 0)
			break;

		pos += s_setc(dest + pos, c, error);
		len++;
	}

	s_setc(dest + pos, 0, error);

	return dest;
}


/*
 *  Unicode-aware version of the ANSI strcmp() function.
 */
S_API int s_strcmp(const char *s1, const char *s2, s_erc *error)
{
	uint32 c1;
	uint32 c2;


	S_CLR_ERR(error);

	if ((s1 == NULL)
	    || (s2 == NULL))
		return -1;

	for (;;)
	{
		c1 = s_getx((char**)&s1, error);
		c2 = s_getx((char**)&s2, error);

		if (c1 != c2)
			return (c1 - c2);

		if (!c1)
			return 0;
	}
}


/*
 *  Unicode-aware version of the ANSI strncmp() function.
 */
S_API int s_strncmp(const char *s1, const char *s2, size_t n, s_erc *error)
{
	uint32 c1;
	uint32 c2;


	S_CLR_ERR(error);

	if ((s1 == NULL)
	    || (s2 == NULL)
	    || (n <= 0))
		return 0;

	for (;;)
	{
		c1 = s_getx((char**)&s1, error);
		c2 = s_getx((char**)&s2, error);

		if (c1 != c2)
			return (c1 - c2);

		if ((!c1) || (--n == 0))
			return 0;
	}
}


/*
 *  Unicode-aware version of the DJGPP stricmp() function.
 */
S_API int s_stricmp(const char *s1, const char *s2, s_erc *error)
{
	uint32 c1;
	uint32 c2;


	S_CLR_ERR(error);

	if ((s1 == NULL)
	    || (s2 == NULL))
		return -1;

	for (;;)
	{
		c1 = s_tolower(s_getx((char**)&s1, error), error);
		c2 = s_tolower(s_getx((char**)&s2, error), error);

		if (c1 != c2)
			return (c1 - c2);

		if (!c1)
			return 0;
	}
}


/*
 *  Unicode-aware version of the DJGPP strnicmp() function.
 */
S_API int s_strnicmp(const char *s1, const char *s2, size_t n, s_erc *error)
{
	uint32 c1;
	uint32 c2;


	S_CLR_ERR(error);

	if ((s1 == NULL)
	    || (s2 == NULL)
	    || (n <= 0))
		return -1;

	for (;;)
	{
		c1 = s_tolower(s_getx((char**)&s1, error), error);
		c2 = s_tolower(s_getx((char**)&s2, error), error);

		if (c1 != c2)
			return (c1 - c2);

		if ((!c1) || (--n == 0))
			return 0;
	}
}


/*
 *  Unicode-aware version of the ANSI strlwr() function.
 *  This function replaces all upper case letters in string with lower case letters.
 */
S_API char *s_strlwr(char *s, s_erc *error)
{
	uint pos;
	uint32 c;
	uint32 lc;


	S_CLR_ERR(error);

	if (s == NULL)
		return NULL;

	pos = 0;

	while ((c = s_getc(s + pos, error)) != 0)
	{
		lc = s_tolower(c, error);

		if (lc != c)
		{
			s_setat(s + pos, 0, lc, error);

			if (S_CHK_ERR(error, S_CONTERR,
						  "s_strlwr",
						  "Set character error"))
				return NULL;
		}

		pos += s_width(s + pos, error);
	}

	return s;
}


/*
 *  Unicode-aware version of the ANSI strupr() function.
 *  This function converts all lower case characters in string to upper case.
 */
S_API char *s_strupr(char *s,  s_erc *error)
{
	uint pos;
	uint32 c;
	uint32 uc;


	S_CLR_ERR(error);

	if (s == NULL)
		return NULL;

	pos = 0;

	while ((c = s_getc(s + pos, error)) != 0)
	{
		uc = s_toupper(c, error);

		if (uc != c)
		{
			s_setat(s + pos, 0, uc, error);

			if (S_CHK_ERR(error, S_CONTERR,
						  "s_strupr",
						  "Set character error"))
				return NULL;
		}

		pos += s_width(s + pos, error);
	}

	return s;
}


/*
 *  Unicode-aware version of the ANSI strchr() function.
 */
S_API const char *s_strchr(const char *s, uint32 c, s_erc *error)
{
	uint32 d;


	S_CLR_ERR(error);

	if (s == NULL)
		return NULL;

	while ((d = s_getc(s, error)) != 0)
	{
		if (c == d)
			return s;

		s += s_width(s, error);
	}

	if (c == 0)
		return s;

	return NULL;
}


/*
 *  Unicode-aware version of the ANSI strrchr() function.
 */
S_API const char *s_strrchr(const char *s, uint32 c, s_erc *error)
{
	const char *last_match = NULL;
	uint32 c1;
	uint pos;


	S_CLR_ERR(error);

	if (s == NULL)
		return NULL;

	pos = 0;

	for (c1 = s_getc(s, error); c1; c1 = s_getc(s + pos, error))
	{
		if (c1 == c)
			last_match = s + pos;

		pos += s_cwidth(c1, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_strrchr",
					  "Character width error"))
			return NULL;
	}

	return last_match;
}


/*
 *  Unicode-aware version of the ANSI strstr() function.
 */
S_API char *s_strstr(const char *haystack, const char *needle, s_erc *error)
{
	uint len;


	S_CLR_ERR(error);

	if ((haystack == NULL)
	    || (needle == NULL))
		return NULL;

	len = s_strlen(needle, error);

	while (s_getc(haystack, error))
	{
		if (s_strncmp(haystack, needle, len, error) == 0)
			return (char *)haystack;

		haystack += s_width(haystack, error);
	}

	return NULL;
}


/*
 *  Unicode-aware version of the ANSI strpbrk() function.
 */
S_API char *s_strpbrk(const char *s, const char *set, s_erc *error)
{
	const char *setp;
	uint32 c;
	uint32 d;


	S_CLR_ERR(error);

 	if ((s == NULL)
	    || (set == NULL))
		return NULL;

	while ((c = s_getc(s, error)) != 0)
	{
		setp = set;

		while ((d = s_getx((char**)&setp, error)) != 0)
		{
			if (c == d)
				return (char *)s;
		}

		s += s_width(s, error);
	}

	return NULL;
}


/*
 *  Unicode-aware version of the ANSI strtok() function.
 */
S_API char *s_strtok(char *s, const char *set, s_erc *error)
{
	static char *last = NULL;


	S_CLR_ERR(error);
	return s_strtok_r(s, set, &last, error);
}


/*
 *  Unicode-aware version of the strtok_r() function.
 */
S_API char *s_strtok_r(char *s, const char *set, char **last, s_erc *error)
{
	char *prev_str;
	char *tok;
	const char *setp;
	uint32 c;
	uint32 sc;


	S_CLR_ERR(error);

	if (last == NULL)
		return NULL;

	if (s == NULL)
	{
		s = *last;

		if (s == NULL)
			return NULL;
	}

	prev_str = s;
	c = s_getx((char**)&s, error);

	setp = set;

	while ((sc = s_getx((char**)&setp, error)) != 0)
	{
		if (c == sc)
		{
			prev_str = s;
			c = s_getx((char**)&s, error);

			setp = set;
		}
	}

	if (c == 0)
	{
		*last = NULL;
		return NULL;
	}

	tok = prev_str;

	for (;;)
	{
		prev_str = s;
		c = s_getx((char**)&s, error);

		setp = set;

		do
		{
			sc = s_getx((char**)&setp, error);
			if (sc == c)
			{
				if (c == 0)
				{
					*last = NULL;
					return tok;
				}
				else
				{
					s += s_setat(prev_str, 0, 0, error);

					if (S_CHK_ERR(error, S_CONTERR,
								  "s_strtok_r",
								  "Character width error"))
						return NULL;

					*last = s;
					return tok;
				}
			}
		} while (sc);
	}
}


/*
 *  Unicode-aware version of the ANSI atof() function. No need to bother
 *  implementing this ourselves, since all floating point numbers are
 *  valid ASCII in any case.
 */
S_API double s_atof(const char *s, s_erc *error)
{
	S_CLR_ERR(error);

	if (s == NULL)
		return 0;

	return atof(s);
}


/*
 *  Unicode-aware version of the ANSI strtol() function.
 */
S_API long s_strtol(const char *s, char **endp, uint base, s_erc *error)
{
	char *myendp;
	long ret;


	S_CLR_ERR(error);

	if (s == NULL)
		return 0;

	ret = strtol(s, &myendp, base);

	if (endp)
		*endp = (char *)s + s_offset(s, (long)myendp, error);

	return ret;
}


/*
 *  Unicode-aware version of the ANSI strtod() function.
 */
S_API double s_strtod(const char *s, char **endp, s_erc *error)
{
	char *myendp;
	double ret;


	S_CLR_ERR(error);

	if (s == NULL)
		return 0;

	ret = strtod(s, &myendp);

	if (endp)
		*endp = (char *)s + s_offset(s, (long)myendp, error);

	return ret;
}


/*
 *  Fetch the error description from the OS and convert to Unicode.
 */
S_API const char *s_strerror(int err)
{
	const char *s = strerror(err);
	return s;
}


/*
 * Validate a given UTF-8 string.
 */
S_API s_bool s_isvalid(const char *s, s_erc *error)
{
	uint i;
	uint len;
	uchar *p = NULL;


	S_CLR_ERR(error);

	p = (uchar*)s;
	len = s_strzsize(s, error);

	for (i = 0; i < len; i++, p++)
	{
		if (s_cforbidden(*p) == TRUE)  /* invalid utf8 */
			return FALSE;
	}

	return TRUE;
}


/*
 * Test to see if string matches a regular expression.
 */
S_API s_bool s_smatches(const char *string, const char *regex, s_erc *error)
{
	s_regex *rx;
	int rv;


	S_CLR_ERR(error);

	if ((string == NULL) || (regex == NULL))
		return FALSE;

	rx = s_regex_comp(regex, S_DOT_EXCLD_NEWLINE, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_smatches",
				  "Failed to compile regular expression"))
		return FALSE;

	rv = s_regex_match(rx, (char*)string, NULL, error);
	S_FREE(rx);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_smatches",
				  "Failed to execute regular expression"))
		return FALSE;

	if (rv > 0)
		return TRUE;

	return FALSE;
}


/*
 * Return the string that is after the first occurance of the given character in the
 * given string.
 */
S_API char *s_safter(const char *haystack, const char *needle, s_erc *error)
{
	char *p;
	char *q;


	S_CLR_ERR(error);

	if ((haystack == NULL) || (needle == NULL))
		return NULL;

	p = s_strstr(haystack, needle, error);

	if (p == NULL)
		return NULL;

	q = s_strdup(p + s_strsize(needle, error), error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_safter",
				  "String duplication error"))
		return NULL;

	return q;
}


/*
 * Return the string that is before the first occurance of the given character in the
 * given string.
 */
S_API char *s_sbefore(const char *haystack, const char *needle, s_erc *error)
{
	char *p;
	char *q;
	int hsl; /* length */
	int hsb; /* bytes */
	int psl; /* length */
	int psb; /* bytes */



	S_CLR_ERR(error);

	if ((haystack == NULL) || (needle == NULL))
		return NULL;

	p = s_strstr(haystack, needle, error);

	if (p == NULL)
		return NULL;

	hsb = s_strsize(haystack, error);
	psb = s_strsize(p, error);
	hsl = s_strlen(haystack, error);
	psl = s_strlen(p, error);

	q = S_MALLOC(char, hsb - psb + 1);

	if (q == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_sbefore",
				  "Failed to allocate memory for new string");
	}
	else
	{
		q = s_strncpy(q, haystack, hsl - psl, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_sbefore",
					  "String copy error"))
			S_FREE(q);
		else
			s_setc(q + s_strsize(q, error), 0, error);
	}

	return q;
}


S_API void s_sappend(char **str1, const char *str2, s_erc *error)
{
	char *ndst;
	char *tmp;
	size_t nsize;


	S_CLR_ERR(error);

	if (str2 == NULL)
		return;

	if (*str1 == NULL)
	{
		ndst = s_strdup(str2, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_sappend",
					  "Failed to duplicate string str2"))
			return;

		*str1 = ndst;
		return;
	}

	nsize = s_strsize(*str1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sappend",
				  "Failed to determine string str1 size"))
		return;

	nsize += s_strzsize(str2, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sappend",
				  "Failed to determine string str2 size"))
		return;

	ndst = S_MALLOC(char, nsize);
	if (ndst == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_sappend",
				  "Failed to allocate memory for new string");
		return;
	}

	s_strcpy(ndst, *str1, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sappend",
				  "Failed to copy string str1"))
	{
		S_FREE(ndst);
		return;
	}

	s_strcat(ndst, str2, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sappend",
				  "Failed to concatenate string str2"))
	{
		S_FREE(ndst);
		return;
	}

	tmp = *str1;
	S_FREE(tmp);
	*str1 = ndst;
}


S_LOCAL char *s_strdup_clib(const char *str)
{
	char *buf;
	size_t ssize;


	if (str == NULL)
		return NULL;

	ssize = strlen(str);
	if (ssize == 0)
		return NULL;

	buf = S_MALLOC(char, ssize + 1);
	if (buf == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR, "s_strdup_clib",
						"Failed to allocate memory for new string");
		return NULL;
	}

	memcpy(buf, str, ssize);
	if (ssize > 0)
		buf[ssize]= '\0';

	return buf;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_bool s_cforbidden(uchar octet)
{
	switch (octet)
	{
	case 0xc0:
	case 0xc1:
	case 0xf5:
	case 0xff:
		return TRUE;
	}

	return FALSE;
}
