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
/* Iterator implementation for SListPy container.                                   */
/* Uses the Python iteration protocol.                                              */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "py_list_itr.h"
#include "py_funcs.h"


/************************************************************************************/
/*                                                                                  */
/* SListPyIteratorClass definition                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * The SListPyIterator class structure.  Same as SIteratorClass as
 * we are not adding any new methods.
 */
typedef SIteratorClass SListPyIteratorClass;


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given object as an #SListPy object.
 * @private
 *
 * @param SELF The given object.
 *
 * @return Given object as #SListPy* type.
 *
 * @note This casting is not safety checked.
 */
#define S_LISTPY(SELF)    ((SListPy *)(SELF))


/**
 * @hideinitializer
 * @private
 *
 * Return the given object as an iterator object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SListPyIterator* type.
 *
 * @note This casting is not safety checked.
 */
#define S_LISTPY_ITER(SELF)    ((SListPyIterator *)(SELF))


/**
 * @hideinitializer
 * @private
 *
 * Return the SListPy's Python object.
 *
 * @param SELF The given SListPy object.
 *
 * @return Given object as #SListPyIterator* type.
 *
 * @note This casting is not safety checked.
 */
#define S_PY_LIST(SELF)							\
	SELF->pyObject->pobject


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SListPyIteratorClass ListPyIteratorClass; /* SListPyIterator class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Function implementations                                                        */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void SListPyIteratorInit(SListPyIterator **self, SListPy *list, s_erc *error)
{
	s_bool is_empty;
	char *py_error;


	S_CLR_ERR(error);

	if (*self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListPyIteratorInit",
				  "Argument \"self\" is NULL");
		return;
	}

	if (list == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListPyIteratorInit",
				  "Argument \"list\" is NULL");
		goto clean_up;
	}

	/* call list method directly, otherwise we get mutex locking
	   clashes */
	is_empty = S_LIST_CALL(S_LIST(list), is_empty)(S_LIST(list), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SListPyIteratorInit",
				  "Call to \"SListIsEmpty\" failed"))
		goto clean_up;

	if (is_empty)
		goto clean_up;

	/* get the iterator object */
	(*self)->iterator = PyObject_GetIter(S_PY_LIST(list));
	if ((*self)->iterator == NULL)
	{
		py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "SListPyIteratorInit",
					  "Call to \"PyObject_GetIter\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "SListPyIteratorInit",
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
					  "SListPyIteratorInit",
					  "Call to \"PyIter_Next\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "SListPyIteratorInit",
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
				  "SListPyIteratorInit",
				  "Call to \"PyIter_Next\" failed. Reported error: %s",
				  py_error);
		S_FREE(py_error);
		goto clean_up;
	}

	/* get reference for list */
	(*self)->list = S_PY_LIST(list);
	Py_INCREF((*self)->list);

	/* set list position */
	(*self)->counter = 0;

	/* all OK */
	return;

	/* clean up code */
clean_up:
	S_DELETE(*self, "SListPyIteratorInit", error);
	*self = NULL;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_list_py_iterator_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&ListPyIteratorClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_list_py_iterator_class_reg",
			  "Failed to add SListPyIteratorClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitListPyIterator(void *obj, s_erc *error)
{
	SListPyIterator *self = obj;

	S_CLR_ERR(error);
	self->iterator = NULL;
	self->c_itr = NULL;
	self->n_itr = NULL;
	self->sobject = NULL;
	self->list = NULL;
	self->counter = 0;
}


static void DestroyListPyIterator(void *obj, s_erc *error)
{
	SListPyIterator *self = obj;

	S_CLR_ERR(error);
	if (self->iterator != NULL)
	{
		Py_DECREF(self->iterator);
		self->iterator = NULL;
	}

	if (self->c_itr != NULL)
	{
		Py_DECREF(self->c_itr);
		self->c_itr = NULL;
	}

	if (self->n_itr != NULL)
	{
		Py_DECREF(self->n_itr);
		self->n_itr = NULL;
	}

	if (self->list != NULL)
	{
		Py_DECREF(self->list);
		self->list = NULL;
	}

	if (self->sobject != NULL)
		S_DELETE(self->sobject, "DestroyListPyIterator", error);
}


static void DisposeListPyIterator(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static SIterator *Next(SIterator *self, s_erc *error)
{
	SListPyIterator *pyItr = S_LISTPY_ITER(self);
	char *py_error;


	S_CLR_ERR(error);

	if (pyItr->n_itr == NULL)
		return NULL;

	if (pyItr->c_itr != NULL)
	{
		Py_DECREF(pyItr->c_itr);
		pyItr->counter++;
	}

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

	return self;
}


static const SObject *Object(SIterator *self, s_erc *error)
{
	SListPyIterator *pyItr = S_LISTPY_ITER(self);


	S_CLR_ERR(error);

	if (pyItr->sobject != NULL)
		S_DELETE(pyItr->sobject, "Object", error);

	pyItr->sobject = s_pyobject_2_sobject(pyItr->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "Object",
				  "Call to \"s_pyobject_2_sobject\" failed"))
		return NULL;

	return pyItr->sobject;
}


static SObject *Unlink(SIterator *self, s_erc *error)
{
	SListPyIterator *pyItr = S_LISTPY_ITER(self);
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

	/* delete it from list */
	if (PySequence_DelItem(pyItr->list, pyItr->counter) == -1)
	{
		char *py_error = s_get_python_error_str();
		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "Unlink",
					  "Call to \"PySequence_DelItem\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "Unlink",
					  "Call to \"PySequence_DelItem\" failed");
		}
	}

	return tmp;
}


/************************************************************************************/
/*                                                                                  */
/* SListPyIterator class initialization                                             */
/*                                                                                  */
/************************************************************************************/

static SListPyIteratorClass ListPyIteratorClass =
{
	/* SObjectClass */
	{
		"SIterator:SListPyIterator",
		sizeof(SListPyIterator),
		{ 0, 1},
		InitListPyIterator,     /* init    */
		DestroyListPyIterator,  /* destroy */
		DisposeListPyIterator,  /* dispose */
		NULL,                   /* compare */
		NULL,                   /* print   */
		NULL,                   /* copy    */
	},
	/* SIteratorClass */
	Next,                         /* next    */
	NULL,                         /* key     */
	Object,                       /* object  */
	Unlink                        /* unlink  */
};

