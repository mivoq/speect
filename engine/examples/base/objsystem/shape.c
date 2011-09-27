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
/* Abstract 2d shape class as an example of Speect's object-oriented                */
/* programming support.                                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "shape.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #SShape,
 * see full description #S_SHAPE_CALL for usage.
 *
 * @param SELF The given #SShape*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage:
 @verbatim
 S_SHAPE_CALL(self, func)(param1, param2, ..., paramN);
 @endverbatim
 * where @c param1, @c param2, ..., @c paramN are the parameters
 * passed to the object function @c func.
 */
#define S_SHAPE_CALL(SELF, FUNC)					\
	((SShapeClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SShape
 * can be called.
 *
 * @param SELF The given #SShape*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_SHAPE_METH_VALID(SELF, FUNC)		\
	S_SHAPE_CALL(SELF, FUNC) ? TRUE : FALSE




/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SShapeClass ShapeClass; /* Shape class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SShapeMove(SShape *self, int newx, int newy, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SShapeMove",
				  "Argument \"self\" is NULL");
		return;
	}

	if (!S_SHAPE_METH_VALID(self, move))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SShapeMove",
				  "Shape method \"move\" not implemented");
		return;
	}


	S_SHAPE_CALL(self, move)(self, newx, newy, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SShapeMove",
			  "Call to class method \"move\" failed");
}


S_API float SShapeArea(const SShape *self, s_erc *error)
{
	float area;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SShapeArea",
				  "Argument \"self\" is NULL");
		return 0.0;
	}

	if (!S_SHAPE_METH_VALID(self, area))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SShapeArea",
				  "Shape method \"area\" not implemented");
		return 0.0;
	}

	area = S_SHAPE_CALL(self, area)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SShapeArea",
				  "Call to class method \"area\" failed"))
		return 0.0;

	return area;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_shape_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&ShapeClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_shape_class_reg",
			  "Failed to register SShapeClass");
}


S_LOCAL void _s_shape_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&ShapeClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_shape_class_free",
			  "Failed to free SShapeClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void DisposeShape(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* Class initialization                                                             */
/*                                                                                  */
/************************************************************************************/

static SShapeClass ShapeClass =
{
	/* SObjectClass */
	{
		"SShape",
		sizeof(SShape),
		{ 0, 1},
		NULL,              /* init    */
		NULL,              /* destroy */
		DisposeShape,      /* dispose */
		NULL,              /* compare */
		NULL,              /* print   */
		NULL,              /* copy    */
	},
	/* SShapeClass */
	NULL,                  /* move    */
	NULL                   /* area    */
};

