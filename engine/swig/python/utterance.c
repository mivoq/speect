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
/* DATE    : November 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SUtterance Python wrapper.                           */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	SObject *py_sutterance_new(const SObject *voice)
	{
		s_erc rv = S_SUCCESS;
		SUtterance *utt;


		utt = (SUtterance*)S_NEW("SUtterance", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to create new SUtterance");
			return NULL;
		}

		SUtteranceInit(&utt, S_VOICE(voice), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to initialize new SUtterance");
			return NULL;
		}

		return S_OBJECT(utt);
	}


	void py_sutterance_set_relation(const SObject *self, const char *name,
									const SObject *relation)
	{
		s_erc rv = S_SUCCESS;
		SRelation *rel = NULL;
		SUtterance *utt = S_UTTERANCE(self);
		s_bool type_ok;


		type_ok = SObjectIsType(relation, "SRelation", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to check relation object type");
			return;
		}

		if (!type_ok)
		{
			PyErr_SetString(PyExc_RuntimeError, "Given relation object not of type 'SRelation'");
			return;
		}

		rel = S_RELATION(relation);
		if (rel->name != NULL)
			S_FREE(rel->name);

		rel->name = s_strdup(name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to copy relation name");
			return;
		}

		SUtteranceSetRelation(utt, rel, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to add relation to utterance");
			rel->utterance = NULL;
			return;
		}
	}


	SObject *py_sutterance_create_relation(SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;
		SRelation *rel;


		rel = SUtteranceNewRelation(S_UTTERANCE(self), name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to create new relation");
			return NULL;
		}

		return S_OBJECT(rel);
	}


	const SObject *py_sutterance_get_relation(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;
		const SRelation *rel;


		rel = SUtteranceGetRelation(S_UTTERANCE(self), name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get relation");
			return NULL;
		}

		return S_OBJECT(rel);
	}


	void py_sutterance_del_relation(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;


		SUtteranceDelRelation(S_UTTERANCE(self), name, &rv);
		if (rv != S_SUCCESS)
			PyErr_Format(PyExc_RuntimeError, "Failed to delete relation '%s'", name);

	}


	s_bool py_sutterance_relation_present(const SObject *self, const char *name)
	{
		s_erc rv = S_SUCCESS;
		s_bool is_present;


		is_present = SUtteranceRelationIsPresent(S_UTTERANCE(self), name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to query presence of relation");
			return FALSE;
		}

		return is_present;
	}


	const SObject *py_sutterance_get_feature(const SObject *self, const char *featname)
	{
		s_erc rv = S_SUCCESS;
		const SObject *feature;


		feature = SUtteranceGetFeature(S_UTTERANCE(self), featname, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_Format(PyExc_RuntimeError, "Failed to get utterance feature '%s'", featname);
			return NULL;
		}

		return S_OBJECT(feature);
	}


	void py_sutterance_set_feature(SObject *self, const char *featname, const SObject *feat)
	{
		s_erc rv = S_SUCCESS;


		SUtteranceSetFeature(S_UTTERANCE(self), featname, feat, &rv);
		if (rv != S_SUCCESS)
			PyErr_Format(PyExc_RuntimeError, "Failed to set utterance feature '%s'", featname);
	}


	s_bool py_sutterance_feat_present(const SObject *self, const char *feat_name)
	{
		s_erc rv = S_SUCCESS;
		s_bool is_present;


		is_present = SUtteranceFeatureIsPresent(S_UTTERANCE(self), feat_name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to query feature presence");
			return FALSE;
		}

		return is_present;
	}


	void py_sutterance_del_feature(SObject *self, const char *feat_name)
	{
		s_erc rv = S_SUCCESS;


		SUtteranceDelFeature(S_UTTERANCE(self), feat_name, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to delete feature");
	}


	const SObject *py_sutterance_features(SObject *self)
	{
		SUtterance *utt = S_UTTERANCE(self);


		if (utt == NULL)
			return NULL;

		return (const SObject*)utt->features;
	}


	const SObject *py_sutterance_relations(SObject *self)
	{
		SUtterance *utt = S_UTTERANCE(self);


		if (utt == NULL)
			return NULL;

		return (const SObject*)utt->relations;
	}


/*
 * Do not delete this delimiter, required for SWIG
 */
%}
