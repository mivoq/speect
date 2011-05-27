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
/* DATE    : 10 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* An abstract data source class.                                                   */
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

#include "datasources/data_source.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #SDatasource.
 * @param SELF The given #SDatasource*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage:
 @verbatim
 S_DATASOURCE_CALL(self, func)(param1, param2, ..., paramN);
 @endverbatim
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_DATASOURCE_CALL(SELF, FUNC)				\
	((SDatasourceClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SDatasource
 * can be called.
 * @param SELF The given #SDatasource*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_DATASOURCE_METH_VALID(SELF, FUNC)			\
	S_DATASOURCE_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SDatasourceClass DatasourceClass; /* Data source class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SDatasourceSetByteOrder(SDatasource *self, s_byte_order order, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SDatasourceSetByteOrder",
			  "Argument \"self\" is NULL");
		return;
	}

	s_mutex_lock(&(self->data_mutex));
	self->byte_order = order;
	s_mutex_unlock(&(self->data_mutex));
}


S_API size_t SDatasourceRead(SDatasource *self, void *ptr, size_t size,
		       size_t nmemb, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SDatasourceRead",
			  "Argument \"self\" is NULL");
		return 0;
	}

	if (ptr == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SDatasourceRead",
			  "Argument \"ptr\" is NULL");
		return 0;
	}

	if (!S_DATASOURCE_METH_VALID(self, read))
	{
		S_WARNING(S_METHINVLD,
			  "SDatasourceRead",
			  "Datasource method \"read\" not implemented");
		return 0;
	}

	s_mutex_lock(&(self->data_mutex));
	self->rd_last = S_DATASOURCE_CALL(self, read)(self, ptr, size, nmemb, error);

	/* caller must decide if end-of-file is an error */
	if ((error != NULL) && (*error == S_IOEOF))
	{
		self->rd_total += self->rd_last;
		s_mutex_unlock(&(self->data_mutex));

		return self->rd_last;
	}

	if (S_CHK_ERR(error, S_CONTERR,
		      "SDatasourceRead",
		      "Failed to read from data source"))
	{
		s_mutex_unlock(&(self->data_mutex));
		return 0;
	}

	self->rd_total += self->rd_last;
	s_mutex_unlock(&(self->data_mutex));

	return self->rd_last;
}


S_API size_t SDatasourceReadAt(SDatasource *self, void *ptr, size_t size,
			 size_t nmemb, long pos, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SDatasourceReadAt",
			  "Argument \"self\" is NULL");
		return 0;
	}

	if (ptr == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SDatasourceReadAt",
			  "Argument \"ptr\" is NULL");
		return 0;
	}

	if (!S_DATASOURCE_METH_VALID(self, read_at))
	{
		S_WARNING(S_METHINVLD,
			  "SDatasourceReadAt",
			  "Datasource method \"read_at\" not implemented");
		return 0;
	}

	s_mutex_lock(&(self->data_mutex));
	self->rd_last = S_DATASOURCE_CALL(self, read_at)(self, ptr, size, nmemb, pos, error);

	/* caller must decide if end-of-file is an error */
	if ((error != NULL) && (*error == S_IOEOF))
	{
		self->rd_total += self->rd_last;
		s_mutex_unlock(&(self->data_mutex));

		return self->rd_last;
	}

	if (S_CHK_ERR(error, S_CONTERR,
		      "SDatasourceReadAt",
		      "Failed to read from data source"))
	{
		s_mutex_unlock(&(self->data_mutex));
		return 0;
	}

	self->rd_total += self->rd_last;
	s_mutex_unlock(&(self->data_mutex));


	return self->rd_last;
}


S_API size_t SDatasourceWrite(SDatasource *self, const void *ptr, size_t size,
			size_t nmemb, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SDatasourceWrite",
			  "Argument \"self\" is NULL");
		return 0;
	}

	if (ptr == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SDatasourceWrite",
			  "Argument \"ptr\" is NULL");
		return 0;
	}

	if (!S_DATASOURCE_METH_VALID(self, write))
	{
		S_WARNING(S_METHINVLD,
			  "SDatasourceWrite",
			  "Datasource method \"write\" not implemented");
		return 0;
	}

	s_mutex_lock(&(self->data_mutex));
	self->wr_last = S_DATASOURCE_CALL(self, write)(self, ptr, size, nmemb, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "SDatasourceWrite",
		      "Failed to write to data source"))
	{
		s_mutex_unlock(&(self->data_mutex));
		return 0;
	}

	self->wr_total += self->wr_last;
	s_mutex_unlock(&(self->data_mutex));

	return self->wr_last;
}


S_API size_t SDatasourceWriteAt(SDatasource *self, const void *ptr, size_t size,
			  size_t nmemb, long pos, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SDatasourceWriteAt",
			  "Argument \"self\" is NULL");
		return 0;
	}

	if (ptr == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SDatasourceWriteAt",
			  "Argument \"ptr\" is NULL");
		return 0;
	}

	if (!S_DATASOURCE_METH_VALID(self, write_at))
	{
		S_WARNING(S_METHINVLD,
			  "SDatasourceWriteAt",
			  "Datasource method \"write_at\" not implemented");
		return 0;
	}

	s_mutex_lock(&(self->data_mutex));
	self->wr_last = S_DATASOURCE_CALL(self, write_at)(self, ptr, size, nmemb, pos, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "SDatasourceWriteAt",
		      "Failed to write to data source"))
	{
		s_mutex_unlock(&(self->data_mutex));
		return 0;
	}

	self->wr_total += self->wr_last;
	s_mutex_unlock(&(self->data_mutex));

	return self->wr_last;
}


S_API long SDatasourceTell(SDatasource *self, s_erc *error)
{
	off_t pos;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SDatasourceTell",
			  "Argument \"self\" is NULL");
		return 0;
	}

	if (!S_DATASOURCE_METH_VALID(self, tell))
	{
		S_WARNING(S_METHINVLD,
			  "SDatasourceTell",
			  "Datasource method \"tell\" not implemented");
		return -1;
	}

	s_mutex_lock(&(self->data_mutex));
	pos = S_DATASOURCE_CALL(self, tell)(self, error);
	s_mutex_unlock(&(self->data_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
		      "SDatasourceTell",
		      "Failed to determine current position in the data source"))
		return 0;

	return pos;
}


S_API void SDatasourceSeek(SDatasource *self, long pos, s_seek_mode mode, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "SDatasourceSeek",
			  "Argument \"self\" is NULL");
		return;
	}

	if (!S_DATASOURCE_METH_VALID(self, seek))
	{
		S_WARNING(S_METHINVLD,
			  "SDatasourceSeek",
			  "Datasource method \"seek\" not implemented");
		return;
	}

	s_mutex_lock(&(self->data_mutex));
	S_DATASOURCE_CALL(self, seek)(self, pos, mode, error);
	s_mutex_unlock(&(self->data_mutex));


	S_CHK_ERR(error, S_CONTERR,
		  "SDatasourceSeek",
		  "Failed to changed data source position");
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_data_source_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&DatasourceClass), error);
	S_CHK_ERR(error, S_CONTERR,
		  "_s_data_source_class_add",
		  "Failed to add SDatasourceClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitDatasource(void *obj, s_erc *error)
{
	SDatasource *self = obj;


	S_CLR_ERR(error);

	self->byte_order = S_BYTEORDER_BE;
	self->rd_last = 0;
	self->wr_last = 0;
	self->rd_total = 0;
	self->wr_total = 0;

	s_mutex_init(&(self->data_mutex));
}


/*
 * We only destroy the mutex if SPCT_USE_THREADS
 * is defined, else there is no need for this
 * destructor.
 */
#ifdef SPCT_USE_THREADS
static void DestroyDatasource(void *obj, s_erc *error)
{
	SDatasource *self = obj;


	S_CLR_ERR(error);

	s_mutex_lock(&(self->data_mutex));
	s_mutex_unlock(&(self->data_mutex));
	s_mutex_destroy(&(self->data_mutex));
}
#endif /* SPCT_USE_THREADS */


static void DisposeDatasource(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SDatasource class initialization                                                 */
/*                                                                                  */
/************************************************************************************/

static SDatasourceClass DatasourceClass =
{
	/* SObjectClass */
	{
		"SDatasource",
		sizeof(SDatasource),
		{ 0, 1},
		InitDatasource,    /* init    */
#ifdef SPCT_USE_THREADS
		DestroyDatasource, /* destroy */
#else /* !SPCT_USE_THREADS */
		NULL,              /* destroy */
#endif /* SPCT_USE_THREADS */
		DisposeDatasource, /* dispose */
		NULL,              /* compare */
		NULL,              /* print   */
		NULL,              /* copy    */
	},
	/* SDatasourceClass */
	NULL,                              /* read           */
	NULL,                              /* read_at        */
	NULL,                              /* write          */
	NULL,                              /* write_at       */
	NULL,                              /* tell           */
	NULL                               /* seek           */
};


