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
/* DATE    : 23 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A memory mapped file data source class.                                          */
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

#include <string.h>
#include "base/strings/strings.h"
#include "base/utils/alloc.h"
#include "datasources/mmapfile_source.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given #SMMapFilesource parent class object as a #SMMapFilesource object.
 * @param SELF The given object.
 * @return Given object as #SMMapFilesource* type.
 * @note This casting is not safety checked.
 */
#define S_MMAPFILESOURCE(SELF)    ((SMMapFilesource *)(SELF))



/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SMMapFilesourceClass MMapFilesourceClass; /* MMapFilesourc class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API SDatasource *SMMapFilesourceOpenFile(const char *path, s_erc *error)
{
	SMMapFilesource *self;
	uint8 *mem;
	size_t map_size;
	s_mmap_file_handle *handle;


	S_CLR_ERR(error);

	self = (SMMapFilesource*)S_NEW("SMMapFilesource", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMMapFilesourceOpenFile",
				  "Failed to create new object"))
		return NULL;

	self->path = s_strdup(path, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SMMapFilesourceOpenFile",
				  "Failed to duplicate file path"))
	{
		S_DELETE(self, "SMMapFilesourceOpenFile", error);
		return NULL;
	}

	handle = s_mmapfile_open(path, &map_size, &mem, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMMapFilesourceOpenFile"
				  "Call to \"s_mmapfile_open\" failed for file \"%s\"",
				  path))
	{
		S_DELETE(self, "SMMapFilesourceOpenFile", error);
		return NULL;
	}

	self->handle = handle;
	self->mem = mem;
	self->map_size = map_size;

	return S_DATASOURCE(self);
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_mmap_file_source_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&MMapFilesourceClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_mmap_file_source_class_add",
			  "Failed to add SMMapFilesourceClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitMMapFilesource(void *obj, s_erc *error)
{
	SMMapFilesource *self = S_MMAPFILESOURCE(obj);


	S_CLR_ERR(error);
	self->path = NULL;
	self->mem = NULL;
	self->map_size = 0;
}


static void DestroyMMapFilesource(void *obj, s_erc *error)
{
	SMMapFilesource *self = S_MMAPFILESOURCE(obj);


	S_CLR_ERR(error);

	s_mmapfile_close(self->handle, self->mem, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Call to \"s_mmapfile_close\" failed for file \"%s\"",
			  self->path? self->path : "NULL");

	if (self->path != NULL)
		S_FREE(self->path);
}


static void DisposeMMapFilesource(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static size_t MMapRead(SDatasource *ds, void *buf, size_t msize,
					   size_t nmemb, s_erc *error)
{
	SMMapFilesource *self = S_MMAPFILESOURCE(ds);
	size_t size = msize*nmemb;
	size_t rv;


	S_CLR_ERR(error);
	memcpy(buf, &self->mem[self->offset], size); /* need boundry error checking */

	rv = size;
	self->offset += size;

	return rv;
}


static size_t MMapReadAt(SDatasource *ds, void *buf, size_t msize,
						 size_t nmemb, long pos, s_erc *error)
{
	SMMapFilesource *self = S_MMAPFILESOURCE(ds);
	size_t size = msize*nmemb;
	size_t rv;


	S_CLR_ERR(error);

	if (pos < 0)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "MMapReadAt",
				  "MMapReadAt bad position %d.\n", pos);
		return 0;
	}

	memcpy(buf, &self->mem[pos], size); /* need boundry error checking */
	rv = size;

	return rv;
}


static long MMapTell(SDatasource *ds, s_erc *error)
{
	S_CLR_ERR(error);

	return (S_MMAPFILESOURCE(ds)->offset);
}


static void MMapSeek(SDatasource *ds, long offs, s_seek_mode mode,
					 s_erc *error)
{
	SMMapFilesource *self = S_MMAPFILESOURCE(ds);
	long nOffs;


	S_CLR_ERR(error);

	/* can we seek to end, what is the size? */
	switch (mode)
	{
	case S_SEEK_SET:
		nOffs = offs;
		break;
	case S_SEEK_CUR:
	default:
		nOffs = self->offset + offs;
		break;
	}

	self->offset = nOffs;
}


/************************************************************************************/
/*                                                                                  */
/* SMMapFilesource class initialization                                             */
/*                                                                                  */
/************************************************************************************/

/*
 * This is exactly the same as SDatasourceClass.
 */
static SMMapFilesourceClass MMapFilesourceClass =
{
	/* SObjectClass */
	{
		"SDatasource:SMMapFilesource",
		sizeof(SMMapFilesource),
		{ 0, 1},
		InitMMapFilesource,    /* init    */
		DestroyMMapFilesource, /* destroy */
		DisposeMMapFilesource, /* dispose */
		NULL,                  /* compare */
		NULL,                  /* print   */
		NULL,                  /* copy    */
	},
	/* SMMapFilesourceClass */
	MMapRead,                            /* read           */
	MMapReadAt,                          /* read_at        */
	NULL,                                /* write          */
	NULL,                                /* write_at       */
	MMapTell,                            /* tell           */
	MMapSeek                             /* seek           */
};
