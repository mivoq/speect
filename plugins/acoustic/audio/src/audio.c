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
/* A simple audio class that can be used as an SObject.                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include "audio.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SAudioClass AudioClass; /* SAudio class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_audio_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&AudioClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_audio_class_reg",
			  "Failed to register SAudioClass");
}


S_LOCAL void _s_audio_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&AudioClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_audio_class_free",
			  "Failed to free SAudioClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SAudio *self = obj;


	S_CLR_ERR(error);
	self->sample_rate = 0;
	self->num_samples = 0;
	self->samples = NULL;
}


static void Destroy(void *obj, s_erc *error)
{
	SAudio *self = obj;


	S_CLR_ERR(error);
	if (self->samples != NULL)
		S_FREE(self->samples);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void Resize(SAudio *self, uint32 new_size, s_erc *error)
{
	float *samples;


	S_CLR_ERR(error);

	samples = S_CALLOC(float, new_size);
	if (samples == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "Resize",
				  "Failed to allocate memory for 'float' object");
		return;
	}

	if (self->samples != NULL)
	{
		memmove(samples, self->samples,
				sizeof(float) * ((new_size < self->num_samples) ? new_size : self->num_samples));

		S_FREE(self->samples);
	}

	self->samples = samples;
	self->num_samples = new_size;
}


static void Scale(SAudio *self, float factor, s_erc *error)
{
	uint32 i;


	S_CLR_ERR(error);
	for (i = 0; i < self->num_samples; i++)
		self->samples[i] = self->samples[i] * factor;
}


/************************************************************************************/
/*                                                                                  */
/* SAudio class initialization                                                      */
/*                                                                                  */
/************************************************************************************/

static SAudioClass AudioClass =
{
	/* SObjectClass */
	{
		"SAudio",
		sizeof(SAudio),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SAudioClass */
	Resize,              /* resize  */
	Scale                /* scale   */
};
