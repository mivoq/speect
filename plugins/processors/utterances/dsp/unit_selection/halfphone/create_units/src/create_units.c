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
/* An utterance processor to create the halfphone units relation stream.            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "create_units.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* SNormalizationUttProc class declaration. */
static SCreateUnitsUttProcClass CreateUnitsUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_create_units_utt_proc_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&CreateUnitsUttProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_create_units_utt_proc_class_reg",
			  "Failed to register SCreateUnitsUttProcClass");
}


S_LOCAL void _s_create_units_utt_proc_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&CreateUnitsUttProcClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_create_units_utt_proc_class_free",
			  "Failed to free SCreateUnitsUttProcClass");
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
	const SRelation *segmentRel;
	SRelation *unitRel = NULL;
	s_bool is_present;
	SItem *segmentItem;
	SItem *leftUnit;
	SItem *rightUnit;
	const char *segment_item_name;
	char *left_unit_name = NULL;
	char *right_unit_name = NULL;
	size_t size;


	S_CLR_ERR(error);

	/* we require the segment relation */
	is_present = SUtteranceRelationIsPresent(utt, "Segment", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceRelationIsPresent\" failed"))
		goto quit_error;

	if (!is_present)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Run",
				  "Failed to find 'Segment' relation in utterance");
		goto quit_error;
	}

	segmentRel = SUtteranceGetRelation(utt, "Segment", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceGetRelation\" failed"))
		goto quit_error;


	/* create the unit relation */
	unitRel = SUtteranceNewRelation(utt, "Unit", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceNewRelation\" failed"))
		goto quit_error;

	/* start at the first item in the segment relation, cast away
	 * const, we want to add daughter items
	 */
	segmentItem = (SItem*)SRelationHead(segmentRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	while (segmentItem != NULL)
	{
		/* left halfphone */
		leftUnit = SRelationAppend(unitRel, NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SRelationAppend\" failed"))
			goto quit_error;

		/* right halfphone */
		rightUnit = SRelationAppend(unitRel, NULL, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SRelationAppend\" failed"))
			goto quit_error;

		/* halfphones are daughters of segment items */
		SItemAddDaughter(segmentItem, leftUnit, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemAddDaughter\" failed"))
			goto quit_error;

		SItemAddDaughter(segmentItem, rightUnit, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemAddDaughter\" failed"))
			goto quit_error;

		segment_item_name = SItemGetName(segmentItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemGetName\" failed"))
			goto quit_error;

		/* add the halfphone names */
		size = s_strzsize(segment_item_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_strzsize\" failed"))
			goto quit_error;

		/* "left-"(5) + segment_item_name */
		left_unit_name = S_CALLOC(char, 5 + size);
		if (left_unit_name == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "Run",
					  "Failed to allocate memory for 'char' object");
			goto quit_error;
		}

		s_strcat(left_unit_name, "left-", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_strcat\" failed"))
			goto quit_error;

		s_strcat(left_unit_name, segment_item_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_strcat\" failed"))
			goto quit_error;

		SItemSetName(leftUnit, left_unit_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemSetName\" failed"))
			goto quit_error;

		S_FREE(left_unit_name);

		/* "right-"(6) + segment_item_name */
		right_unit_name = S_CALLOC(char, 6 + size);
		if (right_unit_name == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "Run",
					  "Failed to allocate memory for 'char' object");
			goto quit_error;
		}

		s_strcat(right_unit_name, "right-", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_strcat\" failed"))
			goto quit_error;

		s_strcat(right_unit_name, segment_item_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"s_strcat\" failed"))
			goto quit_error;

		SItemSetName(rightUnit, right_unit_name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemSetName\" failed"))
			goto quit_error;

		S_FREE(right_unit_name);

		segmentItem = SItemNext(segmentItem, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "Run",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	/* here all is OK */
	return;

	/* error clean-up code */
quit_error:
	if (unitRel != NULL)
	{
		SUtteranceDelRelation(utt, "Unit", error);
		S_CHK_ERR(error, S_CONTERR,
				  "Run",
				  "Call to \"SUtteranceDelRelation\" failed");
	}

	if (left_unit_name != NULL)
		S_FREE(left_unit_name);

	if (right_unit_name != NULL)
		S_FREE(right_unit_name);

	self = NULL; /* to suppress noise about unused parameters */
}


/************************************************************************************/
/*                                                                                  */
/* SCreateUnitsUttProc class initialization                                         */
/*                                                                                  */
/************************************************************************************/

static SCreateUnitsUttProcClass CreateUnitsUttProcClass =
{
	/* SObjectClass */
	{
		"SUttProcessor:SCreateUnitsUttProc",
		sizeof(SCreateUnitsUttProc),
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
