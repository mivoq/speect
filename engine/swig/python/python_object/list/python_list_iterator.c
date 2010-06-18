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

#include "base/objsystem/class.h"
#include "containers/list/list_list_iterator.h"


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
 * @param SELF The given object.
 * @return Given object as #SListPy* type.
 * @note This casting is not safety checked.
 */
#define S_LISTPY(SELF)    ((SListPy *)(SELF))


/**
 * @hideinitializer
 * @private
 * Return the given object as an iterator object.
 * @param SELF The given object.
 * @return Given object as #SListPyIterator* type.
 * @note This casting is not safety checked.
 */
#define S_LISTPY_ITER(SELF)    ((SListPyIterator *)(SELF))


/* get the Python list object */
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

S_LOCAL void SListPyIteratorInit(SIterator **self, SListPy *list, s_erc *error)
{
	SListPyIterator *listPyItr;


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
		S_DELETE(*self, "SListPyIteratorInit", error);
		*self = NULL;
		return;
	}

	listPyItr = S_CAST(*self, SListPyIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SListPyIteratorInit",
				  "Argument \"self\" is not of SListPyIterator type"))
	{
		S_DELETE(*self, "SListPyIteratorInit", error);
		*self = NULL;
		return;
	}

	(*self)->myContainer = S_CONTAINER(list);
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
}


static void DestroyListPyIterator(void *obj, s_erc *error)
{
	SListPyIterator *self = obj;

	S_CLR_ERR(error);
	if (self->iterator != NULL)
	{
		Py_XDECREF(self->iterator);
		self->iterator = NULL;
	}
}


static void DisposeListPyIterator(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static SIterator *First(SIterator *self, s_erc *error)
{
	SListPyIterator *pyItr = S_LISTPY_ITER(self);
	uint32 size;


	S_CLR_ERR(error);
	size = PyObject_Size(S_PY_LIST(S_LISTPY(self->myContainer)));
	if (size == -1)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "First",
					  "Call to \"PyObject_GetIter\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "First",
					  "Call to \"PyObject_GetIter\" failed");
		}

		return NULL;
	}

	if (size == 0)
		return NULL;

	pyItr->iterator = PyObject_GetIter(S_PY_LIST(S_LISTPY(self->myContainer)));
	if (pyItr->iterator == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "First",
					  "Call to \"PyObject_GetIter\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "First",
					  "Call to \"PyObject_GetIter\" failed");
		}

		return NULL;
	}

	return self;
}


static SIterator *Next(SIterator *self, s_erc *error)
{
	SListPyIterator *pyItr = S_LISTPY_ITER(self);


	S_CLR_ERR(error);




	if (pyItr->n_itr == NULL)
		return NULL;

	pyItr->p_itr = pyItr->c_itr;
	pyItr->c_itr = pyItr->n_itr;
	pyItr->n_itr = s_list_element_next((s_list_element*)pyItr->n_itr, error);
	S_CHK_ERR(error, S_CONTERR,
			  "Next",
			  "Failed to move to next element of list");

	return self;
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
	First,                        /* first   */
	NULL,                         /* last    */
	Next,                         /* next    */
	NULL                          /* prev    */
};

