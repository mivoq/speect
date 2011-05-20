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
/* Buffer with exponential growth. The buffer ensures that it is always null padded.*/
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

#include <string.h>
#include "base/utils/types.h"
#include "base/utils/alloc.h"
#include "base/containers/buffer/buffer.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

#define S_BUF_INIT_SIZE 2048


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/*
 * Definition of the buffer.
 */
struct s_buffer
{
    size_t   len;    /* The length of the buffer.      */
    size_t   used;   /* The used length of the buffer. */
    uchar   *data;   /* Buffer data.                   */
};


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void s_buffer_ensure_available(s_buffer *buf, size_t want, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_buffer *s_buffer_new(s_erc *error)
{
	s_buffer *self;


	S_CLR_ERR(error);
	self = S_MALLOC(s_buffer, 1);

	if (self == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_buffer_new",
				  "Failed to allocate memory for object");
		return NULL;
	}

	self->data = NULL;
	self->len = 0;
	self->used = 0;

	return self;
}


S_API void s_buffer_delete(s_buffer *buf, s_erc *error)
{
	S_CLR_ERR(error);

	if (buf != NULL)
	{
		if (buf->data != NULL)
			S_FREE(buf->data);
		S_FREE(buf);
	}
}


S_API void s_buffer_append(s_buffer *buf, const void *data,
						   size_t size, s_erc *error)
{
	S_CLR_ERR(error);

	if ((buf == NULL) || (data == NULL))
		return;

	s_buffer_ensure_available(buf, size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_buffer_append",
				  "Call to \"s_buffer_ensure_available\" failed"))
		return;

	if (size > 0)
	{
		memcpy(buf->data + buf->used, data, size);
		buf->used += size;
		buf->data[buf->used] = 0;
	}
}


S_API void s_buffer_clear(s_buffer *buf, s_erc *error)
{
	S_CLR_ERR(error);

	if (buf == NULL)
		return;

	buf->used = 0;

	if (buf->data)
		buf->data[buf->used] = 0;
}


S_API const uchar *s_buffer_data(s_buffer *buf, s_erc *error)
{
	S_CLR_ERR(error);

	if (buf == NULL)
		return NULL;

	return buf->data;
}


S_API size_t s_buffer_size(s_buffer *buf, s_erc *error)
{
	S_CLR_ERR(error);

	if (buf == NULL)
		return 0;

	return buf->used;
}


S_API void s_buffer_truncate(s_buffer *buf, size_t len, s_erc *error)
{
	S_CLR_ERR(error);

	if (buf == NULL)
		return;

	if (len > buf->used)
		return;

	buf->used = len;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void s_buffer_ensure_available(s_buffer *buf, size_t want, s_erc *error)
{
    size_t need;


    S_CLR_ERR(error);

    /* first call */
    if (buf->data == NULL)
    {
        buf->len = S_BUF_INIT_SIZE;
        buf->data = S_MALLOC(uchar, buf->len);

		if (buf->data == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "s_buffer_ensure_available",
					  "Failed to allocate memory for object");
			return;
		}

        buf->data[0] = 0;
    }

    need = buf->len;

    while (want >= (need - buf->used))
	    need <<= 1;

    if (need != buf->len)
    {
	    buf->data = S_REALLOC(buf->data, uchar, need);

	    if (buf->data == NULL)
	    {
		    S_FTL_ERR(error, S_MEMERROR,
					  "s_buffer_ensure_available",
					  "Failed to allocate memory for object");
		    return;
	    }

	    buf->len = need;
    }
}
