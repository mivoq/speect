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
/* An abstract _structured_ data format reader class.                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "datasources/data_reader.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SDataReaderClass DataReaderClass; /* SDataReader class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_data_reader_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&DataReaderClass, error);
	S_CHK_ERR(error, S_CONTERR,
		  "_s_data_reader_class_add",
		  "Failed to add SDataReaderClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitDataReader(void *obj, s_erc *error)
{
	SDataReader *self = obj;


	S_CLR_ERR(error);
	self->ds = NULL;
}


static void DestroyDataReader(void *obj, s_erc *error)
{
	SDataReader *self = obj;


	S_CLR_ERR(error);

	if (self->ds != NULL)
		S_DELETE(self->ds, "DestroyDataReader", error);
}


static void DisposeDataReader(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SDatasource class initialization                                                 */
/*                                                                                  */
/************************************************************************************/

static SDataReaderClass DataReaderClass =
{
	/* SObjectClass */
	{
		"SDataReader",
		sizeof(SDataReader),
		{ 0, 1},
		InitDataReader,    /* init    */
		DestroyDataReader, /* destroy */
		DisposeDataReader, /* dispose */
		NULL,              /* compare */
		NULL,              /* print   */
		NULL,              /* copy    */
	}
	/* SDataReaderClass */

	/* No class methods, child classes supply methods. */
};


