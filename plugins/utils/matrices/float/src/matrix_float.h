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
/* A simple floating point matrix class which allows us to use                      */
/* floating point matrices as SObjects                                              */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_MATRIX_FLOAT_H__
#define _SPCT_PLUGIN_MATRIX_FLOAT_H__


/**
 * @file matrix_float.h
 * A simple floating point matrix class.
 */


/**
 * @ingroup SObjSystem
 * @defgroup SMatrixFloat Float Matrix Objects
 * A simple floating point matrix class which allows us to use
 * floating point matrices as SObjects.
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
 * Return the given #SMatrixFloat child/parent class object as a
 * #SMatrixFloat object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SMatrixFloat* type.
 * @note This casting is not safety checked.
 */
#define S_MATRIX_FLOAT(SELF)    ((SMatrixFloat *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* SMatrixFloat definition                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * The floateger matrix structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject  obj;

	/**
	 * @protected The matrix data (@code f[rows][columns] @endcode).
	 */
	float  **f;

	/**
	 * @protected The number of rows in the matrix.
	 */
	uint32   row_count;

	/**
	 * @protected The number of columns in the matrix.
	 */
	uint32   col_count;
} SMatrixFloat;


/************************************************************************************/
/*                                                                                  */
/* SMatrixFloatClass definition                                                     */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef of the floating point matrix class. The class does not add any new
 * methods to #SObjectClass and is therefore exactly the same.
 */
typedef SObjectClass SMatrixFloatClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SMatrixFloat plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_matrix_float_class_reg(s_erc *error);

/**
 * Free the #SMatrixFloat plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_matrix_float_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_MATRIX_FLOAT_H__ */
