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


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "rectangle.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #SRectangle,
 * see full description #S_RECTANGLE_CALL for usage.
 *
 * @param SELF The given #SRectangle*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage:
 @verbatim
 S_RECTANGLE_CALL(self, func)(param1, param2, ..., paramN);
 @endverbatim
 * where @c param1, @c param2, ..., @c paramN are the parameters
 * passed to the object function @c func.
 */
#define S_RECTANGLE_CALL(SELF, FUNC)					\
	((SRectangleClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SRectangle
 * can be called.
 *
 * @param SELF The given #SRectangle*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_RECTANGLE_METH_VALID(SELF, FUNC)		\
	S_RECTANGLE_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SRectangleClass RectangleClass; /* Rectangle class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API SRectangle *SRectangleNew(int x, int y, int width, int height, s_erc *error)
{
	SRectangle *self;


	S_CLR_ERR(error);

	self = S_NEW(SRectangle, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SRectangleNew",
				  "Failed to create new object"))
	{
		return NULL;
	}

	S_SHAPE(self)->x = x;
	S_SHAPE(self)->y = y;
	self->width = width;
	self->height = height;

	return self;
}


S_API void SRectangleSetWidth(SRectangle *self, int new_width, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SRectangleSetWidth",
				  "Argument \"self\" is NULL");
		return;
	}

	if (!S_RECTANGLE_METH_VALID(self, set_width))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SRectangleSetWidth",
				  "Rectangle method \"set_width\" not implemented");
		return;
	}


	S_RECTANGLE_CALL(self, set_width)(self, new_width, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SRectangleSetWidth",
			  "Call to class method \"set_width\" failed");
}


S_API void SRectangleSetHeight(SRectangle *self, int new_height, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SRectangleSetHeight",
				  "Argument \"self\" is NULL");
		return;
	}

	if (!S_RECTANGLE_METH_VALID(self, set_height))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SRectangleSetHeight",
				  "Rectangle method \"set_height\" not implemented");
		return;
	}


	S_RECTANGLE_CALL(self, set_height)(self, new_height, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SRectangleSetHeight",
			  "Call to class method \"set_height\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_rectangle_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&RectangleClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_rectangle_class_reg",
			  "Failed to register SRectangleClass");
}


S_LOCAL void _s_rectangle_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&RectangleClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_rectangle_class_free",
			  "Failed to free SRectangleClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitRectangle(void *obj, s_erc *error)
{
	SRectangle *self = obj;


	S_CLR_ERR(error);
	self->width = 0;
	self->height = 0;
}


static void DisposeRectangle(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static char *PrintRectangle(const SObject *self, s_erc *error)
{
	SRectangle *rec = S_RECTANGLE(self);
	const char *type = "[SRectangle] at (%d,%d), width %d, height %d";
	char *buf;

	S_CLR_ERR(error);

	s_asprintf(&buf, error, type, S_SHAPE(rec)->x, S_SHAPE(rec)->y, rec->width, rec->height);
	if (S_CHK_ERR(error, S_CONTERR,
				  "PrintRectangle",
				  "Call to \"s_asprintf\" failed"))
	{
		if (buf != NULL)
			S_FREE(buf);
		return NULL;
	}

	return buf;
}


static void MoveRectangle(SShape *self, int newx, int newy, s_erc *error)
{
	S_CLR_ERR(error);
	self->x = newx;
	self->y = newy;
}


static float AreaRectangle(const SShape *self, s_erc *error)
{
	SRectangle *rec = S_RECTANGLE(self);
	float area;


	S_CLR_ERR(error);
	area = rec->width * rec->height;

	return area;
}


static void SetWidthRectangle(SRectangle *self, int new_width, s_erc *error)
{
	S_CLR_ERR(error);
	self->width = new_width;
}


static void SetHeightRectangle(SRectangle *self, int new_heigth, s_erc *error)
{
	S_CLR_ERR(error);
	self->height = new_heigth;
}


/************************************************************************************/
/*                                                                                  */
/* Class initialization                                                             */
/*                                                                                  */
/************************************************************************************/

static SRectangleClass RectangleClass =
{
	{
		/* SObjectClass */
		{
			"SRectangle",
			sizeof(SRectangle),
			{ 0, 1},
			InitRectangle,     /* init    */
			NULL,              /* destroy */
			DisposeRectangle,  /* dispose */
			NULL,              /* compare */
			PrintRectangle,    /* print   */
			NULL,              /* copy    */
		},
		/* SShapeClass */
		MoveRectangle,         /* move    */
		AreaRectangle,         /* area    */
	},
	/* SRectangleClass */
	SetWidthRectangle,         /* set_width  */
	SetHeightRectangle         /* set_height */
};

