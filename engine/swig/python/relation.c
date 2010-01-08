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
/* C convenience functions for SRelation Python wrapper.                            */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	SObject *py_srelation_new(const char *name, const SObject *utt)
	{
		s_erc rv = S_SUCCESS;
		SRelation *rel;


		rel = (SRelation*)S_NEW("SRelation", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to create new SRelation");
			return NULL;
		}

		SRelationInit(&rel, name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to initialize new SRelation");
			return NULL;
		}

		if (utt != NULL)
		{
			SUtteranceSetRelation(S_UTTERANCE(utt), rel, &rv);
			if (rv != S_SUCCESS)
			{
				PyErr_SetString(PyExc_RuntimeError, "Failed to set new SRelation to utterance");
				S_FORCE_DELETE(rel, "py_srelation_new", &rv);
				return NULL;
			}
		}

		return S_OBJECT(rel);
	}


	void py_srelation_delete(SObject *rel)
	{
		s_erc rv = S_SUCCESS;
		SRelation *self = S_RELATION(rel);


		/*
		 * if the relation is connected delete it
		 * through the utterance, otherwise force delete.
		 */
		if (self->utterance != NULL)
		{
			SUtteranceDelRelation(self->utterance, self->name, &rv);
			if (rv != S_SUCCESS)
				PyErr_SetString(PyExc_RuntimeError, "Failed to delete relation");
		}
		else
		{
			S_FORCE_DELETE(rel, "py_srelation_delete", &rv);
			if (rv != S_SUCCESS)
				PyErr_SetString(PyExc_RuntimeError, "Failed to delete relation");
		}
	}


	const char *py_srelaion_name(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const char *name;


		name = SRelationName(S_RELATION(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get relation name");
			return NULL;
		}

		return name;
	}


	const SObject *py_srelation_utterance(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const SUtterance *utt;


		utt = SRelationUtterance(S_RELATION(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get relation utterance");
			return NULL;
		}

		return S_OBJECT(utt);
	}


	const SObject *py_srelation_head(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const SItem *head;


		head = SRelationHead(S_RELATION(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get relation head");
			return NULL;
		}

		return S_OBJECT(head);
	}


	const SObject *py_srelation_tail(const SObject *self)
	{
		s_erc rv = S_SUCCESS;
		const SItem *tail;


		tail = SRelationTail(S_RELATION(self), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to get relation tail");
			return NULL;
		}

		return S_OBJECT(tail);
	}


	SObject *py_srelation_append(SObject *self, const SObject *toShare)
	{
		s_erc rv = S_SUCCESS;
		const SItem *appended;


		appended = SRelationAppend(S_RELATION(self), S_ITEM(toShare), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to append item");
			return NULL;
		}

		return S_OBJECT(appended);
	}


	SObject *py_srelation_prepend(SObject *self, const SObject *toShare)
	{
		s_erc rv = S_SUCCESS;
		const SItem *prepended;


		prepended = SRelationPrepend(S_RELATION(self), S_ITEM(toShare), &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Failed to prepend item");
			return NULL;
		}

		return S_OBJECT(prepended);
	}

/*
 * Do not delete this delimiter, required for SWIG
 */
%}





