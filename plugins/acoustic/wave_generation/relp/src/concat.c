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
/* RELP unit concatenation.                                                         */
/* Loosely based on us_unit.cc of Festival                                          */
/* (http://www.cstr.ed.ac.uk/projects/festival.html (1.96 beta),                    */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* Festival us_unit.cc license,                                                     */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                              Copyright (c) 1996,1997                             */
/*                             All Rights Reserved.                                 */
/*       Permission is hereby granted, free of charge, to use and distribute        */
/*       this software and its documentation without restriction, including         */
/*       without limitation the rights to use, copy, modify, merge, publish,        */
/*       distribute, sublicense, and/or sell copies of this work, and to            */
/*       permit persons to whom this work is furnished to do so, subject to         */
/*       the following conditions:                                                  */
/*        1. The code must retain the above copyright notice, this list of          */
/*           conditions and the following disclaimer.                               */
/*        2. Any modifications must be clearly marked as such.                      */
/*        3. Original authors' names are not deleted.                               */
/*        4. The authors' names are not used to endorse or promote products         */
/*           derived from this software without specific prior written              */
/*           permission.                                                            */
/*       THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK              */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE           */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author :  Paul Taylor                                                      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include "relp.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static uint32 get_num_frames(SRelp *self, const SRelation *unitRel, s_erc *error);

static void copy_lpc_residuals(SRelp *self, const SRelation *unitRel, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void concat_units(SRelp *self, const SRelation *unitRel, s_erc *error)
{
	S_CLR_ERR(error);


	/* get the number of frames in the track */
	self->num_frames = get_num_frames(self, unitRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "concat_units",
				  "Call to \"get_num_frames\" failed"))
		return;

	if (self->symmetric == FALSE)
	{
		self->pmIndx = S_NEW(SArrayInt, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "concat_units",
					  "Failed to create new 'SArrayInt' object"))
			return;


		self->pmIndx->i = S_CALLOC(sint32, self->num_frames);
		if (self->pmIndx->i == NULL)
		{
			S_FTL_ERR(error, S_CONTERR,
					  "concat_units",
					  "Failed to allocate memory for 'sint32' object");
			return;
		}

		self->pmIndx->count = self->num_frames;
	}

	/* setup new track */
	self->track = S_NEW(STrackFloat, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "concat_units",
				  "Failed to create new 'STrackFloat' object"))
		return;

	self->track->time = S_CALLOC(float, self->num_frames);
	if (self->track->time == NULL)
	{
		S_FTL_ERR(error, S_CONTERR,
				  "concat_units",
				  "Failed to allocate memory for 'float' object");
		return;
	}

	self->track->data->f = S_CALLOC(float*, self->num_frames);
	if (self->track->data->f == NULL)
	{
		S_FTL_ERR(error, S_CONTERR,
				  "concat_units",
				  "Failed to allocate memory for 'float*' object");
		return;
	}

	self->track->data->row_count = self->num_frames;
	self->track->data->col_count = self->num_channels;

	/* setup residuals */
	self->residuals = S_CALLOC(SArrayInt*, self->num_frames);
	if (self->residuals == NULL)
	{
		S_FTL_ERR(error, S_CONTERR,
				  "concat_units",
				  "Failed to allocate memory for 'SArrayInt*' object");
		return;
	}

	/* copy track + res info */
	copy_lpc_residuals(self, unitRel, error);
	S_CHK_ERR(error, S_CONTERR,
			  "concat_units",
			  "Call to \"copy_lpc_residuals\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static uint32 get_num_frames(SRelp *self, const SRelation *unitRel, s_erc *error)
{
	const SItem *unit;
	SItem *sourceUnit = NULL;
	STrackFloat *sourceTrack = NULL;
	uint32 num_frames = 0;


	S_CLR_ERR(error);
	unit = SRelationHead(unitRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_num_frames",
				  "Call to \"SRelationHead\" failed"))
		return 0;

	while (unit != NULL)
	{
		sourceUnit = (SItem*)SItemGetObject(unit, "source-unit", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_num_frames",
					  "Call to \"SItemGetObject\" failed"))
			return 0;

		sourceTrack = (STrackFloat*)SItemGetObject(sourceUnit, "lpc-coefs", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_num_frames",
					  "Call to \"SItemGetObject\" failed"))
			return 0;

		num_frames += sourceTrack->data->row_count;

		unit = SItemNext(unit, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_num_frames",
					  "Call to \"SItemNext\" failed"))
			return 0;
	}

	self->num_channels = sourceTrack->data->col_count;
	return num_frames;
}


static void copy_lpc_residuals(SRelp *self, const SRelation *unitRel, s_erc *error)
{
	const SItem *unit;
	SItem *sourceUnit;
	STrackFloat *sourceTrack;
	SArrayInt *sourceResidual;
	uint32 i;
	uint32 j;
	float prev_time = 0.0;


	S_CLR_ERR(error);
	unit = SRelationHead(unitRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "copy_lpc_residuals",
				  "Call to \"SRelationHead\" failed"))
		return;

	i = 0;
	while (unit != NULL)
	{
		sourceUnit = (SItem*)SItemGetObject(unit, "source-unit", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "copy_lpc_residuals",
					  "Call to \"SItemGetObject\" failed"))
			return;

		sourceTrack = (STrackFloat*)SItemGetObject(sourceUnit, "lpc-coefs", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "copy_lpc_residuals",
					  "Call to \"SItemGetObject\" failed"))
			return;

		sourceResidual = (SArrayInt*)SItemGetObject(sourceUnit, "residuals", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "copy_lpc_residuals",
					  "Call to \"SItemGetObject\" failed"))
			return;

		window_residuals(self, sourceResidual, sourceTrack, i, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "copy_lpc_residuals",
					  "Call to \"window_residuals\" failed"))
			return;

		for (j = 0; j < sourceTrack->data->row_count; j++, i++)
		{
			self->track->data->f[i] = S_CALLOC(float, self->num_channels);
			if (self->track->data->f[i] == NULL)
			{
				S_FTL_ERR(error, S_MEMERROR,
						  "copy_lpc_residuals",
						  "Failed to allocate memory for 'float' object");
				return;
			}

			memcpy(self->track->data->f[i], sourceTrack->data->f[j],
				   self->num_channels * sizeof(float));

			self->track->time[i] = sourceTrack->time[j] + prev_time;
		}

		prev_time = self->track->time[i - 1];
		SItemSetFloat(sourceUnit, "source_end", prev_time, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "copy_lpc_residuals",
					  "Call to \"SItemSetFloat\" failed"))
			return;

		SItemSetInt(sourceUnit, "num_frames", sourceTrack->data->row_count, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "copy_lpc_residuals",
					  "Call to \"SItemSetInt\" failed"))
			return;

		unit = SItemNext(unit, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "copy_lpc_residuals",
					  "Call to \"SItemNext\" failed"))
			return;
	}
}
