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
/* An abstract base cost function class used in unit selection.                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_COST_FUNCTION_H__
#define _SPCT_PLUGIN_COST_FUNCTION_H__


/**
 * @file cost_function.h
 * A cost function class used in unit selection.
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
 * Return the given #SCostFunction child/parent class object as a
 * #SCostFunction object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SCostFunction* type.
 * @note This casting is not safety checked.
 */
#define S_COST_FUNCTION(SELF)    ((SCostFunction *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SCostFunction,
 * see full description #S_COST_FUNCTION_CALL for usage.
 *
 * @param SELF The given #SCostFunction*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_COST_FUNCTION_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_COST_FUNCTION_CALL(SELF, FUNC)				\
	((SCostFunctionClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SCostFunction
 * can be called.
 *
 * @param SELF The given #SCostFunction*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_COST_FUNCTION_METH_VALID(SELF, FUNC)			\
	S_COST_FUNCTION_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* SCostFunction definition                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * The cost function structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject  obj;

	/**
	 * @protected Cost function weight.
	 */
	uint32   weight;

	/**
	 * @protected Feature name.
	 */
	char    *feature_name;
} SCostFunction;


/************************************************************************************/
/*                                                                                  */
/* SCostFunctionClass definition                                                    */
/*                                                                                  */
/************************************************************************************/

/**
 * The cost function class structure.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass  _inherit;

	/* Class methods */
	/**
	 * Set this cost function's feature on the given item.
	 *
	 * @param self The cost function object.
	 * @param item The item for which to set the feature.
	 * @param error Error code.
	 *
	 */
	void (*set_feature)(const SCostFunction *self, SItem *item, s_erc *error);

	/**
	 * Calculate the cost between the two given items.
	 *
	 * @param self The cost function object.
	 * @param item1 The first item.
	 * @param item2 The second item.
	 * @param error Error code.
	 *
	 * @return The cost between the two given items.
	 */
	float (*get_cost)  (const SCostFunction *self, const SItem *item1, const SItem *item2,
						s_erc *error);
} SCostFunctionClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SCostFunction plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_cost_function_class_reg(s_erc *error);

/**
 * Free the #SCostFunction plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_cost_function_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_COST_FUNCTION_H__ */
