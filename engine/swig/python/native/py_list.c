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
/* SList wrappers for Python Lists.                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "py_funcs.h"
#include "py_list_itr.h"
#include "py_list.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/* get the Python list object */
#define S_PY_LIST(SELF)							\
	SELF->pyObject->pobject


/* check that the SListPy object has a non-NULL Python list object */
#define S_CHECK_PY_LIST(SELF, FUNCNAME)									\
	do{																	\
		if (SELF->pyObject == NULL)										\
		{																\
			S_CTX_ERR(error, S_FAILURE,									\
					  FUNCNAME,											\
					  "SListPy object's SPyObject is NULL");			\
			goto failure;												\
		}																\
		if (SELF->pyObject->pobject == NULL)							\
		{																\
			S_CTX_ERR(error, S_FAILURE,									\
					  FUNCNAME,											\
					  "SListPy object's SPyObject's Python object is NULL"); \
			goto failure;												\
		}																\
	} while (0)


/************************************************************************************/
/*                                                                                  */
/*  Static variables                                                                */
/*                                                                                  */
/************************************************************************************/

static SListPyClass ListPyClass; /* SListPy class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SListPyInit(SListPy **self, PyObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if ((*self) == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListPyInit",
				  "Argument \"self\" is NULL");
		goto error_clean_up;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SListPyInit",
				  "Argument \"self\" is NULL");
		goto error_clean_up;
	}

	/* set this list's Python object */
	SPyObjectSet((*self)->pyObject, object, error);
	if (S_CHK_ERR(error, S_ARGERROR,
				  "SListPyInit",
				  "Call to \"SPyObjectSet\" failed"))
		goto error_clean_up;


	/* all OK */
	return;

	/* clean up code */
error_clean_up:
	S_DELETE((*self), "SListPyInit", error);
	(*self) = NULL;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_list_py_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&ListPyClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_list_py_class_add",
			  "Failed to add SListPyClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitListPy(void *obj, s_erc *error)
{
	SListPy *self = obj;


	S_CLR_ERR(error);
	self->pyObject = (SPyObject*)S_NEW("SPyObject", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "InitListPy",
				  "Failed to create new 'SPyObject' object"))
		return;
}


static void DestroyListPy(void *obj, s_erc *error)
{
	SListPy *self = obj;


	S_CLR_ERR(error);

	if (self->pyObject != NULL)
		S_DELETE(self->pyObject, "DestroyListPy", error);
}


static void DisposeListPy(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static s_bool ListPyIsEmpty(const SList *self, s_erc *error)
{
	const SListPy *pList = (const SListPy*)self;
	size_t size;

	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList, "ListPyIsEmpty");

	size = PyList_Size(S_PY_LIST(pList));
	if (size == 0)
		return TRUE;

	return FALSE;

	/* for S_CHECK_PY_LIST */
failure:
	return TRUE;
}


static size_t ListPySize(const SList *self, s_erc *error)
{
	const SListPy *pList = (const SListPy*)self;
	size_t size;

	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList, "ListPySize");

	size = PyList_Size(S_PY_LIST(pList));
	return size;

	/* for S_CHECK_PY_LIST */
failure:
	return 0;
}


static void ListPyAppend(SList *self, const SObject *object, s_erc *error)
{
	SObject *tmp;
	SListPy *pList = (SListPy*)self;
	PyObject *pobject;


	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList, "ListPyAppend");

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* convert to Python object type, list is now owner of object (TRUE) */
	pobject = s_sobject_2_pyobject(object, TRUE, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyAppend",
				  "Call to \"s_sobject_2_pyobject\" failed"))
		goto error;

	/* append */
	if (!PyList_Append(S_PY_LIST(pList), pobject))
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyAppend",
					  "Call to \"PyList_Append\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyAppend",
					  "Call to \"PyList_Append\" failed");
		}

		goto error;
	}

	/* decrement reference of pobject */
	Py_XDECREF(pobject);

	/* all OK */
	return;

error:
	/* decrement the reference counting on error as the object
	 * has not successfully been appended to the list
	*/
	SObjectDecRef(tmp);

	/* decrement reference of pobject */
	Py_XDECREF(pobject);

	/* for S_CHECK_PY_LIST */
failure:
	return;
}


static void ListPyPrepend(SList *self, const SObject *object, s_erc *error)
{
	SObject *tmp;
	SListPy *pList = (SListPy*)self;
	PyObject *pobject;


	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList, "ListPyPrepend");

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* convert to Python object type, list is now owner of object (TRUE) */
	pobject = s_sobject_2_pyobject(object, TRUE, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyPrepend",
				  "Call to \"s_sobject_2_pyobject\" failed"))
		goto error;

	/* prepend, 0 inserts at beginning of list */
	if (!PyList_Insert(S_PY_LIST(pList), 0, pobject))
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyPrepend",
					  "Call to \"PyList_Insert\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyPrepend",
					  "Call to \"PyList_Insert\" failed");
		}

		goto error;
	}

	/* decrement reference of pobject */
	Py_XDECREF(pobject);

	/* all OK */
	return;

error:
	/* decrement the reference counting on error as the object
	 * has not successfully been prepended to the list
	 */
	SObjectDecRef(tmp);

	/* decrement reference of pobject */
	Py_XDECREF(pobject);

	/* for S_CHECK_PY_LIST */
failure:
	return;
}


static void ListPyPush(SList *self, const SObject *object, s_erc *error)
{
	SObject *tmp;
	SListPy *pList = (SListPy*)self;
	PyObject *pobject;


	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList, "ListPyPush");

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* convert to Python object type, list is now owner of object (TRUE) */
	pobject = s_sobject_2_pyobject(object, TRUE, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyPush",
				  "Call to \"s_sobject_2_pyobject\" failed"))
		goto error;

	/* append does the same as push */
	if (!PyList_Append(S_PY_LIST(pList), pobject))
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyPush",
					  "Call to \"PyList_Append\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyPush",
					  "Call to \"PyList_Append\" failed");
		}

		goto error;
	}

	/* decrement reference of pobject */
	Py_XDECREF(pobject);

	/* all OK */
	return;

error:
	/* decrement the reference counting on error as the object
	 * has not successfully been pushed to the list
	 */
	SObjectDecRef(tmp);

	/* decrement reference of pobject */
	Py_XDECREF(pobject);

	/* for S_CHECK_PY_LIST */
failure:
	return;
}


static SObject *ListPyPop(SList *self, s_erc *error)
{
	SObject *tmp;
	SListPy *pList = (SListPy*)self;
	PyObject *pobject;
	size_t size;

	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList, "ListPyPop");

	/* get list size */
	size = PyList_Size(S_PY_LIST(pList));


	/* get the last item, note that PyList_GetItem returns a
	 * *borrowed* reference while PySequence_GetItem returns a *new*
	 * reference.
	 */
	pobject = PySequence_GetItem(S_PY_LIST(pList), size - 1);
	if (pobject == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyPop",
					  "Call to \"PySequence_GetItem\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyPop",
					  "Call to \"PySequence_GetItem\" failed");
		}

		return NULL;
	}

	/* get the Speect object */
	tmp = s_pyobject_2_sobject(pobject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyPop",
				  "Call to \"s_pyobject_2_sobject\" failed"))
		return NULL;

	/* now, delete last item in list */
	if (!PySequence_DelItem(S_PY_LIST(pList), size - 1))
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyPop",
					  "Call to \"PySequence_DelItem\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyPop",
					  "Call to \"PySequence_DelItem\" failed");
		}

		return tmp;
	}

	/* all OK */
	return tmp;

	/* for S_CHECK_PY_LIST */
failure:
	return NULL;
}


static void ListPyReverse(SList *self, s_erc *error)
{
	SListPy *pList = (SListPy*)self;


	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList, "ListPyReverse");


	if (!PyList_Reverse(S_PY_LIST(pList)))
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyReverse",
					  "Call to \"PyList_Reverse\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyReverse",
					  "Call to \"PyList_Reverse\" failed");
		}

		return;
	}

	/* for S_CHECK_PY_LIST */
failure:
	return;
}


static s_bool ListPyValPresent(const SList *self, const SObject *val, s_erc *error)
{
	SListPy *pList = (SListPy*)self;
	PyObject *pobject;
	int rv;


	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList, "ListPyValPresent");

	/* convert to Python object type */
	pobject = s_sobject_2_pyobject((SObject*)val, FALSE, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyValPresent",
				  "Call to \"s_sobject_2_pyobject\" failed"))
		return FALSE;

	rv = PySequence_Contains(S_PY_LIST(pList), pobject);

	/* decrement reference of pobject */
	Py_XDECREF(pobject);

	if (rv == -1)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyValPresent",
					  "Call to \"PySequence_Contains\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyValPresent",
					  "Call to \"PySequence_Contains\" failed");
		}

		return FALSE;
	}

	if (rv == 1)
		return TRUE;

	return FALSE;

	/* for S_CHECK_PY_LIST */
failure:
	return FALSE;
}


static SIterator *ListPyIterator(const SContainer *self, s_erc *error)
{
	SListPy *pList = (SListPy*)self;
	SListPyIterator *itr;


	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList, "ListPyIterator");

	itr = (SListPyIterator*)S_NEW("SListPyIterator", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyIterator",
				  "Failed to create new iterator"))
		return NULL;

	SListPyIteratorInit(&itr, pList, error);
	S_CHK_ERR(error, S_CONTERR,
			  "ListPyIterator",
			  "Failed to initialize iterator");

	return S_ITERATOR(itr);

	/* for S_CHECK_PY_LIST */
failure:
	return NULL;
}


/************************************************************************************/
/*                                                                                  */
/* SListPy class initialization                                                     */
/*                                                                                  */
/************************************************************************************/

static SListPyClass ListPyClass =
{
	{
		/* SObjectClass */
		{
			"SContainer:SList:SListPy",
			sizeof(SListPy),
			{ 0, 1},
			InitListPy,          /* init    */
			DestroyListPy,       /* destroy */
			DisposeListPy,       /* dispose */
			NULL,                /* compare */
			NULL,                /* print   */
			NULL,                /* copy    */
		},
		/* SContainerClass */
		ListPyIterator,          /* get_iterator */
	},
	/* SListClass */
	ListPyIsEmpty,                /* is_empty      */
	ListPySize,                   /* size          */
	ListPyAppend,                 /* append        */
	ListPyPrepend,                /* prepend       */
	NULL,                         /* insert_before */
	NULL,                         /* insert_after  */
	NULL,                         /* merge         */
	NULL,                         /* copy          */
	ListPyPush,                   /* push          */
	ListPyPop,                    /* pop           */
	ListPyReverse,                /* reverse       */
	NULL,                         /* nth           */
	ListPyValPresent              /* val_present   */
};
