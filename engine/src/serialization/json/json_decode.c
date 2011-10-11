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
/* DATE    : 27 April 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* JSON string decoder.                                                             */
/*                                                                                  */
/* Adapted from Yet Another JSON Library (YAJL) (0.4.0)                             */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* Yet Another JSON Library License                                                 */
/*                                                                                  */
/* Copyright 2007, Lloyd Hilaiel.                                                   */
/*                                                                                  */
/* Redistribution and use in source and binary forms, with or without               */
/* modification, are permitted provided that the following conditions are           */
/* met:                                                                             */
/*                                                                                  */
/*  1. Redistributions of source code must retain the above copyright               */
/*     notice, this list of conditions and the following disclaimer.                */
/*                                                                                  */
/*  2. Redistributions in binary form must reproduce the above copyright            */
/*     notice, this list of conditions and the following disclaimer in              */
/*     the documentation and/or other materials provided with the                   */
/*     distribution.                                                                */
/*                                                                                  */
/*  3. Neither the name of Lloyd Hilaiel nor the names of its                       */
/*     contributors may be used to endorse or promote products derived              */
/*     from this software without specific prior written permission.                */
/*                                                                                  */
/* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR             */
/* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED                   */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           */
/* DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,               */
/* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES               */
/* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR               */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)               */
/* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,              */
/* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING            */
/* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE               */
/* POSSIBILITY OF SUCH DAMAGE.                                                      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/strings/strings.h"
#include "serialization/json/json_decode.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void s_hex_to_digit(uint32 *val, const uchar *hex, s_erc *error);

static void s_utf32_to_utf8(uint32 codepoint, char *utf8_buf, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void s_json_string_decode(s_buffer *buf, const uchar *str, size_t len, s_erc *error)
{
	size_t beg = 0;
	size_t end = 0;


	S_CLR_ERR(error);

	while (end < len)
	{
		if (str[end] == '\\')
		{
			char utf8_buf[5];
			const char * unescaped = "?";


			s_buffer_append(buf, str + beg, end - beg, error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "s_json_string_decode",
				      "Failed to append to buffer"))
				return;

			switch (str[++end])
			{
			case 'r': unescaped = "\r"; break;
			case 'n': unescaped = "\n"; break;
			case '\\': unescaped = "\\"; break;
			case '/': unescaped = "/"; break;
			case '"': unescaped = "\""; break;
			case 'f': unescaped = "\f"; break;
			case 'b': unescaped = "\b"; break;
			case 't': unescaped = "\t"; break;
			case 'u':
			{
				uint32 codepoint = 0;

				s_hex_to_digit(&codepoint, str + ++end, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "s_json_string_decode",
					      "Call to s_hex_to_digit failed"))
					return;

				end+=3;

				/* check if this is a surrogate */
				if ((codepoint & 0xFC00) == 0xD800)
				{
					end++;
					if (str[end] == '\\' && str[end + 1] == 'u')
					{
						uint32 surrogate = 0;


						s_hex_to_digit(&surrogate, str + end + 2, error);
						if (S_CHK_ERR(error, S_CONTERR,
							      "s_json_string_decode",
							      "Call to s_hex_to_digit failed"))
							return;

						codepoint =
							(((codepoint & 0x3F) << 10) |
							 ((((codepoint >> 6) & 0xF) + 1) << 16) |
							 (surrogate & 0x3FF));
						end += 5;
					}
					else
					{
						unescaped = "?";
						break;
					}
				}
				s_utf32_to_utf8(codepoint, utf8_buf, error);
				if (S_CHK_ERR(error, S_CONTERR,
					      "s_json_string_decode",
					      "Call to s_utf32_to_utf8 failed"))
					return;

				unescaped = utf8_buf;
				break;
			}
			default:
				S_CTX_ERR(error, S_FAILURE,
					  "s_json_string_decode",
					  "Reached unreachable code!");
				return;
			}

			s_buffer_append(buf, unescaped, s_strsize(unescaped, error), error);
			if (S_CHK_ERR(error, S_CONTERR,
				      "s_json_string_decode",
				      "Failed to append to buffer"))
				return;

			beg = ++end;
		}
		else
		{
			end++;
		}
	}

	s_buffer_append(buf, str + beg, end - beg, error);
	S_CHK_ERR(error, S_CONTERR,
		  "s_json_string_decode",
		  "Failed to append to buffer");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void s_hex_to_digit(uint32 *val, const uchar *hex, s_erc *error)
{
	uint32 i;


	S_CLR_ERR(error);

	for (i = 0; i < 4; i++)
	{
		uchar c = hex[i];

		if (c >= 'A')
			c = (c & ~0x20) - 7;
		c -= '0';

		if ((c & 0xF0))
		{
			S_CTX_ERR(error, S_FAILURE,
				  "s_hex_to_digit",
				  "(c & 0xF0) = 1");
			return;
		}

		(*val) = ((*val) << 4) | c;
	}
}


static void s_utf32_to_utf8(uint32 codepoint, char *utf8_buf, s_erc *error)
{
	S_CLR_ERR(error);


	if (codepoint < 0x80)
	{
		utf8_buf[0] = (char) codepoint;
		utf8_buf[1] = 0;
	}
	else if (codepoint < 0x0800)
	{
		utf8_buf[0] = (char) ((codepoint >> 6) | 0xC0);
		utf8_buf[1] = (char) ((codepoint & 0x3F) | 0x80);
		utf8_buf[2] = 0;
	}
	else if (codepoint < 0x10000)
	{
		utf8_buf[0] = (char) ((codepoint >> 12) | 0xE0);
		utf8_buf[1] = (char) (((codepoint >> 6) & 0x3F) | 0x80);
		utf8_buf[2] = (char) ((codepoint & 0x3F) | 0x80);
		utf8_buf[3] = 0;
	}
	else if (codepoint < 0x200000)
	{
		utf8_buf[0] =(char)((codepoint >> 18) | 0xF0);
		utf8_buf[1] =(char)(((codepoint >> 12) & 0x3F) | 0x80);
		utf8_buf[2] =(char)(((codepoint >> 6) & 0x3F) | 0x80);
		utf8_buf[3] =(char)((codepoint & 0x3F) | 0x80);
		utf8_buf[4] = 0;
	}
	else
	{
		utf8_buf[0] = '?';
		utf8_buf[1] = 0;
	}
}
