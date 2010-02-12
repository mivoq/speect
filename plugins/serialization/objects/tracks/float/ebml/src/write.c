/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* Serialized object writer functions for time aligned floating track               */
/* data types in ebml format.                                                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "track_float.h"
#include "ebml.h"
#include "element_id.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void write_float_track(SEbmlWrite *writer, const SObject *object, s_erc *error)
{
	const STrackFloat *self = (STrackFloat*)object;
	uint32 row_counter;
	uint32 col_counter;


	S_CLR_ERR(error);

	/* start S_EBML_ID_OBJECT_DATA container */
	S_EBMLWRITE_CALL(writer, start_container)(writer, S_EBML_ID_OBJECT_DATA,
											  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_float_track",
				  "Call to SEbmlWrite method \"start_container\" failed"))
		return;

	/* write the row count of the data */
	S_EBMLWRITE_CALL(writer, write_uint)(writer, S_TRACK_FLOAT_EBML_ROW_COUNT,
										 self->data->row_count, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_float_track",
				  "Call to SEbmlWrite method \"write_uint\" failed"))
		return;

	/* write the column count of the data */
	S_EBMLWRITE_CALL(writer, write_uint)(writer, S_TRACK_FLOAT_EBML_COL_COUNT,
										 self->data->col_count, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_float_track",
				  "Call to SEbmlWrite method \"write_uint\" failed"))
		return;

	/* if it's an empty track then we stop, and return */
	if ((self->data->row_count == 0) || (self->data->col_count == 0))
	{
		S_EBMLWRITE_CALL(writer, stop_container)(writer, error);
		S_CHK_ERR(error, S_CONTERR,
				  "write_float_track",
				  "Call to SEbmlWrite method \"stop_container\" failed");
		return;
	}

	/* start S_TRACK_FLOAT_EBML_ROW_DATA container */
	S_EBMLWRITE_CALL(writer, start_container)(writer, S_TRACK_FLOAT_EBML_ROW_DATA,
											  error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_float_track",
				  "Call to SEbmlWrite method \"start_container\" failed"))
		return;

	/* now write all the data elements */
	for (row_counter = 0; row_counter < self->data->row_count; row_counter++)
	{
		/* write row time */
		S_EBMLWRITE_CALL(writer, write_float)(writer, S_TRACK_FLOAT_EBML_TIME,
											  self->time[row_counter], error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_float_track",
					  "Call to SEbmlWrite method \"write_float\" failed"))
			return;

		/* start S_TRACK_FLOAT_EBML_COL_DATA container */
		S_EBMLWRITE_CALL(writer, start_container)(writer, S_TRACK_FLOAT_EBML_COL_DATA,
												  error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_float_track",
					  "Call to SEbmlWrite method \"start_container\" failed"))
			return;

		for (col_counter = 0; col_counter < self->data->col_count; col_counter++)
		{

			S_EBMLWRITE_CALL(writer, write_float)(writer, S_TRACK_FLOAT_EBML_DATA_ELEMENT,
												  self->data->f[row_counter][col_counter], error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "write_float_track",
						  "Call to SEbmlWrite method \"write_float\" failed"))
				return;
		}

		/* close S_TRACK_FLOAT_EBML_COL_DATA column container */
		S_EBMLWRITE_CALL(writer, stop_container)(writer, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "write_float_track",
					  "Call to SEbmlWrite method \"stop_container\" failed"))
			return;
	}

	/* close S_TRACK_FLOAT_EBML_ROW_DATA row container */
	S_EBMLWRITE_CALL(writer, stop_container)(writer, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "write_float_track",
				  "Call to SEbmlWrite method \"stop_container\" failed"))
		return;

	/* finally close S_EBML_ID_OBJECT_DATA data object container and return */
	S_EBMLWRITE_CALL(writer, stop_container)(writer, error);
	S_CHK_ERR(error, S_CONTERR,
			  "write_float_track",
			  "Call to SEbmlWrite method \"stop_container\" failed");
}
