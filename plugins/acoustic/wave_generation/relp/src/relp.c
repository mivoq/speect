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
/* A Residual excited linear prediction (RELP) waveform generator.                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "relp.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SRelpClass RelpClass; /* SRelp class declaration. */

/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_relp_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&RelpClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_relp_class_reg",
			  "Failed to register SRelpClass");
}


S_LOCAL void _s_relp_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&RelpClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_relp_class_free",
			  "Failed to free SRelpClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SRelp *self = obj;


	S_CLR_ERR(error);
	self->residuals = NULL;
	self->track = NULL;
	self->num_frames = 0;
	self->num_channels = 0;
	self->sample_rate = 0;
	self->window = NULL;
	self->symmetric = TRUE;
	self->mapping = S_RELP_LINEAR;
	self->map = NULL;
	self->target = NULL;
	self->wave_res = NULL;
	self->pmIndx = NULL;
	self->window_factor = 0.0;
}


static void Destroy(void *obj, s_erc *error)
{
	SRelp *self = obj;
	uint32 i;


	S_CLR_ERR(error);
	if (self->residuals != NULL)
	{
		for (i = 0; i < self->num_frames; i++)
			S_DELETE(self->residuals[i], "Destroy", error);
		S_FREE(self->residuals);
	}

	if (self->track != NULL)
		S_DELETE(self->track, "Destroy", error);

	if (self->window != NULL)
		S_DELETE(self->window, "Destroy", error);

	if (self->map != NULL)
		S_DELETE(self->map, "Destroy", error);

	if (self->target != NULL)
		S_DELETE(self->target, "Destroy", error);

	if (self->wave_res != NULL)
		S_DELETE(self->wave_res, "Destroy", error);

	if (self->pmIndx != NULL)
		S_DELETE(self->pmIndx, "Destroy", error);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void InitRelp(SRelp **self, uint32 sample_rate, const char *window_class,
					 s_bool window_is_symmetric, s_relp_mapping mapping, float window_gain,
					 s_erc *error)
{
	S_CLR_ERR(error);

	(*self)->sample_rate = sample_rate;
	(*self)->window = (SWinFunc*)S_NEW(window_class, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "InitRelp",
				  "Failed to create new '%s' object",
				  window_class))
		goto quit_error;

	(*self)->symmetric = window_is_symmetric;
	(*self)->mapping = mapping;
	(*self)->window_factor = window_gain;

	return;

quit_error:
	S_DELETE((*self), "InitRelp", error);
	*self = NULL;
}


static void ConcatUnits(SRelp *self, const SRelation *unitRel, s_erc *error)
{
	S_CLR_ERR(error);

	if (unitRel == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "ConcatUnits",
				  "Argument \"unitRel\" is NULL");
		return;
	}

	concat_units(self, unitRel, error);
	S_CHK_ERR(error, S_CONTERR,
			  "ConcatUnits",
			  "Call to \"concat_units\" failed");
}


static void MapTracks(SRelp *self, s_erc *error)
{
	S_CLR_ERR(error);

	/* currently only linear mapping, should that change then a switch
	   must go here */
	linear_mapping(self, error);
	S_CHK_ERR(error, S_CONTERR,
			  "MapTracks",
			  "Call to \"linear_mapping\" failed");
}


static SAudio *Generate(SRelp *self, s_erc *error)
{
	SAudio *audio;


	S_CLR_ERR(error);

	audio = synthesis(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Generate",
				  "Call to \"synthesis\" failed"))
		return NULL;

	return audio;
}


/************************************************************************************/
/*                                                                                  */
/* SRelp class initialization                                                       */
/*                                                                                  */
/************************************************************************************/

static SRelpClass RelpClass =
{
	/* SObjectClass */
	{
		"SRelp",
		sizeof(SRelp),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SRelpClass */
	InitRelp,            /* init_relp     */
	ConcatUnits,         /* concat_units  */
	MapTracks,           /* map_tracks    */
	Generate             /* generate      */
};
