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
/* POSIX file system paths.                                                         */
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
#include "base/utils/platform/posix/posix_path.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_bool s_posix_path_is_absolute(const char *path, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/* if filename is absolute, just copy it to dest. Otherwise, build a
   path to it by considering it is relative to base_path. URL are
   supported. */
S_LOCAL char *s_posix_path_combine(const char *base_path, const char *filename, s_erc *error)
{
	const char *p;
	const char *p1;
	size_t len;
	s_bool is_absolute;
	char *combined_path = NULL;
	char *tmp;

	if(filename == NULL)
	{
		filename = "";
	}
	S_CLR_ERR(error);
	if( (base_path == NULL) || (*base_path == '\0') )
	{
		/* if base path is empty, consider only filename */
		is_absolute = TRUE;
	} else {
		is_absolute = s_posix_path_is_absolute(filename, error);
		if (S_CHK_ERR(error, S_CONTERR,
		              "s_posix_path_combine",
		              "Call to \"s_posix_path_is_absolute\" failed"))
			return NULL;
	}
	if (is_absolute)
	{
		combined_path = s_strdup(filename, error);
		if (S_CHK_ERR(error, S_CONTERR,
			      "s_posix_path_combine",
			      "Call to \"s_strdup\" failed"))
			return NULL;

		return combined_path;
	}

	/* sanity, check that path does end with '/' */
	len = strlen(base_path);
	while(len > 0 && base_path[len - 1] == '/') {
		len--;
	}

	p = s_strchr(base_path, ':', error);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_posix_path_combine",
		      "Call to \"s_strchr\" failed"))
		return NULL;

	if (p)
		p++;
	else
		p = base_path;

	p1 = base_path+len;
	if (p1 > p)
		p = p1;

	len = p - base_path;
	len++;
	tmp = S_CALLOC(char, len + 1);
	if (tmp == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
			  "s_posix_path_combine",
			  "Failed to allocate memory for 'char' object");
		return NULL;
	}
	memcpy(tmp, base_path, len-1);
	tmp[len-1] = '/';
	tmp[len] = '\0';

	s_asprintf(&combined_path, error, "%s%s", tmp, filename);
	S_FREE(tmp);
	if (S_CHK_ERR(error, S_CONTERR,
		      "s_posix_path_combine",
		      "Call to \"s_asprintf\" failed"))
	{
		S_FREE(combined_path);
		return NULL;
	}

	return combined_path;
}

static char *s_strdupupto(const char *src, const char *to, s_erc *error)
{
        char *s;
        size_t size;


        S_CLR_ERR(error);

        if (src == NULL)
                return NULL;

        size = to-src+1;

        s = S_MALLOC(char, size+1);

        if (s == NULL)
        {
                S_FTL_ERR(error, S_MEMERROR,
                                  "s_strdupupto",
                                  "Failed to allocate memory to new string");
        }
        else
        {
                s_strzcpy(s, src, size, error);

                if (S_CHK_ERR(error, S_CONTERR,
                                          "s_strdupupto",
                                          "String copy error"))
                {
                        S_FREE(s);
                        return NULL;
                }
        }
        s[size] = '\0';

        return s;
}

/* get the base path of the given path and file name */
S_LOCAL char *s_posix_get_base_path(const char *path, s_erc *error)
{
	char *base_path;
	const char *ptr;
	ptr = NULL;
	S_CLR_ERR(error);
	if(path != NULL) {
		ptr = s_strrchr(path, '/', error);
		if (S_CHK_ERR(error, S_CONTERR,
		              "s_posix_get_base_path",
		              "Call to \"s_strrchr\" failed"))
			return NULL;
		if (ptr != NULL)
		{
			if(ptr[1] == 0)
			{
				while((ptr > path) && (*ptr == '/'))
				{
					ptr--;
				}
				if(ptr > path) {
					while((ptr > path) && (*ptr != '/'))
					{
						ptr--;
					}
					if((*ptr != '/') && (ptr == path)) {
						path=".";
						ptr=path;
					} else {
						while((ptr > path) && (*ptr == '/'))
						{
							ptr--;
						}
					}
				}
			} else {
				while((ptr > path) && (*ptr == '/'))
				{
					ptr--;
				}
			}
			if(*ptr != '/') {
				ptr++;
			}
			if(ptr == path) {
				ptr++;
			}
		} else {
			path=".";
			ptr=path;
		}
	} else {
		path=".";
		ptr=path;
	}
	if(ptr == path) {
		ptr++;
	}

	base_path = s_strdupupto(path, ptr, error);
	if (S_CHK_ERR(error, S_CONTERR,
	              "s_posix_get_base_path",
	              "Call to \"s_sbefore\" failed"))
		return NULL;


	return base_path;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/
static s_bool is_protocol_name_character(uint32 c)
{
	return (((c <= 'z') && (c >= 'a')) || (c == '+') || (c == '-') || (c == '_') || ((c <= 'Z') && (c >= 'A')) || ((c <= '9') && (c >= '0')));
}

static s_bool s_posix_path_is_absolute(const char *path, s_erc *error)
{
	const char *p;


	S_CLR_ERR(error);

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_posix_path_is_absolute",
			  "Argument \"path\" is NULL");
		return FALSE;
	}

	p = path;
	while(is_protocol_name_character(*p))
	{
		p++;
	}
	if(*p == ':')
	{
		p++;
	}
	else
	{
		p = path;
	}

	if (*p == '/')
		return TRUE;

	return FALSE;
}
