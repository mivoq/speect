/************************************************************************************/
/* Copyright (c) 2010 The Department of Arts and Culture,                           */
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

#include "pobject.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SPyObjectClass PyObjectClass; /* SPyObject class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_pyobject_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&PyObjectClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_pyobject_class_reg",
			  "Failed to register SPyObjectClass");
}


S_LOCAL void _s_pyobject_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&PyObjectClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_pyobject_class_free",
			  "Failed to free SPyObjectClass");
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
	Py_XDECREF(self->pobject);
	self->pobject = NULL;
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
		S_CTX_ERR(error, S_FAILURE,
				  "Compare",
				  "Call to \"PyObject_Cmp\" failed");
		return FALSE;
	}

	if (p_result == 0)
		return TRUE;

	return FALSE;
}


static char *Print(const SObject *self, s_erc *error)
{
	char *buf;
	char *pbuf;
	PyObject *objectString;


	S_CLR_ERR(error);
	if (S_PYOBJECT(self)->pobject == NULL)
		return NULL;

	objectString = PyObject_Str(S_PYOBJECT(self)->pobject);
	if (objectString == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Print",
				  "Call to \"PyObject_Str\" failed");
		return NULL;
	}

	pbuf = PyString_AsString(objectString);
	if (pbuf == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Print",
				  "Call to \"PyString_AsString\" failed");
		Py_XDECREF(objectString);
		return NULL;
	}

	buf = s_strdup(pbuf, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Print",
				  "Call to \"s_strdup\" failed"))
	{
		Py_XDECREF(objectString);
		return NULL;
	}

	Py_XDECREF(objectString);
	return buf;
}


static void Set(SVoid *self, void *ptr, s_erc *error)
{
	SPyObject *spo = S_PYOBJECT(self);


	S_CLR_ERR(error);
	Py_XINCREF((PyObject*)ptr);
	spo->pobject = ptr;
}


static void Reset(SVoid *self, void *ptr, s_erc *error)
{
	SPyObject *spo = S_PYOBJECT(self);


	S_CLR_ERR(error);
	Py_XINCREF((PyObject*)ptr);

	if (spo->pobject != NULL)
		Py_XDECREF((PyObject*)spo->pobject);
	spo->pobject = ptr;
}


static const void *Get(const SVoid *self, s_erc *error)
{
	SPyObject *spo = S_PYOBJECT(self);


	S_CLR_ERR(error);
	if (spo->pobject != NULL)
		Py_XINCREF((PyObject*)spo->pobject);

	return (const void*)spo->pobject;
}


/************************************************************************************/
/*                                                                                  */
/* SPyObject class initialization                                                   */
/*                                                                                  */
/************************************************************************************/

/*
 * The Speect Python object class structure, does not add any methods
 * to #SVoidClass, and therefore exactly the same.
 */

static SPyObjectClass PyObjectClass =
{
	{
		/* SObjectClass */
		"SVoid:SPyObject",
		sizeof(SPyObject),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		Compare,         /* compare */
		Print,           /* print   */
		NULL,            /* copy    */
	},
	/* SVoidClass */
	Set,    /* set   */
	Reset,  /* reset */
	Get     /* get   */
};
