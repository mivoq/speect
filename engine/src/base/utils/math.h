/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Miscellaneous math funtions used in the Speect Engine.                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_BASE_MATH_H__
#define _SPCT_BASE_MATH_H__


/**
 * @file math.h
 * Miscellaneous math funtions.
 */


/**
 * @ingroup SBaseUtils
 * @defgroup SMath Math Functions
 * Basic math functions, macros and constants used in the Speect Engine
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <math.h>
#include "include/common.h"
#include "base/utils/types.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Math Constants                                                                   */
/*                                                                                  */
/************************************************************************************/

/**
 * Definition of floating point differnce tollerance.
 */
#define S_FLOAT_TOLERANCE 0.000001


/**
 * Definition of Pi to 20 decimal places.
 */
#define S_PI 3.14159265358979323846


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * Absolute of a value.
 * @hideinitializer
 *
 * @param X Number of which to get absolute value of.
 *
 * @return Absolute value of @a X.
 *
 * @note Macro is type independent.
 */
#define S_ABS(X)    ((X) < 0 ? -(X) : (X))


/**
 * Minimum of two values.
 * @hideinitializer
 *
 * @param X Number x.
 * @param Y Number y.
 *
 * @return The minimum of @a X and @a Y.
 *
 * @note Macro is type independent.
 */
#define S_MIN(X, Y) (((X) < (Y)) ? (X) : (Y))


/**
 * Maximum of two values.
 * @hideinitializer
 *
 * @param X Number x.
 * @param Y Number y.
 *
 * @return The maximum of @a X and @a Y.
 *
 * @note Macro is type independent.
 */
#define S_MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


/**
 * Test if number is larger or equal to another number,
 * A >= X.
 * @hideinitializer
 *
 * @param A Number to test.
 * @param X Number to test against.
 *
 * @return 1 if A is larger or equal to X, else 0.
 *
 * @note Macro is type independent.
 */
#define S_NUM_LE(A, X) (((A) < (X)) ? 0 : 1)


/**
 * Test if number is smaller or equal to another number,
 * A <= X.
 * @hideinitializer
 *
 * @param A Number to test.
 * @param X Number to test against.
 *
 * @return 1 if A is smaller or equal to X, else 0.
 *
 * @note Macro is type independent.
 */
#define S_NUM_SE(A, X) (((A) > (X)) ? 0 : 1)


/**
 * Number in range, X <= A <= Y. Tests whether the given
 * number lies in the given range (inclusive).
 * @hideinitializer
 *
 * @param A The number to test.
 * @param X The left range boundary.
 * @param Y The right range boundary.
 *
 * @return 0 if not in range, else 1.
 *
 * @note Macro is type independent.
 */
#define S_NUM_IN_RANGE(A, X, Y) (S_NUM_LE(A, X) && S_NUM_SE(A, Y))


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Get the relative difference between two floating point
 * values. Relative distance is defined as the ratio of the difference
 * to the larger of the two values.
 *
 * @param a floating point value.
 * @param b floating point value.
 *
 * @return Relative difference between @a a and @a b.
 */
S_API double s_rel_diff(double a, double b);


/**
 * Test if two floating point numbers are equal within a certain
 * tolerance. If the relative diffence s_rel_diff() between the two
 * values are less or equal to #S_FLOAT_TOLERANCE then they are equal.
 *
 * @param a floating point value.
 * @param b floating point value.
 *
 * @return #TRUE or #FALSE.
 */
S_API s_bool s_float_equal(double a, double b);


/**
 * Return the logarithm (base 2) of the given number.
 *
 * @param a floating point value of which to calculate log2.
 *
 * @return logarithm (base 2) or @c -HUGE_VAL if @a a is
 * s_float_equal() to 0.
 */
S_API double s_log2(double a);


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

#endif /* _SPCT_BASE_MATH_H__ */
