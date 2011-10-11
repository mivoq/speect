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
/* Serialized object reader functions for integer array data types                  */
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

S_LOCAL void read_int_array(SEbmlRead *reader, SObject *object, s_erc *error)
{
	SArrayInt *self = (SArrayInt*)object;
	uint32 id;
	s_bool container_exhausted;
	uint32 counter;


	S_CLR_ERR(error);

	/* read S_EBML_ID_OBJECT_DATA container, id is checked in the
	 * 'read_object' method of the SEbmlRead class
	 */
	id = S_EBMLREAD_CALL(reader, container)(reader, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "read_int_array",
				  "Call to SEbmlRead \"container\" failed"))
		return;

	while (1)
	{
		container_exhausted = S_EBMLREAD_CALL(reader, container_at_end)(reader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_int_array",
					  "Call to SEbmlRead \"container_at_end\" failed"))
			return;

		if (container_exhausted)
			break; /* we are finished reading the array object */

		/* peek id for array object elements */
		id = S_EBMLREAD_CALL(reader, peek_id)(reader, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "read_int_array",
					  "Call to SEbmlRead \"peek_id\" failed"))
			return;

		switch(id)
		{
		case S_ARRAY_INT_EBML_COUNT:
		{
			self->count = S_EBMLREAD_CALL(reader, read_uint)(reader, &id, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_int_array",
						  "Call to SEbmlRead \"read_uint\" failed"))
				return;

			break;
		}
		case S_ARRAY_INT_EBML_DATA:
		{
			self->i = S_MALLOC(sint32, self->count);
			if (self->i == NULL)
			{
				S_FTL_ERR(error, S_MEMERROR,
						  "read_int_array",
						  "Failed to allocate memory for 'sint32' object");
				return;
			}

			/* read S_ARRAY_INT_EBML_DATA container */
			id = S_EBMLREAD_CALL(reader, container)(reader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_int_array",
						  "Call to SEbmlRead \"container\" failed"))
				return;

			counter = 0;
			while (1)
			{
				container_exhausted = S_EBMLREAD_CALL(reader, container_at_end)(reader, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_int_array",
							  "Call to SEbmlRead \"container_at_end\" failed"))
					return;

				if (container_exhausted)
					break; /* we are finished reading the array data */

				/* peek id for array data elements */
				id = S_EBMLREAD_CALL(reader, peek_id)(reader, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "read_int_array",
							  "Call to SEbmlRead \"peek_id\" failed"))
					return;

				switch(id)
				{
				case S_ARRAY_INT_EBML_DATA_ELEMENT:
				{
					/* read int data S_ARRAY_INT_EBML_DATA_ELEMENT */
					if (counter >= self->count)
					{
						S_CTX_ERR(error, S_FAILURE,
								  "read_int_array",
								  "More data elements, %d, than expected %d",
								  counter, self->count);
						return;
					}

					self->i[counter++] = S_EBMLREAD_CALL(reader,
														 read_sint)(reader,
																	&id,
																	error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_int_array",
								  "Call to SEbmlRead \"read_sint\" failed"))
						return;

					break;
				}
				default:
					/* unknown elements, skip */
					S_WARNING(S_FAILURE,
							  "read_int_array",
							  "Skipping element with unknown id '0x%x'",
							  id);

					S_EBMLREAD_CALL(reader, element_skip)(reader, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "read_int_array",
								  "Call to SEbmlRead \"element_skip\" failed"))
						return;
				}
			}

			break;
		}
		default:
			/* unknown elements, skip */
			S_WARNING(S_FAILURE,
					  "read_int_array",
					  "Skipping element with unknown id '0x%x'",
					  id);

			S_EBMLREAD_CALL(reader, element_skip)(reader, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "read_int_array",
						  "Call to SEbmlRead \"element_skip\" failed"))
				return;
		}
	}
}


