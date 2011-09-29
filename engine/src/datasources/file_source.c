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
/* DATE    : 20 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A file data source class.                                                        */
/*                                                                                  */
/* Adapted from Agar toolkit http://libagar.org/ (1.3.2)                            */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
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

#include <errno.h>
#include "base/strings/strings.h"
#include "base/utils/alloc.h"
#include "datasources/file_source.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given #SFilesource parent class object as a #SFilesource object.
 * @param SELF The given object.
 * @return Given object as #SFilesource* type.
 * @note This casting is not safety checked.
 */
#define S_FILESOURCE(SELF)    ((SFilesource *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SFilesourceClass FilesourceClass; /* MMapFilesourc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API SDatasource *SFilesourceOpenHandle(FILE *f, s_erc *error)
{
	SFilesource *self;


	S_CLR_ERR(error);

	if (f == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SFilesourceOpenHandle",
				  "Argument \"f\" is NULL");
		return NULL;
	}

	self = S_NEW(SFilesource, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SFilesourceOpenHandle",
				  "Failed to create new object"))
		return NULL;

	self->file = f;

	return S_DATASOURCE(self);
}


S_API SDatasource *SFilesourceOpenFile(const char *path, const char *mode, s_erc *error)
{
	FILE *f;
	SFilesource *self;


	S_CLR_ERR(error);

	if (path == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SFileSourceOpenFile",
				  "Argument \"path\" is NULL");
		return NULL;
	}

	if (mode == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SFileSourceOpenFile",
				  "Argument \"mode\" is NULL");
		return NULL;
	}

	if ((f = fopen(path, mode)) == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SFileSourceOpenFile",
				  "Failed to open file \"%s\", reported error \"%s\".",
				  path, s_strerror(errno));
		return NULL;
	}

	self = S_NEW(SFilesource, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SFileSourceOpenFile",
				  "Failed to create new object"))
	{
		fclose(f);
		return NULL;
	}

	self->path = s_strdup(path, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SFileSourceOpenFile",
				  "Failed to duplicate file path"))
	{
		S_DELETE(self, "SFileSourceOpenFile", error);
		return NULL;
	}

	self->file = f;

	return S_DATASOURCE(self);
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_file_source_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&FilesourceClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_file_source_class_add",
			  "Failed to add SFilesourceClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitFilesource(void *obj, s_erc *error)
{
	SFilesource *self = obj;


	S_CLR_ERR(error);

	self->path = NULL;
	self->file = NULL;
}


static void DestroyFilesource(void *obj, s_erc *error)
{
	SFilesource *self = obj;


	S_CLR_ERR(error);

	if (self->file != NULL)
	{
		int rv;
		rv = fclose(self->file);

		if (rv != 0)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "DestroyFilesource",
					  "Failed to close file source");
		}
	}

	if (self->path != NULL)
		S_FREE(self->path);
}


static void DisposeFilesource(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static size_t FileRead(SDatasource *ds, void *buf, size_t size,
					   size_t nmemb, s_erc *error)
{
	SFilesource *self = S_FILESOURCE(ds);
	FILE *f = self->file;
	size_t rv;


	S_CLR_ERR(error);
	clearerr(f);
	rv = fread(buf, size, nmemb, f) * size;

	if (rv < (nmemb*size))
	{
		if (ferror(f))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "FileRead",
					  "Read error, file \"%s\"",
					  (self->path ? self->path : "file path unknown"));
		}
		else if (feof(f))
			S_NEW_ERR(error, S_IOEOF); /* caller must decide if this is an error */
	}

	return rv;
}


static size_t FileReadAt(SDatasource *ds, void *buf, size_t size,
						 size_t nmemb, long pos, s_erc *error)
{
	SFilesource *self = S_FILESOURCE(ds);
	FILE *f = self->file;
	size_t rv;
	long savedPos;


	S_CLR_ERR(error);
	savedPos = ftell(f);

	if (savedPos == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "FileReadAt",
				  "Failed to tell file position of file \"%s\", reported error \"%s\"",
				  (self->path ? self->path : "file path unknown"),
				  s_strerror(errno));
		return 0;
	}

	if (fseek(f, pos, SEEK_SET) == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "FileReadAt",
				  "Failed to seek to file position of file \"%s\", reported error \"%s\"",
				  (self->path ? self->path : "file path unknown"),
				  s_strerror(errno));
		return 0;
	}

	clearerr(f);
	rv = fread(buf, size, nmemb, f) * size;

	if (rv < (nmemb*size))
	{
		if (ferror(f))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "FileReadAt",
					  "Read error, file \"%s\"",
					  (self->path ? self->path : "file path unknown"));
		}
		else if (feof(f))
		{
			S_NEW_ERR(error, S_IOEOF); /* caller must decide if this is an error */
		}
	}

	if (fseek(f, savedPos, SEEK_SET) == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "FileReadAt",
				  "Failed to seek to original file position of file \"%s\", reported error \"%s\"",
				  (self->path ? self->path : "file path unknown"),
				  s_strerror(errno));
	}

	return rv;
}


static size_t FileWrite(SDatasource *ds, const void *buf,
						size_t size, size_t nmemb, s_erc *error)
{
	SFilesource *self = S_FILESOURCE(ds);
	FILE *f = self->file;
	size_t rv;


	S_CLR_ERR(error);
	clearerr(f);
	rv = fwrite(buf, size, nmemb, f) * size;

	if (rv < (nmemb*size))
	{
		if (ferror(f))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "FileWrite",
					  "Write error, file \"%s\"",
					  (self->path ? self->path : "file path unknown"));
		}
		else if (feof(f))
		{
			S_CTX_ERR(error, S_IOEOF,
					  "FileWrite",
					  "End of file during write, file \"%s\"",
					  (self->path ? self->path : "file path unknown"));
		}
	}

	return rv;
}


static size_t FileWriteAt(SDatasource *ds, const void *buf, size_t size,
						  size_t nmemb, long pos, s_erc *error)
{
	SFilesource *self = S_FILESOURCE(ds);
	FILE *f = self->file;
	size_t rv;
	long savedPos;


	S_CLR_ERR(error);
	savedPos = ftell(f);

	if (savedPos == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "FileWriteAt",
				  "Failed to tell file position of file \"%s\", reported error \"%s\"",
				  (self->path ? self->path : "file path unknown"),
				  s_strerror(errno));
		return 0;
	}

	if (fseek(f, pos, SEEK_SET) == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "FileWriteAt",
				  "Failed to seek to file position of file \"%s\", reported error \"%s\"",
				  (self->path ? self->path : "file path unknown"),
				  s_strerror(errno));
		return 0;
	}

	clearerr(f);
	rv = fwrite(buf, size, nmemb, f) * size;

	if (rv < (nmemb*size))
	{
		if (ferror(f))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "FileWriteAt",
					  "Write error, file \"%s\"",
					  (self->path ? self->path : "file path unknown"));
		}
		else if (feof(f))
		{
			S_CTX_ERR(error, S_IOEOF,
					  "FileWriteAt",
					  "End of file during write, file \"%s\"",
					  (self->path ? self->path : "file path unknown"));
		}
	}

	if (fseek(f, savedPos, SEEK_SET) == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "FileWriteAt",
				  "Failed to seek to original file position of file \"%s\", reported error \"%s\"",
				  (self->path ? self->path : "file path unknown"),
				  s_strerror(errno));
	}

	return rv;
}


static long FileTell(SDatasource *ds, s_erc *error)
{
	SFilesource *self = S_FILESOURCE(ds);
	long rv;


	S_CLR_ERR(error);
	rv = ftell(self->file);

	if (rv == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "FileTell",
				  "Failed to tell file position of file \"%s\", reported error \"%s\"",
				  (self->path ? self->path : "file path unknown"),
				  s_strerror(errno));
		return 0;
	}

	return rv;
}


static void FileSeek(SDatasource *ds, long pos,
					 s_seek_mode mode, s_erc *error)
{
	SFilesource *self = S_FILESOURCE(ds);
	int rv;

	S_CLR_ERR(error);
	rv = fseek(self->file, pos,
			   (mode == S_SEEK_SET) ? SEEK_SET :
			   (mode == S_SEEK_CUR) ? SEEK_CUR :
			   SEEK_END);

	if (rv == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "FileSeek",
				  "Failed to seek to given file position of file \"%s\", reported error \"%s\"",
				  (self->path ? self->path : "file path unknown"),
				  s_strerror(errno));
	}
}


/************************************************************************************/
/*                                                                                  */
/* SFilesource class initialization                                                 */
/*                                                                                  */
/************************************************************************************/

/*
 * This is exactly the same as SDatasourceClass.
 */
static SFilesourceClass FilesourceClass =
{
	/* SObjectClass */
	{
		"SDatasource:SFilesource",
		sizeof(SFilesource),
		{ 0, 1},
		InitFilesource,    /* init    */
		DestroyFilesource, /* destroy */
		DisposeFilesource, /* dispose */
		NULL,              /* compare */
		NULL,              /* print   */
		NULL,              /* copy    */
	},
	/* SFilesourceClass */
	FileRead,                            /* read           */
	FileReadAt,                          /* read_at        */
	FileWrite,                           /* write          */
	FileWriteAt,                         /* write_at       */
	FileTell,                            /* tell           */
	FileSeek                             /* seek           */
};
