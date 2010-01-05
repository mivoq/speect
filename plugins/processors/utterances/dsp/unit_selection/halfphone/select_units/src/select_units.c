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
/* An utterance processor to select the halfphone units relation stream.            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "halfphone_db.h"
#include "viterbi.h"
#include "cost_function.h"
#include "select_units.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SSelectUnitsUttProc class declaration. */
static SSelectUnitsUttProcClass SelectUnitsUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_bool units_contiguous(const SItem *leftItem, const SItem *rightItem,
							   s_erc *error);

static float run_cost_functions(const SList *costFunctions,
								const SItem *item1, const SItem *item2,
								const char *costs_name, s_erc *error);

static SViterbiPath *extend_path(SViterbiPath *p, SViterbiCandidate *c, SMap *features,
								 s_erc *error);

static void count_pauses(SItem *item, uint32 *counter, s_erc *error);

static SViterbiCandidate *get_candidates(const SItem *s, SMap *features, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_select_units_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&SelectUnitsUttProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_select_units_utt_proc_class_reg",
			  "Failed to register SSelectUnitsUttProcClass");
}


S_LOCAL void _s_select_units_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&SelectUnitsUttProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_select_units_utt_proc_class_free",
			  "Failed to free SSelectUnitsUttProcClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static float run_cost_functions(const SList *costFunctions,
								const SItem *item1, const SItem *item2,
								const char *costs_name, s_erc *error)
{
	SIterator *itr;
	int counter;
	const SMap *costFuncMap;
	const SCostFunction *costFunc;
	float total_cost = 0.0;
	float function_cost = 0.0;
	uint32 total_weight = 0;


	S_CLR_ERR(error);

	if (costFunctions == NULL)
		return 0;

	itr = SListIterator(costFunctions, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "run_cost_functions",
				  "Call to \"SListIterator\" failed"))
		return 0;

	counter = 0;
	while (itr != NULL)
	{
		/* get cost function map */
		costFuncMap = (const SMap*)SListIteratorValue(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_cost_functions",
					  "Call to \"SListIteratorValue\" failed"))
		{
			S_DELETE(itr, "run_cost_functions", error);
			return total_cost;
		}

		/* get cost function */
		costFunc = (const SCostFunction*)SMapGetObjectDef(costFuncMap, "_function", NULL,
														  error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_cost_functions",
					  "Call to \"SMapGetObjectDef\" failed"))
		{
			S_DELETE(itr, "run_cost_functions", error);
			return total_cost;
		}

		if (costFunc == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "run_cost_functions",
					  "Failed to find function for '%s' function definition #%d",
					  costs_name, counter);
			S_DELETE(itr, "run_cost_functions", error);
			return total_cost;
		}

		/* some cost functions do not define 'set_feature' */
		if (!S_COST_FUNCTION_METH_VALID(costFunc, get_cost))
		{
			itr = SIteratorNext(itr);
			counter++;
			continue;
		}

		function_cost = S_COST_FUNCTION_CALL(costFunc, get_cost)(costFunc, item1, item2,
																 error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "run_cost_functions",
					  "Call to SCostFunction (%s, #d) method \"get_cost\" failed",
					  costs_name, counter))
		{
			S_DELETE(itr, "run_cost_functions", error);
			return total_cost;
		}

		total_cost += (function_cost * costFunc->weight);
		total_weight += costFunc->weight;

		itr = SIteratorNext(itr);
		counter++;
	}

	return total_cost/total_weight;
}


/* this isn't completely right, but it will do for now */
static s_bool units_contiguous(const SItem *leftItem, const SItem *rightItem, s_erc *error)
{
	const char *left_file_id;
	const char *right_file_id;
	int scomp;

	S_CLR_ERR(error);
	left_file_id = SItemGetString(leftItem, "file_id", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "units_contiguous",
				  "Call to \"SItemGetString\" failed"))
		return FALSE;

	right_file_id = SItemGetString(rightItem, "file_id", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "units_contiguous",
				  "Call to \"SItemGetString\" failed"))
		return FALSE;

	scomp = s_strcmp(left_file_id, right_file_id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "units_contiguous",
				  "Call to \"s_strcmp\" failed"))
		return FALSE;

	if (scomp == 0)
	{
		sint32 left_segment_id;
		sint32 right_segment_id;


		left_segment_id = SItemGetInt(leftItem, "segment_id", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "units_contiguous",
					  "Call to \"SItemGetInt\" failed"))
			return FALSE;

		right_segment_id = SItemGetInt(rightItem, "segment_id", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "units_contiguous",
					  "Call to \"SItemGetInt\" failed"))
			return FALSE;

		/*
		 * halfphone items are contiguous if they originate from the same
		 * parent segment item.
		 */
		if (left_segment_id == right_segment_id)
			return TRUE;
	}

	return FALSE;
}


static SViterbiPath *extend_path(SViterbiPath *p, SViterbiCandidate *c, SMap *features,
								 s_erc *error)
{
	SViterbiPath *newPath;
	const SList *joinCosts;
	SItem *left;
	SItem *right;
	float cost;
	s_bool is_contiguous;


	S_CLR_ERR(error);

	joinCosts = (SList*)SMapGetObject(features, "join costs", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "extend_path",
				  "Call to \"SMapGetObject\" failed"))
		return NULL;

	newPath = (SViterbiPath*)S_NEW("SViterbiPath", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "extend_path",
				  "Failed to create new 'SViterbiPath' object"))
		return NULL;

	newPath->from = p;
	newPath->state = c->pos;
	newPath->c = c;

	if ((p == NULL) || (p->c == NULL))
	{
		newPath->score = c->score;  /* cost = 0, nothing previous to join to */
		SMapSetFloat(newPath->features, "lscore", 0.0, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "extend_path",
					  "Call to \"SMapSetFloat\" failed"))
		{
			S_DELETE(newPath, "extend_path", error);
			return NULL;
		}
	}
	else
	{
		left = (SItem*)p->c->name;
		right = (SItem*)c->name;

		/* ABY :check this in taylor */
		is_contiguous = units_contiguous(left, right, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "extend_path",
					  "Call to \"units_contiguous\" failed"))
		{
			S_DELETE(newPath, "extend_path", error);
			return NULL;
		}

		if (is_contiguous)
		{
			/* default to zero cost if units contiguous in database */
			cost = 0.0;
		}
		else
		{
			cost = run_cost_functions(joinCosts, left, right, "join costs", error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "extend_path",
						  "Call to \"run_cost_functions\" failed"))
			{
				S_DELETE(newPath, "extend_path", error);
				return NULL;
			}
		}

		SMapSetFloat(newPath->features, "lscore", cost, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "extend_path",
					  "Call to \"SMapSetFloat\" failed"))
		{
			S_DELETE(newPath, "extend_path", error);
			return NULL;
		}

		/* ABY: here tc 2 jc weights apply */
		newPath->score = c->score + p->score + cost;
	}

	return newPath;
}


/*
 *A function to count the pauses. This is a hack to limit the number
 * of pauses that are used in candidates for selection. It speeds up
 * the selection process. Note that this makes an assumption that the
 * silence phone of the phoneset is '#'.
 */
static void count_pauses(SItem *item, uint32 *counter, s_erc *error)
{
	int scomp;
	const char *name;


	name = SItemGetName(item, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "count_pauses",
				  "Call to \"SItemGetName\" failed"))
		return;

	scomp = s_strcmp(name, "left-#", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "count_pauses",
				  "Call to \"s_strcmp\" failed"))
		return;

	if (scomp == 0)
	{
		(*counter)++;
		return;
	}

	scomp = s_strcmp(name, "right-#", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "count_pauses",
				  "Call to \"s_strcmp\" failed"))
		return;

	if (scomp == 0)
	{
		(*counter)++;
		return;
	}
}


static SViterbiCandidate *get_candidates(const SItem *s, SMap *features, s_erc *error)
{
	SViterbiCandidate *candidate = NULL;
	SViterbiCandidate *allCandidates = NULL;
	const SHalfphoneDB *unitDB;
	const SList *targetCosts;
	sint32 min_3_gram;
	sint32 min_2_gram;
	const char *left_context;
	const char *right_context;
	const char *unit_name;
	const SList *candidateList;
	SIterator *itr = NULL;
	SItem *candidateItem;
	uint32 hack_counter = 0;


	S_CLR_ERR(error);

	unitDB = (SHalfphoneDB*)SMapGetObject(features, "unit_db", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_candidates",
				  "Call to \"SMapGetObject\" failed"))
		goto quit_error;

	targetCosts = (SList*)SMapGetObject(features, "target costs", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_candidates",
				  "Call to \"SMapGetObject\" failed"))
		goto quit_error;

	/* init with big value (if not defined) so that we dont get cutoff */
	min_3_gram = SMapGetIntDef(features, "min-3-gram-context", 1000000, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_candidates",
				  "Call to \"SMapGetIntDef\" failed"))
		goto quit_error;

	/* init with big value (if not defined) so that we dont get cutoff */
	min_2_gram = SMapGetIntDef(features, "min-2-gram-context", 1000000, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_candidates",
				  "Call to \"SMapGetIntDef\" failed"))
		goto quit_error;

	unit_name = SItemGetName(s, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_candidates",
				  "Call to \"SItemGetName\" failed"))
		goto quit_error;

	/* get the item's context */
	left_context = SItemGetString(s, "left_context", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_candidates",
				  "Call to \"SItemGetString\" failed"))
		goto quit_error;

	right_context = SItemGetString(s, "right_context", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_candidates",
				  "Call to \"SItemGetString\" failed"))
		goto quit_error;

	candidateList = S_HALFPHONE_DB_CALL(unitDB, get_units)(unitDB, unit_name,
														   left_context, right_context,
														   (uint)min_3_gram, (uint)min_2_gram,
														   error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_candidates",
				  "Call to SHalfPhoneDB method \"get_units\" failed"))
		goto quit_error;

	if (candidateList == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "get_candidates",
				  "Failed to find candidates in unit database for unit type '%s'",
				  unit_name);
		goto quit_error;
	}

	itr = SListIterator(candidateList, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_candidates",
				  "Call to \"SListIterator\" failed"))
		goto quit_error;

	/* now create the candidate list */
	while (itr != NULL)
	{
		candidate = (SViterbiCandidate*)S_NEW("SViterbiCandidate", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_candidates",
					  "Failed to create new 'SViterbiCandidate' object"))
			goto quit_error;

		candidate->next = allCandidates;
		allCandidates = candidate;

		candidateItem = (SItem*)SListIteratorValue(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_candidates",
					  "Call to \"SListIteratorValue\" failed"))
			goto quit_error;

		candidate->name = (SObject*)candidateItem;  /* candidate */
		candidate->s = (SItem*)s;  /* target */

		count_pauses(candidateItem, &hack_counter, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_candidates",
					  "Call to \"count_pauses\" failed"))
			goto quit_error;

		/* calculate the target cost */
		candidate->score = run_cost_functions(targetCosts, s, candidateItem, "target costs", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_candidates",
					  "Call to \"run_cost_functions\" failed"))
			goto quit_error;

		itr = SIteratorNext(itr);

		/* limit the number of pauses used in selection to 15 */
		if (hack_counter == 15)
		{
			S_DELETE(itr, "get_candidates", error);
			break;
		}
	}

	return allCandidates;

quit_error:
	if (itr != NULL)
		S_DELETE(itr, "get_candidates", error);

	if (allCandidates != NULL)
		S_DELETE(allCandidates, "get_candidates", error);

	return NULL;
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


static void Run(const SUttProcessor *self, SUtterance *utt,
				s_erc *error)
{
	SRelation *unitRel;
	s_bool is_present;
	SHalfphoneDB *unitDB;
	const SVoice *voice;
	SViterbi *viterbi = NULL;
	const SMap *viterbiConfig;
	const SMap *halfphoneDBConfig;
	const SList *joinCosts;
	const SList *targetCosts;
	const SMap *allCostFunctions;


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

	unitRel = (SRelation*)SUtteranceGetRelation(utt, "Unit", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		goto quit_error;

	voice = SUtteranceVoice(utt, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceVoice\" failed"))
		goto quit_error;

	if (voice == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "SSelectUnitsUttProc utterance processor requires an utterance to have a voice");
		goto quit_error;
	}

	/* get "unit selection cost functions" in voice features */
	allCostFunctions = (const SMap*)SVoiceGetFeature(voice, "unit selection cost functions", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SVoiceGetFeature\" failed"))
		goto quit_error;

	/* nothing, return */
	if (allCostFunctions == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to get 'unit selection cost functions' in voice features");
		goto quit_error;
	}

	/* get target cost functions */
	targetCosts = (SList*)SMapGetObjectDef(allCostFunctions, "target costs", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto quit_error;

	/* get join cost functions */
	joinCosts = (SList*)SMapGetObjectDef(allCostFunctions, "join costs", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto quit_error;

	/* get halfphone unit database */
	unitDB = (SHalfphoneDB*)SVoiceGetData(voice, "unit_db", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SVoiceGetData\" failed"))
		goto quit_error;

	if (unitDB == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find unit database 'unit_db', in voice data");
		goto quit_error;
	}

	/* get halfphone db config params */
	halfphoneDBConfig = (SMap*)SMapGetObjectDef(self->features, "halfphone database config", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto quit_error;

	/* get viterbi config params */
	viterbiConfig = (SMap*)SMapGetObjectDef(self->features, "viterbi config", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto quit_error;

	/* create viterbi */
	viterbi = (SViterbi*)S_NEW("SViterbi", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Failed to create new 'SViterbi' object"))
		goto quit_error;

	/* initialize viterbi, (-1 = dynamic states) */
	S_VITERBI_CALL(viterbi, init_viterbi)(&viterbi, unitRel,
										  get_candidates, extend_path,
										  -1, viterbiConfig, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Failed to initialize new 'SViterbi' object"))
		goto quit_error;

	if (halfphoneDBConfig != NULL)
	{
		const SObject *tmp;


		tmp = SMapGetObjectDef(halfphoneDBConfig, "min-3-gram-context", NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SMapGetObjectDef\" failed"))
			goto quit_error;

		if (tmp != NULL)
		{
			S_VITERBI_CALL(viterbi, set_feature)(viterbi, "min-3-gram-context", tmp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to SViterbi method \"set_feature\" failed"))
				goto quit_error;
		}

		tmp = SMapGetObjectDef(halfphoneDBConfig, "min-2-gram-context", NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SMapGetObjectDef\" failed"))
			goto quit_error;

		if (tmp != NULL)
		{
			S_VITERBI_CALL(viterbi, set_feature)(viterbi, "min-2-gram-context", tmp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "Run",
						  "Call to SViterbi method \"set_feature\" failed"))
				goto quit_error;
		}
	}

	S_VITERBI_CALL(viterbi, set_feature)(viterbi, "unit_db", S_OBJECT(unitDB), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to SViterbi method \"set_feature\" failed"))
		goto quit_error;

	if (targetCosts != NULL)
	{
		S_VITERBI_CALL(viterbi, set_feature)(viterbi, "target costs", S_OBJECT(targetCosts), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to SViterbi method \"set_feature\" failed"))
			goto quit_error;
	}

	if (joinCosts != NULL)
	{
		S_VITERBI_CALL(viterbi, set_feature)(viterbi, "join costs", S_OBJECT(joinCosts), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to SViterbi method \"set_feature\" failed"))
			goto quit_error;
	}

	/* viterbi search */
	S_VITERBI_CALL(viterbi, search)(viterbi, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to SViterbi method \"search\" failed"))
		goto quit_error;

	/* copy best source units to units relation under "source-unit" feature */
	S_VITERBI_CALL(viterbi, copy_best_result)(viterbi, "source-unit", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to SViterbi method \"copy_best_result\" failed"))
		goto quit_error;

	/* here all is OK */
	S_DELETE(viterbi, "Run", error);
	return;



/* error clean up and normal clean up */
quit_error:
	if (viterbi != NULL)
		S_DELETE(viterbi, "Run", error);
}


/************************************************************************************/
/*                                                                                  */
/* SSelectUnitsUttProc class initialization                                         */
/*                                                                                  */
/************************************************************************************/

static SSelectUnitsUttProcClass SelectUnitsUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SSelectUnitsUttProc",
		sizeof(SSelectUnitsUttProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SUttProcessorClass */
	NULL,                /* initialize */
	Run                  /* run        */
};
