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
/* AUTHOR  : Cristian Andrighetto                                                   */
/*           based on Aby Louw work.                                                */
/* DATE    : 13 December 2016                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A generic data source class.                                                     */
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
#include "datasources/generic_source.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given #SGenericsource parent class object as a #SGenericsource object.
 * @param SELF The given object.
 * @return Given object as #SGenericsource* type.
 * @note This casting is not safety checked.
 */
#define S_GENERICSOURCE(SELF)    ((SGenericsource *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SGenericsourceClass GenericsourceClass; /* Genericsource class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API SDatasource *SGenericsourceOpen(void* ptr, const char *mode, SIOFunctions* io_functs, s_erc *error)
{

	SGenericsource *self;


	S_CLR_ERR(error);

	/* check if mode parameter is correct */
	if (mode == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SGenericsourceOpen",
				  "Argument \"mode\" is NULL");
		return NULL;
	}

	if (io_functs == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			"SGenericsourceOpen",
			"Argument \"io_functs\" is NULL");
		return NULL;
	}

	self = S_NEW(SGenericsource, error);

	if (S_CHK_ERR(error, S_CONTERR,
		"SGenericsourceOpen",
		"Failed to create new object")) {
		io_functs->close(ptr);
		return NULL;
	}
	self->ptr = ptr;
	self->io_functs = io_functs;

	return S_DATASOURCE(self);
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_generic_source_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&GenericsourceClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "s_generic_source_class_add",
			  "Failed to add SGenericsourceClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitGenericsource(void *obj, s_erc *error)
{
	SGenericsource *self = obj;


	S_CLR_ERR(error);

	self->ptr = NULL;
	self->io_functs = NULL;
}


static void DestroyGenericsource(void *obj, s_erc *error)
{
	SGenericsource *self = obj;


	S_CLR_ERR(error);

	if (self->ptr != NULL)
	{
		int rv;
		rv = self->io_functs->close(self->ptr);

		if (rv != 0)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "DestroyGenericsource",
					  "Failed to close source");
		}
	}

}


static void DisposeGenericsource(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}

static size_t GenericRead(SDatasource *ds, void *buf, size_t size,
	size_t nmemb, s_erc *error) {

	SGenericsource *self = S_GENERICSOURCE(ds);
	void *ptr = self->ptr;
	ssize_t rv;


	S_CLR_ERR(error);
	rv = self->io_functs->read(ptr, buf, size*nmemb) * size;

	if (rv < (ssize_t)(size))
	{
		if (rv == -1 )
		{
			S_CTX_ERR(error, S_FAILURE,
				"GenericRead",
				"Cannot read memory" );
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
				"GenericRead",
				"Readed only %d bytes", rv);
		}
	}

	return rv;
}

static size_t GenericWrite(SDatasource *ds, const void *buf, size_t size,
	size_t nmemb, s_erc *error) {

	SGenericsource *self = S_GENERICSOURCE(ds);
	void* ptr = self->ptr;
	ssize_t rv;


	S_CLR_ERR(error);
	rv = self->io_functs->write(ptr, buf, size*nmemb)* size;

	if (rv < (ssize_t)(size))
	{
		if (rv == -1)
		{
			S_CTX_ERR(error, S_FAILURE,
				"GenericWrite",
				"Cannot read memory");
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
				"GenericWrite",
				"Written only %d bytes", rv);
		}
	}

	return rv;

}

static void GenericSeek(SDatasource *ds, long pos,
	s_seek_mode mode, s_erc *error) {

	SGenericsource *self = S_GENERICSOURCE(ds);
	int rv;

	S_CLR_ERR(error);
	rv = self->io_functs->seek(self->ptr,
		(ptrdiff_t *)&pos,
		(mode == S_SEEK_SET) ? SEEK_SET :
		(mode == S_SEEK_CUR) ? SEEK_CUR :
		SEEK_END);

	if (rv == -1)
	{
		S_CTX_ERR(error, S_FAILURE,
			"GenericSeek",
			"Failed to seek to given memory position of memory area");
	}
}

/************************************************************************************/
/*                                                                                  */
/* SGenericsource class initialization                                              */
/*                                                                                  */
/************************************************************************************/

/*
 * This is exactly the same as SDatasourceClass.
 */
static SGenericsourceClass GenericsourceClass =
{
	/* SObjectClass */
	{
		"SDatasource:SGenericsource",
		sizeof(SGenericsource),
		{ 0, 1},
		InitGenericsource,    /* init    */
		DestroyGenericsource, /* destroy */
		DisposeGenericsource, /* dispose */
		NULL,                 /* compare */
		NULL,                 /* print   */
		NULL,                 /* copy    */
	},
	/* SGenericsourceClass */
	GenericRead,                   /* read           */
	NULL,                          /* read_at        */
	GenericWrite,                  /* write          */
	NULL,                          /* write_at       */
	NULL,                          /* tell           */
	GenericSeek                    /* seek           */
};
