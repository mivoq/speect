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
/* Read a lexicon from a JSON format file.                                          */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "serialized_lex.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void set_lex_info(SLexicon *lex, const SMap *lexDef, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SLexiconJSON *s_read_lexicon_json(const char *path, s_erc *error)
{
	SLexiconJSON *lex = NULL;
	SMap *parsedFile = NULL;
	const SMap *tmpMap;
	const SObject *tmp;
	s_bool is_present;
	size_t entries_size;


	S_CLR_ERR(error);

	/* create lexicon */
	lex = (SLexiconJSON*)S_NEW("SLexiconJSON", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Failed to create new lexicon object"))
		goto quit_error;

	/* read the JSON file into a SMAP */
	parsedFile = s_json_parse_config_file(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"s_json_parse_config_file\" failed"))
		goto quit_error;

	/* get "lexicon-definition" key */
	tmp = SMapGetObjectDef(parsedFile, "lexicon-definition", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto quit_error;

	if (tmp == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_read_lexicon_json",
				  "Lexicon file does not have a 'lexicon-definition' key");
		goto quit_error;
	}

	/* cast to make sure it's a map */
	tmpMap = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Lexicon file key 'lexicon-definition' must be a map type"))
		goto quit_error;

	set_lex_info(S_LEXICON(lex), tmpMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"set_lex_info\" failed"))
		goto quit_error;

	/* get lexicon features, if any */
	is_present = SMapObjectPresent(parsedFile, "features", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"SMapObjectIsPresent\" failed"))
		goto quit_error;

	if (is_present)
	{
		SObject *featuresObject;
		SMap *featuresMap;


		featuresObject = SMapObjectUnlink(parsedFile, "features", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_lexicon_json",
					  "Call to \"SMapObjectUnlink\" failed"))
			goto quit_error;

		/* cast to make sure it's a map */
		featuresMap = S_CAST(featuresObject, SMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_lexicon_json",
					  "Lexicon file key 'features' must be a map type"))
		{
			S_DELETE(featuresObject, "s_read_lexicon_json", error);
			goto quit_error;
		}

		/* give the features map to the lexicon */
		S_LEXICON(lex)->features = featuresMap;
	}

	/* get lexicon entries */
	tmp = SMapGetObjectDef(parsedFile, "lexicon-entries", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto quit_error;

	if (tmp == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_read_lexicon_json",
				  "Lexicon file does not have a 'lexicon-entries' key");
		goto quit_error;
	}

	/* cast to make sure it's a map */
	tmpMap = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Lexicon file key 'lexicon-entries' must be a map type"))
		goto quit_error;

	/* get the size of the map */
	entries_size = SMapSize(tmpMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"SMapSize\" failed"))
		goto quit_error;

	/* the entries map is a SMapList (default for JSON reader), we
	 * want a SMapHashTable. Create a new one with the given size.
	 */
	lex->entries = (SMap*)S_NEW("SMapHashTable", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Failed to create new 'SMapHashTable' object"))
		goto quit_error;

	/* initialize with the read size */
	SMapHashTableInit(&(lex->entries), entries_size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"SMapHashTableInit\" failed"))
		goto quit_error;

	/* copy the entries from the read JSON to the lexicon (from
	 * SMapList to SMapHashTable)
	 */
	SMapCopy(lex->entries, tmpMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_lexicon_json",
				  "Call to \"SMapCopy\" failed"))
		goto quit_error;

	/* done */
	goto quit;


	/* errors start clean up code here */
quit_error:
	if (lex != NULL)
		S_DELETE(lex, "s_read_lexicon_json", error);  /* sets lex = NULL */

	/* normal exit start clean up code here */
quit:
	if (parsedFile != NULL)
		S_DELETE(parsedFile, "s_read_lexicon_json", error);

	return lex;
}

/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void set_lex_info(SLexicon *lex, const SMap *lexDef, s_erc *error)
{
	const SObject *tmpObject;
	const char *tmp_string;
	const SMap *versionMap;


	S_CLR_ERR(error);

	/* get lexicon name */
	tmp_string = SMapGetStringDef(lexDef, "name", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_lex_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_lex_info",
				  "'lexicon-definition' does not have a 'name' key");
		return;
	}

	lex->info->name = s_strdup(tmp_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_lex_info",
				  "Call to \"s_strdup\" failed"))
		return;

	/* get lexicon description */
	tmp_string = SMapGetStringDef(lexDef, "description", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_lex_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_lex_info",
				  "'lexicon-definition' does not have a 'description' key");
		return;
	}

	lex->info->description = s_strdup(tmp_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_lex_info",
				  "Call to \"s_strdup\" failed"))
		return;

	/* get lexicon language */
	tmp_string = SMapGetStringDef(lexDef, "language", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_lex_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_lex_info",
				  "'lexicon-definition' does not have a 'language' key");
		return;
	}

	lex->info->language = s_strdup(tmp_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_lex_info",
				  "Call to \"s_strdup\" failed"))
		return;

	/* get lexicon lang_code */
	tmp_string = SMapGetStringDef(lexDef, "lang-code", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_lex_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_lex_info",
				  "'lexicon-definition' does not have a 'lang_code' key");
		return;
	}

	lex->info->lang_code = s_strdup(tmp_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_lex_info",
				  "Call to \"s_strdup\" failed"))
		return;

	/* get lexicon version */
	tmpObject = SMapGetObjectDef(lexDef, "version", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_lex_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmpObject == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_lex_info",
				  "'lexicon-definition' does not have a 'version' key");
		return;
	}

	versionMap = S_CAST(tmpObject, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_lex_info",
				  "'lexicon-definition' key 'version' must be a map type"))
		return;

	lex->info->version.major = SMapGetInt(versionMap, "major", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_lex_info",
				  "Call to \"SMapGetInt\" failed"))
		return;

	lex->info->version.minor = SMapGetInt(versionMap, "minor", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_lex_info",
				  "Call to \"SMapGetInt\" failed"))
		return;
}
