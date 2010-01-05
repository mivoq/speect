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
/* An abstract syllabification class.                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_SYLLABIFICATION_H__
#define _SPCT_PLUGIN_SYLLABIFICATION_H__


/**
 * @file syllabification.h
 * An abstract syllabification class
 */


/**
 * @defgroup SSyllabification Syllabification
 * An abstract syllabification class. No methods are implemented.
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
 * Return the given #SSyllabification child/parent class object as a
 * #SSyllabification object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SSyllabification* type.
 * @note This casting is not safety checked.
 */
#define S_SYLLABIFICATION(SELF)    ((SSyllabification *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SSyllabification,
 * see full description #S_SYLLABIFICATION_CALL for usage.
 *
 * @param SELF The given #SSyllabification*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_SYLLABIFICATION_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_SYLLABIFICATION_CALL(SELF, FUNC)					\
	((SSyllabificationClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SSyllabification
 * can be called.
 *
 * @param SELF The given #SSyllabification*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_SYLLABIFICATION_METH_VALID(SELF, FUNC)		\
	S_SYLLABIFICATION_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* SSyllabification definition                                                      */
/*                                                                                  */
/************************************************************************************/

/**
 * The SSyllabification structure. Does not add any members to the
 * #SObject object, therefore exactly the same.
 */
typedef SObject SSyllabification;


/************************************************************************************/
/*                                                                                  */
/* SSyllabificationClass definition                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * The SSyllabificationClass structure.
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
	 * Syllabify the given phone list of the given word item.
	 *
	 * @param word The word item.
	 * @param phoneList The list of phones for the given word item.
	 * @param error Error code.
	 *
	 * @return A List of lists where the primary list is syllables and
	 * the secondary lists are the phones in the syllables. for example :
	 * for mathematics phonelist is : @code (m , ae , th, ax, m, ae, t, ih, k, s) @endcode,
	 * the returned list is : @code ((m, ae), (th, ax), (m, ae), (t, ih, k, s)) @endcode
	 *
	 * @note The caller is responsible for the memory of the returned list.
	 */
	SList *(*syllabify)  (const SItem *word, const SList *phoneList, s_erc *error);
} SSyllabificationClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SSyllabification plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_syllabification_class_reg(s_erc *error);

/**
 * Free the #SSyllabification plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_syllabification_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_SYLLABIFICATION_H__  */
