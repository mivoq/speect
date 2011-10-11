/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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
/* SWIG common C convenience functions for SLexicon.                                */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Extend the SLexicon class                                                        */
/*                                                                                  */
/************************************************************************************/

typedef struct
{
	SMap *features;
} SLexicon;

%nodefaultctor SLexicon;

%types(SLexicon = SObject, SObject*);

%extend SLexicon
{
	const char *name(s_erc *error)
	{
		const char *name;


		S_CLR_ERR(error);
		if (!S_LEXICON_METH_VALID($self, get_name))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "name",
					  "Lexicon method \"get_name\" not implemented");
			return NULL;
		}

		name = S_LEXICON_CALL($self, get_name)($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return name;
	}


	const char *description(s_erc *error)
	{
		const char *description;


		S_CLR_ERR(error);
		if (!S_LEXICON_METH_VALID($self, get_description))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "description",
					  "Lexicon method \"get_description\" not implemented");
			return NULL;
		}

		description = S_LEXICON_CALL($self, get_description)($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return description;
	}


	const char *language(s_erc *error)
	{
		const char *language;


		S_CLR_ERR(error);
		if (!S_LEXICON_METH_VALID($self, get_language))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "language",
					  "Lexicon method \"get_language\" not implemented");
			return NULL;
		}

		language = S_LEXICON_CALL($self, get_language)($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return language;
	}


	const char *lang_code(s_erc *error)
	{
		const char *lang_code;


		S_CLR_ERR(error);
		if (!S_LEXICON_METH_VALID($self, get_lang_code))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "lang_code",
					  "Lexicon method \"get_lang_code\" not implemented");
			return NULL;
		}

		lang_code = S_LEXICON_CALL($self, get_lang_code)($self, error);
		if (*error != S_SUCCESS)
			return NULL;

		return lang_code;
	}


	s_version *version(s_erc *error)
	{
		S_CLR_ERR(error);
		if (!S_LEXICON_METH_VALID($self, get_version))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "version",
					  "Lexicon method \"get_version\" not implemented");
			return NULL;
		}

		return (s_version*)S_LEXICON_CALL($self, get_version)($self, error);
	}
}

