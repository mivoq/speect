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


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "circle.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #SCircle,
 * see full description #S_CIRCLE_CALL for usage.
 *
 * @param SELF The given #SCircle*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage:
 @verbatim
 S_CIRCLE_CALL(self, func)(param1, param2, ..., paramN);
 @endverbatim
 * where @c param1, @c param2, ..., @c paramN are the parameters
 * passed to the object function @c func.
 */
#define S_CIRCLE_CALL(SELF, FUNC)					\
	((SCircleClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SCircle
 * can be called.
 *
 * @param SELF The given #SCircle*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_CIRCLE_METH_VALID(SELF, FUNC)		\
	S_CIRCLE_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SCircleClass CircleClass; /* Circle class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API SCircle *SCircleNew(int x, int y, int radius, const char *colour, s_erc *error)
{
	SCircle *self;


	S_CLR_ERR(error);

	self = S_NEW(SCircle, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SCircleNew",
				  "Failed to create new object"))
	{
		return NULL;
	}

	S_SHAPE(self)->x = x;
	S_SHAPE(self)->y = y;
	self->radius = radius;
	self->colour = s_strdup(colour, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SCircleNew",
				  "Call to \"s_strdup\" failed"))
	{
		S_DELETE(self, "SCircleNew", error);
		return NULL;
	}

	return self;
}


S_API void SCircleSetRadius(SCircle *self, int new_radius, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SCircleSetRadius",
				  "Argument \"self\" is NULL");
		return;
	}

	if (!S_CIRCLE_METH_VALID(self, set_radius))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SCircleSetRadius",
				  "Circle method \"set_radius\" not implemented");
		return;
	}


	S_CIRCLE_CALL(self, set_radius)(self, new_radius, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SCircleSetRadius",
			  "Call to class method \"set_radius\" failed");
}


S_API void SCircleSetColour(SCircle *self, const char *new_colour, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SCircleSetColour",
				  "Argument \"self\" is NULL");
		return;
	}

	if (new_colour == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SCircleSetColour",
				  "Argument \"new_colour\" is NULL");
		return;
	}

	if (!S_CIRCLE_METH_VALID(self, set_colour))
	{
		S_CTX_ERR(error, S_METHINVLD,
				  "SCircleSetColour",
				  "Circle method \"set_colour\" not implemented");
		return;
	}


	S_CIRCLE_CALL(self, set_colour)(self, new_colour, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SCircleSetColour",
			  "Call to class method \"set_colour\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_circle_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&CircleClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_circle_class_reg",
			  "Failed to register SCircleClass");
}


S_LOCAL void _s_circle_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&CircleClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_circle_class_free",
			  "Failed to free SCircleClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitCircle(void *obj, s_erc *error)
{
	SCircle *self = obj;


	S_CLR_ERR(error);
	self->radius = 0;
	self->colour = NULL;
}


static void DestroyCircle(void *obj, s_erc *error)
{
	SCircle *self = obj;


	S_CLR_ERR(error);
	if (self->colour != NULL)
	{
		S_FREE(self->colour);
	}
}


static void DisposeCircle(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static char *PrintCircle(const SObject *self, s_erc *error)
{
	SCircle *cir = S_CIRCLE(self);
	const char *type = "[SCircle] at (%d,%d), radius %d, colour %s";
	char *buf;

	S_CLR_ERR(error);

	if (cir->colour == NULL)
	{
		s_asprintf(&buf, error, type, S_SHAPE(cir)->x, S_SHAPE(cir)->y, cir->radius, "None");
	}
	else
	{
		s_asprintf(&buf, error, type, S_SHAPE(cir)->x, S_SHAPE(cir)->y, cir->radius, cir->colour);
	}

	if (S_CHK_ERR(error, S_CONTERR,
				  "PrintCircle",
				  "Call to \"s_asprintf\" failed"))
	{
		if (buf != NULL)
			S_FREE(buf);
		return NULL;
	}

	return buf;
}


static void MoveCircle(SShape *self, int newx, int newy, s_erc *error)
{
	SShapeClass *shapeClass = NULL;


	S_CLR_ERR(error);
	shapeClass = (SShapeClass*)s_class_find("SShape", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MoveCircle",
				  "Call to \"s_class_find\" failed"))
		return;

	shapeClass->move(self, newx, newy, error);
}


static float AreaCircle(const SShape *self, s_erc *error)
{
	SCircle *cir = S_CIRCLE(self);
	float area;


	S_CLR_ERR(error);
	area = S_PI * cir->radius * cir->radius;

	return area;
}


static void SetRadiusCircle(SCircle *self, int new_radius, s_erc *error)
{
	S_CLR_ERR(error);
	self->radius = new_radius;
}


static void SetColourCircle(SCircle *self, const char *new_colour, s_erc *error)
{
	S_CLR_ERR(error);
	if (self->colour != NULL)
		S_FREE(self->colour);

	self->colour = s_strdup(new_colour, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SetColourCircle",
			  "Call to \"s_strdup\" failed");
}


/************************************************************************************/
/*                                                                                  */
/* Class initialization                                                             */
/*                                                                                  */
/************************************************************************************/

static SCircleClass CircleClass =
{
	{
		/* SObjectClass */
		{
			"SCircle",
			sizeof(SCircle),
			{ 0, 1},
			InitCircle,     /* init    */
			DestroyCircle,  /* destroy */
			DisposeCircle,  /* dispose */
			NULL,           /* compare */
			PrintCircle,    /* print   */
			NULL,           /* copy    */
		},
		/* SShapeClass */
		MoveCircle,         /* move    */
		AreaCircle,         /* area    */
	},
	/* SCircleClass */
	SetRadiusCircle,        /* set_radius */
	SetColourCircle         /* set_colour */
};

