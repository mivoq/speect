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
/* An utterance processor to process (extract the cost features) the                */
/* halfphone units relation stream.                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "cost_function.h"
#include "process_units.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SProcessUnitsUttProc class declaration. */
static SProcessUnitsUttProcClass ProcessUnitsUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void create_cost_functions(const SList *costFunctions, const SVoice *voice,
								  const char *costs_name, s_erc *error);

static void process_cost_functions(const SList *costFunctions, const SItem *headItem,
								   const char *costs_name, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_process_units_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&ProcessUnitsUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_process_units_utt_proc_class_reg",
			  "Failed to register SProcessUnitsUttProcClass");
}


S_LOCAL void _s_process_units_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&ProcessUnitsUttProcClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_process_units_utt_proc_class_free",
			  "Failed to free SProcessUnitsUttProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void create_cost_functions(const SList *costFunctions, const SVoice *voice,
								  const char *costs_name, s_erc *error)
{
	SIterator *itr;
	int counter;
	const char *plugin_name;
	const char *class_name;
	const char *feature_name;
	sint32 weight;
	SPlugin *costPlugin;
	SCostFunction *costFunc;
	SMap *costFuncMap;


	S_CLR_ERR(error);

	itr = S_ITERATOR_GET(costFunctions, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "create_cost_functions",
				  "Call to \"S_ITERATOR_GET\" failed"))
		return;

	counter = 0;
	while (itr != NULL)
	{
		costFuncMap = S_CAST(SIteratorObject(itr, error), SMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_cost_functions",
					  "Call to \"SIteratorObject/S_CAST (SMap)\" failed"))
		{
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		plugin_name = SMapGetStringDef(costFuncMap, "plug-in", NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_cost_functions",
					  "Call to \"SMapGetStringDef\" failed"))
		{
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		if (plugin_name == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_cost_functions",
					  "'%s' function #%d does not have a 'plug-in' feature",
					  costs_name, counter);
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		class_name = SMapGetStringDef(costFuncMap, "class", NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_cost_functions",
					  "Call to \"SMapGetStringDef\" failed"))
		{
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		if (class_name == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_cost_functions",
					  "'%s' function #%d does not have a 'class' feature",
					  costs_name, counter);
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		feature_name = SMapGetStringDef(costFuncMap, "feature-name", NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_cost_functions",
					  "Call to \"SMapGetStringDef\" failed"))
		{
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		if (feature_name == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_cost_functions",
					  "'%s' function #%d does not have a 'feature-name' feature",
					  costs_name, counter);
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		/* random number to check quickly if it is defined or not */
		weight = SMapGetIntDef(costFuncMap, "weight", -31415, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_cost_functions",
					  "Call to \"SMapGetIntDef\" failed"))
		{
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		if (weight == -31415)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "create_cost_functions",
					  "'%s' function #%d does not have a 'weight' feature",
					  costs_name, counter);
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		/* load plug-in and create the cost function */
		costPlugin = s_pm_load_plugin(plugin_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_cost_functions",
					  "Call to \"s_pm_load_plugin\" failed"))
		{
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		/* add plug-in to voice plug-ins*/
		SListPush(voice->plugins, S_OBJECT(costPlugin), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_cost_functions",
					  "Call to \"SListPush\" failed"))
		{
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		costFunc = (SCostFunction*)S_NEW(class_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_cost_functions",
					  "Failed to create new '%s' object", class_name))
		{
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		costFunc->feature_name = s_strdup(feature_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_cost_functions",
					  "Call to \"s_strdup\" failed"))
		{
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		costFunc->weight = (uint32)weight;

		/* set the cost function in the map under '_function' */
		SMapSetObject(costFuncMap, "_function", S_OBJECT(costFunc), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "create_cost_functions",
					  "Call to \"SMapSetObject\" failed"))
		{
			S_DELETE(itr, "create_cost_functions", error);
			return;
		}

		itr = SIteratorNext(itr);
		counter++;
	}
}


static void process_cost_functions(const SList *costFunctions, const SItem *headItem,
								   const char *costs_name, s_erc *error)
{
	SIterator *itr;
	int counter;
	const SMap *costFuncMap;
	const SCostFunction *costFunc;
	SItem *segmentItem;
	SItem *leftUnit;
	SItem *rightUnit;
	const SObject *feature;


	S_CLR_ERR(error);

	itr = S_ITERATOR_GET(costFunctions, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "process_cost_functions",
				  "Call to \"S_ITERATOR_GET\" failed"))
		return;

	counter = 0;
	while (itr != NULL)
	{
		/* get cost function map */
		costFuncMap = (const SMap*)SIteratorObject(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_cost_functions",
					  "Call to \"SIteratorObject\" failed"))
		{
			S_DELETE(itr, "process_cost_functions", error);
			return;
		}

		/* get cost function */
		costFunc = (const SCostFunction*)SMapGetObjectDef(costFuncMap, "_function", NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "process_cost_functions",
					  "Call to \"SMapGetObjectDef\" failed"))
		{
			S_DELETE(itr, "process_cost_functions", error);
			return;
		}

		if (costFunc == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "process_cost_functions",
					  "Failed to find function for '%s' function definition #%d",
					  costs_name, counter);
			S_DELETE(itr, "process_cost_functions", error);
			return;
		}

		/* some cost functions do not define 'set_feature' */
		if (!S_COST_FUNCTION_METH_VALID(costFunc, set_feature))
		{
			itr = SIteratorNext(itr);
			counter++;
			continue;
		}

		segmentItem = (SItem*)headItem;
		while (segmentItem != NULL)
		{
			/* process this item */
			S_COST_FUNCTION_CALL(costFunc, set_feature)(costFunc, segmentItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_cost_functions",
						  "Call to SCostFunction method \"set_feature\" failed for '%s' function #%d",
						  costs_name, counter))
			{
				S_DELETE(itr, "process_cost_functions", error);
				return;
			}

			/* get the feature and set it on the halfphone units */
			feature = SItemGetObject(segmentItem, costFunc->feature_name, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_cost_functions",
						  "Call to \"SItemGetObject\" failed"))
			{
				S_DELETE(itr, "process_cost_functions", error);
				return;
			}

			leftUnit = SItemDaughter(segmentItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_cost_functions",
						  "Call to \"SItemDaughter\" failed"))
			{
				S_DELETE(itr, "process_cost_functions", error);
				return;
			}

			SItemSetObject(leftUnit, costFunc->feature_name, feature, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_cost_functions",
						  "Call to \"SItemSetObject\" failed"))
			{
				S_DELETE(itr, "process_cost_functions", error);
				return;
			}

			rightUnit = SItemNext(leftUnit, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_cost_functions",
						  "Call to \"SItemNext\" failed"))
			{
				S_DELETE(itr, "process_cost_functions", error);
				return;
			}

			SItemSetObject(rightUnit, costFunc->feature_name, feature, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_cost_functions",
						  "Call to \"SItemSetObject\" failed"))
			{
				S_DELETE(itr, "process_cost_functions", error);
				return;
			}

			segmentItem = SItemNext(segmentItem, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "process_cost_functions",
						  "Call to \"SItemNext\" failed"))
			{
				S_DELETE(itr, "process_cost_functions", error);
				return;
			}
		}

		itr = SIteratorNext(itr);
		counter++;
	}
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static void Initialize(SUttProcessor *self, const SVoice *voice, s_erc *error)
{
	const SObject *tmp;
	const SMap *allCostFunctions;
	const SList *costFunctions;


	S_CLR_ERR(error);

	/* check if "unit selection cost functions" are defined in
	   voice features */
	tmp = SVoiceGetFeature(voice, "unit selection cost functions", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SVoiceGetFeature\" failed"))
		return;

	/* nothing, return */
	if (tmp == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Initialize",
				  "SProcessUnitsUttProc utterance processor could not find 'unit selection cost functions' "
				  "in voice features");
		return;
	}

	allCostFunctions = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"S_CAST (SMap)\" failed"))
		return;

	/* get target cost functions */
	tmp = SMapGetObjectDef(allCostFunctions, "target costs", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (tmp != NULL)
	{
		costFunctions = S_CAST(tmp, SList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"S_CAST (SList)\" failed"))
			return;

		/* create target cost functions */
		create_cost_functions(costFunctions, voice, "target costs", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"create_cost_functions\" failed"))
			return;
	}

	/* get join cost functions */
	tmp = SMapGetObjectDef(allCostFunctions, "join costs", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Initialize",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (tmp != NULL)
	{
		costFunctions = S_CAST(tmp, SList, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"S_CAST (SList)\" failed"))
			return;

		/* create join cost functions */
		create_cost_functions(costFunctions, voice, "join costs", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Initialize",
					  "Call to \"create_cost_functions\" failed"))
			return;
	}

	S_UNUSED(self);
}


static void Run(const SUttProcessor *self, SUtterance *utt,
				s_erc *error)
{
	const SRelation *segmentRel;
	s_bool is_present;
	const SVoice *voice;
	const SList *costFunctions;
	const SMap *allCostFunctions;
	const SItem *headItem;


	S_CLR_ERR(error);

	/* we require the segment relation */
	is_present = SUtteranceRelationIsPresent(utt, "Segment", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		return;

	if (!is_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Segment' relation in utterance");
		return;
	}

	segmentRel = SUtteranceGetRelation(utt, "Segment", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		return;

	headItem = SRelationHead(segmentRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		return;

	voice = SUtteranceVoice(utt, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceVoice\" failed"))
		return;

	if (voice == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "SProcessUnitsUttProc utterance processor requires an utterance to have a voice");
		return;
	}

	/* get "unit selection cost functions" in voice features */
	allCostFunctions = (const SMap*)SVoiceGetFeature(voice, "unit selection cost functions", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SVoiceGetFeature\" failed"))
		return;

	/* nothing, return */
	if (allCostFunctions == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to get 'unit selection cost functions' in voice features");
		return;
	}

	/* get target cost functions */
	costFunctions = (SList*)SMapGetObjectDef(allCostFunctions, "target costs", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (costFunctions != NULL)
	{
		/* process target cost functions */
		process_cost_functions(costFunctions, headItem, "target costs", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"process_cost_functions\" failed"))
			return;
	}

	/* get join cost functions */
	costFunctions = (SList*)SMapGetObjectDef(allCostFunctions, "join costs", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SMapGetObjectDef\" failed"))
		return;

	if (costFunctions != NULL)
	{
		/* process target cost functions */
		process_cost_functions(costFunctions, headItem, "join costs", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"process_cost_functions\" failed"))
			return;
	}

	S_UNUSED(self);
}


/************************************************************************************/
/*                                                                                  */
/* SCreateUnitsUttProc class initialization                                         */
/*                                                                                  */
/************************************************************************************/

static SProcessUnitsUttProcClass ProcessUnitsUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SProcessUnitsUttProc",
		sizeof(SProcessUnitsUttProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SUttProcessorClass */
	Initialize,          /* initialize */
	Run                  /* run        */
};
