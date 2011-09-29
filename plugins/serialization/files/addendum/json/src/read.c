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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Read a addendum from a JSON format file.                                         */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "addendum_json.h"


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void set_addendum_info(SAddendum *addendum, const SMap *addendumDef,
							  s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL SAddendumJSON *s_read_addendum_json(const char *path, s_erc *error)
{
	SAddendumJSON *addendum = NULL;
	SMap *parsedFile = NULL;
	const SMap *tmpMap;
	const SObject *tmp;
	s_bool is_present;
	size_t entries_size;


	S_CLR_ERR(error);

	/* create addendum */
	addendum = S_NEW(SAddendumJSON, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_addendum_json",
				  "Failed to create new addendum object"))
		goto quit_error;

	/* read the JSON file into a SMAP */
	parsedFile = s_json_parse_config_file(path, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_addendum_json",
				  "Call to \"s_json_parse_config_file\" failed"))
		goto quit_error;

	/* get "addendum-definition" key */
	tmp = SMapGetObjectDef(parsedFile, "addendum-definition", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_addendum_json",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto quit_error;

	if (tmp == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_read_addendum_json",
				  "Addendum file does not have a 'addendum-definition' key");
		goto quit_error;
	}

	/* cast to make sure it's a map */
	tmpMap = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_addendum_json",
				  "Addendum file key 'addendum-definition' must be a map type"))
		goto quit_error;

	set_addendum_info(S_ADDENDUM(addendum), tmpMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_addendum_json",
				  "Call to \"set_addendum_info\" failed"))
		goto quit_error;

	/* get addendum features, if any */
	is_present = SMapObjectPresent(parsedFile, "features", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_addendum_json",
				  "Call to \"SMapObjectIsPresent\" failed"))
		goto quit_error;

	if (is_present)
	{
		SObject *featuresObject;
		SMap *featuresMap;


		featuresObject = SMapObjectUnlink(parsedFile, "features", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_addendum_json",
					  "Call to \"SMapObjectUnlink\" failed"))
			goto quit_error;

		/* cast to make sure it's a map */
		featuresMap = S_CAST(featuresObject, SMap, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_read_addendum_json",
					  "Addendum file key 'features' must be a map type"))
		{
			S_DELETE(featuresObject, "s_read_addendum_json", error);
			goto quit_error;
		}

		/* give the features map to the addendum */
		S_ADDENDUM(addendum)->features = featuresMap;
	}

	/* get addendum entries */
	tmp = SMapGetObjectDef(parsedFile, "addendum-entries", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_addendum_json",
				  "Call to \"SMapGetObjectDef\" failed"))
		goto quit_error;

	if (tmp == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_read_addendum_json",
				  "Addendum file does not have a 'addendum-entries' key");
		goto quit_error;
	}

	/* cast to make sure it's a map */
	tmpMap = S_CAST(tmp, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_addendum_json",
				  "Addendum file key 'addendum-entries' must be a map type"))
		goto quit_error;

	/* get the size of the map */
	entries_size = SMapSize(tmpMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_addendum_json",
				  "Call to \"SMapSize\" failed"))
		goto quit_error;

	/* the entries map is a SMapList (default for JSON reader), we
	 * want a SMapHashTable. Create a new one with the given size.
	 */
	addendum->entries = S_MAP(S_NEW(SMapHashTable, error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_addendum_json",
				  "Failed to create new 'SMapHashTable' object"))
		goto quit_error;

	/* resize with the read size */
	SMapHashTableResize(S_MAPHASHTABLE(addendum->entries), entries_size, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_addendum_json",
				  "Call to \"SMapHashTableResize\" failed"))
		goto quit_error;

	/* copy the entries from the read JSON to the addendum (from
	 * SMapList to SMapHashTable)
	 */
	SMapCopy(addendum->entries, tmpMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_read_addendum_json",
				  "Call to \"SMapCopy\" failed"))
		goto quit_error;

	/* done */
	goto quit;


	/* errors start clean up code here */
quit_error:
	if (addendum != NULL)
		S_DELETE(addendum, "s_read_addendum_json", error);  /* sets lex = NULL */

	/* normal exit start clean up code here */
quit:
	if (parsedFile != NULL)
		S_DELETE(parsedFile, "s_read_addendum_json", error);

	return addendum;
}

/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void set_addendum_info(SAddendum *addendum, const SMap *addendumDef, s_erc *error)
{
	const SObject *tmpObject;
	const char *tmp_string;
	const SMap *versionMap;


	S_CLR_ERR(error);

	/* get addendum name */
	tmp_string = SMapGetStringDef(addendumDef, "name", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_addendum_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_addendum_info",
				  "'addendum-definition' does not have a 'name' key");
		return;
	}

	addendum->info->name = s_strdup(tmp_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_addendum_info",
				  "Call to \"s_strdup\" failed"))
		return;

	/* get addendum description */
	tmp_string = SMapGetStringDef(addendumDef, "description", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_addendum_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_addendum_info",
				  "'addendum-definition' does not have a 'description' key");
		return;
	}

	addendum->info->description = s_strdup(tmp_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_addendum_info",
				  "Call to \"s_strdup\" failed"))
		return;

	/* get addendum language */
	tmp_string = SMapGetStringDef(addendumDef, "language", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_addendum_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_addendum_info",
				  "'addendum-definition' does not have a 'language' key");
		return;
	}

	addendum->info->language = s_strdup(tmp_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_addendum_info",
				  "Call to \"s_strdup\" failed"))
		return;

	/* get addendum lang_code */
	tmp_string = SMapGetStringDef(addendumDef, "lang-code", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_addendum_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmp_string == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_addendum_info",
				  "'addendum-definition' does not have a 'lang_code' key");
		return;
	}

	addendum->info->lang_code = s_strdup(tmp_string, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_addendum_info",
				  "Call to \"s_strdup\" failed"))
		return;

	/* get addendum version */
	tmpObject = SMapGetObjectDef(addendumDef, "version", NULL, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_addendum_info",
				  "Call to \"SMapGetStringDef\" failed"))
		return;

	if (tmpObject == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "set_addendum_info",
				  "'addendum-definition' does not have a 'version' key");
		return;
	}

	versionMap = S_CAST(tmpObject, SMap, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_addendum_info",
				  "'addendum-definition' key 'version' must be a map type"))
		return;

	addendum->info->version.major = SMapGetInt(versionMap, "major", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_addendum_info",
				  "Call to \"SMapGetInt\" failed"))
		return;

	addendum->info->version.minor = SMapGetInt(versionMap, "minor", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "set_addendum_info",
				  "Call to \"SMapGetInt\" failed"))
		return;
}
