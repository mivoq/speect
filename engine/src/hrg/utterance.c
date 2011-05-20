/************************************************************************************/
/* Copyright (c) 2008-2011 The Department of Arts and Culture,                      */
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
/* DATE    : 1 November 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* The Utterance class implementation. An Utterance consists of a set of Relations  */
/* which in turn consists of a set of Items.                                        */
/*                                                                                  */
/* Loosely based on cst_utterance of Flite, http://cmuflite.org (1.3),              */
/* EST_Utterance of Edinburgh Speech Tools,                                         */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96) and Utterance of         */
/* FreeTTS, http://sourceforge.net/projects/freetts (1.2).                          */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned projects.                                                              */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*  Flite license, cst_utterance                                                    */
/*                                                                                  */
/*                       Language Technologies Institute                            */
/*                          Carnegie Mellon University                              */
/*                             Copyright (c) 1999                                   */
/*                             All Rights Reserved.                                 */
/*                                                                                  */
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
/*                                                                                  */
/*       CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK               */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE            */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author:  Alan W Black                                                      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* Edinburgh Speech Tools license,  EST_Utterance                                   */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                           Copyright (c) 1995,1996                                */
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
/* FreeTTS license, Utterance                                                       */
/*                                                                                  */
/*      Portions Copyright 2001-2004 Sun Microsystems, Inc.                         */
/*      Portions Copyright 1999-2001 Language Technologies Institute,               */
/*      Carnegie Mellon University.                                                 */
/*      All Rights Reserved.  Use is subject to license terms.                      */
/*                                                                                  */
/*      Permission is hereby granted, free of charge, to use and distribute         */
/*      this software and its documentation without restriction, including          */
/*      without limitation the rights to use, copy, modify, merge, publish,         */
/*      distribute, sublicense, and/or sell copies of this work, and to             */
/*      permit persons to whom this work is furnished to do so, subject to          */
/*      the following conditions:                                                   */
/*                                                                                  */
/*       1. The code must retain the above copyright notice, this list of           */
/*          conditions and the following disclaimer.                                */
/*       2. Any modifications must be clearly marked as such.                       */
/*       3. Original authors' names are not deleted.                                */
/*       4. The authors' names are not used to endorse or promote products          */
/*          derived from this software without specific prior written               */
/*          permission.                                                             */
/*                                                                                  */
/*      SUN MICROSYSTEMS, INC., CARNEGIE MELLON UNIVERSITY AND THE                  */
/*      CONTRIBUTORS TO THIS WORK DISCLAIM ALL WARRANTIES WITH REGARD TO THIS       */
/*      SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND           */
/*      FITNESS, IN NO EVENT SHALL SUN MICROSYSTEMS, INC., CARNEGIE MELLON          */
/*      UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR      */
/*      CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF      */
/*      USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR       */
/*      OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR      */
/*      PERFORMANCE OF THIS SOFTWARE.                                               */
/*                                                                                  */
/*      Authors: Willie Walker, Paul Lamere, Philip Kwok                            */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "hrg/utterance.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SUtteranceClass UtteranceClass; /* SUtterance class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SUtteranceInit(SUtterance **self, const SVoice *voice, s_erc *error)
{
	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceInit",
				  "Argument \"self\" is NULL");
		return;
	}

	(*self)->voice = (SVoice*)voice;
}


S_API const SVoice *SUtteranceVoice(const SUtterance *self, s_erc *error)
{
	const SVoice *voice;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceVoice",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_UTTERANCE_METH_VALID(self, voice))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SUtteranceVoice",
				  "Utterance method \"voice\" not implemented");
		return NULL;
	}

	voice = S_UTTERANCE_CALL(self, voice)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SUtteranceVoice",
				  "Call to class method \"voice\" failed"))
		return NULL;

	return voice;
}


S_API SRelation *SUtteranceNewRelation(SUtterance *self, const char *name, s_erc *error)
{
	SRelation *rel;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceNewRelation",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceNewRelation",
				  "Argument \"name\" is NULL");
		return NULL;
	}

	if (!S_UTTERANCE_METH_VALID(self, new_relation))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SUtteranceNewRelation",
				  "Utterance method \"new_relation\" not implemented");
		return NULL;
	}

	s_mutex_lock(&(self->utt_mutex));
	rel = S_UTTERANCE_CALL(self, new_relation)(self, name, error);
	s_mutex_unlock(&(self->utt_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "SUtteranceNewRelation",
				  "Call to class method \"new_relation\" failed"))
		return NULL;

	return rel;
}


S_API void SUtteranceSetRelation(SUtterance *self, SRelation *rel, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceSetRelation",
				  "Argument \"self\" is NULL");
		return;
	}

	if (rel == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceSetRelation",
				  "Argument \"rel\" is NULL");
		return;
	}

	if (rel->name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceSetRelation",
				  "Relation name is NULL. Can only set named relations.");
		return;
	}

	if (!S_UTTERANCE_METH_VALID(self, set_relation))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SUtteranceSetRelation",
				  "Utterance method \"set_relation\" not implemented");
		return;
	}

	s_mutex_lock(&(self->utt_mutex));
	S_UTTERANCE_CALL(self, set_relation)(self, rel, error);
	s_mutex_unlock(&(self->utt_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "SUtteranceSetRelation",
			  "Call to class method \"set_relation\" failed");
}


S_API const SRelation *SUtteranceGetRelation(const SUtterance *self, const char *name,
											 s_erc *error)
{
	const SRelation *rel;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceGetRelation",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceGetRelation",
				  "Argument \"name\" is NULL");
		return NULL;
	}

	if (!S_UTTERANCE_METH_VALID(self, get_relation))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SUtteranceGetRelation",
				  "Utterance method \"get_relation\" not implemented");
		return NULL;
	}

	s_mutex_lock((s_mutex*)&(self->utt_mutex));
	rel = S_UTTERANCE_CALL(self, get_relation)(self, name, error);
	s_mutex_unlock((s_mutex*)&(self->utt_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "SUtteranceGetRelation",
				  "Call to class method \"get_relation\" failed"))
		return NULL;

	return rel;
}


S_API void SUtteranceDelRelation(SUtterance *self, const char *name, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceDelRelation",
				  "Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceDelRelation",
				  "Argument \"name\" is NULL");
		return;
	}

	if (!S_UTTERANCE_METH_VALID(self, del_relation))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SUtteranceDelRelation",
				  "Utterance method \"del_relation\" not implemented");
		return;
	}

	s_mutex_lock(&(self->utt_mutex));
	S_UTTERANCE_CALL(self, del_relation)(self, name, error);
	s_mutex_unlock(&(self->utt_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "SUtteranceDelRelation",
			  "Call to class method \"del_relation\" failed");
}


S_API s_bool SUtteranceRelationIsPresent(const SUtterance *self, const char *name,
										 s_erc *error)
{
	s_bool have_rel;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceRelationIsPresent",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceRelationIsPresent",
				  "Argument \"name\" is NULL");
		return FALSE;
	}

	if (!S_UTTERANCE_METH_VALID(self, have_relation))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SUtteranceRelationIsPresent",
				  "Utterance method \"have_relation\" not implemented");
		return FALSE;
	}

	s_mutex_lock((s_mutex*)&(self->utt_mutex));
	have_rel = S_UTTERANCE_CALL(self, have_relation)(self, name, error);
	s_mutex_unlock((s_mutex*)&(self->utt_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "SUtteranceRelationIsPresent",
				  "Call to class method \"have_relation\" failed"))
		return FALSE;

	return have_rel;
}


S_API SList *SUtteranceRelationsKeys(const SUtterance *self, s_erc *error)
{
	SList *keys;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceRelationsKeys",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_UTTERANCE_METH_VALID(self, relations_keys))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SUtteranceRelationsKeys",
				  "Utterance method \"relations_keys\" not implemented");
		return NULL;
	}

	s_mutex_lock((s_mutex*)&(self->utt_mutex));
	keys = S_UTTERANCE_CALL(self, relations_keys)(self, error);
	s_mutex_unlock((s_mutex*)&(self->utt_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "SUtteranceRelationsKeys",
				  "Call to class method \"relations_keys\" failed"))
		return NULL;

	return keys;
}


S_LOCAL sint32 SUtteranceGetNextId(const SUtterance *self, s_erc *error)
{
	sint32 id;


	s_mutex_lock((s_mutex*)&(self->utt_id_mutex));

	id = SMapGetInt(self->features, "_id", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SUtteranceGetNextId",
				  "Call to \"SMapGetInt\" failed"))
	{
		s_mutex_unlock((s_mutex*)&(self->utt_id_mutex));
		return 0;
	}

	SMapSetInt(self->features, "_id", ++id, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SUtteranceGetNextId",
				  "Call to \"SMapSetInt\" failed"))
	{
		s_mutex_unlock((s_mutex*)&(self->utt_id_mutex));
		return 0;
	}

	s_mutex_unlock((s_mutex*)&(self->utt_id_mutex));

	return id;
}


S_API const SObject *SUtteranceGetFeature(const SUtterance *self,
										  const char *name,
										  s_erc *error)
{
	const SObject *object;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceGetFeature",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceGetFeature",
				  "Argument \"name\" is NULL");
		return NULL;
	}

	object = SMapGetObject(self->features, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SUtteranceGetFeature",
				  "Call to \"SMapGetObject\" failed"))
		return NULL;

	return object;
}


S_API void SUtteranceSetFeature(SUtterance *self, const char *name,
								const SObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceSetFeature",
				  "Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceSetFeature",
				  "Argument \"name\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceSetFeature",
				  "Argument \"object\" is NULL");
		return;
	}

	SMapSetObject(self->features, name, object, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SUtteranceSetFeature",
			  "Call to \"SMapSetObject\" failed");
}


S_API void SUtteranceDelFeature(SUtterance *self, const char *name,
								s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceDelFeature",
				  "Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceDelFeature",
				  "Argument \"name\" is NULL");
		return;
	}

	SMapObjectDelete(self->features, name, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SUtteranceDelFeature",
			  "Call to \"SMapObjectDelete\" failed");
}


S_API s_bool SUtteranceFeatureIsPresent(const SUtterance *self, const char *name,
										s_erc *error)
{
	s_bool feat_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceFeatureIsPresent",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceFeatureIsPresent",
				  "Argument \"name\" is NULL");
		return FALSE;
	}

	feat_present = SMapObjectPresent(self->features, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SUtteranceFeatureIsPresent",
				  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	return feat_present;
}


S_API SList *SUtteranceFeatKeys(const SUtterance *self, s_erc *error)
{
	SList *keys;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUtteranceFeatKeys",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	keys = SMapGetKeys(self->features, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SUtteranceFeatKeys",
				  "Call to \"SMapGetKeys\" failed"))
		return NULL;

	return keys;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_utterance_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&UtteranceClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_utterance_class_add",
			  "Failed to add SUtteranceClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitUtterance(void *obj, s_erc *error)
{
	SUtterance *self = obj;


	S_CLR_ERR(error);

	self->voice = NULL;

	self->features = S_MAP(S_NEW("SMapList", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "InitUtterance",
				  "Failed to create new map-list features"))
		return;

	self->relations = S_MAP(S_NEW("SMapList", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "InitUtterance",
				  "Failed to create new map-list relations"))
		return;

	/* item id's */
	SMapSetInt(self->features, "_id", 0, error);
	S_CHK_ERR(error, S_CONTERR,
			  "InitUtterance",
			  "Call to SMapSetInt failed");

	s_mutex_init(&(self->utt_mutex));
	s_mutex_init(&(self->utt_id_mutex));
}


static void DestroyUtt(void *obj, s_erc *error)
{
	SUtterance *self = obj;
	SIterator *itr;
	SRelation *rel;

	S_CLR_ERR(error);

	s_mutex_lock(&(self->utt_mutex));

	S_DELETE(self->features, "DestroyUtt", error);

	/* Relations don't delete their contents (no dispose) */
	itr = S_ITERATOR_GET(self->relations, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "DestroyUtt",
				  "Call to \"S_ITERATOR_GET\" failed"))
	{
		S_DELETE(self->relations, "DestroyUtt", error);
		s_mutex_unlock(&(self->utt_mutex));
		s_mutex_destroy(&(self->utt_mutex));
		return;
	}

	while (itr)
	{
		rel = S_RELATION(SIteratorUnlink(itr, error));
		if (S_CHK_ERR(error, S_CONTERR,
					  "DestroyUtt",
					  "Call to \"SMapIteratorUnlink\" failed"))
		{
			S_DELETE(self->relations, "DestroyUtt", error);
			S_DELETE(itr, "DestroyUtt", error);
			s_mutex_unlock(&(self->utt_mutex));
			s_mutex_destroy(&(self->utt_mutex));
			return;
		}

		S_FORCE_DELETE(rel, "DestroyUtt", error);

		itr = SIteratorNext(itr);
	}

	S_DELETE(self->relations, "DestroyUtt", error);
	s_mutex_unlock(&(self->utt_mutex));
	s_mutex_destroy(&(self->utt_mutex));
	s_mutex_destroy(&(self->utt_id_mutex));
}


static void DisposeUtt(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static const SVoice *UttGetVoice(const SUtterance *self, s_erc *error)
{
	S_CLR_ERR(error);
	return self->voice;
}


static const SRelation *UttGetRelation(const SUtterance *self, const char *name,
									   s_erc *error)
{
	const SRelation *rel;
	const SObject *object;


	S_CLR_ERR(error);
	object = SMapGetObject(self->relations, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "UttGetRelation",
				  "Call to \"SMapObjectGet\" failed"))
		return NULL;

	if (object == NULL)
		return NULL;

	rel = S_CAST(object, SRelation, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "UttGetRelation",
				  "Cast from SObject to SRelation failed"))
		return NULL;

	return rel;
}


/* forward declaration */
static s_bool UttRelationPresent(const SUtterance *self, const char *name, s_erc *error);


static void UttDeleteRelation(SUtterance *self, const char *name, s_erc *error)
{
	/* Relation vals don't delete their contents */
	SObject *rel;
	s_bool is_present;


	S_CLR_ERR(error);

	/* check if the named relation is present */
	is_present = UttRelationPresent(self, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "UttDeleteRelation",
				  "Call to \"UttRelationPresent\" failed"))
		return;

	if (!is_present)
		return;

	/* unlink and force delete */
	rel = SMapObjectUnlink(self->relations, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "UttDeleteRelation",
				  "Call to \"SMapObjectUnlink\" failed"))
		return;

	S_FORCE_DELETE(rel, "UttDeleteRelation", error);
}


/* forward declaration */
static void UttSetRelation(SUtterance *self, SRelation *rel, s_erc *error);


static SRelation *UttNewRelation(SUtterance *self, const char *name, s_erc *error)
{
	SRelation *newRel;


	S_CLR_ERR(error);

	newRel = (SRelation*)S_NEW("SRelation", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "UttNewRelation",
				  "Failed to create new relation"))
		return NULL;

	SRelationInit(&newRel, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "UttNewRelation",
				  "Failed to initialize new relation"))
		return NULL;

	UttSetRelation(self, newRel, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "UttNewRelation",
				  "Call to \"UttSetRelation\" failed"))
	{
		S_FORCE_DELETE(newRel, "UttNewRelation", error);
		return NULL;
	}

	return newRel;
}


static void UttSetRelation(SUtterance *self, SRelation *rel, s_erc *error)
{
	S_CLR_ERR(error);

	rel->utterance = self;

	SMapSetObject(self->relations, rel->name, S_OBJECT(rel), error);
	S_CHK_ERR(error, S_CONTERR,
			  "UttSetRelation",
			  "Call to \"SMapSetObject\" failed");
}


static s_bool UttRelationPresent(const SUtterance *self, const char *name, s_erc *error)
{
	s_bool rel_present;


	S_CLR_ERR(error);
	rel_present = SMapObjectPresent(self->relations, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "UttRelationPresent",
				  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	return rel_present;
}


static SList *UttRelationsKeys(const SUtterance *self, s_erc *error)
{
	SList *keys;


	S_CLR_ERR(error);
	keys = SMapGetKeys(self->relations, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "UttRelationsKeys",
				  "Call to \"SMapGetKeys\" failed"))
		return NULL;

	return keys;
}


/************************************************************************************/
/*                                                                                  */
/* SUtterance class initialization                                                  */
/*                                                                                  */
/************************************************************************************/

static SUtteranceClass UtteranceClass =
{
	/* SObjectClass */
	{
		"SUtterance",
		sizeof(SUtterance),
		{ 0, 1},
		InitUtterance,     /* init    */
		DestroyUtt,        /* destroy */
		DisposeUtt,        /* dispose */
		NULL,              /* compare */
		NULL,              /* print   */
		NULL,              /* copy    */
	},
	/* SUtteranceClass */
	UttGetVoice,                /* voice            */
	UttNewRelation,             /* new_relation     */
	UttGetRelation,             /* get_relation     */
	UttSetRelation,             /* set_relation     */
	UttDeleteRelation,          /* delete_relation  */
	UttRelationPresent,         /* have_relation    */
	UttRelationsKeys            /* relations_keys   */
};


