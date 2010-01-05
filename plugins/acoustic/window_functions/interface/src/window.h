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
/* An abstract window function class.                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_WINDOW_FUNC_H__
#define _SPCT_PLUGIN_WINDOW_FUNC_H__


/**
 * @file window.h
 * An abstract window function class.
 */


/**
 * @defgroup SWinFunc Window Functions
 * An abstract window function class. No methods are implemented.
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
 * Return the given #SWinFunc child/parent class object as a
 * #SWinFunc object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SWinFunc* type.
 * @note This casting is not safety checked.
 */
#define S_WINFUNC(SELF)    ((SWinFunc *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SWinFunc,
 * see full description #S_WINFUNC_CALL for usage.
 *
 * @param SELF The given #SWinFunc*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_WINFUNC_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_WINFUNC_CALL(SELF, FUNC)					\
	((SWinFuncClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SWinFunc
 * can be called.
 *
 * @param SELF The given #SWinFunc*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_WINFUNC_METH_VALID(SELF, FUNC)		\
	S_WINFUNC_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* SWinFunc definition                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * The SWinFunc structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject       obj;

	/**
	 * @protected The window function name.
	 */
	const char   *name;

	/**
	 * @protected Samples of the function.
	 */
	float        *samples;

	/**
	 * @protected Number of samples.
	 */
	uint32        num_samples;

	/**
	 * @protected Window centre point.
	 */
	sint32        centre;
} SWinFunc;

/************************************************************************************/
/*                                                                                  */
/* SWinFuncClass definition                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * The SWinFuncClass structure.
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
	 * Initialize a window function to the given size and centre point.
	 *
	 * @param self The window function object.
	 * @param size The requested size of the window.
	 * @param centre The centre sample of the window (relative to the
	 * given size), -1 for symmetric windows.
	 * @param error Error code.
	 *
	 * @note If the function fails then the given window function
	 * object is deleted and the @c self variable is set to @c NULL.
	 */
	void (*init_window)(SWinFunc **self, sint32 size, sint32 centre, s_erc *error);
} SWinFuncClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SWinFunc plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_winfunc_class_reg(s_erc *error);

/**
 * Free the #SWinFunc plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_winfunc_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_WINDOW_FUNC_H__ */
