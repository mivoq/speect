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
/* An abstract g2p class from which specific implementations can inherit.           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_G2P_H__
#define _SPCT_PLUGIN_G2P_H__


/**
 * @file g2p.h
 * An abstract g2p class from which specific implementations can inherit.
 */


/**
 * @defgroup SG2P G2P
 * An abstract g2p class from which specific implementations can inherit.
 * No class methods are implemented.
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
 * Return the given #SG2P child/parent class object as a
 * #SG2P object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SG2P* type.
 * @note This casting is not safety checked.
 */
#define S_G2P(SELF)    ((SG2P *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SG2P,
 * see full description #S_G2P_CALL for usage.
 *
 * @param SELF The given #SG2P*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_G2P_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_G2P_CALL(SELF, FUNC)					\
	((SG2PClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SG2P
 * can be called.
 *
 * @param SELF The given #SG2P*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_G2P_METH_VALID(SELF, FUNC)		\
	S_G2P_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The g2p information structure.
 */
typedef struct
{
	/**
	 * The name of the g2p.
	 */
	char       *name;

	/**
	 * A short description of the g2p.
	 */
	char       *description;

	/**
	 * The g2p language.
	 */
	char       *language;

	/**
	 * The g2p language code, <tt>ISO 639-2</tt>.
	 * @note See http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes
	 */
	char       *lang_code;

	/**
	 * G2P version.
	 */
	s_version   version;
} s_g2p_info;


/************************************************************************************/
/*                                                                                  */
/* SG2P definition                                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * The SG2P structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject       obj;

	/**
	 * @protected G2P information.
	 */
	s_g2p_info   *info;

	/**
	 * @protected Features.
	 */
	SMap         *features;
} SG2P;

/************************************************************************************/
/*                                                                                  */
/* SG2PClass definition                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * The SG2PClass structure.
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
	 * Get the name of the given g2p.
	 *
	 * @param self The given g2p.
	 * @param error Error code.
	 *
	 * @return The name of the given g2p.
	 */
	const char      *(*get_name)           (const SG2P *self, s_erc *error);

	/**
	 * Get the description of the given g2p.
	 *
	 * @param self The given g2p.
	 * @param error Error code.
	 *
	 * @return The description of the given g2p.
	 */
	const char      *(*get_description)    (const SG2P *self, s_erc *error);

	/**
	 * Get the language of the given g2p.
	 *
	 * @param self The given g2p.
	 * @param error Error code.
	 *
	 * @return The language of the given g2p.
	 */
	const char      *(*get_language)       (const SG2P *self, s_erc *error);

	/**
	 * Get the language code (<tt>ISO 639-2</tt>) of the given g2p.
	 *
	 * @param self The given g2p.
	 * @param error Error code.
	 *
	 * @return The language of the given g2p.
	 *
	 * @note See http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes
	 * for the language codes.
	 */
	const char      *(*get_lang_code)      (const SG2P *self, s_erc *error);

	/**
	 * Get the version of the given g2p.
	 *
	 * @param self The given g2p.
	 * @param error Error code.
	 *
	 * @return The version of the given g2p.
	 */
	const s_version *(*get_version)        (const SG2P *self, s_erc *error);

	/**
	 * Get the g2p feature object of the named key.
	 *
	 * @param self The given g2p.
	 * @param key The string key of the feature object to get.
	 * @param error Error code.
	 *
	 * @return Pointer to the feature object of the named key, or @a NULL if
	 * feature of named key is not present in g2p.
	 */
	const SObject   *(*get_feature)        (const SG2P *self, const char *key,
											s_erc *error);

	/**
	 * Apply the g2p to the given word to get a phone list.
	 *
	 * @param self The given g2p.
	 * @param word The word to convert to a phone list.
	 * @param error Error code.
	 *
	 * @return A list of phones for the given word or @c NULL if the
	 * g2p failed. If the g2p failed a warning will be set in the
	 * error log.
	 *
	 * @note Caller is responsible for memory of returned phone list
	 */
	SList           *(*apply)              (const SG2P *self, const char *word,
											s_erc *error);

	/**
	 * Apply the g2p to the grapheme at given index in the given word
	 * to get a phone. This function can be seen as a single step of
	 * the #SG2PClass::apply function. It returns the phone of the
	 * grapheme at the index.
	 *
	 * @param self The given g2p.
	 * @param word The word.
	 * @param index The index of the grapheme, in the word, for which
	 * the phone is required (indexing starts at @c 0).
	 * @param error Error code.
	 *
	 * @return The phone of the grapheme at the index in the word.
	 *
	 * @note The returned phone can be @c NULL, this is not
	 * necessarily an error.
	 */
	const char      *(*apply_at)           (const SG2P *self, const char *word,
											uint index, s_erc *error);

} SG2PClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SG2P plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_g2p_class_reg(s_erc *error);

/**
 * Free the #SG2P plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_g2p_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_G2P_H__  */
