/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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
/* DATE    : May 2010                                                               */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Windows file system paths.                                                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include "base/strings/strings.h"
#include "base/strings/sprint.h"
#include "base/strings/char.h"
#include "base/utils/alloc.h"
#include "base/utils/platform/win32/win32_path.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_bool s_win32_path_is_absolute(const char *path, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/* if filename is absolute, just copy it to dest. Otherwise, build a
   path to it by considering it is relative to base_path. URL are
   supported. */
S_API char *s_win32_path_combine(const char *base_path, const char *filename, s_erc *error)
{
	const char *p;
	const char *p1;
	size_t len;
	s_bool is_absolute;
	char *combined_path;
	char *tmp;
	uint32 c;


	S_CLR_ERR(error);
	is_absolute = s_win32_path_is_absolute(filename, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_win32_path_combine",
		      "Call to \"s_win32_path_is_absolute\" failed"))
		return NULL;

	if (is_absolute)
	{
		combined_path = s_strdup(filename, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "s_win32_path_combine",
			      "Call to \"s_strdup\" failed"))
			return NULL;

		return combined_path;
	}

	/* sanity, check that path does end with '/' or '\' */
	len = s_strlen(base_path, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_win32_path_combine",
		      "Call to \"s_strlen\" failed"))
		return NULL;

	c = s_getat(base_path, (uint)(len - 1), error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_win32_path_combine",
		      "Call to \"s_getat\" failed"))
		return NULL;

	if ((c != '/') && (c != '\\'))
	{
		S_CTX_ERR(error, S_FAILURE,
			  "s_win32_path_combine",
			  "Given base path \"%s\" does not end with a '/' or '\\' ",
			  base_path);
		return NULL;
	}

	p = s_strchr(base_path, ':', error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_win32_path_combine",
		      "Call to \"s_strchr\" failed"))
		return NULL;

	if (p)
		p++;
	else
		p = base_path;

	p1 = s_strrchr(base_path, '/', error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_win32_path_combine",
		      "Call to \"s_strrchr\" failed"))
		return NULL;

	/* win32 */
	{
		const char *p2;
		p2 = s_strrchr(base_path, '\\', error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "s_win32_path_combine",
			      "Call to \"s_strrchr\" failed"))
			return NULL;

		if (!p1 || p2 > p1)
			p1 = p2;
	}

	if (p1)
		p1++;
	else
		p1 = base_path;

	if (p1 > p)
		p = p1;

	len = p - base_path;
	tmp = S_CALLOC(char, len + 1);
	if (tmp == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
			  "s_win32_path_combine",
			  "Failed to allocate memory for 'char' object");
		return NULL;
	}

	memcpy(tmp, base_path, len);
	tmp[len] = '\0';

	s_asprintf(&combined_path, error, "%s%s", tmp, filename);
	S_FREE(tmp);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_win32_path_combine",
		      "Call to \"s_asprintf\" failed"))
	{
		S_FREE(combined_path);
		return NULL;
	}

	return combined_path;
}


/* get the base path of the given absolute path and file name */
S_API char *s_win32_get_base_path(const char *absolute_filename, s_erc *error)
{
	char *base_path;
	const char *ptr;
	size_t plen;
	uint32 c;
	s_bool is_absolute;


	S_CLR_ERR(error);
	if (absolute_filename == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_win32_get_base_path",
			  "Argument \"absolute_filename\" is NULL");
		return NULL;
	}

	is_absolute = s_win32_path_is_absolute(absolute_filename, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_win32_get_base_path",
		      "Call to \"s_win32_path_is_absolute\" failed"))
		return NULL;

	if (!is_absolute)
	{
		S_CTX_ERR(error, S_FAILURE,
			  "s_win32_get_base_path",
			  "Given absolute file name \"%s\" is not absolute",
			  absolute_filename);
		return NULL;
	}

	/* sanity, check that path does not end with '/' or '\\' */
	plen = s_strlen(absolute_filename, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_win32_get_base_path",
		      "Call to \"s_strlen\" failed"))
		return NULL;

	c = s_getat(absolute_filename, (uint)(plen - 1), error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_win32_get_base_path",
		      "Call to \"s_getat\" failed"))
		return NULL;

	if ((c == '/') || (c == '\\'))
	{
		S_CTX_ERR(error, S_FAILURE,
			  "s_win32_get_base_path",
			  "Given absolute filename \"%s\" ends with a '/' or '\\' ",
			  absolute_filename);
		return NULL;
	}

	/* get last path separator */
	ptr = s_strrchr(absolute_filename, '/', error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_win32_get_base_path",
		      "Call to \"s_strrchr\" failed"))
		return NULL;

	/* win32 */
	{
		const char *p2;
		p2 = s_strrchr(absolute_filename, '\\', error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "s_win32_get_base_path",
			      "Call to \"s_strrchr\" failed"))
			return NULL;

		if (!ptr || p2 > ptr)
			ptr = p2;
	}

	if (ptr != NULL)
		ptr++;  /* add '/' or '\\' to end of base_path */

	base_path = s_sbefore(absolute_filename, ptr, error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_win32_get_base_path",
		      "Call to \"s_sbefore\" failed"))
		return NULL;


	return base_path;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_bool s_win32_path_is_absolute(const char *path, s_erc *error)
{
	const char *p;


	S_CLR_ERR(error);

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_win32_path_is_absolute",
			  "Argument \"path\" is NULL");
		return FALSE;
	}

	/* specific case for names like: "\\.\d:" */
	if (*path == '/' || *path == '\\')
		return TRUE;

	p = s_strchr(path, ':', error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_win32_path_is_absolute",
		      "Call to \"s_strchr\" failed"))
		return FALSE;

	if (p)
		p++;
	else
		p = path;

	if (*p == '/' || *p == '\\')
		return TRUE;

	return FALSE;
}
