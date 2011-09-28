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
/* 2d rectangle class as an example of Speect's object-oriented                     */
/* programming support.                                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_RECTANGLE_EXAMPLE_H__
#define _SPCT_RECTANGLE_EXAMPLE_H__


/**
 * @file rectangle.h
 * 2d rectangle class as an example of Speect's object-oriented
 * programming support.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "shape.h"


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
 * Return the given #SRectangle child class object as a rectangle
 * object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SRectangle* type.
 *
 * @note This casting is not safety checked.
 */
#define S_RECTANGLE(SELF)    ((SRectangle *)(SELF))


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SRectangle definition                                                            */
/*                                                                                  */
/************************************************************************************/

/**
 * @ingroup SRectangle
 * A 2d rectangle class as an example of Speect's object-oriented programming support.
 * @extends SShape
 */
typedef struct
{
	/**
	 * @protected Inherit from #SShape.
	 */
	SShape     obj;

	/**
	 * @protected Rectangle width.
	 */
    int width;

	/**
	 * @protected Rectangle height.
	 */
    int height;

} SRectangle;


/************************************************************************************/
/*                                                                                  */
/* SRectangleClass definition                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The rectangle class structure.
 * @extends SShapeClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SShapeClass.
	 */
	SShapeClass  _inherit;

	/* Class methods */
	/**
	 * @protected SetWidth function pointer.
	 * Set the width of the given rectangle.
	 *
	 * @param  self The rectangle to set the width of.
	 * @param  new_width The new width of the rectangle.
	 * @param  error Error code.
	 */
	void  (* const set_width)   (SRectangle *self, int new_width, s_erc *error);

	/**
	 * @protected SetHeight function pointer.
	 * Set the height of the given rectangle.
	 *
	 * @param  self The rectangle to set the height of.
	 * @param  new_height The new height of the rectangle.
	 * @param  error Error code.
	 */
	void  (* const set_height)   (SRectangle *self, int new_height, s_erc *error);

} SRectangleClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Create a new rectangle from the given parameters.
 * @public @memberof SRectangle
 *
 * @param x x-coordinate of rectangle top left hand position.
 * @param y y-coordinate of rectangle top left hand position.
 * @param width Rectangle width.
 * @param height Rectangle height.
 * @param error error Error code.
 *
 * @return Pointer to newly created rectangle.
 */
S_API SRectangle *SRectangleNew(int x, int y, int width, int height, s_erc *error);


/**
 * Set the width of the given rectangle.
 * @public @memberof SRectangle
 *
 * @param self The rectangle to set the width of.
 * @param new_width The new width of the rectangle.
 * @param error Error code.
 */
S_API void SRectangleSetWidth(SRectangle *self, int new_width, s_erc *error);


/**
 * Set the height of the given rectangle.
 * @public @memberof SRectangle
 *
 * @param self The rectangle to set the height of.
 * @param new_height The new height of the rectangle.
 * @param error Error code.
 */
S_API void SRectangleSetHeight(SRectangle *self, int new_height, s_erc *error);


/**
 * Register the SRectangle class to the object system.
 * @private @memberof SRectangle
 *
 * @param error Error code.
 */
S_LOCAL void _s_rectangle_class_reg(s_erc *error);


/**
 * Free the SRectangle class from the object system.
 * @private @memberof SRectangle
 *
 * @param error Error code.
 */
S_LOCAL void _s_rectangle_class_free(s_erc *error);


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

#endif /* _SPCT_RECTANGLE_EXAMPLE_H__ */
