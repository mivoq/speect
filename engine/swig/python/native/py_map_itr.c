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
/* DATE    : June 2010                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Iterator implementation for SMapPy container.                                    */
/* Uses the Python iteration protocol.                                              */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "py_map_itr.h"
#include "py_funcs.h"


/************************************************************************************/
/*                                                                                  */
/* SMapPyIteratorClass definition                                                   */
/*                                                                                  */
/************************************************************************************/

/**
 * The SMapPyIterator class structure.  Same as SIteratorClass as
 * we are not adding any new methods.
 */
typedef SIteratorClass SMapPyIteratorClass;


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given object as an #SMapPy object.
 * @private
 *
 * @param SELF The given object.
 *
 * @return Given object as #SMapPy* type.
 *
 * @note This casting is not safety checked.
 */
#define S_MAPPY(SELF)    ((SMapPy *)(SELF))


/**
 * @hideinitializer
 * @private
 *
 * Return the given object as an iterator object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SMapPyIterator* type.
 *
 * @note This casting is not safety checked.
 */
#define S_MAPPY_ITER(SELF)    ((SMapPyIterator *)(SELF))


/**
 * @hideinitializer
 * @private
 *
 * Return the SMapPy's Python object.
 *
 * @param SELF The given SMapPy object.
 *
 * @return Given object as #SMapPyIterator* type.
 *
 * @note This casting is not safety checked.
 */
#define S_PY_MAP(SELF)							\
	SELF->pyObject->pobject


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SMapPyIteratorClass MapPyIteratorClass; /* SMapPyIterator class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Function implementations                                                        */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void SMapPyIteratorInit(SMapPyIterator **self, SMapPy *map, s_erc *error)
{
	size_t size;
	char *py_error;


	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapPyIteratorInit",
				  "Argument \"self\" is NULL");
		return;
	}

	if (map == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapPyIteratorInit",
				  "Argument \"map\" is NULL");
		goto clean_up;
	}

	size = SMapSize(S_MAP(map), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapPyIteratorInit",
				  "Call to \"SMapSize\" failed"))
		goto clean_up;

	if (size == 0)
		goto clean_up;

	/* get the iterator object */
	(*self)->iterator = PyObject_GetIter(S_PY_MAP(map));
	if ((*self)->iterator == NULL)
	{
		py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "SMapPyIteratorInit",
					  "Call to \"PyObject_GetIter\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "SMapPyIteratorInit",
					  "Call to \"PyObject_GetIter\" failed");
		}

		goto clean_up;
	}


	/* get first element of list and set as current element */
	(*self)->c_itr = PyIter_Next((*self)->iterator);

	if ((*self)->c_itr == NULL)
	{
		/* list is not empty, so this must be an error */
		py_error = s_get_python_error_str();
		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "SMapPyIteratorInit",
					  "Call to \"PyIter_Next\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "SMapPyIteratorInit",
					  "Call to \"PyIter_Next\" failed");
		}

		goto clean_up;
	}

	/* get next element of list */
	(*self)->n_itr = PyIter_Next((*self)->iterator);

	/* next might be null, so this is not necessarily an error */
	py_error = s_get_python_error_str();
	if (py_error)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SMapPyIteratorInit",
				  "Call to \"PyIter_Next\" failed. Reported error: %s",
				  py_error);
		S_FREE(py_error);
		goto clean_up;
	}

	/* all OK */
	return;

	/* clean up code */
clean_up:
	S_DELETE(*self, "SMapPyIteratorInit", error);
	*self = NULL;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_map_py_iterator_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&MapPyIteratorClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_map_py_iterator_class_reg",
			  "Failed to add SMapPyIteratorClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitMapPyIterator(void *obj, s_erc *error)
{
	SMapPyIterator *self = obj;

	S_CLR_ERR(error);
	self->iterator = NULL;
	self->c_itr = NULL;
	self->n_itr = NULL;
	self->sobject = NULL;
	self->key = NULL;
}


static void DestroyMapPyIterator(void *obj, s_erc *error)
{
	SMapPyIterator *self = obj;

	S_CLR_ERR(error);
	if (self->iterator != NULL)
	{
		Py_XDECREF(self->iterator);
		self->iterator = NULL;
	}

	if (self->c_itr != NULL)
	{
		Py_XDECREF(self->c_itr);
		self->c_itr = NULL;
	}

	if (self->n_itr != NULL)
	{
		Py_XDECREF(self->n_itr);
		self->n_itr = NULL;
	}

	if (self->sobject != NULL)
		S_DELETE(self->sobject, "DestroyMapPyIterator", error);

	if (self->key != NULL)
		S_FREE(self->key);
}


static void DisposeMapPyIterator(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static SIterator *Next(SIterator *self, s_erc *error)
{
	SMapPyIterator *pyItr = S_MAPPY_ITER(self);
	char *py_error;


	S_CLR_ERR(error);

	if (pyItr->n_itr == NULL)
		return NULL;

	if (pyItr->c_itr != NULL)
		Py_XDECREF(pyItr->c_itr);

	pyItr->c_itr = pyItr->n_itr;

	/* get next element of list */
	pyItr->n_itr = PyIter_Next(pyItr->iterator);

	/* next might be null, so this is not necessarily an error */
	py_error = s_get_python_error_str();
	if (py_error)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "Next",
				  "Call to \"PyIter_Next\" failed. Reported error: %s",
				  py_error);
		S_FREE(py_error);
		return NULL;
	}

	if (pyItr->sobject != NULL)
		S_DELETE(pyItr->sobject, "Next", error);

	if (pyItr->key != NULL)
		S_FREE(pyItr->key);

	return self;
}


static const char *Key(SIterator *self, s_erc *error)
{
	SMapPyIterator *pyItr = S_MAPPY_ITER(self);


	S_CLR_ERR(error);

	if (pyItr->key != NULL)
		S_FREE(pyItr->key);

	pyItr->key = s_get_pyobject_str(pyItr->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Key",
				  "Call to \"s_get_pyobject_str\" failed"))
		return NULL;

	return (const char*)pyItr->key;
}


static const SObject *Object(SIterator *self, s_erc *error)
{
	SMapPyIterator *pyItr = S_MAPPY_ITER(self);


	S_CLR_ERR(error);

	if (pyItr->sobject != NULL)
		S_DELETE(pyItr->sobject, "Object", error);

	pyItr->sobject = s_pyobject_2_sobject(pyItr->c_itr, error);
	Py_DECREF(pyItr->c_itr);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Object",
				  "Call to \"s_pyobject_2_sobject\" failed"))
		return NULL;

	return pyItr->sobject;
}


static SObject *Unlink(SIterator *self, s_erc *error)
{
	SMapPyIterator *pyItr = S_MAPPY_ITER(self);
	SObject *tmp;


	S_CLR_ERR(error);
	if (pyItr->c_itr == NULL)
		return NULL;

	tmp = s_pyobject_2_sobject(pyItr->c_itr, error);
	Py_DECREF(pyItr->c_itr);
	pyItr->c_itr = NULL;
	if (S_CHK_ERR(error, S_CONTERR,
				  "Unlink",
				  "Call to \"s_pyobject_2_sobject\" failed"))
		return NULL;

	return tmp;
}


/************************************************************************************/
/*                                                                                  */
/* SMapPyIterator class initialization                                              */
/*                                                                                  */
/************************************************************************************/

static SMapPyIteratorClass MapPyIteratorClass =
{
	/* SObjectClass */
	{
		"SIterator:SMapPyIterator",
		sizeof(SMapPyIterator),
		{ 0, 1},
		InitMapPyIterator,     /* init    */
		DestroyMapPyIterator,  /* destroy */
		DisposeMapPyIterator,  /* dispose */
		NULL,                  /* compare */
		NULL,                  /* print   */
		NULL,                  /* copy    */
	},
	/* SIteratorClass */
	Next,                         /* next    */
	Key,                          /* key     */
	Object,                       /* object  */
	Unlink                        /* unlink  */
};

