/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* DATE    : 8 September 2008                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Viterbi decoder implementation.                                                */
/* Loosly based on EST_Viterbi of Edinburgh Speech Tools,                           */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96).                         */
/* Note that this is a derived work of EST_Viterbi, some comments from EST_Viterbi  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* Edinburgh Speech Tools license,  EST_Viterbi                                     */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                            Copyright (c) 1996,1997                               */
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
/*       Author :  Alan W Black                                                     */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "viterbi.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

#define vit_big_number 1.0e10


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SViterbiClass ViterbiClass;


/************************************************************************************/
/*                                                                                  */
/* Static function definitions                                                      */
/*                                                                                  */
/************************************************************************************/

static void init_paths_array(SViterbiPoint *n, int num_states, s_erc *error);

static void prune_initialize(SViterbi *self, SViterbiPoint *p,
							 double *best_score, double *best_candidate_score,
							 double *score_cutoff, double *candidate_cutoff,
							 int *cand_count);

static void init_dynamic_states(SViterbiPoint *p, SViterbiCandidate *cands,
								s_erc *error);

static s_bool better_than(SViterbi *self, const float a, const float b);

static void add_path(SViterbi *self, SViterbiPoint *p, SViterbiPath *np, s_erc *error);

static void add_paths(SViterbi *self, SViterbiPoint *pt, SViterbiPath *np, s_erc *error);

static SViterbiPath *find_best_end(SViterbi *self, s_erc *error);

static void debug_output(SViterbiPoint *p, SViterbiCandidate *c, SViterbiPath *np,
						 int i, s_erc *error);

static void set_viterbi_params(SViterbi *self, const SMap *viterbiConfig, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* local functions to register and free classes */
S_LOCAL void _s_viterbi_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&ViterbiClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_viterbi_class_reg",
			  "Failed to register SViterbiClass");
}


S_LOCAL void _s_viterbi_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&ViterbiClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_viterbi_class_free",
			  "Failed to free SViterbiClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void init_paths_array(SViterbiPoint *n, int num_states, s_erc *error)
{
	/* Create the states array and initialize it to NULL (calloc) */
	n->num_states = num_states;
	n->stPaths = S_CALLOC(SViterbiPath*, num_states);
	if (n->stPaths == NULL)
		S_FTL_ERR(error, S_MEMERROR,
				  "init_paths_array",
				  "Failed to allocate memory for 'SViterbiPath*' object");
}


static s_bool better_than(SViterbi *self, const float a, const float b)
{
	/*
	 * Some thing big is better, others want it to be as small as possible
	 * this just tells you if a is better than b by checking the variable
	 * in the decoder itself.
	 */

	/* For probabilities big_is_good == TRUE (or log probabilities) */

	if (self->big_is_good == TRUE)
		return (a > b);
	else
		return (a < b);
}


/* Find best candidate, count them and set some vars.*/
static void prune_initialize(SViterbi *self, SViterbiPoint *p,
							 double *best_score, double *best_candidate_score,
							 double *score_cutoff, double *candidate_cutoff,
							 int *cand_count)
{
	SViterbiCandidate *c;


	if (self->big_is_good == TRUE)
	{
		(*best_score) = -vit_big_number;
		(*best_candidate_score) = -vit_big_number;
		(*score_cutoff) = -vit_big_number;
		(*candidate_cutoff) = -self->cand_prune_envelope_width;
	}
	else
	{
		(*best_candidate_score) = vit_big_number;
		(*best_score) = vit_big_number;
		(*score_cutoff) = vit_big_number;
		(*candidate_cutoff) = self->cand_prune_envelope_width;
	}

	for ((*cand_count) = 0, c = p->cands; c; c = c->next, (*cand_count)++)
		if (better_than(self, c->score, (*best_candidate_score)))
			(*best_candidate_score) = c->score;

	(*candidate_cutoff) += (*best_candidate_score);
}


static void init_dynamic_states(SViterbiPoint *p, SViterbiCandidate *cands, s_erc *error)
{
	/*
	 * In a special (hmm maybe not so special), the number of "states"
	 * is the number of candidates
	 */
	SViterbiCandidate *c;
	int i;


	S_CLR_ERR(error);
	for (i = 0, c = cands; c != NULL; c = c->next, i++)
		c->pos = i;

	init_paths_array(p,i, error);
	S_CHK_ERR(error, S_CONTERR,
			  "init_dynamic_states",
			  "Call to \"init_paths_array\" failed");
}


static void add_path(SViterbi *self, SViterbiPoint *p, SViterbiPath *np, s_erc *error)
{
	/*
	 * We are doing true viterbi so we need only keep the best
	 * path for each state.  This means we can index into the
	 * array of paths ending at P and only keep np if its score
	 * is better than any other path of that state
	 */
	S_CLR_ERR(error);

	if ((np->state < 0) || (np->state > p->num_states))
	{
		S_CTX_ERR(error, S_FAILURE,
				  "add_path",
				  "Path state error, %d", np->state);
		return;
	}
	else if ((p->stPaths[np->state] == NULL)
			 || (better_than(self, np->score, p->stPaths[np->state]->score)))
	{
		/*  This new one is better so replace it */
		if (p->stPaths[np->state] != NULL)
			S_DELETE(p->stPaths[np->state], "add_path", error);
		p->stPaths[np->state] = np;
	}
	else
		S_DELETE(np, "add_path", error);
}


static void add_paths(SViterbi *self, SViterbiPoint *pt, SViterbiPath *np, s_erc *error)
{
	/* Add set of paths */
	SViterbiPath *p;
	SViterbiPath *nextP;


	S_CLR_ERR(error);

	for (p = np; p != NULL; p = nextP)
	{
		nextP = p->next;  /* need to save this as p could be deleted */

		add_path(self, pt, p, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "add_paths",
					  "Call to \"add_path\" failed"))
			return;
	}
}


/* Add new path to point,  Prunes as appropriate to strategy */
static void general_add_path(SViterbi *self, SViterbiPoint *p, SViterbiPath *np, s_erc *error)
{
	SViterbiPath *pp;


	S_CLR_ERR(error);

	if (p == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "general_add_path",
				  "Tried to add path to NULL point");
		return;
	}

	if ((self->beam_width == 0) ||                        /* Add if no beam restrictions or */
	    (p->num_paths < self->beam_width) ||              /* beam not filled  or            */
	    (better_than(self, np->score, p->paths->score)))  /* this is better than best       */
	{
		SViterbiPath **l = &p->paths;
		SViterbiPath *a;

		for (a = p->paths; /* scary */ ; a = a->next)
		{
			if ((a == NULL) || (better_than(self, a->score, np->score)))
			{   /* Add new path here */
				np->next = a;
				*l = np;
				p->num_paths++;
				break;
			}
			l = &a->next;
		}

		/* Prune the first one of the list */
		if ((self->beam_width > 0) &&
		    (p->num_paths > self->beam_width))
		{
			pp = p->paths;
			p->paths = pp->next;
			pp->next = NULL;
			p->num_paths--;
			S_DELETE(pp, "general_add_path", error);
		}
	}
	else
	{
		S_DELETE(np, "general_add_path", error);  /* failed to make it */
	}
}


static SViterbiPath *find_best_end(SViterbi *self, s_erc *error)
{
	SViterbiPoint *t;
	double best = 0.0;
	double worst = 0.0;
	SViterbiPath *p;
	SViterbiPath *bestP;
	int i;


	S_CLR_ERR(error);

	if (self->big_is_good == TRUE)
		worst = -vit_big_number;	/* worst possible */
	else
		worst = vit_big_number;	    /* worst possible */

	best = worst;		/* hopefully we can find something better */

	for (i = 0, t = self->timeLine; t->next != NULL; t = t->next, i++)
	{
		if ((t->num_states == 0) && (t->num_paths == 0))
		{
			S_CTX_ERR(error, S_FAILURE,
					  "find_best_end",
					  "No paths at frame %d", i);
			return NULL;
		}
	}

	if (self->num_states != 0)
	{
		for (i = 0; i < t->num_states; i++)
		{
			if ((t->stPaths[i] != NULL) &&
			    (better_than(self, t->stPaths[i]->score, best)))
			{
				best = t->stPaths[i]->score;
				bestP = t->stPaths[i];
			}
		}
	}
	else
	{
		for (p = t->paths; p != NULL; p = p->next)
		{
			if (better_than(self, p->score, best))
			{
				best = p->score;
				bestP = p;
			}
		}
	}

	return bestP;
}


static void debug_output(SViterbiPoint *p, SViterbiCandidate *c, SViterbiPath *np,
						 int i, s_erc *error)
{
 	char *buf;
	const char *name;
	float lscore;


	S_CLR_ERR(error);

	lscore = SMapGetFloat(np->features, "lscore", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "debug_output",
				  "Call to \"SMapGetFloat\" failed"))
		return;

	name = SItemGetName(p->s, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "debug_output",
				  "Call to \"SItemGetName\" failed"))
		return;

	S_DEBUG(S_DBG_INFO, "Point item name '%s'", name);

	buf = SObjectPrint(c->name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "debug_output",
				  "Call to \"SObjectPrint\" failed"))
		return;

	if (buf != NULL)
	{
		S_DEBUG(S_DBG_INFO, "Candidate item:\n '%s'", buf);
		S_FREE(buf);
	}

	S_DEBUG(S_DBG_INFO, "%1.3f B %1.3f (%1.3f) st %d s %1.3f ",
			np->c->score,
			(np->c->score == 0 ? 0 :
			 (lscore/np->c->score)), lscore, np->state,
			np->score);

	if (p->stPaths[i] == NULL)
		S_DEBUG(S_DBG_INFO, "(I)\n");
	else
	{
		S_DEBUG(S_DBG_INFO, "\n");
		buf = SObjectPrint(p->stPaths[i]->c->name, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "debug_output",
					  "Call to \"SObjectPrint\" failed"))
			return;

		if (buf != NULL)
		{
			S_DEBUG(S_DBG_INFO, "State path [%d] item:\n '%s'", i, buf);
			S_FREE(buf);
		}
	}

	S_DEBUG(S_DBG_INFO, ".........................");
}


static void set_viterbi_params(SViterbi *self, const SMap *viterbiConfig, s_erc *error)
{
	const char *tmp;
	int tmp_int;
	float tmp_float;
	int scomp;


	S_CLR_ERR(error);
	tmp = SMapGetStringDef(viterbiConfig, "debug", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_viterbi_params",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp != NULL)
	{
		scomp = s_strcmp("TRUE", tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_viterbi_params",
					  "Call to \"s_strcmp\" failed"))
			return;

		if (scomp == 0)
			self->debug = TRUE;
	}

	tmp = SMapGetStringDef(viterbiConfig, "trace", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_viterbi_params",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp != NULL)
	{
		scomp = s_strcmp("TRUE", tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_viterbi_params",
					  "Call to \"s_strcmp\" failed"))
			return;

		if (scomp == 0)
			self->trace = TRUE;
	}

	tmp_int = SMapGetIntDef(viterbiConfig, "beam-width", -33121, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_viterbi_params",
				  "Call to \"SMapGetIntDef\" failed"))
		return;

	if (tmp_int != -33121)
		self->beam_width = tmp_int;

	tmp_int = SMapGetIntDef(viterbiConfig, "cand-width", -33121, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_viterbi_params",
				  "Call to \"SMapGetIntDef\" failed"))
		return;

	if (tmp_int != -33121)
		self->cand_width = tmp_int;

	tmp = SMapGetStringDef(viterbiConfig, "do-pruning", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_viterbi_params",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp != NULL)
	{
		scomp = s_strcmp("TRUE", tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_viterbi_params",
					  "Call to \"s_strcmp\" failed"))
			return;

		if (scomp == 0)
			self->do_pruning = TRUE;
	}

	if (self->do_pruning)
	{
		tmp_float = SMapGetFloatDef(viterbiConfig, "path-prune-envelope-width", 0.0, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_viterbi_params",
					  "Call to \"SMapGetFloatDef\" failed"))
			return;

		if (tmp_float != 0)
			self->path_prune_envelope_width = tmp_float;

		tmp_float = SMapGetFloatDef(viterbiConfig, "cand-prune-envelope-width", 0.0, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "set_viterbi_params",
					  "Call to \"SMapGetFloatDef\" failed"))
			return;

		if (tmp_float != 0)
			self->cand_prune_envelope_width = tmp_float;
	}
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SViterbi *self = obj;


	S_CLR_ERR(error);
	self->num_states = 0;
	self->beam_width = 0;
	self->cand_width = 0;
	self->big_is_good = FALSE; /* for probabilities it is */
	self->userPathFunc = NULL;
	self->userCandFunc = NULL;
	self->timeLine = NULL;
	self->debug = FALSE;
	self->trace = FALSE;
	self->do_pruning = FALSE;
	self->path_prune_envelope_width = -1.0;
	self->cand_prune_envelope_width = -1.0;

	self->features = (SMap*)S_NEW("SMapList", error);
	S_CHK_ERR(error, S_CONTERR,
			  "Init",
			  "Failed to create new 'SMap' object");
}


static void Destroy(void *obj, s_erc *error)
{
	SViterbi *self = obj;

	if (self->features != NULL)
		S_DELETE(self->features, "Destroy", error);

	if (self->timeLine != NULL)
		S_DELETE(self->timeLine, "Destroy", error);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/* Creates a time line with each point pointing at each item in p */
static void InitViterbi(SViterbi **self, SRelation *rel,
						candFunc userCandFunc, pathFunc userPathFunc,
						int num_states, const SMap *config, s_erc *error)
{
	const SItem *item;
	SViterbiPoint *t;
	SViterbiPoint *n;


	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "InitViterbi",
				  "Argument \"self\" is NULL");
		goto quit_error;
	}

	if (rel == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "InitViterbi",
				  "Argument \"rel\" is NULL");
		goto quit_error;
	}

	if (userCandFunc == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "InitViterbi",
				  "Argument \"userCandFunc\" is NULL");
		goto quit_error;
	}

	if (userPathFunc == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "InitViterbi",
				  "Argument \"userPathFunc\" is NULL");
		goto quit_error;
	}

	/* set user callback functions */
	(*self)->userCandFunc = userCandFunc;
	(*self)->userPathFunc = userPathFunc;

	/* set number of states */
	(*self)->num_states = num_states;

	if (config != NULL)
	{
		set_viterbi_params(*self, config, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "InitViterbi",
					  "Call to \"set_viterbi_params\" failed"))
			goto quit_error;
	}

	item = SRelationHead(rel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "InitViterbi",
				  "Call to \"SRelationHead\" failed"))
		goto quit_error;

	t = NULL;
	while (item != NULL)
	{
		n = (SViterbiPoint*)S_NEW("SViterbiPoint", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "InitViterbi",
					  "Failed to create new 'SViterbiPoint' object"))
			goto quit_error;
		n->s = item;

		if ((*self)->num_states > 0)
		{
			init_paths_array(n, (*self)->num_states, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "InitViterbi",
						  "Call to \"init_paths_array\" failed"))
				goto quit_error;
		}

		if (t == NULL)
			(*self)->timeLine = n;
		else
			t->next = n;

		t = n;

		item = SItemNext(item, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "InitViterbi",
					  "Call to \"SItemNext\" failed"))
			goto quit_error;
	}

	/* Extra one at the end for final paths */
	n = (SViterbiPoint*)S_NEW("SViterbiPoint", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "InitViterbi",
				  "Failed to allocate new 'SViterbiPoint' object"))
		goto quit_error;

	if ((*self)->num_states > 0)
	{
		init_paths_array(n, (*self)->num_states, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "InitViterbi",
					  "Call to \"init_paths_array\" failed"))
			goto quit_error;
	}

	/* Need init path on first point so a search can start */
	if ((*self)->num_states == 0)   /* general search case */
	{
		(*self)->timeLine->paths = (SViterbiPath*)S_NEW("SViterbiPath", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "InitViterbi",
					  "Failed to allocate new 'SViterbiPath' object"))
			goto quit_error;
	}

	if ((*self)->num_states == -1)
	{
		init_paths_array((*self)->timeLine, 1, error); /* a start point */
		if (S_CHK_ERR(error, S_CONTERR,
					  "InitViterbi",
					  "Call to \"init_paths_array\" failed"))
			goto quit_error;
	}

	if (t == NULL)
		(*self)->timeLine = n;   /* its an empty stream */
	else
		t->next = n;

	/* all OK here */
	return;

	/* error cleanup code here */
quit_error:
	if (*self != NULL)
	{
		S_DELETE(*self, "InitViterbi", error);
		*self = NULL;
	}
}


static void SetBeamWidth(SViterbi *self, int width, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetBeamWidth",
				  "Argument \"self\" is NULL");
		return;
	}
	self->beam_width = width;
}


static void SetCandWidth(SViterbi *self, int width, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetCandWidth",
				  "Argument \"self\" is NULL");
		return;
	}
	self->cand_width = width;
}


static void SetBigIsGood(SViterbi *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetBigIsGood",
				  "Argument \"self\" is NULL");
		return;
	}
	self->big_is_good = TRUE;
}


static void SetDebug(SViterbi *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetDebug",
				  "Argument \"self\" is NULL");
		return;
	}
	self->debug = TRUE;
}


static void SetTrace(SViterbi *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetTrace",
				  "Argument \"self\" is NULL");
		return;
	}
	self->trace = TRUE;
}


static void SetFeature(SViterbi *self, const char *key,
					   const SObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetFeature",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetFeature",
				  "Argument \"key\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetFeature",
				  "Argument \"object\" is NULL");
		return;
	}

	SMapSetObject(self->features, key, object, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetFeature",
			  "Call to \"SMapSetObject\" failed");
}


static void SetPruningParams(SViterbi *self, float cand_beam, float path_beam,
							 s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SetPruningParams",
				  "Argument \"self\" is NULL");
		return;
	}

	self->do_pruning = TRUE;
	self->path_prune_envelope_width = path_beam;
	self->cand_prune_envelope_width = cand_beam;
}


/* Searches for the best path */
static void Search(SViterbi *self, s_erc *error)
{
	SViterbiPoint *p;
	SViterbiPath *t;
	SViterbiPath *np;
	SViterbiCandidate *c;
	int i;
	double best_score = 0.0;
	double score_cutoff = 0.0;
	double best_candidate_score;
	double candidate_cutoff;
	int dcount;
	int pcount;
	int cand_count;
	int cands_considered;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "Search",
				  "Argument \"self\" is NULL");
		return;
	}

	for (p = self->timeLine; p->next != NULL; p = p->next)
	{
		/* For each point in time find the candidates */
		p->cands = (*self->userCandFunc)(p->s, self->features, error);  /* P(S|B) */
		if (S_CHK_ERR(error, S_CONTERR,
					  "Search",
					  "Call to \"userCandFunc\" failed"))
			return;


		if (self->do_pruning == TRUE)
		{
			prune_initialize(self, p, &best_score,
							 &best_candidate_score, &score_cutoff,
							 &candidate_cutoff, &cand_count);
		}

		if (self->num_states != 0)  /* true viterbi -- optimized for states */
		{
			if (self->num_states == -1)  /* special case, dynamic state size */
			{
				init_dynamic_states(p->next, p->cands, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "Search",
							  "Call to \"init_dynamic_states\" failed"))
					return;
			}

			cands_considered = 0; /* moved by simonk */

			for (i = 0; i < p->num_states; i++)
			{
				/* for each path up to here */

				/* cands_considered = 0; simonk */

				if (((p == self->timeLine) && i == 0) || (p->stPaths[i] != NULL))
				{
					for (c = p->cands; c != NULL; c = c->next)
					{
						/*
						 * for each new candidate
						 * candidate pruning (a.k.a. observation pruning)
						 */
						if ((self->do_pruning == FALSE)
						    || better_than(self, c->score, candidate_cutoff))
						{
							cands_considered++;

							/* Find path extension costs */
							np = (*self->userPathFunc)(p->stPaths[i], c, self->features, error);
							if (S_CHK_ERR(error, S_CONTERR,
										  "Search",
										  "Call to \"userPathFunc\" failed"))
								return;

							if (self->debug == TRUE)
								/* not going to halt if error occurs here. */
								debug_output(p,c,np,i, error);

							if ((self->do_pruning == TRUE)
							    && better_than(self, np->score, best_score))
							{
								best_score = np->score;
								if (self->big_is_good == TRUE)
									score_cutoff = best_score
										- self->path_prune_envelope_width;
								else
									score_cutoff = best_score
										+ self->path_prune_envelope_width;
							}
							/*
							 * can prune here, although score_cutoff will
							 * be generally too generous at this point.
							 * It's unclear whether this pruning takes more
							 * time than it saves !
							 */
							if ((self->do_pruning == FALSE)
							    || better_than(self, np->score, score_cutoff))
							{
								add_paths(self, p->next, np, error);
								if (S_CHK_ERR(error, S_CONTERR,
											  "Search",
											  "Call to \"add_paths\" failed"))
									return;
							}
							else
								S_DELETE(np, "Search", error);
						}
					}
				}
			}

			if (self->do_pruning == TRUE)
			{
				if (self->big_is_good == TRUE)
					score_cutoff =
						best_score - self->path_prune_envelope_width;
				else
					score_cutoff =
						best_score + self->path_prune_envelope_width;

				if (self->trace == TRUE)
				{
					S_DEBUG(S_DBG_INFO,
							"SViterbi->search: Considered %d of "
							"%d candidate paths.\n FRAME: best score %f; "
							"score cutoff %f\n best candidate score %f; "
							"candidate cutoff %f", cands_considered,
							cand_count * p->num_states,  best_score,
							score_cutoff, best_candidate_score,
							candidate_cutoff);
				}

				dcount = 0;
				pcount = 0;

				for (i = 0; i < p->next->num_states; i++)
				{
					if(p->next->stPaths[i] != NULL)
					{
						pcount++;
						if (!better_than(self, p->next->stPaths[i]->score,
										 score_cutoff))
						{
							S_DELETE(p->next->stPaths[i], "Search", error);
							p->next->stPaths[i] = NULL;
							dcount++;
						}
					}
				}

				if (self->trace == TRUE)
				{
					S_DEBUG(S_DBG_INFO, "SViterbi->search: Pruned %d of %d paths",
							dcount, pcount);
				}
			}
		}
		else     /* general beam search */
		{
			for (t = p->paths; t != NULL; t = t->next)
			{   /* for each path up to here */
				for (c = p->cands; c != NULL; c = c->next)
				{   /* for each new candidate */
					np = (*self->userPathFunc)(t,c,self->features, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "Search",
								  "Call to \"userPathFunc\" failed"))
						return;

					general_add_path(self, p->next, np, error);  /* be a little cleverer */
					if (S_CHK_ERR(error, S_CONTERR,
								  "Search",
								  "Call to \"general_add_path\" failed"))
						return;
				}
			}
		}
	}
}


/* Finds best path through the search space (previously searched) */
static const SViterbiPath *GetBestEnd(SViterbi *self, s_erc *error)
{
	SViterbiPath *bestPathEnd;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "GetBestEnd",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if ((self->timeLine == NULL) || (self->timeLine->next == NULL))
		return NULL;  /* it's an empty list so it has succeeded */

	bestPathEnd = find_best_end(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "GetBestEnd",
				  "Call to \"find_best_end\" failed"))
		return NULL;

	if (bestPathEnd == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "GetBestEnd",
				  "Viterbi failed to find an answer");
		return NULL;
	}

	return (const SViterbiPath*)bestPathEnd;
}


static void CopyBestResult(SViterbi *self, const char *feat_name, s_erc *error)
{
	/*
	 * Finds best path through the search space (previously searched)
	 * Adds best searched unit under feature name 'feat_name' to
	 * relation with which viterbi was initialized.
	 */

	SViterbiPath *bestPathEnd;
	const SObject *lscore;


	S_CLR_ERR(error);
	if ((self->timeLine == NULL) || (self->timeLine->next == NULL))
		return;  /* it's an empty list so it has succeeded */

	bestPathEnd = find_best_end(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "CopyBestResult",
				  "Call to \"find_best_end\" failed"))
		return;

	if (bestPathEnd == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "CopyBestResult",
				  "Viterbi failed to find an answer");
		return;
	}

	for (/* NOP */; bestPathEnd != NULL; bestPathEnd = bestPathEnd->from)
	{
		if (bestPathEnd->c != NULL)
		{
			SItemSetObject(bestPathEnd->c->s, feat_name, bestPathEnd->c->name, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "CopyBestResult",
						  "Call to \"SItemSetObject\" failed"))
				return;

			lscore = SMapGetObjectDef(bestPathEnd->features, "lscore", NULL, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "CopyBestResult",
						  "Call to \"SMapGetObjectDef\" failed"))
				return;

			if (lscore == NULL)
			{
				SItemSetFloat(bestPathEnd->c->s, "_score", 0.0, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "CopyBestResult",
							  "Call to \"SItemSetFloat\" failed"))
					return;
			}
			else
			{
				SItemSetObject(bestPathEnd->c->s, "_score", lscore, error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "CopyBestResult",
							  "Call to \"SItemSetObject\" failed"))
					return;
			}
		}
	}
}


/************************************************************************************/
/*                                                                                  */
/* SViterbi class initialization                                                    */
/*                                                                                  */
/************************************************************************************/

static SViterbiClass ViterbiClass =
{
	/* SObjectClass */
	{
		"SViterbi",
		sizeof(SViterbi),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SViterbiClass */
	InitViterbi,         /* init_viterbi       */
	SetBeamWidth,        /* set_beam_width     */
	SetCandWidth,        /* set_cand_width     */
	SetBigIsGood,        /* set_big_is_good    */
	SetDebug,            /* set_debug          */
	SetTrace,            /* set_trace          */
	SetFeature,          /* set_feature        */
	SetPruningParams,    /* set_pruning_params */
	Search,              /* search             */
	CopyBestResult,      /* copy_best_result   */
	GetBestEnd           /* get_best_end       */
};
