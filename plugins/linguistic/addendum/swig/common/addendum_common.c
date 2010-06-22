/************************************************************************************/
/* Copyright (c) 2010 The Department of Arts and Culture,                           */
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
/* DATE    : June 2010                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SWIG common C convenience functions for SAddendum.                               */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Extend the SAddendum class                                                       */
/*                                                                                  */
/************************************************************************************/

typedef struct
{
	SMap *features;
} SAddendum;

%nodefaultctor SPhoneset;

%types(SAddendum = SObject, SObject*);

%extend SAddendum
{
	const char *name(s_erc *error)
	{
		const char *name;


		name = S_ADDENDUM_CALL($self, get_name)($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return name;
	}


	const char *description(s_erc *error)
	{
		const char *description;


		description = S_ADDENDUM_CALL($self, get_description)($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return description;
	}


	const char *language(s_erc *error)
	{
		const char *language;


		language = S_ADDENDUM_CALL($self, get_language)($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return language;
	}


	const char *lang_code(s_erc *error)
	{
		const char *lang_code;


		lang_code = S_ADDENDUM_CALL($self, get_lang_code)($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return lang_code;
	}


	s_version *version(s_erc *error)
	{
		return (s_version*)S_ADDENDUM_CALL($self, get_version)($self, error);
	}


	void add_word(const char *word, SMap *features, s_erc *error)
	{
		S_ADDENDUM_CALL($self, add_word)($self, word, features, error);
	}
}

