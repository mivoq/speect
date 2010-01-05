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
/* An abstract addendum class, very much the same as the SLexicon class.            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_ADDENDUM_H__
#define _SPCT_PLUGIN_ADDENDUM_H__


/**
 * @file addendum.h
 * An abstract addendum class.
 */


/**
 * @defgroup SAddendum Addendum
 * An abstract addendum class. No methods are implemented.
 *
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
 * Return the given #SAddendum child/parent class object as a
 * #SAddendum object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SAddendum* type.
 * @note This casting is not safety checked.
 */
#define S_ADDENDUM(SELF)    ((SAddendum *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SAddendum,
 * see full description #S_ADDENDUM_CALL for usage.
 *
 * @param SELF The given #SAddendum*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_ADDENDUM_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_ADDENDUM_CALL(SELF, FUNC)					\
	((SAddendumClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SAddendum
 * can be called.
 *
 * @param SELF The given #SAddendum*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_ADDENDUM_METH_VALID(SELF, FUNC)		\
	S_ADDENDUM_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The addendum information structure.
 */
typedef struct
{
	/**
	 * The name of the addendum.
	 */
	char       *name;

	/**
	 * A short description of the addendum.
	 */
	char       *description;

	/**
	 * The addendum language.
	 */
	char       *language;

	/**
	 * The addendum language code, <tt>ISO 639-2</tt>.
	 * @note See http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes
	 */
	char       *lang_code;

	/**
	 * Addendum version.
	 */
	s_version   version;
} s_addendum_info;


/************************************************************************************/
/*                                                                                  */
/* SAddendum definition                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * The SAddendum structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject          obj;

	/**
	 * @protected Addendum information.
	 */
	s_addendum_info *info;

	/**
	 * @protected Features.
	 */
	SMap            *features;
} SAddendum;


/************************************************************************************/
/*                                                                                  */
/* SAddendumClass definition                                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * The SAddendumClass structure.
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
	 * Get the name of the given addendum.
	 *
	 * @param self The given addendum.
	 * @param error Error code.
	 *
	 * @return The name of the given addendum.
	 */
	const char      *(*get_name)           (const SAddendum *self, s_erc *error);

	/**
	 * Get the description of the given addendum.
	 *
	 * @param self The given addendum.
	 * @param error Error code.
	 *
	 * @return The description of the given addendum.
	 */
	const char      *(*get_description)    (const SAddendum *self, s_erc *error);

	/**
	 * Get the language of the given addendum.
	 *
	 * @param self The given addendum.
	 * @param error Error code.
	 *
	 * @return The language of the given addendum.
	 */
	const char      *(*get_language)       (const SAddendum *self, s_erc *error);

	/**
	 * Get the language code (<tt>ISO 639-2</tt>) of the given addendum.
	 *
	 * @param self The given addendum.
	 * @param error Error code.
	 *
	 * @return The language of the given addendum.
	 *
	 * @note See http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes
	 * for the language codes.
	 */
	const char      *(*get_lang_code)      (const SAddendum *self, s_erc *error);

	/**
	 * Get the version of the given addendum.
	 *
	 * @param self The given addendum.
	 * @param error Error code.
	 *
	 * @return The version of the given addendum.
	 */
	const s_version *(*get_version)        (const SAddendum *self, s_erc *error);

	/**
	 * Get the addendum feature object of the named key.
	 *
	 * @param self The given addendum.
	 * @param key The string key of the feature object to get.
	 * @param error Error code.
	 *
	 * @return Pointer to the feature object of the named key, or @a NULL if
	 * feature of named key is not present in addendum.
	 */
	const SObject   *(*get_feature)        (const SAddendum *self, const char *key,
											s_erc *error);

	/**
	 * Get a word from the addendum.
	 *
	 * @param self The given addendum.
	 * @param word The word to get.
	 * @param features Specific features which might distinguish the
	 * word if multiple entries of the word exists in the addendum. If
	 * @c NULL then the first entry of the word is returned.
	 * @param syllabified A flag that the addendum sets if the returned
	 * word is syllabified,  @c TRUE, else it is set to @c FALSE.
	 * @param error Error code.
	 *
	 * @retval phones A list of phones for the given word (no syllable
	 * list was defined in the addendum), and @c syllabified will be @c FALSE.
	 * @retval syllables A list of syllables, where the syllables are
	 * lists of phones, and @c syllabified will be @c TRUE.
	 * @retval NULL If word was not found in the addendum, or error
	 * occurred.
	 *
	 * @note The caller is responsible for the memory of the returned
	 * list.
	 */
	SList           *(*get_word)           (const SAddendum *self, const char *word,
											const SMap *features, s_bool *syllabified,
											s_erc *error);

	/**
	 * Add a word to the addendum.
	 *
	 * @param self The given addendum.
	 * @param word The word to add.
	 * @param features Specific features of the word. Features are
	 * used to distinguish homographs (i.e. part-of-speech). At least
	 * a "phones" or a "syllables" feature must exists in the given @c
	 * features map.
	 * @param error Error code.
	 *
	 * @note The addendum takes hold a the @c features object.
	 * @note The added entry is not saved in the addendum file (not
	 * persistent).
	 */
	void             (*add_word)           (SAddendum *self, const char *word,
											SMap *features, s_erc *error);
} SAddendumClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SAddendum plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_addendum_class_reg(s_erc *error);

/**
 * Free the #SAddendum plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_addendum_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_ADDENDUM_H__  */
