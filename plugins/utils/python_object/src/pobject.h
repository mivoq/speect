/************************************************************************************/
/* Copyright (c) 2010 The Department of Arts and Culture,                           */
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
/* DATE    : April 2010                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Python object interface for Speect.                                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_SPYOBJECT_H__
#define _SPCT_PLUGIN_SPYOBJECT_H__


/**
 * @file pobject.h
 * A Python object interface for Speect.
 */


/**
 * @defgroup SPyObject Speect PyObject
 * A Python object interface for Speect.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "Python.h"
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
 * Return the given #SPyObject child/parent class object as a
 * #SPyObject object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SPyObject* type.
 * @note This casting is not safety checked.
 */
#define S_PYOBJECT(SELF)    ((SPyObject *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SPyObject,
 * see full description #S_PYOBJECT_CALL for usage.
 *
 * @param SELF The given #SPyObject*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_PYOBJECT_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_PYOBJECT_CALL(SELF, FUNC)					\
	((SPyObjectClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SPyObject
 * can be called.
 *
 * @param SELF The given #SPyObject*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_PYOBJECT_METH_VALID(SELF, FUNC)		\
	S_PYOBJECT_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* SPyObject definition                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * The Speect Python object structure.
 * @extends SVoid
 */
typedef struct
{
	/**
	 * @protected Inherit from #SVoid.
	 */
	SVoid     obj;

	/**
	 * @protected The Python object.
	 */
	PyObject *pobject;
} SPyObject;


/************************************************************************************/
/*                                                                                  */
/* SPyObjectClass definition                                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * The Speect Python object class structure, does not add any methods
 * to #SVoidClass, and therefore exactly the same.
 */
typedef SVoidClass SPyObjectClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SPyObject plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_pyobject_class_reg(s_erc *error);

/**
 * Free the #SPyObject plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_pyobject_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_SPYOBJECT_H__ */
