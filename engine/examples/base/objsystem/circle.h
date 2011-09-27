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
/* 2d circle class as an example of Speect's object-oriented                        */
/* programming support.                                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_CIRCLE_EXAMPLE_H__
#define _SPCT_CIRCLE_EXAMPLE_H__


/**
 * @file circle.h
 * 2d circle class as an example of Speect's object-oriented
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
 * Return the given #SCircle child class object as a circle
 * object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SCircle* type.
 *
 * @note This casting is not safety checked.
 */
#define S_CIRCLE(SELF)    ((SCircle *)(SELF))


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SCircle definition                                                               */
/*                                                                                  */
/************************************************************************************/

/**
 * @ingroup SCircle
 * A 2d circle class as an example of Speect's object-oriented programming support.
 * @extends SShape
 */
typedef struct
{
	/**
	 * @protected Inherit from #SShape.
	 */
	SShape     obj;

	/**
	 * @protected x-coordinate of circle top left hand position.
	 */
	int x;

	/**
	 * @protected y-coordinate of circle top left hand position.
	 */
	int y;

	/**
	 * @protected Circle radius.
	 */
    int radius;

	/**
	 * @protected Circle colour.
	 */
    char *colour;

} SCircle;


/************************************************************************************/
/*                                                                                  */
/* SCircleClass definition                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * The circle class structure.
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
	 * @protected SetRadius function pointer.
	 * Set the radius of the given circle.
	 *
	 * @param  self The circle to set the radius of.
	 * @param  new_radius The new radius of the circle.
	 * @param  error Error code.
	 */
	void  (* const set_radius)   (SCircle *self, int new_radius, s_erc *error);

	/**
	 * @protected SetColour function pointer.
	 * Set the colour of the given circle.
	 *
	 * @param  self The circle to set the colour of.
	 * @param  new_colour The new colour of the circle.
	 * @param  error Error code.
	 */
	void  (* const set_colour)   (SCircle *self, const char *new_colour, s_erc *error);

} SCircleClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Create a new circle from the given parameters.
 * @public @memberof SCircle
 *
 * @param x x-coordinate of circle top left hand position.
 * @param y y-coordinate of circle top left hand position.
 * @param radius Circle radius.
 * @param colour Circle colour.
 * @param error error Error code.
 *
 * @return Pointer to newly created circle.
 */
S_API SCircle *SCircleNew(int x, int y, int radius, const char *colour, s_erc *error);


/**
 * Set the radius of the given circle.
 * @public @memberof SCircle
 *
 * @param self The circle to set the radius of.
 * @param new_radius The new radius of the circle.
 * @param error Error code.
 */
S_API void SCircleSetRadius(SCircle *self, int new_radius, s_erc *error);


/**
 * Set the colour of the given circle.
 * @public @memberof SCircle
 *
 * @param self The circle to set the colour of.
 * @param new_colour The new colour of the circle.
 * @param error Error code.
 */
S_API void SCircleSetColour(SCircle *self, const char *new_colour, s_erc *error);


/**
 * Register the SCircle class to the object system.
 * @private @memberof SCircle
 *
 * @param error Error code.
 */
S_LOCAL void _s_circle_class_reg(s_erc *error);


/**
 * Free the SCircle class from the object system.
 * @private @memberof SCircle
 *
 * @param error Error code.
 */
S_LOCAL void _s_circle_class_free(s_erc *error);


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

#endif /* _SPCT_CIRCLE_EXAMPLE_H__ */
