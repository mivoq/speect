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
/* An abstract lexicon class.                                                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_LEXICON_H__
#define _SPCT_PLUGIN_LEXICON_H__


/**
 * @file lexicon.h
 * An abstract lexicon class
 */


/**
 * @defgroup SLexicon Lexicon
 * An abstract lexicon class. No methods are implemented.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given #SLexicon child/parent class object as a
 * #SLexicon object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SLexicon* type.
 * @note This casting is not safety checked.
 */
#define S_LEXICON(SELF)    ((SLexicon *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SLexicon,
 * see full description #S_LEXICON_CALL for usage.
 *
 * @param SELF The given #SLexicon*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_LEXICON_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_LEXICON_CALL(SELF, FUNC)					\
	((SLexiconClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SLexicon
 * can be called.
 *
 * @param SELF The given #SLexicon*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_LEXICON_METH_VALID(SELF, FUNC)		\
	S_LEXICON_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The lexicon information structure.
 */
typedef struct
{
	/**
	 * The name of the lexicon.
	 */
	char       *name;

	/**
	 * A short description of the lexicon.
	 */
	char       *description;

	/**
	 * The lexicon language.
	 */
	char       *language;

	/**
	 * The lexicon language code, <tt>ISO 639-2</tt>.
	 * @note See http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes
	 */
	char       *lang_code;

	/**
	 * Lexicon version.
	 */
	s_version   version;
} s_lex_info;


/************************************************************************************/
/*                                                                                  */
/* SLexicon definition                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * The SLexicon structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject       obj;

	/**
	 * @protected Lexicon information.
	 */
	s_lex_info   *info;

	/**
	 * @protected Features.
	 */
	SMap         *features;
} SLexicon;

/************************************************************************************/
/*                                                                                  */
/* SLexiconClass definition                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * The SLexiconClass structure.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass _inherit;

	/* Class methods */
	/**
	 * Get the name of the given lexicon.
	 *
	 * @param self The given lexicon.
	 * @param error Error code.
	 *
	 * @return The name of the given lexicon.
	 */
	const char      *(*get_name)           (const SLexicon *self, s_erc *error);

	/**
	 * Get the description of the given lexicon.
	 *
	 * @param self The given lexicon.
	 * @param error Error code.
	 *
	 * @return The description of the given lexicon.
	 */
	const char      *(*get_description)    (const SLexicon *self, s_erc *error);

	/**
	 * Get the language of the given lexicon.
	 *
	 * @param self The given lexicon.
	 * @param error Error code.
	 *
	 * @return The language of the given lexicon.
	 */
	const char      *(*get_language)       (const SLexicon *self, s_erc *error);

	/**
	 * Get the language code (<tt>ISO 639-2</tt>) of the given lexicon.
	 *
	 * @param self The given lexicon.
	 * @param error Error code.
	 *
	 * @return The language of the given lexicon.
	 *
	 * @note See http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes
	 * for the language codes.
	 */
	const char      *(*get_lang_code)      (const SLexicon *self, s_erc *error);

	/**
	 * Get the version of the given lexicon.
	 *
	 * @param self The given lexicon.
	 * @param error Error code.
	 *
	 * @return The version of the given lexicon.
	 */
	const s_version *(*get_version)        (const SLexicon *self, s_erc *error);

	/**
	 * Get the lexicon feature object of the named key.
	 *
	 * @param self The given lexicon.
	 * @param key The string key of the feature object to get.
	 * @param error Error code.
	 *
	 * @return Pointer to the feature object of the named key, or @a NULL if
	 * feature of named key is not present in lexicon.
	 */
	const SObject   *(*get_feature)        (const SLexicon *self, const char *key,
											s_erc *error);

	/**
	 * Get a word from the lexicon.
	 *
	 * @param self The given lexicon.
	 * @param word The word to get.
	 * @param features Specific features which might distinguish the
	 * word if multiple entries of the word exists in the lexicon. If
	 * @c NULL then the first entry of the word is returned.
	 * @param syllabified A flag that the lexicon sets if the returned
	 * word is syllabified,  @c TRUE, else it is set to @c FALSE.
	 * @param error Error code.
	 *
	 * @retval phones A list of phones for the given word (no syllable
	 * list was defined in the lexicon), and @c syllabified will be @c FALSE.
	 * @retval syllables A list of syllables, where the syllables are
	 * lists of phones, and @c syllabified will be @c TRUE.
	 * @retval NULL If word was not found in the lexicon, or error
	 * occurred.
	 *
	 * @note The caller is responsible for the memory of the returned
	 * list.
	 */
	SList           *(*get_word)           (const SLexicon *self, const char *word,
											const SMap *features, s_bool *syllabified,
											s_erc *error);
} SLexiconClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SLexicon plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_lexicon_class_reg(s_erc *error);

/**
 * Free the #SLexicon plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_lexicon_class_free(s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


/**
 * @}
 * end documentation
 */

#endif /* _SPCT_PLUGIN_LEXICON_H__  */
