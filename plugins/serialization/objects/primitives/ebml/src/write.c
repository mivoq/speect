/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* Serialized object writer functions for Speect primitives data types              */
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
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void write_int(SEbmlWrite *writer, const SObject *object, s_erc *error)
{
	sint32 val;


	S_CLR_ERR(error);

	if (!S_EBMLWRITE_METH_VALID(writer, write_sint))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "write_int",
				  "SEbmlWrite method \"write_sint\" not implemented");
		return;
	}

	val = SObjectGetInt(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_int",
				  "Call to \"SObjectGetInt\" failed"))
		return;


	S_EBMLWRITE_CALL(writer, write_sint)(writer, S_EBML_ID_OBJECT_DATA, val, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_int",
			  "Call to SEbmlWrite \"write_sint\" failed");
}


S_LOCAL void write_float(SEbmlWrite *writer, const SObject *object, s_erc *error)
{
	float val;


	S_CLR_ERR(error);

	if (!S_EBMLWRITE_METH_VALID(writer, write_float))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "write_float",
				  "SEbmlWrite method \"write_float\" not implemented");
		return;
	}

	val = SObjectGetFloat(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_int",
				  "Call to \"SObjectGetFloat\" failed"))
		return;

	S_EBMLWRITE_CALL(writer, write_float)(writer, S_EBML_ID_OBJECT_DATA, val, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_float",
			  "Call to SEbmlWrite \"write_float\" failed");
}


S_LOCAL void write_string(SEbmlWrite *writer, const SObject *object, s_erc *error)
{
	const char *val;


	S_CLR_ERR(error);

	if (!S_EBMLWRITE_METH_VALID(writer, write_utf8))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "write_string",
				  "SEbmlWrite method \"write_utf8\" not implemented");
		return;
	}

	val = SObjectGetString(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_string",
				  "Call to \"SObjectGetString\" failed"))
		return;

	S_EBMLWRITE_CALL(writer, write_utf8)(writer, S_EBML_ID_OBJECT_DATA, val, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_string",
			  "Call to SEbmlWrite \"write_utf8\" failed");
}


