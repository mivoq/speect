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
/* UTF8 string printing functions.                                                  */
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

#include <stdio.h>
#include <string.h>
#include "base/strings/sprint.h"
#include "base/utils/alloc.h"
#include "base/strings/strings.h"
#include "base/utils/math.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

#define S_SPRINT_FLAG_LEFT_JUSTIFY             1
#define S_SPRINT_FLAG_FORCE_PLUS_SIGN          2
#define S_SPRINT_FLAG_FORCE_SPACE              4
#define S_SPRINT_FLAG_ALTERNATE_CONVERSION     8
#define S_SPRINT_FLAG_PAD_ZERO                 16
#define S_SPRINT_FLAG_SHORT_INT                32
#define S_SPRINT_FLAG_LONG_INT                 64
#define S_SPRINT_FLAG_LONG_DOUBLE              128

/* no long long in C89 */
#define S_LONGEST long


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/*
 *  Helper for reading an integer from the varargs list.
 */
#define s_va_int(args, flags)					\
	(											\
		((flags) & S_SPRINT_FLAG_SHORT_INT) ?	\
		va_arg(args, signed int)				\
		:										\
		(((flags) & S_SPRINT_FLAG_LONG_INT) ?	\
		 va_arg(args, signed long int)			\
		 :										\
		 va_arg(args, signed int))				\
		)


/*
 *  Helper for reading an unsigned integer from the varargs list.
 */
#define s_va_uint(args, flags)					\
	(											\
		((flags) & S_SPRINT_FLAG_SHORT_INT) ?	\
		va_arg(args, unsigned int)				\
		:										\
		(((flags) & S_SPRINT_FLAG_LONG_INT) ?	\
		 va_arg(args, unsigned long int)		\
		 :										\
		 va_arg(args, unsigned int))			\
		)


/*
 *  Helper to add a plus sign or space in front of a number.
 */
#define s_sprint_plus_sign(len, error)							\
	{															\
		if (info->flags & S_SPRINT_FLAG_FORCE_PLUS_SIGN)		\
		{														\
			pos += s_setc(string_arg->data + pos, '+', error);	\
			len++;												\
		}														\
		else if (info->flags & S_SPRINT_FLAG_FORCE_SPACE)		\
		{														\
			pos += s_setc(string_arg->data + pos, ' ', error);	\
			len++;												\
		}														\
	}


/* format specifier */
#define S_SPRINT_NEXT_C(error)					\
	{											\
		format += s_width(format, error);		\
		c = s_getc(format, error);				\
	}




/************************************************************************************/
/*                                                                                  */
/* Typedefs and Data types                                                          */
/*                                                                                  */
/************************************************************************************/

typedef struct
{
	int flags;
	int field_width;
	int precision;
	int num_special;
} s_sprint_info;


typedef struct s_string_arg
{
	char                  *data;
	int                    size;
	struct s_string_arg   *next;
} s_string_arg;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static int s_sprint_char(s_string_arg *string_arg, s_sprint_info *info,
						 long val,  s_erc *error);

static int s_sprint_i(s_string_arg *string_arg, unsigned long val,
					  int precision,  s_erc *error);

static int s_sprint_int(s_string_arg *string_arg, s_sprint_info *info,
						long val, s_erc *error);

static int s_sprint_unsigned(s_string_arg *string_arg, s_sprint_info *info,
							 unsigned long val, s_erc *error);

static int s_sprint_hex(s_string_arg *string_arg, s_sprint_info *info, int caps,
						unsigned long val, s_erc *error);

static int s_sprint_octal(s_string_arg *string_arg, s_sprint_info *info,
						  unsigned long val, s_erc *error);

static int s_sprint_float(s_string_arg *string_arg, s_sprint_info *info,
						  double val, int conversion, s_erc *error);

static int s_sprint_string(s_string_arg *string_arg, s_sprint_info *info,
						   const char *s, s_erc *error);

static int s_decode_format_string(char *buf, s_string_arg *string_arg,
								  const char *format, va_list args, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/*
 *  Enhanced Unicode-aware version of the ANSI vsprintf() function
 *  than can handle the size (in bytes) of the destination buffer.
 */
S_API int s_vszprintf(char *buf, size_t count, const char *format, va_list args, s_erc *error)
{
	char *decoded_format;
	char *df;
	s_string_arg *string_args;
	s_string_arg *iter_arg;
	uint32 c;
	int len;
	int tmp;


	S_CLR_ERR(error);

	if ((buf == NULL)
	    || (count == 0)
	    || (format == NULL))
		return 0;

	/* decoding can only lower the length of the format string */
	decoded_format = S_MALLOC(char, s_strzsize(format, error));

	if (decoded_format == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_vszprintf",
				  "Failed to allocate memory to decoded format string");
		return 0;
	}

	df = decoded_format;

	/* allocate first item */
	string_args = S_MALLOC(s_string_arg, 1);

	if (string_args == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_vszprintf",
				  "Failed to allocate memory to string_args object");
		S_FREE(decoded_format);
		return 0;
	}

	string_args->next = NULL;

	/* 1st pass: decode */
	len = s_decode_format_string(decoded_format, string_args, format, args, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_vszprintf",
				  "Failed to decode format string"))
	{
		S_FREE(decoded_format);

		while (string_args->next)
		{
			S_FREE(string_args->data);
			iter_arg = string_args;
			string_args = string_args->next;
			S_FREE(iter_arg);
		}

		S_FREE(string_args);
		return 0;
	}

	tmp = count - s_cwidth(0, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_vszprintf",
				  "Character width error"))
	{
		S_FREE(decoded_format);

		while (string_args->next)
		{
			S_FREE(string_args->data);
			iter_arg = string_args;
			string_args = string_args->next;
			S_FREE(iter_arg);
		}

		S_FREE(string_args);
		return 0;
	}

	iter_arg = string_args;

	/* 2nd pass: concatenate */
	while ((c = s_getx(&decoded_format, error)) != 0)
	{
		if (c == '%')
		{
			if ((c = s_getx(&decoded_format, error)) == '%')
			{
				/* percent sign escape */
				tmp -= s_cwidth('%', error);
				if (tmp < 0)
					break;
				buf += s_setc(buf, '%', error);
			}
			else if (c == 's')
			{
				/* string argument */
				s_strzcpy(buf, iter_arg->data, count + s_cwidth(0, error), error);

				if (S_CHK_ERR(error, S_CONTERR,
							  "s_vszprintf",
							  "String copy error"))
				{
					S_FREE(decoded_format);

					while (string_args->next)
					{
						S_FREE(string_args->data);
						iter_arg = string_args;
						string_args = string_args->next;
						S_FREE(iter_arg);
					}

					S_FREE(string_args);
					return 0;
				}

				buf += iter_arg->size;
				tmp -= iter_arg->size;

				if (tmp < 0)
				{
					buf += count;
					break;
				}
				iter_arg = iter_arg->next;
			}
		}
		else
		{
			/* normal character */
			tmp -= s_cwidth(c, error);

			if (S_CHK_ERR(error, S_CONTERR,
						  "s_vszprintf",
						  "Character width error"))
			{
				S_FREE(decoded_format);

				while (string_args->next)
				{
					S_FREE(string_args->data);
					iter_arg = string_args;
					string_args = string_args->next;
					S_FREE(iter_arg);
				}

				S_FREE(string_args);
				return 0;
			}

			if (tmp < 0)
				break;

			buf += s_setc(buf, c, error);
		}
	}

	s_setc(buf, 0, error);

	/* free allocated resources */
	while (string_args->next)
	{
		S_FREE(string_args->data);
		iter_arg = string_args;
		string_args = string_args->next;
		S_FREE(iter_arg);
	}

	S_FREE(string_args);
	S_FREE(df);  /* alias for decoded_format */

	return len;
}


/*
 *  Unicode-aware version of the ANSI sprintf() function.
 */
S_API int s_sprintf(char *buf, s_erc *error, const char *format, ...)
{
	int ret;
	va_list ap;


	S_CLR_ERR(error);

	if ((buf == NULL)
	    || (format == NULL))
		return 0;

	va_start(ap, format);
	ret = s_vszprintf(buf, INT_MAX, format, ap, error);
	va_end(ap);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sprintf",
				  "Failed internal s_vszprintf"))
		return 0;

	return ret;
}


/*
 *  Enhanced Unicode-aware version of the ANSI sprintf() function
 *  that can handle the size (in bytes) of the destination buffer.
 */
S_API int s_szprintf(char *buf, size_t count, s_erc *error, const char *format, ...)
{
	int ret;
	va_list ap;


	S_CLR_ERR(error);

	if ((buf == NULL)
	    || (count == 0)
	    || (format == NULL))
		return (0);

	va_start(ap, format);
	ret = s_vszprintf(buf, count, format, ap, error);
	va_end(ap);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_szprintf",
				  "Failed internal s_vszprintf"))
		return 0;

	return ret;
}


/*
 * Works like s_vszprintf except that it allocates memory to the correct
 * size for the return buffer.
 */
S_API int s_vasprintf(char **ret, const char *format, va_list args, s_erc *error)
{
	char *buf = NULL;
	char *decoded_format = NULL;
	char *df = NULL;
	s_string_arg *string_args = NULL;
	s_string_arg *iter_arg = NULL;
	uint32 c = 0;
	int len = 0;
	int count = INT_MAX;


	S_CLR_ERR(error);

	if (format == NULL)
		return 0;

	/* decoding can only lower the length of the format string */
	decoded_format = S_MALLOC(char, s_strzsize(format, error));

	if (decoded_format == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_vasprintf",
				  "Failed to allocate memory to decoded format string");
		return 0;
	}

	df = decoded_format;

	/* allocate first item */
	string_args = S_MALLOC(s_string_arg, 1);

	if (string_args == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_vasprintf",
				  "Failed to allocate memory to string_args object");
		S_FREE(decoded_format);
		return 0;
	}

	string_args->next = NULL;

	/* 1st pass: decode */
	len = s_decode_format_string(decoded_format, string_args, format, args, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_vasprintf",
				  "Failed to decode format string"))
	{
		S_FREE(decoded_format);

		while (string_args->next)
		{
			S_FREE(string_args->data);
			iter_arg = string_args;
			string_args = string_args->next;
			S_FREE(iter_arg);
		}

		S_FREE(string_args);
		return 0;
	}

	/* now allocate memory for buffer */
	/* ABY: HACK here, not sure how to handle size correctly */
	(*ret) = S_MALLOC(char, len*2 + 1);

	if ((*ret) == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_vasprintf",
				  "Failed to allocate memory to return string");
		S_FREE(decoded_format);

		while (string_args->next)
		{
			S_FREE(string_args->data);
			iter_arg = string_args;
			string_args = string_args->next;
			S_FREE(iter_arg);
		}

		S_FREE(string_args);
		return 0;
	}

	buf = (*ret);

	count -= s_cwidth(0, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_vasprintf",
				  "Character width error"))
	{
		S_FREE(decoded_format);

		while (string_args->next)
		{
			S_FREE(string_args->data);
			iter_arg = string_args;
			string_args = string_args->next;
			S_FREE(iter_arg);
		}

		S_FREE(string_args);
		return 0;
	}

	iter_arg = string_args;

	/* 2nd pass: concatenate */
	while ((c = s_getx(&decoded_format, error)) != 0)
	{
		if (c == '%')
		{
			if ((c = s_getx(&decoded_format, error)) == '%')
			{
				/* percent sign escape */
				count -= s_cwidth('%', error);
				if (count < 0)
					break;
				buf += s_setc(buf, '%', error);
			}
			else if (c == 's')
			{
				/* string argument */
				s_strzcpy(buf, iter_arg->data, count +
						  s_cwidth(0, error), error);

				if (S_CHK_ERR(error, S_CONTERR,
							  "s_vasprintf",
							  "String copy error"))
				{
					S_FREE(decoded_format);

					while (string_args->next)
					{
						S_FREE(string_args->data);
						iter_arg = string_args;
						string_args = string_args->next;
						S_FREE(iter_arg);
					}

					S_FREE(string_args);
					return 0;
				}

				buf += iter_arg->size;
				count -= iter_arg->size;
				if (count < 0)
				{
					buf += count;
					break;
				}
				iter_arg = iter_arg->next;
			}
		}
		else
		{
			/* normal character */
			count -= s_cwidth(c, error);

			if (S_CHK_ERR(error, S_CONTERR,
						  "s_vasprintf",
						  "Character width error"))
			{
				S_FREE(decoded_format);

				while (string_args->next)
				{
					S_FREE(string_args->data);
					iter_arg = string_args;
					string_args = string_args->next;
					S_FREE(iter_arg);
				}

				S_FREE(string_args);
				return 0;
			}

			if (count < 0)
				break;
			buf += s_setc(buf, c, error);
		}
	}

	s_setc(buf, 0, error);

	/* free allocated resources */
	while (string_args->next)
	{
		S_FREE(string_args->data);
		iter_arg = string_args;
		string_args = string_args->next;
		S_FREE(iter_arg);
	}

	S_FREE(string_args);
	S_FREE(df);  /* alias for decoded_format */

	return len;
}


S_API int s_asprintf(char **ret, s_erc *error, const char *format, ...)
{
	char *buf;
	va_list args;


	S_CLR_ERR(error);

	va_start(args, format);
	s_vasprintf(&buf, format, args, error);
	va_end(args);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_asprintf",
				  "Failed internal s_vasprintf"))
		return 0;

	(*ret) = buf;

	return (s_strzsize(buf, error));
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 *  Helper for formatting a character.
 */
static int s_sprint_char(s_string_arg *string_arg, s_sprint_info *info,
						 long val, s_erc *error)
{
	int pos = 0;
	size_t len;


	S_CLR_ERR(error);

	/* 4 is max width of UTF-8 ? */
	len = S_MAX(1, info->field_width) * 4 + s_cwidth(0, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sprint_char",
				  "Character width error"))
		return 0;

	/* 1 character max for... a character */
	string_arg->data = S_MALLOC(char, len);

	if (string_arg->data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_sprint_char",
				  "Failed to allocate memory for 'string_arg->data' object");
		return 0;
	}

	pos += s_setc(string_arg->data, val, error);

	string_arg->size = pos;

	s_setc(string_arg->data + pos, 0, error);

	return 1;
}


/*
 *   Helper for formatting an integer.
 */
static int s_sprint_i(s_string_arg *string_arg, unsigned long val,
					  int precision,  s_erc *error)
{
	char tmp[24];  /* for 64-bit integers */
	int i = 0;
	int pos = string_arg->size;
	int len;


	S_CLR_ERR(error);

	do
	{
		tmp[i++] = val % 10;
		val /= 10;
	} while (val);

	for (len = i; len < precision; len++)
		pos += s_setc(string_arg->data + pos, '0', error);

	while (i > 0)
		pos += s_setc(string_arg->data + pos, tmp[--i] + '0', error);

	string_arg->size = pos;
	s_setc(string_arg->data + pos, 0, error);

	return len;
}


/*
 *  Helper for formatting a signed integer.
 */
static int s_sprint_int(s_string_arg *string_arg, s_sprint_info *info,
						long val, s_erc *error)
{
	int pos = 0;
	int len = 0;
	size_t tmp;


	S_CLR_ERR(error);

	/* 4 is max width of UTF-8 ? */
	tmp = S_MAX(24, info->field_width) * 4 + s_cwidth(0, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sprint_int",
				  "Character width error"))
		return 0;

	/* 24 characters max for a 64-bit integer */
	string_arg->data = S_MALLOC(char, tmp);

	if (string_arg->data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_sprint_int",
				  "Failed to allocate memory for 'string_arg->data' object");
		return 0;
	}

	if (val < 0)
	{
		val = -val;
		pos += s_setc(string_arg->data + pos, '-', error);
		len++;
	}
	else
		s_sprint_plus_sign(len, error);

	info->num_special = len;

	string_arg->size = pos;

	return s_sprint_i(string_arg, val, info->precision, error) + info->num_special;
}


/*
 *  Helper for formatting an unsigned integer.
 */
static int s_sprint_unsigned(s_string_arg *string_arg, s_sprint_info *info,
							 unsigned long val, s_erc *error)
{
	int pos = 0;
	size_t len;


	S_CLR_ERR(error);

	/* 4 is max width of UTF-8 ? */
	len = S_MAX(24, info->field_width) * 4 + s_cwidth(0, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sprint_unsigned",
				  "Character width error"))
		return 0;

	/* 24 characters max for a 64-bit integer */
	string_arg->data = S_MALLOC(char, len);

	if (string_arg->data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_sprint_unsigned",
				  "Failed to allocate memory for 'string_arg->data' object");
		return 0;
	}

	s_sprint_plus_sign(info->num_special, error);

	string_arg->size = pos;

	return (s_sprint_i(string_arg, val, info->precision, error) + info->num_special);
}


/*
 *  Helper for formatting a hex integer.
 */
static int s_sprint_hex(s_string_arg *string_arg, s_sprint_info *info, int caps,
						unsigned long val, s_erc *error)
{
	static char hex_digit_caps[] = "0123456789ABCDEF";
	static char hex_digit[] = "0123456789abcdef";
	char tmp[24];  /* for 64-bit integers */
	char *table;
	int pos = 0;
	int i = 0;
	int len;
	size_t stmp;


	S_CLR_ERR(error);

	/* 4 is max width of UTF-8 ? */
	stmp = S_MAX(24, info->field_width) * 4 + s_cwidth(0, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sprint_hex",
				  "Character width error"))
		return 0;

	/* 24 characters max for a 64-bit integer */
	string_arg->data = S_MALLOC(char, stmp);

	if (string_arg->data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_sprint_hex",
				  "Failed to allocate memory for 'string_arg->data' object");
		return 0;
	}

	s_sprint_plus_sign(info->num_special, error);

	if (info->flags & S_SPRINT_FLAG_ALTERNATE_CONVERSION)
	{
		pos += s_setc(string_arg->data + pos, '0', error);
		pos += s_setc(string_arg->data + pos, 'x', error);
		info->num_special += 2;
	}

	do
	{
		tmp[i++] = val & 15;
		val >>= 4;
	} while (val);

	for (len = i; len < info->precision; len++)
		pos += s_setc(string_arg->data + pos, '0', error);

	if (caps)
		table = hex_digit_caps;
	else
		table = hex_digit;

	while (i > 0)
		pos += s_setc(string_arg->data + pos, table[(int)tmp[--i]], error);

	string_arg->size = pos;
	s_setc(string_arg->data + pos, 0, error);

	return (len + info->num_special);
}


/*
 *  Helper for formatting an octal integer.
 */
static int s_sprint_octal(s_string_arg *string_arg, s_sprint_info *info,
						  unsigned long val, s_erc *error)
{
	char tmp[24];  /* for 64-bit integers */
	int pos = 0;
	int i = 0;
	int len;
	size_t stmp;


	S_CLR_ERR(error);

	/* 4 is max width of UTF-8 ? */
	stmp = S_MAX(24, info->field_width) * 4 + s_cwidth(0, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sprint_octal",
				  "Character width error"))
		return 0;

	/* 24 characters max for a 64-bit integer */
	string_arg->data = S_MALLOC(char, stmp);

	if (string_arg->data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_sprint_octal",
				  "Failed to allocate memory for 'string_arg->data' object");
		return 0;
	}

	s_sprint_plus_sign(info->num_special, error);

	if (info->flags & S_SPRINT_FLAG_ALTERNATE_CONVERSION)
	{
		pos += s_setc(string_arg->data + pos, '0', error);
		info->num_special++;
	}

	do
	{
		tmp[i++] = val & 7;
		val >>= 3;
	} while (val);

	for (len = i; len < info->precision; len++)
		pos += s_setc(string_arg->data+pos, '0', error);

	while (i > 0)
		pos += s_setc(string_arg->data+pos, tmp[--i] + '0', error);

	string_arg->size = pos;
	s_setc(string_arg->data + pos, 0, error);

	return (len + info->num_special);
}


/*
 *  Helper for formatting a float (piggyback on the libc implementation).
 */
static int s_sprint_float(s_string_arg *string_arg, s_sprint_info *info,
						  double val, int conversion, s_erc *error)
{
	char format[256];
	char tmp[256];
	int len = 0;
	int size;


	S_CLR_ERR(error);

	format[len++] = '%';

	if (info->flags & S_SPRINT_FLAG_LEFT_JUSTIFY)
		format[len++] = '-';

	if (info->flags & S_SPRINT_FLAG_FORCE_PLUS_SIGN)
		format[len++] = '+';

	if (info->flags & S_SPRINT_FLAG_FORCE_SPACE)
		format[len++] = ' ';

	if (info->flags & S_SPRINT_FLAG_ALTERNATE_CONVERSION)
		format[len++] = '#';

	if (info->flags & S_SPRINT_FLAG_PAD_ZERO)
		format[len++] = '0';

	if (info->field_width > 0)
		len += sprintf(format + len, "%d", info->field_width);

	if (info->precision >= 0)
		len += sprintf(format + len, ".%d", info->precision);

	format[len++] = conversion;
	format[len] = 0;

	len = sprintf(tmp, format, val);

	/* 4 is max width of UTF-8 ? */
	size = len * 4 + s_cwidth(0, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sprint_float",
				  "Character width error"))
		return 0;

	string_arg->data = S_MALLOC(char, size);

	if (string_arg->data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_sprint_float",
				  "Failed to allocate memory for 'string_arg->data' object");
		return 0;
	}

	string_arg->data = s_strzcpy(string_arg->data, tmp, size, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sprint_float",
				  "String copy error"))
	{
		S_FREE(string_arg->data);
		return 0;
	}

	info->field_width = 0;

	string_arg->size = s_strsize(string_arg->data, error);

	return len;
}


/*
 *  Helper for formatting a string.
 */
static int s_sprint_string(s_string_arg *string_arg, s_sprint_info *info,
						   const char *s, s_erc *error)
{
	int pos = 0;
	int len = 0;
	uint32 c;
	size_t tmp;


	S_CLR_ERR(error);

	/* 4 is max width of UTF-8 ? */
	tmp = S_MAX(s_strlen(s, error), (uint)info->field_width) * 4 + s_cwidth(0, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sprint_string",
				  "Character width error"))
		return 0;

	/* 24 characters max for a 64-bit integer */
	string_arg->data = S_MALLOC(char, tmp);

	if (string_arg->data == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_sprint_string",
				  "Failed to allocate memory for 'string_arg->data' object");
		return 0;
	}

	while ((c = s_getx((char**)&s, error)) != 0)
	{
		if ((info->precision >= 0) && (len >= info->precision))
			break;

		pos += s_setc(string_arg->data + pos, c, error);
		len++;
	}

	string_arg->size = pos;
	s_setc(string_arg->data + pos, 0, error);

	return len;
}


/*
 *  Worker function for decoding the format string (with those pretty '%' characters)
 */
static int s_decode_format_string(char *buf, s_string_arg *string_arg,
								  const char *format, va_list args, s_erc *error)
{
	s_sprint_info info;
	int *pstr_pos;
	int done;
	int slen;
	uint32 c;
	int i;
	int pos;
	int shift;
	int shiftbytes;
	int shiftfiller;
	int len = 0;


	S_CLR_ERR(error);

	while ((c = s_getx((char**)&format, error)) != 0)
	{

		if (c == '%')
		{
			if ((c = s_getc(format, error)) == '%')
			{
				/* percent sign escape */
				format += s_width(format, error);
				buf += s_setc(buf, '%', error);
				buf += s_setc(buf, '%', error);
				len++;
			}
			else
			{
				/* set default conversion flags */
				info.flags = 0;
				info.field_width = 0;
				info.precision = -1;
				info.num_special = 0;

				/* check for conversion flags */
				done = FALSE;

				do
				{
					switch (c)
					{
					case '-':
						info.flags |= S_SPRINT_FLAG_LEFT_JUSTIFY;
						S_SPRINT_NEXT_C(error);
						break;

					case '+':
						info.flags |= S_SPRINT_FLAG_FORCE_PLUS_SIGN;
						S_SPRINT_NEXT_C(error);
						break;

					case ' ':
						info.flags |= S_SPRINT_FLAG_FORCE_SPACE;
						S_SPRINT_NEXT_C(error);
						break;

					case '#':
						info.flags |= S_SPRINT_FLAG_ALTERNATE_CONVERSION;
						S_SPRINT_NEXT_C(error);
						break;

					case '0':
						info.flags |= S_SPRINT_FLAG_PAD_ZERO;
						S_SPRINT_NEXT_C(error);
						break;

					default:
						done = TRUE;
						break;
					}

				} while (!done);

				/* check for a field width specifier */
				if (c == '*')
				{
					S_SPRINT_NEXT_C(error);
					info.field_width = va_arg(args, int);
					if (info.field_width < 0)
					{
						info.flags |= S_SPRINT_FLAG_LEFT_JUSTIFY;
						info.field_width = -info.field_width;
					}
				}
				else if ((c >= '0') && (c <= '9'))
				{
					info.field_width = 0;
					do
					{
						info.field_width *= 10;
						info.field_width += c - '0';
						S_SPRINT_NEXT_C(error);
					} while ((c >= '0') && (c <= '9'));
				}

				/* check for a precision specifier */
				if (c == '.')
					S_SPRINT_NEXT_C(error);

				if (c == '*')
				{
					S_SPRINT_NEXT_C(error);
					info.precision = va_arg(args, int);
					if (info.precision < 0)
						info.precision = 0;
				}
				else if ((c >= '0') && (c <= '9'))
				{
					info.precision = 0;
					do
					{
						info.precision *= 10;
						info.precision += c - '0';
						S_SPRINT_NEXT_C(error);
					} while ((c >= '0') && (c <= '9'));
				}

				/* check for size qualifiers */
				done = FALSE;

				do
				{
					switch (c)
					{
					case 'h':
						info.flags |= S_SPRINT_FLAG_SHORT_INT;
						S_SPRINT_NEXT_C(error);
						break;

					case 'l':
						info.flags |= S_SPRINT_FLAG_LONG_INT;
						S_SPRINT_NEXT_C(error);
						break;

					case 'L':
						info.flags |= (S_SPRINT_FLAG_LONG_DOUBLE | S_SPRINT_FLAG_LONG_INT);
						S_SPRINT_NEXT_C(error);
						break;

					default:
						done = TRUE;
						break;
					}

				} while (!done);

				/* format the data */
				switch (c)
				{
				case 'c':
					/* character */
					slen = s_sprint_char(string_arg, &info, va_arg(args, int), error);

					if (S_CHK_ERR(error, S_CONTERR,
								  "s_decode_format_string",
								  "s_sprint_char error"))
						return 0;

					S_SPRINT_NEXT_C(error);
					break;

				case 'd':
				case 'i':
					/* signed integer */
					slen = s_sprint_int(string_arg, &info, s_va_int(args, info.flags), error);

				if (S_CHK_ERR(error, S_CONTERR,
						      "s_decode_format_string",
						      "s_sprint_int error"))
					return 0;

				S_SPRINT_NEXT_C(error);
				break;

				case 'D':
					/* signed long integer */
					slen = s_sprint_int(string_arg, &info, s_va_int(args, info.flags | S_SPRINT_FLAG_LONG_INT), error);

					if (S_CHK_ERR(error, S_CONTERR,
								  "s_decode_format_string",
								  "s_sprint_int error"))
						return 0;

					S_SPRINT_NEXT_C(error);
					break;

				case 'e':
				case 'E':
				case 'f':
				case 'g':
				case 'G':
					/* double */
					if (info.flags & S_SPRINT_FLAG_LONG_DOUBLE)
						slen = s_sprint_float(string_arg, &info, va_arg(args, long double), c, error);
					else
						slen = s_sprint_float(string_arg, &info, va_arg(args, double), c, error);

				if (S_CHK_ERR(error, S_CONTERR,
						      "s_decode_format_string",
						      "s_sprint_float error"))
					return 0;

				S_SPRINT_NEXT_C(error);
				break;

				case 'n':
					/* store current string position */
					pstr_pos = va_arg(args, int *);
					*pstr_pos = len;
					slen = -1;
					S_SPRINT_NEXT_C(error);
					break;

				case 'o':
					/* unsigned octal integer */
					slen = s_sprint_octal(string_arg, &info, s_va_uint(args, info.flags), error);

					if (S_CHK_ERR(error, S_CONTERR,
								  "s_decode_format_string",
								  "s_sprint_octal error"))
						return 0;

					S_SPRINT_NEXT_C(error);
					break;

				case 'p':
					/* pointer */
					slen = s_sprint_hex(string_arg, &info, FALSE, (unsigned long)(va_arg(args, void *)), error);

					if (S_CHK_ERR(error, S_CONTERR,
								  "s_decode_format_string",
								  "s_sprint_hex error"))
						return 0;

					S_SPRINT_NEXT_C(error);
					break;

				case 's':
					/* string */
					slen = s_sprint_string(string_arg, &info, va_arg(args, char *), error);

					if (S_CHK_ERR(error, S_CONTERR,
								  "s_decode_format_string",
								  "s_sprint_string error"))
						return 0;

					S_SPRINT_NEXT_C(error);
					break;

				case 'u':
					/* unsigned integer */
					slen = s_sprint_unsigned(string_arg, &info, s_va_uint(args, info.flags), error);

					if (S_CHK_ERR(error, S_CONTERR,
								  "s_decode_format_string",
								  "s_sprint_unsigned error"))
						return 0;

					S_SPRINT_NEXT_C(error);
					break;

				case 'U':
					/* unsigned long integer */
					slen = s_sprint_unsigned(string_arg, &info, s_va_uint(args, info.flags | S_SPRINT_FLAG_LONG_INT), error);

					if (S_CHK_ERR(error, S_CONTERR,
								  "s_decode_format_string",
								  "s_sprint_unsigned error"))
						return 0;

					S_SPRINT_NEXT_C(error);
					break;

				case 'x':
				case 'X':
					/* unsigned hex integer */
					slen = s_sprint_hex(string_arg, &info, (c == 'X'), s_va_uint(args, info.flags), error);

				if (S_CHK_ERR(error, S_CONTERR,
						      "s_decode_format_string",
						      "s_sprint_hex error"))
					return 0;

				S_SPRINT_NEXT_C(error);
				break;

				default:
					/* weird stuff... */
					slen = -1;
					break;
				}

				if (slen >= 0)
				{
					if (slen < info.field_width)
					{
						if (info.flags & S_SPRINT_FLAG_LEFT_JUSTIFY)
						{
							/* left align the result */
							pos = string_arg->size;
							while (slen < info.field_width)
							{
								pos += s_setc(string_arg->data + pos, ' ', error);
								slen++;
							}

							string_arg->size = pos;
							s_setc(string_arg->data  +pos, 0, error);
						}
						else
						{
							/* right align the result */
							shift = info.field_width - slen;

							if (shift > 0)
							{
								pos = 0;

								if (info.flags & S_SPRINT_FLAG_PAD_ZERO)
								{
									shiftfiller = '0';

									for (i = 0; i < info.num_special; i++)
										pos += s_width(string_arg->data + pos, error);
								}
								else
									shiftfiller = ' ';

								shiftbytes = shift * s_cwidth(shiftfiller, error);

								if (S_CHK_ERR(error, S_CONTERR,
											  "s_decode_format_string",
											  "Character width error"))
									return 0;

								memmove(string_arg->data + pos + shiftbytes, string_arg->data + pos,
										string_arg->size-pos + s_cwidth(0, error));

								if (S_CHK_ERR(error, S_CONTERR,
											  "s_decode_format_string",
											  "Character width error"))
									return 0;

								string_arg->size += shiftbytes;
								slen += shift;

								for (i = 0; i < shift; i++)
									pos += s_setc(string_arg->data + pos, shiftfiller, error);
							}
						}
					}

					buf += s_setc(buf, '%', error);
					buf += s_setc(buf, 's', error);
					len += slen;

					/* allocate next item */
					string_arg->next = S_MALLOC(s_string_arg, 1);

					if (string_arg->next == NULL)
					{
						S_FTL_ERR(error, S_MEMERROR,
								  "s_decode_format_string",
								  "Failed to allocate memory for 'string_arg->next' object");
						return 0;
					}

					string_arg = string_arg->next;
					string_arg->next = NULL;
				}
			}
		}
		else
		{
			/* normal character */
			buf += s_setc(buf, c, error);
			len++;
		}
	}

	s_setc(buf, 0, error);

	return len;
}
