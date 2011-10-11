/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Serialized object writer functions for integer array data types                  */
/* in ebml format.                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "array_int.h"
#include "ebml.h"
#include "element_id.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void write_int_array(SEbmlWrite *writer, const SObject *object, s_erc *error)
{
	const SArrayInt *self = (SArrayInt*)object;
	uint32 counter;


	S_CLR_ERR(error);

	/* start S_EBML_ID_OBJECT_DATA container */
	S_EBMLWRITE_CALL(writer, start_container)(writer, S_EBML_ID_OBJECT_DATA,
											  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_int_array",
				  "Call to SEbmlWrite method \"start_container\" failed"))
		return;

	/* write the count of the data */
	S_EBMLWRITE_CALL(writer, write_uint)(writer, S_ARRAY_INT_EBML_COUNT,
										 self->count, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_int_array",
				  "Call to SEbmlWrite method \"write_uint\" failed"))
		return;

	/* if it's an empty array then we stop, and return */
	if (self->count == 0)
	{
		S_EBMLWRITE_CALL(writer, stop_container)(writer, error);
		S_CHK_ERR(error, S_CONTERR,
				  "write_int_array",
				  "Call to SEbmlWrite method \"stop_container\" failed");
		return;
	}

	/* start S_ARRAY_INT_EBML_DATA container */
	S_EBMLWRITE_CALL(writer, start_container)(writer, S_ARRAY_INT_EBML_DATA,
											  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_int_array",
				  "Call to SEbmlWrite method \"start_container\" failed"))
		return;

	/* now write all the data elements */
	for (counter = 0; counter < self->count; counter++)
	{
		S_EBMLWRITE_CALL(writer, write_sint)(writer, S_ARRAY_INT_EBML_DATA_ELEMENT,
											 self->i[counter], error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_int_array",
					  "Call to SEbmlWrite method \"write_sint\" failed"))
			return;
	}

	/* stop S_ARRAY_INT_EBML_DATA container */
	S_EBMLWRITE_CALL(writer, stop_container)(writer, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_int_array",
				  "Call to SEbmlWrite method \"stop_container\" failed"))
		return;

	/* finally close S_EBML_ID_OBJECT_DATA container and return */
	S_EBMLWRITE_CALL(writer, stop_container)(writer, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_int_array",
			  "Call to SEbmlWrite method \"stop_container\" failed");
}
