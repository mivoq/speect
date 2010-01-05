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
/* An abstract phoneset class. The phones in the phoneset have binary features,     */
/* i.e. a phone either has a specific named feature or it doesn't.                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_PHONESET_H__
#define _SPCT_PLUGIN_PHONESET_H__


/**
 * @file phoneset.h
 * An abstract phoneset class.
 */


/**
 * @defgroup SPhoneset Phoneset
 * An abstract phoneset class. The phones in the phoneset have binary features,
 * i.e. a phone either has a specific named feature or it doesn't. No
 * methods are implemented.
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
 * Return the given #SPhoneset child/parent class object as a
 * #SPhoneset object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SPhoneset* type.
 * @note This casting is not safety checked.
 */
#define S_PHONESET(SELF)    ((SPhoneset *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SPhoneset,
 * see full description #S_PHONESET_CALL for usage.
 *
 * @param SELF The given #SPhoneset*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_PHONESET_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters
 * passed to the object function @c func.
 */
#define S_PHONESET_CALL(SELF, FUNC)					\
	((SPhonesetClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SPhoneset
 * can be called.
 *
 * @param SELF The given #SPhoneset*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_PHONESET_METH_VALID(SELF, FUNC)		\
	S_PHONESET_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The phoneset information structure.
 */
typedef struct
{
	/**
	 * The name of the phoneset.
	 */
	char       *name;

	/**
	 * A short description of the phoneset.
	 */
	char       *description;

	/**
	 * The phoneset language.
	 */
	char       *language;

	/**
	 * The phoneset language code, <tt>ISO 639-2</tt>.
	 * @note See http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes
	 */
	char       *lang_code;

	/**
	 * Phoneset version.
	 */
	s_version   version;
} s_phoneset_info;


/************************************************************************************/
/*                                                                                  */
/* SPhoneset definition                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * The SPhoneset structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject          obj;

	/**
	 * @protected Phoneset information.
	 */
	s_phoneset_info *info;

	/**
	 * @protected Features
	 */
	SMap            *features;
} SPhoneset;


/************************************************************************************/
/*                                                                                  */
/* SPhonesetClass definition                                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * The SPhonesetClass structure.
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
	 * Get the name of the given phoneset.
	 *
	 * @param self The given phoneset.
	 * @param error Error code.
	 *
	 * @return The name of the given phoneset.
	 */
	const char      *(*get_name)           (const SPhoneset *self, s_erc *error);

	/**
	 * Get the description of the given phoneset.
	 *
	 * @param self The given phoneset.
	 * @param error Error code.
	 *
	 * @return The description of the given phoneset.
	 */
	const char      *(*get_description)    (const SPhoneset *self, s_erc *error);

	/**
	 * Get the language of the given phoneset.
	 *
	 * @param self The given phoneset.
	 * @param error Error code.
	 *
	 * @return The language of the given phoneset.
	 */
	const char      *(*get_language)       (const SPhoneset *self, s_erc *error);

	/**
	 * Get the language code (<tt>ISO 639-2</tt>) of the given phoneset.
	 *
	 * @param self The given phoneset.
	 * @param error Error code.
	 *
	 * @return The language of the given phoneset.
	 *
	 * @note See http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes
	 * for the language codes.
	 */
	const char      *(*get_lang_code)      (const SPhoneset *self, s_erc *error);

	/**
	 * Get the version of the given phoneset.
	 *
	 * @param self The given phoneset.
	 * @param error Error code.
	 *
	 * @return The version of the given phoneset.
	 */
	const s_version *(*get_version)        (const SPhoneset *self, s_erc *error);

	/**
	 * Get the phoneset feature object of the named key.
	 *
	 * @param self The given phoneset.
	 * @param key The string key of the feature object to get.
	 * @param error Error code.
	 *
	 * @return Pointer to the feature object of the named key, or @a NULL if
	 * feature of named key is not present in phoneset.
	 */
	const SObject   *(*get_feature)        (const SPhoneset *self, const char *key,
											s_erc *error);

	/**
	 * Query if the phone in the phoneset has the named feature. The
	 * phones in the phoneset have binary features, i.e. a phone
	 * either has a specific named feature or it doesn't.
	 *
	 * @param self The given phoneset.
	 * @param phone The phone for which the feature list is requested.
	 * @param feature The feature for which the query is done.
	 * @param error Error code.
	 *
	 * @return @c TRUE if the feature is defined for the given phone,
	 * else @c FALSE. If the given phone is not in the phoneset the @c
	 * FALSE is returned.
	 */
	s_bool           (*phone_has_feature)  (const SPhoneset *self, const char *phone,
											const char *feature, s_erc *error);

	/**
	 * Query if the phone is present in the phoneset.
	 *
	 * @param self The given phoneset.
	 * @param phone The phone for which the query is done.
	 * @param error Error code.
	 *
	 * @return @c TRUE if the phone is present else @c FALSE.
	 */
	s_bool           (*has_phone)          (const SPhoneset *self, const char *phone,
											s_erc *error);

	/**
	 * Get a list of the given phone's features.
	 *
	 * @param self The given phoneset.
	 * @param phone The phone for which the feature list is requested.
	 * @error Error code.
	 *
	 * @return A list of string objects (use #SObjectGetString to get
	 * the string from the object) which are the features of the
	 * phones, or @c NULL if the phone is not in the phoneset.
	 */
	const SList     *(*get_phone_features) (const SPhoneset *self, const char *phone,
											s_erc *error);

	/**
	 * Get a list of phones in the phoneset.
	 *
	 * @param self The given phoneset.
	 * @param error Error code.
	 *
	 * @return A list of phones that are in the phoneset.
	 *
	 * @note The caller is responsible for the memory of the returned
	 * list.
	 */
	SList           *(*get_phone_list)     (const SPhoneset *self, s_erc *error);
} SPhonesetClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SPhoneset plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_phoneset_class_reg(s_erc *error);

/**
 * Free the #SPhoneset plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_phoneset_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_PHONESET_H__ */
