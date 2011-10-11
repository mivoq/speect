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
/* DATE    : November 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SWIG common C convenience functions for SUtterance.                              */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


typedef struct
{
	SVoice *voice;
	SMap   *features;
} SUtterance;


%types(SUtterance = SObject);

%extend SUtterance
{
	SUtterance(const SVoice *voice=NULL, s_erc *error)
	{
		SUtterance *utt;


		utt = S_NEW(SUtterance, error);
		if (*error != S_SUCCESS)
			return NULL;

		SUtteranceInit(&utt, S_VOICE(voice), error);
		if (*error != S_SUCCESS)
			return NULL;

		return utt;
	}

	~SUtterance()
	{
		s_erc error;

		S_CLR_ERR(&error);
		S_DELETE($self, "~SUtterance()", &error);
	}


	SRelation *relation_new(const char *name, s_erc *error)
	{
		SRelation *rel;


		rel = SUtteranceNewRelation($self, name, error);

		if (*error != S_SUCCESS)
			return NULL;

		return rel;
	}


	const SRelation *relation_get(const char *name, s_erc *error)
	{
		const SRelation *rel;
		s_bool is_present;


		is_present = SUtteranceRelationIsPresent($self, name, error);
		if (*error != S_SUCCESS)
			return NULL;

		if (!is_present)
			return NULL;

		rel = SUtteranceGetRelation($self, name, error);
		if (*error != S_SUCCESS)
			return NULL;

		return rel;
	}


	void relation_set(SRelation *rel, s_erc *error)
	{
		SUtteranceSetRelation($self, rel, error);
	}


	void relation_del(const char *name, s_erc *error)
	{
		SUtteranceDelRelation($self, name, error);
	}

};

