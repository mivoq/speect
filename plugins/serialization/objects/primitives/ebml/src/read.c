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
/* DATE    : 19 November 2009                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Serialized object reader functions for Speect primitives data types              */
/* in ebml format.                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "ebml.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void skip_primitive(SEbmlRead *reader, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void read_int(SEbmlRead *reader, SObject *object, s_erc *error)
{
	sint32 val;
	uint32 id;


	S_CLR_ERR(error);

	if (!S_EBMLREAD_METH_VALID(reader, read_sint))
	{
		skip_primitive(reader, error);
		S_CTX_ERR(error, S_METHINVLD,
				  "read_int",
				  "SEbmlRead method \"read_sint\" not implemented");
		return;
	}

	val = S_EBMLREAD_CALL(reader, read_sint)(reader, &id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_int",
				  "Call to SEbmlRead \"read_sint\" failed"))
	{
		skip_primitive(reader, NULL);
		return;
	}

	if (id != S_EBML_ID_OBJECT_DATA)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_int",
				  "Read id (0x%X) does not equal ebml object data id (0x%X)", id, S_EBML_ID_OBJECT_DATA);
		return;
	}

	SObjectResetInt(object, val, error);
	S_CHK_ERR(error, S_CONTERR,
			  "read_int",
			  "Call to \"SObjectResetInt\" failed");
}


S_LOCAL void read_float(SEbmlRead *reader, SObject *object, s_erc *error)
{
	float val;
	uint32 id;


	S_CLR_ERR(error);

	if (!S_EBMLREAD_METH_VALID(reader, read_float))
	{
		skip_primitive(reader, error);
		S_CTX_ERR(error, S_METHINVLD,
				  "read_float",
				  "SEbmlRead method \"read_float\" not implemented");
		return;
	}

	val = S_EBMLREAD_CALL(reader, read_float)(reader, &id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_float",
				  "Call to SEbmlRead \"read_float\" failed"))
	{
		skip_primitive(reader, NULL);
		return;
	}

	if (id != S_EBML_ID_OBJECT_DATA)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "read_float",
				  "Read id (0x%X) does not equal ebml object data id (0x%X)", id, S_EBML_ID_OBJECT_DATA);
		return;
	}

	SObjectResetFloat(object, val, error);
	S_CHK_ERR(error, S_CONTERR,
			  "read_float",
			  "Call to \"SObjectResetFloat\" failed");
}


S_LOCAL void read_string(SEbmlRead *reader, SObject *object, s_erc *error)
{
	char *val;
	uint32 id;


	S_CLR_ERR(error);

	if (!S_EBMLREAD_METH_VALID(reader, read_utf8))
	{
		skip_primitive(reader, error);
		S_CTX_ERR(error, S_METHINVLD,
				  "read_string",
				  "SEbmlRead method \"read_utf8\" not implemented");
		return;
	}

	val = S_EBMLREAD_CALL(reader, read_utf8)(reader, &id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_string",
				  "Call to SEbmlRead \"read_utf8\" failed"))
		return;

	if (id != S_EBML_ID_OBJECT_DATA)
	{
		skip_primitive(reader, NULL);
		S_CTX_ERR(error, S_FAILURE,
				  "read_string",
				  "Read id (0x%X) does not equal ebml object data id (0x%X)", id, S_EBML_ID_OBJECT_DATA);
		return;
	}

	SObjectResetString(object, val, error);
	S_CHK_ERR(error, S_CONTERR,
			  "read_string",
			  "Call to \"SObjectResetString\" failed");

	S_FREE(val);
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void skip_primitive(SEbmlRead *reader, s_erc *error)
{
	S_CLR_ERR(error);

	if (!S_EBMLREAD_METH_VALID(reader, element_skip))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "skip_primitive",
				  "SEbmlRead method \"element_skip\" not implemented");
		return;
	}

	S_EBMLREAD_CALL(reader, element_skip)(reader, error);
	S_CHK_ERR(error, S_CONTERR,
			  "skip_primitive",
			  "Call to SEbmlRead \"element_skip\" failed");
}

