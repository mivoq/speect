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
/* DATE    : 17 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Float I/O routines.                                                              */
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

#include "base/utils/byteswap.h"
#include "datasources/load_real.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API float s_float_read(SDatasource *ds, s_erc *error)
{
	float f;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_float_read",
			  "Argument \"ds\" is NULL");
		return 0.0;
	}

	SDatasourceRead(ds, &f, sizeof(float), 1, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_float_read",
		      "Failed to read float from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		return s_swap_be_flt(f);

	return s_swap_le_flt(f);
}


S_API float s_float_read_at(SDatasource *ds, long pos, s_erc *error)
{
	float f;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_float_read_at",
			  "Argument \"ds\" is NULL");
		return 0.0;
	}

	SDatasourceReadAt(ds, &f, sizeof(float), 1, pos, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_float_read_at",
		      "Failed to read float from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		return s_swap_be_flt(f);

	return s_swap_le_flt(f);
}


S_API void s_float_write(SDatasource *ds, float f, s_erc *error)
{
	float fv;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_float_write",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		fv = s_swap_be_flt(f);
	else
		fv = s_swap_le_flt(f);

	SDatasourceWrite(ds, &fv, sizeof(float), 1, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_float_write",
		  "Failed to write float to data source");
}


S_API void s_float_write_at(SDatasource *ds, float f, long pos, s_erc *error)
{
	float fv;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_float_write_at",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		fv = s_swap_be_flt(f);
	else
		fv = s_swap_le_flt(f);

	SDatasourceWriteAt(ds, &fv, sizeof(float), 1, pos, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_float_write_at",
		  "Failed to write float to data source");
}


S_API double s_double_read(SDatasource *ds, s_erc *error)
{
	double d;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_double_read",
			  "Argument \"ds\" is NULL");
		return 0.0;
	}

	SDatasourceRead(ds, &d, sizeof(double), 1, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_double_read",
		      "Failed to read double from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		return s_swap_be_dbl(d);

	return s_swap_le_dbl(d);
}


S_API double s_double_read_at(SDatasource *ds, long pos, s_erc *error)
{
	double d;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_double_read_at",
			  "Argument \"ds\" is NULL");
		return 0.0;
	}

	SDatasourceReadAt(ds, &d, sizeof(double), 1, pos, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_double_read_at",
		      "Failed to read double from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		return s_swap_be_dbl(d);

	return s_swap_le_dbl(d);
}


S_API void s_double_write(SDatasource *ds, double d, s_erc *error)
{
	double dv;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_double_write",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		dv = s_swap_be_dbl(d);
	else
		dv = s_swap_le_dbl(d);

	SDatasourceWrite(ds, &dv, sizeof(double), 1, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_double_write",
		  "Failed to write double to data source");
}


S_API void s_double_write_at(SDatasource *ds, double d, long pos, s_erc *error)
{
	double dv;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_double_write_at",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		dv = s_swap_be_dbl(d);
	else
		dv = s_swap_le_dbl(d);

	SDatasourceWriteAt(ds, &dv, sizeof(double), 1, pos, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_double_write_at",
		  "Failed to write double to data source");
}


#ifdef SPCT_LONG_DOUBLE
S_API long double s_ldouble_read(SDatasource *ds, s_erc *error)
{
	long double ld;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_ldouble_read",
			  "Argument \"ds\" is NULL");
		return 0.0;
	}

	SDatasourceRead(ds, &ld, sizeof(double), 1, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_ldouble_read",
		      "Failed to read double from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		return s_swap_be_ldbl(ld);

	return s_swap_le_ldbl(ld);
}


S_API long double s_ldouble_read_at(SDatasource *ds, long pos, s_erc *error)
{
	long double ld;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_ldouble_read_at",
			  "Argument \"ds\" is NULL");
		return 0.0;
	}

	SDatasourceReadAt(ds, &ld, sizeof(double), 1, pos, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_ldouble_read_at",
		      "Failed to read double from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		return s_swap_be_ldbl(ld);

	return s_swap_le_ldbl(ld);
}


S_API void s_ldouble_write(SDatasource *ds, long double ld, s_erc *error)
{
	long double ldv;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_ldouble_write",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		ldv = s_swap_be_ldbl(ld);
	else
		ldv = s_swap_le_ldbl(ld);

	SDatasourceWrite(ds, &ldv, sizeof(double), 1, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_ldouble_write",
		  "Failed to write double to data source");
}


S_API void s_ldouble_write_at(SDatasource *ds, long double ld, long pos, s_erc *error)
{
	long double ldv;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_ldouble_write_at",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		ldv = s_swap_be_dbl(ld);
	else
		ldv = s_swap_le_dbl(ld);

	SDatasourceWriteAt(ds, &ldv, sizeof(double), 1, pos, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_ldouble_write_at",
		  "Failed to write double to data source");
}
#endif /* SPCT_LONG_DOUBLE */
