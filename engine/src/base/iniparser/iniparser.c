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
/* DATE    : 9 November 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Initialization file parser.                                                      */
/* Based on iniParser version 3.0b (beta), http://ndevilla.free.fr/iniparser/       */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* iniParser license.                                                               */
/*                                                                                  */
/* Copyright (c) 2000-2007 by Nicolas Devillard.                                    */
/* MIT License                                                                      */
/*                                                                                  */
/* Permission is hereby granted, free of charge, to any person obtaining a          */
/* copy of this software and associated documentation files (the "Software"),       */
/* to deal in the Software without restriction, including without limitation        */
/* the rights to use, copy, modify, merge, publish, distribute, sublicense,         */
/* and/or sell copies of the Software, and to permit persons to whom the            */
/* Software is furnished to do so, subject to the following conditions:             */
/*                                                                                  */
/* The above copyright notice and this permission notice shall be included in       */
/* all copies or substantial portions of the Software.                              */
/*                                                                                  */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING          */
/* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER              */
/* DEALINGS IN THE SOFTWARE.                                                        */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include <ctype.h>
#include "base/errdbg/errdbg_macros.h"
#include "base/containers/hashtable/hash_table.h"
#include "base/strings/strings.h"
#include "base/utils/math.h"
#include "base/utils/alloc.h"
#include "base/iniparser/iniparser.h"


/************************************************************************************/
/*                                                                                  */
/* Typedef                                                                          */
/*                                                                                  */
/************************************************************************************/

/*
 * This enum stores the status for each parsed line (internal use only).
 */
typedef enum
{
    S_LINE_UNPROCESSED,
    S_LINE_ERROR,
    S_LINE_EMPTY,
    S_LINE_COMMENT,
    S_LINE_SECTION,
    S_LINE_VALUE
} s_line_status;


struct s_ini_parser
{
	char         *path;
	s_hash_table *dict;
};


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

#define S_ASCIILINESZ      1024

#define S_INI_INVALID_KEY ((char*)-1)


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static char *strstrip(char *s);

static char *strlwc(const char *s);

static s_line_status iniparser_line(char *input_line, char *section,
									char *key, char *value);

static void dict_free(void *key, void *data, s_erc *error);

static s_ini_parser *parser_create(const char *path);

static void parser_free(s_ini_parser *parser);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_ini_parser *s_iniparser_load(const char *path)
{
	FILE *fhandle;
    char line[S_ASCIILINESZ];
    char section[S_ASCIILINESZ];
    char key[S_ASCIILINESZ];
    char tmp[S_ASCIILINESZ];
    char val[S_ASCIILINESZ];
	char *dict_key;
	char *dict_val;
	int last;
    int len;
    int lineno;
	s_ini_parser *parser;
	s_erc error;


	S_CLR_ERR(&error);

	if (path == NULL)
	{
		S_ERR_PRINT(S_ARGERROR, "s_iniparser_load",
					"Argument \"path\" is NULL");
		return NULL;
	}

	fhandle = fopen(path, "r");
	if (fhandle == NULL)
	{
		char *err_str = s_error_str(S_FAILURE);

#ifdef SPCT_USE_THREADS
		fprintf(stderr, "[ERROR (%s) %lu] Failed to open ini file '%s'"
				" (in function 's_iniparser_load', %s, %d)\n",
				err_str, s_thread_id(), path, __FILE__, __LINE__);
#else /* !SPCT_USE_THREADS */
		fprintf(stderr, "[ERROR (%s)] Failed to open ini file '%s'"
				" (in function 's_iniparser_load', %s, %d)\n",
				err_str, path, __FILE__, __LINE__);
#endif /* SPCT_USE_THREADS */
		S_FREE(err_str);
		return NULL;
	}

	clearerr(fhandle);

	parser = parser_create(path);
	if (parser == NULL)
	{
		S_ERR_PRINT(S_FAILURE, "s_iniparser_load",
					"Call to \"parser_create\" failed");
		fclose(fhandle);
        return NULL;
    }

    memset(line, 0, S_ASCIILINESZ);
    memset(section, 0, S_ASCIILINESZ);
    memset(key, 0, S_ASCIILINESZ);
    memset(val, 0, S_ASCIILINESZ);
	memset(tmp, 0, S_ASCIILINESZ);
    last = 0;
	lineno = 0;
	len = 0;

	while (fgets(line+last, S_ASCIILINESZ-last, fhandle) != NULL)
	{
        lineno++;
        len = (int)strlen(line)-1;

        /* Safety check against buffer overflows */
        if ((line[len] != '\n') &&
			(feof(fhandle) == 0))
		{
			char *err_str = s_error_str(S_FAILURE);

#ifdef SPCT_USE_THREADS
			fprintf(stderr, "[ERROR (%s) %lu] input line too long in '%s' (%d)"
					" (in function 's_iniparser_load', %s, %d)\n",
					err_str, s_thread_id(), path, lineno, __FILE__, __LINE__);
#else /* !SPCT_USE_THREADS */
			fprintf(stderr, "[ERROR (%s)] input line too long in '%s' (%d)"
					" (in function 's_iniparser_load', %s, %d)\n",
					err_str, path, lineno, __FILE__, __LINE__);
#endif /* SPCT_USE_THREADS */
			S_FREE(err_str);
			parser_free(parser);
			fclose(fhandle);
			return NULL;
		}

		/* Get rid of \n and spaces at end of line */
        while ((len >=0 )
			   && ((line[len] == '\n')
				   || (isspace(line[len]))))
		{
            line[len] = 0;
            len--;
        }

        /* Detect multi-line */
        if (line[len] == '\\')
		{
            /* Multi-line value */
            last = len;
            continue;
        }
		else
		{
            last = 0;
        }

        switch (iniparser_line(line, section, key, val))
		{
		case S_LINE_EMPTY:
		case S_LINE_COMMENT:
            break ;

		case S_LINE_SECTION:
			dict_key = s_strdup_clib(section);
			dict_val = s_strdup_clib("NULL");
			s_hash_table_add(parser->dict, dict_key, strlen(dict_key),
							 dict_val, &error);
            break ;

		case S_LINE_VALUE:
            sprintf(tmp, "%s:%s", section, key);
			dict_key = s_strdup_clib(tmp);
			dict_val = s_strdup_clib(val);
			s_hash_table_add(parser->dict, dict_key, strlen(dict_key),
							 dict_val, &error);
			break ;

		case S_LINE_ERROR:
		{
			char *err_str;


			error = S_FAILURE;
			err_str = s_error_str(S_FAILURE);

#ifdef SPCT_USE_THREADS
			fprintf(stderr, "[ERROR (%s) %lu] syntax error in '%s' (%d)"
					" (in function 's_iniparser_load', %s, %d)\n",
					err_str, s_thread_id(), path, lineno, __FILE__, __LINE__);
#else /* !SPCT_USE_THREADS */
			fprintf(stderr, "[ERROR (%s)] syntax error in '%s' (%d)"
					" (in function 's_iniparser_load', %s, %d)\n",
					err_str, path, lineno, __FILE__, __LINE__);
#endif /* SPCT_USE_THREADS */
			S_FREE(err_str);
            break;
		}
		default:
            break ;
        }

		memset(line, 0, S_ASCIILINESZ);
        last = 0;

		if (error != S_SUCCESS)
		{
			char *err_str = s_error_str(error);

#ifdef SPCT_USE_THREADS
			fprintf(stderr, "[ERROR (%s) %lu] failed to parse ini file '%s'"
					" (in function 's_iniparser_load', %s, %d)\n",
					err_str, s_thread_id(), path, __FILE__, __LINE__);
#else /* !SPCT_USE_THREADS */
			fprintf(stderr, "[ERROR (%s)] failed to parse ini file '%s'"
					" (in function 's_iniparser_load', %s, %d)\n",
					err_str, path, __FILE__, __LINE__);
#endif /* SPCT_USE_THREADS */
			S_FREE(err_str);
            break;
		}
    }

    if (error != S_SUCCESS)
	{
		parser_free(parser);
		parser = NULL;
	}

    fclose(fhandle);
    return parser;
}


S_API void s_iniparser_free(s_ini_parser *inip)
{
	if (inip == NULL)
		return;

	parser_free(inip);
}


S_API const char *s_iniparser_get_string(const s_ini_parser *inip,
										 const char *key,
										 const char *def)
{
    char *lc_key;
    const char *sval;
	s_hash_element *kv;
	s_erc error;


	if ((inip == NULL)
		|| (key == NULL)
		|| (inip->dict == NULL))
        return def ;

	S_CLR_ERR(&error);

	lc_key = strlwc(key);

	kv = s_hash_table_find(inip->dict, lc_key,
						   strlen(lc_key), &error);

	if ((kv == NULL)
		|| (error != S_SUCCESS))
		return def;

	sval = s_hash_element_get_data(kv, &error);
	if ((sval == NULL)
		|| (error != S_SUCCESS))
		return def;

    return (const char *)sval;
}


S_API int s_iniparser_get_int(const s_ini_parser *inip,
							  const char *key,
							  int def)
{
    const char *str;


	str = s_iniparser_get_string(inip, key, S_INI_INVALID_KEY);

	if (str == S_INI_INVALID_KEY)
		return def;

    return (int)strtol(str, NULL, 0);
}


S_API double s_iniparser_get_double(const s_ini_parser *inip,
									const char *key,
									double def)
{
    const char *str;


	str = s_iniparser_get_string(inip, key, S_INI_INVALID_KEY);

	if (str == S_INI_INVALID_KEY)
		return def;

    return atof(str);
}


S_API s_bool s_iniparser_get_boolean(const s_ini_parser *inip,
									 const char *key,
									 s_bool def)
{
    const char *c;
	s_bool ret = FALSE;


	c = s_iniparser_get_string(inip, key, S_INI_INVALID_KEY);

	if (c == S_INI_INVALID_KEY)
		return def;

	if (c[0]=='y' || c[0]=='Y' || c[0]=='1' || c[0]=='t' || c[0]=='T')
	{
        ret = TRUE;
    }
	else if (c[0]=='n' || c[0]=='N' || c[0]=='0' || c[0]=='f' || c[0]=='F')
	{
        ret = FALSE;
    }
	else
	{
        ret = def;
    }

    return ret;
}


S_API s_bool s_iniparser_entry_present(const s_ini_parser *inip, const char *entry)
{
    s_bool found = FALSE;

    if (s_iniparser_get_string(inip, entry, S_INI_INVALID_KEY)
		!= S_INI_INVALID_KEY)
	{
        found = TRUE;
    }

    return found;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

/*
 *  Load a single line from an INI file
 *  @param    section     Output space to store section
 *  @param    key         Output space to store key
 *  @param    value       Output space to store value
 *  @return   line_status value
 */
static s_line_status iniparser_line(char *input_line, char *section,
									char *key, char *value)
{
    s_line_status sta;
    char line[S_ASCIILINESZ+1];
    int len;

    strcpy(line, strstrip(input_line));
    len = (int)strlen(line);

    sta = S_LINE_UNPROCESSED;

    if (len<1)
	{
        /* Empty line */
        sta = S_LINE_EMPTY ;
    }
	else if (line[0] == '#')
	{
        /* Comment line */
        sta = S_LINE_COMMENT ;
    }
	else if (sscanf(line, "[%[^]]", section) == 1)
	{
        /* Section name */
        strcpy(section, strstrip(section));
        strcpy(section, strlwc(section));
        sta = S_LINE_SECTION ;
    }
	else if ((sscanf(line, "%[^=] = \"%[^\"]\"", key, value) == 2)
			 ||  (sscanf(line, "%[^=] = '%[^\']'", key, value) == 2)
			 ||  (sscanf(line, "%[^=] = %[^;#]", key, value) == 2))
	{
        /* Usual key=value, with or without comments */
        strcpy(key, strstrip(key));
        strcpy(key, strlwc(key));
        strcpy(value, strstrip(value));

        /*
         * sscanf cannot handle '' or "" as empty values
         * this is done here
         */
        if (!strcmp(value, "\"\"") || (!strcmp(value, "''")))
		{
            value[0]= 0;
        }
        sta = S_LINE_VALUE ;
    }
	else if (sscanf(line, "%[^=] = %[;#]", key, value)==2
			 ||  sscanf(line, "%[^=] %[=]", key, value) == 2)
	{
        /*
         * Special cases:
         * key=
         * key=;
         * key=#
         */
        strcpy(key, strstrip(key));
        strcpy(key, strlwc(key));
        value[0] = 0;
        sta = S_LINE_VALUE;
	}
	else
	{
        /* Generate syntax error */
        sta = S_LINE_ERROR;
    }

    return sta;
}


/*
 * Remove blanks at the beginning and the end of a string.
 * @param s	String to parse.
 * @return ptr to statically allocated string.
 *
 *  This function returns a pointer to a statically allocated string,
 *  which is identical to the input string, except that all blank
 *  characters at the end and the beg. of the string have been removed.
 *  Do not free or modify the returned string! Since the returned string
 *  is statically allocated, it will be modified at each function call
 *  (not re-entrant).
 */
static char *strstrip(char *s)
{
    static char l[S_ASCIILINESZ+1];
	char *last;

    if (s == NULL)
		return NULL;

	while (isspace((int)*s) && *s)
		s++;

	memset(l, 0, S_ASCIILINESZ+1);
	strcpy(l, s);
	last = l + strlen(l);

	while (last > l)
	{
		if (!isspace((int)*(last-1)))
			break;
		last --;
	}

	*last = (char)0;

	return (char*)l;
}


/*
 *  Convert a string to lowercase.
 *  @param	s	String to convert.
 *  @return	ptr to statically allocated string.
 *
 *  This function returns a pointer to a statically allocated string
 *  containing a lowercased version of the input string. Do not free
 *  or modify the returned string! Since the returned string is statically
 *  allocated, it will be modified at each function call (not re-entrant).
 */
/*--------------------------------------------------------------------------*/
static char *strlwc(const char *s)
{
    static char l[S_ASCIILINESZ+1];
    int i ;

    if (s == NULL)
		return NULL;

    memset(l, 0, S_ASCIILINESZ+1);
    i=0;

	while (s[i] && i<S_ASCIILINESZ)
	{
        l[i] = (char)tolower((int)s[i]);
        i++ ;
    }

    l[S_ASCIILINESZ] = (char)0;

    return l ;
}


static void dict_free(void *key, void *data, s_erc *error)
{
	char *dict_key;
	char *dict_val;


	S_CLR_ERR(error);

	dict_key = key;
	S_FREE(dict_key);

	dict_val = data;
	S_FREE(dict_val);
}


static s_ini_parser *parser_create(const char *path)
{
	s_ini_parser *parser;
	s_erc error;


	S_CLR_ERR(&error);
	parser = S_MALLOC(s_ini_parser, 1);
	if (parser == NULL)
	{
		S_FTL_ERR_PRINT(S_MEMERROR,
						"parser_create",
						"Failed to allocate memory for s_ini_parser object");
		return NULL;
	}

	parser->dict = s_hash_table_new(dict_free, (size_t)ceil(s_log2(S_ASCIILINESZ)), &error);
	if ((parser->dict == NULL) || (error != S_SUCCESS))
	{
		S_ERR_PRINT(S_FAILURE,
					"parser_create",
					"Call to \"s_hash_table_new\" failed");
		S_FREE(parser);
		return NULL;
    }

	parser->path = s_strdup_clib(path);

	return parser;
}


static void parser_free(s_ini_parser *parser)
{
	s_erc error;


	S_CLR_ERR(&error);

	if (parser->dict != NULL)
	{
		s_hash_table_delete(parser->dict, &error);
		if (error != S_SUCCESS)
			S_ERR_PRINT(S_FAILURE,
						"parser_free",
						"Call to \"s_hash_table_delete\" failed");
	}

	if (parser->path != NULL)
		S_FREE(parser->path);

	S_FREE(parser);
}


