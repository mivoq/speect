/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "Python.h"
#include "swigpyrun.h"
#include "py_funcs.h"
#include "py_object.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SPyObjectClass PyObjectClass; /* SPyObject class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SPyObjectSet(SPyObject *self, PyObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SPyObjectSet",
				  "Argument \"self\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SPyObjectSet",
				  "Argument \"object\" is NULL");
		return;
	}

	if (self->pobject != NULL)
		Py_XDECREF(self->pobject);

	/* increment reference */
	Py_XINCREF(object);
	self->pobject = object;
}


S_API PyObject *SPyObjectGet(const SPyObject *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SPyObjectGet",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (self->pobject == NULL)
		Py_RETURN_NONE;

	/* increment reference */
	Py_XINCREF(self->pobject);
	return self->pobject;
}


S_LOCAL void _s_pyobject_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&PyObjectClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_pyobject_class_reg",
			  "Failed to register SPyObjectClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SPyObject *self = obj;


	S_CLR_ERR(error);
	self->pobject = NULL;
}


static void Destroy(void *obj, s_erc *error)
{
	SPyObject *self = obj;


	S_CLR_ERR(error);

	if (self->pobject != NULL)
	{
		/* decrement reference before destroy */
		Py_XDECREF(self->pobject);
		self->pobject = NULL;
	}
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static s_bool Compare(const SObject *first, const SObject *second, s_erc *error)
{
	SPyObject *p1 = S_PYOBJECT(first);
	SPyObject *p2 = S_PYOBJECT(second);
	PyObject *o1;
	PyObject *o2;
	int rv;
	int p_result;


	S_CLR_ERR(error);
	o1 = p1->pobject;
	o2 = p2->pobject;

	if ((o1 == NULL) || (o2 == NULL))
		return FALSE;

	rv = PyObject_Cmp(o1, o2, &p_result);
	if (rv == -1)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "Compare",
					  "Call to \"PyObject_Cmp\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "Compare",
					  "Call to \"PyObject_Cmp\" failed");
		}

		return FALSE;
	}

	if (p_result == 0)
		return TRUE;

	return FALSE;
}


static char *Print(const SObject *self, s_erc *error)
{
	char *buf;


	S_CLR_ERR(error);
	if (S_PYOBJECT(self)->pobject == NULL)
		return NULL;

	buf = s_get_pyobject_str(S_PYOBJECT(self)->pobject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Print",
				  "Call to \"s_get_pyobject_str\" failed"))
		return NULL;

	return buf;
}


/************************************************************************************/
/*                                                                                  */
/* SPyObject class initialization                                                   */
/*                                                                                  */
/************************************************************************************/


static SPyObjectClass PyObjectClass =
{
	/* SObjectClass */
	"SPyObject",
	sizeof(SPyObject),
	{ 0, 1},
	Init,            /* init    */
	Destroy,         /* destroy */
	Dispose,         /* dispose */
	Compare,         /* compare */
	Print,           /* print   */
	NULL             /* copy    */
};
