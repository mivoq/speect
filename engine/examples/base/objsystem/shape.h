/************************************************************************************/
/* Copyright (c) 2011 The Department of Arts and Culture,                           */
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
/* DATE    : September 2011                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* 2d shape class as an example of Speect's object-oriented programming support.    */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_SHAPE_EXAMPLE_H__
#define _SPCT_SHAPE_EXAMPLE_H__


/**
 * @file shape.h
 * 2d shape class as an example of Speect's object-oriented
 * programming support.
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
 * @name Utility Macros
 * @{
 */


/**
 * @hideinitializer
 * Return the given #SShape child class object as a shape
 * object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SShape* type.
 *
 * @note This casting is not safety checked.
 */
#define S_SHAPE(SELF)    ((SShape *)(SELF))


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SShape definition                                                                */
/*                                                                                  */
/************************************************************************************/

/**
 * @ingroup SShape
 * A 2d shape class as an example of Speect's
 * object-oriented programming support.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject     obj;

	/**
	 * @protected x-coordinate of rectangle top left hand position.
	 */
	int x;

	/**
	 * @protected y-coordinate of rectangle top left hand position.
	 */
	int y;
} SShape;


/************************************************************************************/
/*                                                                                  */
/* SShapeClass definition                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * The abstract shape class structure.
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
	 * @protected Move function pointer.
	 * Move the given shape to the given coordinates.
	 *
	 * @param  self The shape to move.
	 * @param  newx The new x coordinate.
	 * @param  newy The new y coordinate.
	 * @param  error Error code.
	 */
	void  (* const move)   (SShape *self, int newx, int newy, s_erc *error);

	/**
	 * @protected Area function pointer.
	 * Calculate and return the given shape's area.
	 *
	 * @param self The shape to calculate the area of.
	 * @param  error Error code.
	 *
	 * @return The given shape's area.
	 */
	float (* const area)   (const SShape *self, s_erc *error);

} SShapeClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Move the given shape to the given coordinates.
 * @public @memberof SShape
 *
 * @param self The shape to move.
 * @param newx The new x coordinate.
 * @param newy The new y coordinate.
 * @param error Error code.
 */
S_API void SShapeMove(SShape *self, int newx, int newy, s_erc *error);


/**
 * Calculate and return the given shape's area.
 * @public @memberof SShape
 *
 * @param self The shape to calculate the area of.
 * @param error Error code.
 *
 * @return The given shape's area.
 */
S_API float SShapeArea(const SShape *self, s_erc *error);


/**
 * Register the SShape class to the object system.
 * @private @memberof SShape
 *
 * @param error Error code.
 */
S_LOCAL void _s_shape_class_reg(s_erc *error);


/**
 * Free the SShape class from the object system.
 * @private @memberof SShape
 *
 * @param error Error code.
 */
S_LOCAL void _s_shape_class_free(s_erc *error);


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

#endif /* _SPCT_SHAPE_EXAMPLE_H__ */
