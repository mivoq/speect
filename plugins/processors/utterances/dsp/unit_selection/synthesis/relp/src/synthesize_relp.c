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
/* An utterance processor to do RELP synthesis of a units relation stream.          */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "relp.h"
#include "synthesize_relp.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SRELPSynthUnitsUttProc class declaration. */
static SRELPSynthUnitsUttProcClass RELPSynthUnitsUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_relp_synth_units_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&RELPSynthUnitsUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_relp_synth_units_utt_proc_class_reg",
			  "Failed to register SRELPSynthUnitsUttProcClass");
}


S_LOCAL void _s_relp_synth_units_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&RELPSynthUnitsUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_relp_synth_units_utt_proc_class_free",
			  "Failed to free SRELPSynthUnitsUttProcClass");
}

/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

/* we need to delete the window plug-in if any */
static void Destroy(void *obj, s_erc *error)
{
	SUttProcessor *self = obj;
	const SObject *tmp;
	SPlugin *winPlugin;


	S_CLR_ERR(error);

	/* check if a window plug-in is defined as a feature */
	tmp = SMapGetObjectDef(self->features, "_window_plugin", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Destroy",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (tmp == NULL)
		return;

	/* unlink it */
	winPlugin = (SPlugin*)SMapObjectUnlink(self->features, "_window_plugin", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Destroy",
				  "Call to \"SMapObjectUnlink\" failed"))
		return;

	/* delete the plug-in */
	S_DELETE(winPlugin, "Destroy", error);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void Initialize(SUttProcessor *self, const SVoice *voice, s_erc *error)
{
	const SMap* windowConfig;
	SPlugin *winPlugin;
	const char *win_plugin_name;
	const char *win_class_name;


	S_CLR_ERR(error);

	/* get the window information */
	windowConfig = (SMap*)SMapGetObjectDef(self->features, "window-type", NULL,
										   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapGetObjectDef\" failed"))
	{
		/* this is here to silence the compiler's
		 * messages about unused parameters */
		voice = NULL;
		return;
	}

	if (windowConfig == NULL)
	{
		/* we use hanning windows as defaults */
		win_plugin_name = "hanning-window-func.spi";
		win_class_name = "SWinHann";
	}
	else
	{
		win_plugin_name = SMapGetStringDef(windowConfig, "plug-in",
										   "hanning-window-func.spi", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"SMapGetStringDef\" failed"))
			return;

		win_class_name = SMapGetStringDef(windowConfig, "class",
										  "SWinHann", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"SMapGetStringDef\" failed"))
			return;
	}

	SMapSetString(self->features, "_window_class", win_class_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapSetString\" failed"))
		return;

	/* load the window plugin */
	winPlugin = s_pm_load_plugin(win_plugin_name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"s_pm_load_plugin\" failed"))
		return;

	/* add plug-in to features */
	SMapSetObject(self->features, "_window_plugin", S_OBJECT(winPlugin), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapSetObject\" failed"))
	{
		S_DELETE(winPlugin, "Initialize", error);
		return;
	}
}


static void Run(const SUttProcessor *self, SUtterance *utt,
				s_erc *error)
{
	const SRelation *unitRel;
	SRelp *relpSynth = NULL;
	uint32 sample_rate;
	const char *window_class;
	s_bool window_is_symmetric;
	s_relp_mapping mapping;
	float window_gain;
	const char *tmp;
	int scomp;
	SAudio *audio = NULL;
	s_bool is_present;


	S_CLR_ERR(error);

	/* we require the unit relation */
	is_present = SUtteranceRelationIsPresent(utt, "Unit", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit_error;

	if (!is_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Unit' relation in utterance");
		goto quit_error;
	}

	unitRel = SUtteranceGetRelation(utt, "Unit", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		goto quit_error;

	/* get config parameters */
	sample_rate = (uint32)SMapGetIntDef(self->features, "sample-rate", 16000, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SMapGetIntDef\" failed"))
		goto quit_error;

	window_class = SMapGetString(self->features, "_window_class", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SMapGetString\" failed"))
		goto quit_error;

	tmp = SMapGetStringDef(self->features, "symmetric-windows", "FALSE", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SMapGetStringDef\" failed"))
		goto quit_error;

	scomp = s_strcmp(tmp, "FALSE", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"s_strcmp\" failed"))
		goto quit_error;

	if (scomp == 0)
		window_is_symmetric = FALSE;
	else
		window_is_symmetric = TRUE;

	mapping = S_RELP_LINEAR; /* currently the only option */

	window_gain = SMapGetFloatDef(self->features, "window-factor", 1.0, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SMapGetFloatDef\" failed"))
		goto quit_error;

	/* create a relp object */
	relpSynth = (SRelp*)S_NEW("SRelp", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Failed to create new 'SRelp' object"))
		goto quit_error;

	/* initialize relp object */
	S_RELP_CALL(relpSynth, init_relp)(&relpSynth, sample_rate, window_class,
									  window_is_symmetric, mapping, window_gain, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to SRelp method \"init_relp\" failed"))
		goto quit_error;

	/* concatenate unit tracks */
	S_RELP_CALL(relpSynth, concat_units)(relpSynth, unitRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to SRelp method \"concat_units\" failed"))
		goto quit_error;

	/* map tracks */
	S_RELP_CALL(relpSynth, map_tracks)(relpSynth, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to SRelp method \"map_tracks\" failed"))
		goto quit_error;

	/* do synthesis */
	audio = S_RELP_CALL(relpSynth, generate)(relpSynth, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to SRelp method \"generate\" failed"))
		goto quit_error;

	/* here all is OK */
	S_DELETE(relpSynth, "Run", error);

	/* set audio feature in utterance */
	SUtteranceSetFeature(utt, "audio", S_OBJECT(audio), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceSetFeature\" failed"))
		goto quit_error;

	/* all OK here */
	return;

	/* error clean-up code */
quit_error:
	if (relpSynth != NULL)
		S_DELETE(relpSynth, "Run", error);

	if (audio != NULL)
		S_DELETE(audio, "Run", error);

	return;
}


/************************************************************************************/
/*                                                                                  */
/* SCreateUnitsUttProc class initialization                                         */
/*                                                                                  */
/************************************************************************************/

static SRELPSynthUnitsUttProcClass RELPSynthUnitsUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SRELPSynthUnitsUttProc",
		sizeof(SRELPSynthUnitsUttProc),
		{ 0, 1},
		NULL,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SUttProcessorClass */
	Initialize,          /* initialize */
	Run                  /* run        */
};
