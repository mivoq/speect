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
/* SWIG common C convenience functions for SRelation.                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


typedef struct
{
} SRelation;


%types(SRelation = SObject);

%extend SRelation
{
	SRelation(const SUtterance *utt, const char *name, s_erc *error)
	{
		SRelation *rel;


		rel = (SRelation*)S_NEW("SRelation", error);
		if (*error != S_SUCCESS)
			return NULL;

		SRelationInit(&rel, name, error);
		if (*error != S_SUCCESS)
			return NULL;

		if (utt != NULL)
		{
			SUtteranceSetRelation(S_UTTERANCE(utt), rel, error);
			if (*error != S_SUCCESS)
			{
				S_FORCE_DELETE(rel, "SRelation", error);
				return NULL;
			}
		}

		return rel;
	}


	~SRelation()
	{
		s_erc error;

		S_CLR_ERR(&error);
		/* don't delete the relation, the utterance will delete it,
		 * this is just here so that SWIG will not create a
		 * destructor, and that SWIG has something to call, otherwise
		 * we get a lot of messages about memory leaks (which aren't real)
		 */
	}


	const SItem *append(const SItem *toShare=NULL, s_erc *error)
	{
		const SItem *appended;


		appended = SRelationAppend($self, toShare, error);
		if (*error != S_SUCCESS)
			return NULL;

		return appended;
	}


	const SItem *prepend(const SItem *toShare=NULL, s_erc *error)
	{
		const SItem *prepended;


		prepended = SRelationPrepend($self, toShare, error);
		if (*error != S_SUCCESS)
			return NULL;

		return prepended;
	}


	const SItem *head(s_erc *error)
	{
		const SItem *item;


		item = SRelationHead($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return item;
	}


	const SItem *tail(s_erc *error)
	{
		const SItem *item;


		item = SRelationTail($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return item;
	}


	const char *name(s_erc *error)
	{
		const char *name;


		name = SRelationName($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return name;
	}


	const SUtterance *utterance(s_erc *error)
	{
		const SUtterance *utt;


		utt = SRelationUtterance($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return utt;
	}
}
