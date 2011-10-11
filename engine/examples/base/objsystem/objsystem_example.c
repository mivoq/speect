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
/* 2d shape objects, providing examples of Speect's object-oriented                 */
/* programming support.                                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "speect.h"
#include "shape.h"
#include "rectangle.h"
#include "circle.h"



/*
 * register shape, rectangle and circle objects with the Speect object
 * system.
 */
void register_objects(s_erc *error)
{
	s_erc local_err = S_SUCCESS;

	S_CLR_ERR(error);

	_s_shape_class_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "register_objects",
				  "Call to \"_s_shape_class_reg\" failed"))
		return;

	_s_rectangle_class_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "register_objects",
				  "Call to \"_s_rectangle_class_reg\" failed"))
	{
		_s_shape_class_free(&local_err);
		return;
	}

	_s_circle_class_reg(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "register_objects",
				  "Call to \"_s_circle_class_reg\" failed"))
	{
		_s_rectangle_class_free(&local_err);
		_s_shape_class_free(&local_err);
		return;
	}
}


/*
 * free shape, rectangle and circle objects from the Speect object
 * system.
 */
void free_objects(s_erc *error)
{
	s_erc local_err = S_SUCCESS;

	S_CLR_ERR(error);

	_s_circle_class_free(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "register_objects",
				  "Call to \"_s_circle_class_free\" failed"))
		S_NEW_ERR(error, local_err);

	_s_rectangle_class_free(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "register_objects",
				  "Call to \"_s_rectangle_class_free\" failed"))
		S_NEW_ERR(error, local_err);

	_s_shape_class_free(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "register_objects",
				  "Call to \"_s_shape_class_free\" failed"))
		S_NEW_ERR(error, local_err);
}


int main()
{
	s_erc error = S_SUCCESS;
	SCircle *circleShape = NULL;
	SRectangle *rectangleShape = NULL;
	int rv = 0;
	char *buf = NULL;
	int registered = 0;
	float area = 0.0;


	/*
	 * initialize speect
	 */
	error = speect_init(NULL);
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/*
	 * register SShape, SRectangle and SCircle classes with the Speect
	 * object system.
	 */
	register_objects(&error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SRectangleNew\" failed"))
	{
		rv = 1;
		goto quit;
	}
	else
	{
		registered = 1;
	}

	/*
	 * create new rectangle
	 */
	rectangleShape = SRectangleNew(10, 15, 100, 140, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SRectangleNew\" failed"))
	{
		rv = 1;
		goto quit;
	}

	/*
	 * Print the SRectangle object as an SObject
	 */
	buf = SObjectPrint(S_OBJECT(rectangleShape), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectPrint\" failed"))
	{
		rv = 1;
		goto quit;
	}

	printf("%s\n", buf);
	S_FREE(buf);

	/* get the rectangle's area */
	area = SShapeArea(S_SHAPE(rectangleShape), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SShapeArea\" failed"))
	{
		rv = 1;
		goto quit;
	}

	printf("rectangle area = %f\n\n", area);


	/*
	 * create new circle
	 */
	circleShape = SCircleNew(20, 62, 70, "green", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SCircleNew\" failed"))
	{
		rv = 1;
		goto quit;
	}

	/*
	 * Print the SCircle object as an SObject
	 */
	buf = SObjectPrint(S_OBJECT(circleShape), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectPrint\" failed"))
	{
		rv = 1;
		goto quit;
	}

	printf("%s\n", buf);
	S_FREE(buf);

	/* get the circle's area */
	area = SShapeArea(S_SHAPE(circleShape), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SShapeArea\" failed"))
	{
		rv = 1;
		goto quit;
	}

	printf("circle area = %f\n\n", area);


	/* move the rectangle to new coordinates */
	SShapeMove(S_SHAPE(rectangleShape), 631, 103, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SShapeMove\" failed"))
	{
		rv = 1;
		goto quit;
	}

	/* move the circle to new coordinates */
	SShapeMove(S_SHAPE(circleShape), 311, 67, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SShapeMove\" failed"))
	{
		rv = 1;
		goto quit;
	}

	/* set the width and height of the rectangle */
	SRectangleSetWidth(rectangleShape, 32, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SRectangleSetWidth\" failed"))
	{
		rv = 1;
		goto quit;
	}


	SRectangleSetHeight(rectangleShape, 102, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SRectangleSetHeight\" failed"))
	{
		rv = 1;
		goto quit;
	}

	/* set the radius and colour of the circle */
	SCircleSetRadius(circleShape, 34, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SCircleSetRadius\" failed"))
	{
		rv = 1;
		goto quit;
	}

	SCircleSetColour(circleShape, "red", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SCircleSetColour\" failed"))
	{
		rv = 1;
		goto quit;
	}

	/* print rectangle and circle attributes after all changes */
	printf("attributes after changes\n");

	/*
	 * Print the SRectangle object as an SObject
	 */
	buf = SObjectPrint(S_OBJECT(rectangleShape), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectPrint\" failed"))
	{
		rv = 1;
		goto quit;
	}

	printf("%s\n", buf);
	S_FREE(buf);

	/* get the rectangle's area */
	area = SShapeArea(S_SHAPE(rectangleShape), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SShapeArea\" failed"))
	{
		rv = 1;
		goto quit;
	}

	printf("rectangle area = %f\n\n", area);

	/*
	 * Print the SCircle object as an SObject
	 */
	buf = SObjectPrint(S_OBJECT(circleShape), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SObjectPrint\" failed"))
	{
		rv = 1;
		goto quit;
	}

	printf("%s\n", buf);
	S_FREE(buf);

	/* get the circle's area */
	area = SShapeArea(S_SHAPE(circleShape), &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Call to \"SShapeArea\" failed"))
	{
		rv = 1;
		goto quit;
	}

	printf("circle area = %f\n\n", area);


quit:
	if (circleShape != NULL)
		S_DELETE(circleShape, "main", &error);

	if (rectangleShape != NULL)
		S_DELETE(rectangleShape, "main", &error);

	if (registered)
		free_objects(&error);

	/*
	 * quit speect
	 */
	error = speect_quit();
	if (error != S_SUCCESS)
	{
		printf("Call to 'speect_quit' failed\n");
		return 1;
	}

	return rv;
}
