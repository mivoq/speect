/************************************************************************************/
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
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
/* DATE    : June 2012                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Windows specific string functions.                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/strings/platform/win32/win32_strings.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API char *s_utf8_from_utf16(const wchar_t* from, int n, s_erc *error)
{
	int n_bytes;
	char *to;


	S_CLR_ERR(error);

	if (from == NULL)
		return NULL;

	/* get required UTF-8 string length */
	n_bytes = WideCharToMultiByte(CP_UTF8, 0, from, n, NULL, 0, NULL, NULL);
	if (n_bytes == 0)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_utf8_from_utf16",
				  "Call to \"WideCharToMultiByte\" failed");
		return NULL;
	}

	if (n != -1)
		n_bytes += 1; /* null terminated */

	to = S_CALLOC(char, n_bytes);
	if (to == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_utf8_from_utf16",
				  "Failed to allocate memory for new UTF-8 string");
		return NULL;
	}

	/* do conversion */
	n_bytes = WideCharToMultiByte(CP_UTF8, 0, from, n, to, n_bytes + 1, NULL, NULL);
	if (n_bytes == 0)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_utf8_from_utf16",
				  "Call to \"WideCharToMultiByte\" failed");
		S_FREE(to);
		return NULL;
	}

	return to;
}
