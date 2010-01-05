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
/* An abstract halfphone database class.                                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_HALFPHONE_DB_H__
#define _SPCT_PLUGIN_HALFPHONE_DB_H__


/**
 * @file halfphone_db.h
 * An abstract halfphone database class
 */


/**
 * @defgroup SHalfphoneDB Halfphone Database
 * An abstract halfphone database class. No methods are implemented.
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
 * Return the given #SHalfphoneDB child/parent class object as a
 * #SHalfphoneDB object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SHalfphoneDB* type.
 * @note This casting is not safety checked.
 */
#define S_HALFPHONE_DB(SELF)    ((SHalfphoneDB *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SHalfphoneDB,
 * see full description #S_HALFPHONE_DB_CALL for usage.
 *
 * @param SELF The given #SHalfphoneDB*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_HALFPHONE_DB_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_HALFPHONE_DB_CALL(SELF, FUNC)					\
	((SHalfphoneDBClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SHalfphoneDB
 * can be called.
 *
 * @param SELF The given #SHalfphoneDB*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_HALFPHONE_DB_METH_VALID(SELF, FUNC)		\
	S_HALFPHONE_DB_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The halfphone database information structure.
 */
typedef struct
{
	/**
	 * The name of the halfphone database.
	 */
	char       *name;

	/**
	 * A short description of the halfphone database.
	 */
	char       *description;

	/**
	 * The halfphone database language.
	 */
	char       *language;

	/**
	 * The halfphone database language code, <tt>ISO 639-2</tt>.
	 * @note See http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes
	 */
	char       *lang_code;

	/**
	 * Halfphone database version.
	 */
	s_version   version;
} s_halfphone_db_info;


/************************************************************************************/
/*                                                                                  */
/* SHalfphoneDB definition                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * The SHalfphoneDB structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject       obj;

	/**
	 * @protected Halfphone database information.
	 */
	s_halfphone_db_info   *info;

	/**
	 * @protected Features.
	 */
	SMap         *features;
} SHalfphoneDB;

/************************************************************************************/
/*                                                                                  */
/* SHalfphoneDBClass definition                                                     */
/*                                                                                  */
/************************************************************************************/

/**
 * The SHalfphoneDBClass structure.
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
	 * Get the name of the given halfphone database.
	 *
	 * @param self The given halfphone database.
	 * @param error Error code.
	 *
	 * @return The name of the given halfphone database.
	 */
	const char      *(*get_name)           (const SHalfphoneDB *self, s_erc *error);

	/**
	 * Get the description of the given halfphone database.
	 *
	 * @param self The given halfphone database.
	 * @param error Error code.
	 *
	 * @return The description of the given halfphone database.
	 */
	const char      *(*get_description)    (const SHalfphoneDB *self, s_erc *error);

	/**
	 * Get the language of the given halfphone database.
	 *
	 * @param self The given halfphone database.
	 * @param error Error code.
	 *
	 * @return The language of the given halfphone database.
	 */
	const char      *(*get_language)       (const SHalfphoneDB *self, s_erc *error);

	/**
	 * Get the language code (<tt>ISO 639-2</tt>) of the given halfphone database.
	 *
	 * @param self The given halfphone database.
	 * @param error Error code.
	 *
	 * @return The language of the given halfphone database.
	 *
	 * @note See http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes
	 * for the language codes.
	 */
	const char      *(*get_lang_code)      (const SHalfphoneDB *self, s_erc *error);

	/**
	 * Get the version of the given halfphone database.
	 *
	 * @param self The given halfphone database.
	 * @param error Error code.
	 *
	 * @return The version of the given halfphone database.
	 */
	const s_version *(*get_version)        (const SHalfphoneDB *self, s_erc *error);

	/**
	 * Get the halfphone database feature object of the named key.
	 *
	 * @param self The given halfphone database.
	 * @param key The string key of the feature object to get.
	 * @param error Error code.
	 *
	 * @return Pointer to the feature object of the named key, or @a NULL if
	 * feature of named key is not present in halfphone database.
	 */
	const SObject   *(*get_feature)        (const SHalfphoneDB *self, const char *key,
											s_erc *error);

	/**
	 * Get a list of units (#SItem) from the halfphone database that
	 * conform to the given context.
	 *
	 * @param self The given halfphone database.
	 * @param unit_type The unit type to get.
	 * @param left_context The left context of the units to get (can
	 * be @c NULL).
	 * @param right_context The right context of the units to get (can
	 * be @c NULL).
	 * @param min_triphones The minimum number of triphone contexts
	 * required. If this number is not satisfied then diphone contexts
	 * are also searched for.
	 * @param min_diphones The minimum number of diphone contexts
	 * required. If this number is not satisfied then all units of the
	 * given unit type are returned.
	 * @param error Error code.
	 *
	 * @return A list (#SList) of unit items (#SItem) that satisfy the
	 * given context, or @c NULL if no such units.
	 *
	 * @note The items in the list @b must be regarded as constants
	 * and not be modified in any way.
	 */
	const SList     *(*get_units)          (const SHalfphoneDB *self, const char *unit_type,
											const char *left_context, const char *right_context,
											uint min_triphones, uint min_diphones, s_erc *error);
} SHalfphoneDBClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SHalfphoneDB plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_halfphone_db_class_reg(s_erc *error);

/**
 * Free the #SHalfphoneDB plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_halfphone_db_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_HALFPHONE_DB_H__ */
