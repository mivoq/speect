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
/* DATE    : 15 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Integer I/O routines.                                                            */
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

#include "datasources/load_int.h"
#include "base/utils/byteswap.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API uint8 s_uint8_read(SDatasource *ds, s_erc *error)
{
	uint8 i;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_uint8_read",
			  "Argument \"ds\" is NULL");
		return 0;
	}

	SDatasourceRead(ds, &i, sizeof(uint8), 1, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_uint8_read",
		      "Failed to read uint8 from data source"))
		return 0;

	return i;
}


S_API uint8 s_uint8_read_at(SDatasource *ds, long pos, s_erc *error)
{
	uint8 i;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_uint8_read_at",
			  "Argument \"ds\" is NULL");
		return 0;
	}

	SDatasourceReadAt(ds, &i, sizeof(uint8), 1, pos, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_uint8_read_at",
		      "Failed to read uint8 from data source"))
		return 0;

	return i;
}


S_API void s_uint8_write(SDatasource *ds, uint8 i, s_erc *error)
{
	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_uint8_write",
			  "Argument \"ds\" is NULL");
		return;
	}

	SDatasourceWrite(ds, &i, sizeof(uint8), 1, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_uint8_write",
		  "Failed to write uint8 to data source");
}


S_API void s_uint8_write_at(SDatasource *ds, uint8 i, long pos, s_erc *error)
{
	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_uint8_write_at",
			  "Argument \"ds\" is NULL");
		return;
	}

	SDatasourceWriteAt(ds, &i, sizeof(uint8), 1, pos, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_uint8_write_at",
		  "Failed to write uint8 to data source");
}


S_API uint16 s_uint16_read(SDatasource *ds, s_erc *error)
{
	uint16 i;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_uint16_read",
			  "Argument \"ds\" is NULL");
		return 0;
	}

	SDatasourceRead(ds, &i, sizeof(uint16), 1, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_uint16_read",
		      "Failed to read uint16 from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be16(i);
	else
		i = s_swap_le16(i);

	return i;
}


S_API uint16 s_uint16_read_at(SDatasource *ds, long pos, s_erc *error)
{
	uint16 i;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_uint16_read_at",
			  "Argument \"ds\" is NULL");
		return 0;
	}

	SDatasourceReadAt(ds, &i, sizeof(uint16), 1, pos, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_uint16_read_at",
		      "Failed to read uint16 from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be16(i);
	else
		i = s_swap_le16(i);

	return i;
}


S_API void s_uint16_write(SDatasource *ds, uint16 i, s_erc *error)
{
	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_uint16_write",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be16(i);
	else
		i = s_swap_le16(i);

	SDatasourceWrite(ds, &i, sizeof(uint16), 1, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_uint16_write",
		  "Failed to write uint16 to data source");
}


S_API void s_uint16_write_at(SDatasource *ds, uint16 i, long pos, s_erc *error)
{
	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_uint16_write_at",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be16(i);
	else
		i = s_swap_le16(i);

	SDatasourceWriteAt(ds, &i, sizeof(uint16), 1, pos, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_uint16_write_at",
		  "Failed to write uint16 to data source");
}


S_API uint32 s_uint32_read(SDatasource *ds, s_erc *error)
{
	uint32 i;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_uint32_read",
			  "Argument \"ds\" is NULL");
		return 0;
	}

	SDatasourceRead(ds, &i, sizeof(uint32), 1, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_uint32_read",
		      "Failed to read uint16 from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be32(i);
	else
		i = s_swap_le32(i);

	return i;
}


S_API uint32 s_uint32_read_at(SDatasource *ds, long pos, s_erc *error)
{
	uint32 i;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_uint32_read_at",
			  "Argument \"ds\" is NULL");
		return 0;
	}

	SDatasourceReadAt(ds, &i, sizeof(uint32), 1, pos, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_uint32_read_at",
		      "Failed to read uint32 from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be32(i);
	else
		i = s_swap_le32(i);

	return i;
}


S_API void s_uint32_write(SDatasource *ds, uint32 i, s_erc *error)
{
	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_uint32_write",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be32(i);
	else
		i = s_swap_le32(i);

	SDatasourceWrite(ds, &i, sizeof(uint32), 1, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_uint32_write",
		  "Failed to write uint32 to data source");
}


S_API void s_uint32_write_at(SDatasource *ds, uint32 i, long pos, s_erc *error)
{
	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_uint32_write_at",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be32(i);
	else
		i = s_swap_le32(i);

	SDatasourceWriteAt(ds, &i, sizeof(uint32), 1, pos, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_uint32_write_at",
		  "Failed to write uint32 to data source");
}


S_API sint8 s_sint8_read(SDatasource *ds, s_erc *error)
{
	sint8 i;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_sint8_read",
			  "Argument \"ds\" is NULL");
		return 0;
	}

	SDatasourceRead(ds, &i, sizeof(sint8), 1, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_sint8_read",
		      "Failed to read sint8 from data source"))
		return 0;

	return i;
}


S_API sint8 s_sint8_read_at(SDatasource *ds, long pos, s_erc *error)
{
	sint8 i;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_sint8_read_at",
			  "Argument \"ds\" is NULL");
		return 0;
	}

	SDatasourceReadAt(ds, &i, sizeof(sint8), 1, pos, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_sint8_read_at",
		      "Failed to read sint8 from data source"))
		return 0;

	return i;
}


S_API void s_sint8_write(SDatasource *ds, sint8 i, s_erc *error)
{
	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_sint8_write",
			  "Argument \"ds\" is NULL");
		return;
	}

	SDatasourceWrite(ds, &i, sizeof(sint8), 1, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_sint8_write",
		  "Failed to write sint8 to data source");
}


S_API void s_sint8_write_at(SDatasource *ds, sint8 i, long pos, s_erc *error)
{
	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_sint8_write_at",
			  "Argument \"ds\" is NULL");
		return;
	}

	SDatasourceWriteAt(ds, &i, sizeof(sint8), 1, pos, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_sint8_write_at",
		  "Failed to write sint8 to data source");
}


S_API sint16 s_sint16_read(SDatasource *ds, s_erc *error)
{
	sint16 i;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_sint16_read",
			  "Argument \"ds\" is NULL");
		return 0;
	}

	SDatasourceRead(ds, &i, sizeof(sint16), 1, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_sint16_read",
		      "Failed to read sint16 from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be16(i);
	else
		i = s_swap_le16(i);

	return i;
}


S_API sint16 s_sint16_read_at(SDatasource *ds, long pos, s_erc *error)
{
	sint16 i;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_sint16_read_at",
			  "Argument \"ds\" is NULL");
		return 0;
	}

	SDatasourceReadAt(ds, &i, sizeof(sint16), 1, pos, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_sint16_read_at",
		      "Failed to read sint16 from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be16(i);
	else
		i = s_swap_le16(i);

	return i;
}


S_API void s_sint16_write(SDatasource *ds, sint16 i, s_erc *error)
{
	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_sint16_write",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be16(i);
	else
		i = s_swap_le16(i);

	SDatasourceWrite(ds, &i, sizeof(sint16), 1, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_sint16_write",
		  "Failed to write sint16 to data source");
}


S_API void s_sint16_write_at(SDatasource *ds, sint16 i, long pos, s_erc *error)
{
	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_sint16_write_at",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be16(i);
	else
		i = s_swap_le16(i);

	SDatasourceWriteAt(ds, &i, sizeof(sint16), 1, pos, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_sint16_write_at",
		  "Failed to write sint16 to data source");
}


S_API sint32 s_sint32_read(SDatasource *ds, s_erc *error)
{
	sint32 i;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_sint32_read",
			  "Argument \"ds\" is NULL");
		return 0;
	}

	SDatasourceRead(ds, &i, sizeof(sint32), 1, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_sint32_read",
		      "Failed to read sint16 from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be32(i);
	else
		i = s_swap_le32(i);

	return i;
}


S_API sint32 s_sint32_read_at(SDatasource *ds, long pos, s_erc *error)
{
	sint32 i;


	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_sint32_read_at",
			  "Argument \"ds\" is NULL");
		return 0;
	}

	SDatasourceReadAt(ds, &i, sizeof(sint32), 1, pos, error);

	if (S_CHK_ERR(error, S_CONTERR,
		      "s_sint32_read_at",
		      "Failed to read sint32 from data source"))
		return 0;

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be32(i);
	else
		i = s_swap_le32(i);

	return i;
}


S_API void s_sint32_write(SDatasource *ds, sint32 i, s_erc *error)
{
	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_sint32_write",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be32(i);
	else
		i = s_swap_le32(i);

	SDatasourceWrite(ds, &i, sizeof(sint32), 1, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_sint32_write",
		  "Failed to write sint32 to data source");
}


S_API void s_sint32_write_at(SDatasource *ds, sint32 i, long pos, s_erc *error)
{
	S_CLR_ERR(error);

	if (ds == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
			  "s_sint32_write_at",
			  "Argument \"ds\" is NULL");
		return;
	}

	if (ds->byte_order == S_BYTEORDER_BE)
		i = s_swap_be32(i);
	else
		i = s_swap_le32(i);

	SDatasourceWriteAt(ds, &i, sizeof(sint32), 1, pos, error);

	S_CHK_ERR(error, S_CONTERR,
		  "s_sint32_write_at",
		  "Failed to write sint32 to data source");
}


