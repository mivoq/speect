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

#include "python_list.h"


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
	S_CHECK_PY_LIST(pList);

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
	S_CHECK_PY_LIST(pList);

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
	S_CHECK_PY_LIST(pList);

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* convert to Python object type, list is now owner of object (TRUE) */
	pobject = sobject_2_pyobject(object, error, TRUE);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyAppend",
				  "Call to \"sobject_2_pyobject\" failed"))
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
	S_CHECK_PY_LIST(pList);

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* convert to Python object type, list is now owner of object (TRUE) */
	pobject = sobject_2_pyobject(object, error, TRUE);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyPrepend",
				  "Call to \"sobject_2_pyobject\" failed"))
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


static void ListPyInsertBefore(SList *self, SIterator *itr,
							   const SObject *object, s_erc *error)
{
	SListPy *lList = (SListPy*)self;
	SListPyIterator *listItr;
	s_list_element *current_element;
	s_list_element *inserted_element;
	SObject *tmp;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return;

	listItr = (SListPyIterator*)itr;
	current_element = (s_list_element*)listItr->c_itr;

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* insert */
	inserted_element = s_list_insert_before(current_element, (void*)tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyInsertBefore",
				  "Call to s_list_insert_before failed"))
	{
		/* remove reference */
		SObjectDecRef(tmp);
		return;
	}

	listItr->p_itr = inserted_element;
}


static void ListPyInsertAfter(SList *self, SIterator *itr,
								const SObject *object, s_erc *error)
{
	SListPy *lList = (SListPy*)self;
	SListPyIterator *listItr;
	s_list_element *current_element;
	s_list_element *inserted_element;
	SObject *tmp;


	S_CLR_ERR(error);
	if (lList->list == NULL)
		return;

	listItr = (SListPyIterator*)itr;
	current_element = (s_list_element*)listItr->c_itr;

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* insert */
	inserted_element = s_list_insert_after(current_element, (void*)tmp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyInsertAfter",
				  "Call to s_list_insert_after failed"))
	{
		/* remove reference */
		SObjectDecRef(tmp);
		return;
	}

	listItr->n_itr = inserted_element;
}


static void ListPyMerge(SList *self, const SList *with, s_erc *error)
{
	SListPy *lList = (SListPy*)self;
	SIterator *itr;
	SObject *tmp;


	itr = SListIterator(with, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyMerge",
				  "Failed to get an iterator to \'with\' list container"))
		return;

	while (itr)
	{
		/*
		 * get object of "with", cast away const as we
		 * want to increase the ref count
		 */
		tmp = (SObject*)SListIteratorValue(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ListPyMerge",
					  "Call to \"SListIteratorValue\" for \'with\' list container failed"))
		{
			S_DELETE(itr, "ListPyMerge", error);
			return;
		}

		/* increment reference count */
		SObjectIncRef(tmp);

		/* append */
		s_list_append(lList->list, (void*)tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ListPyMerge",
					  "Call to \"s_list_append\" failed"))
		{
			SObjectDecRef(tmp);
			S_DELETE(itr, "ListPyMerge", error);
			return;
		}

		itr = SIteratorNext(itr);
	}
}


static SList *ListPyCopy(SList *dst, const SList *src, s_erc *error)
{
	SListPy *pList = (SListPy*)src;
	PyObject *pobject;
	s_bool made_new = FALSE;
	PyObject *newDstList;
	size_t size;
	size_t i;


	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList);

	size = PyList_Size(S_PY_LIST(pList));

	if (dst == NULL)
	{
		made_new = TRUE;
		newDstList = PyList_New(0); /* 0 so that we can use
									   PyList_Append */
		if (newDstList == NULL)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "ListPyCopy",
						  "Call to \"PyList_New\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "ListPyCopy",
						  "Call to \"PyList_New\" failed");
			}

			return NULL;
		}
	}

	for (i = 0; i < size; i++)
	{
		pobject = PySequence_GetItem(S_PY_LIST(pList), i);
		if (pobject == NULL)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "ListPyCopy",
						  "Call to \"PySequence_GetItem\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "ListPyCopy",
						  "Call to \"PySequence_GetItem\" failed");
			}

			if (made_new) /* delete the newly made list */
				Py_XDECREF(newDstList);

			return NULL;
		}

		if (made_new)
		{
			if (!PyList_Append(newDstList, pobject))
			{
				char *py_error = s_get_python_error_str();

				if (py_error)
				{
					S_CTX_ERR(error, S_FAILURE,
							  "ListPyCopy",
							  "Call to \"PySequence_GetItem\" failed. Reported error: %s",
							  py_error);
					S_FREE(py_error);
				}
				else
				{
					S_CTX_ERR(error, S_FAILURE,
							  "ListPyCopy",
							  "Call to \"PySequence_GetItem\" failed");
				}

				if (made_new) /* delete the newly made list */
					Py_XDECREF(newDstList);
			}
		}
		else
		{
			if (!PyList_Append(S_PY_LIST(dst), pobject))
			{
				char *py_error = s_get_python_error_str();

				if (py_error)
				{
					S_CTX_ERR(error, S_FAILURE,
							  "ListPyCopy",
							  "Call to \"PySequence_GetItem\" failed. Reported error: %s",
							  py_error);
					S_FREE(py_error);
				}
				else
				{
					S_CTX_ERR(error, S_FAILURE,
							  "ListPyCopy",
							  "Call to \"PySequence_GetItem\" failed");
				}

				if (made_new) /* delete the newly made list */
					Py_XDECREF(newDstList);
			}
		}

		/* decrement reference */
		Py_XDECREF(pobject);




	}



	for (itr = s_list_first(listSrc->list, error);
	     itr != NULL;
	     itr = s_list_element_next(itr, error))
	{
		if (S_CHK_ERR(error, S_FAILURE,
					  "ListPyCopy",
					  "Failed to get iterator loop elements"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "ListPyCopy", error);
			return NULL;
		}

		tmp = s_list_element_get(itr, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ListPyCopy",
					  "Call to \"s_list_element_get\" failed"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "ListPyCopy", error);
			return NULL;
		}

		S_LIST_CALL(dst, append)(dst, tmp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "ListPyCopy",
					  "Call to class method \"append\" failed"))
		{
			if (made_new == TRUE)
				S_DELETE(dst, "ListPyCopy", error);
			return NULL;
		}
	}

	return dst;
}


static void ListPyPush(SList *self, const SObject *object, s_erc *error)
{
	SObject *tmp;
	SListPy *pList = (SListPy*)self;
	PyObject *pobject;


	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList);

	/* cast away const */
 	tmp = (SObject*)object;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* convert to Python object type, list is now owner of object (TRUE) */
	pobject = sobject_2_pyobject(object, error, TRUE);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyPush",
				  "Call to \"sobject_2_pyobject\" failed"))
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
	S_CHECK_PY_LIST(pList);

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
	tmp = pyobject_2_sobject(pobject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyPop",
				  "Call to \"pyobject_2_sobject\" failed"))
		return NULL;

	/* remove list reference */
	SObjectDecRef(tmp);

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


static ListPyReverse(SList *self, s_erc *error)
{
	SObject *tmp;
	SListPy *pList = (SListPy*)self;
	PyObject *pobject;


	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList);


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



static const SObject *ListPyNth(SList *self, uint32 n, s_erc *error)
{
	SObject *tmp;
	SListPy *pList = (SListPy*)self;
	PyObject *pobject;


	S_CLR_ERR(error);
	S_CHECK_PY_LIST(pList);


	pobject = PyList_GetItem(S_PY_LIST(pList), (size_t)n);
	if (pobject == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyNth",
					  "Call to \"PyList_GetItem\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ListPyNth",
					  "Call to \"PyList_GetItem\" failed");
		}

		return NULL;
	}

	/* get the Speect object */
	tmp = pyobject_2_sobject(pobject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyNth",
				  "Call to \"pyobject_2_sobject\" failed"))
		return NULL;

	return (const SObject*)tmp;


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
	S_CHECK_PY_LIST(pList);

	/* convert to Python object type */
	pobject = sobject_2_pyobject((SObject*)val, error, FALSE);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyValPresent",
				  "Call to \"sobject_2_pyobject\" failed"))
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


static SIterator *ListPyIterator(const SList *self, s_erc *error)
{
	SListPy *lList = (SListPy*)self;
	SIterator *itr;


	S_CLR_ERR(error);

	itr = S_ITERATOR(S_NEW("SListPyIterator", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyIterator",
				  "Failed to create new iterator"))
		return NULL;

	SListPyIteratorInit(&itr, lList, error);
	S_CHK_ERR(error, S_CONTERR,
			  "ListPyIterator",
			  "Failed to initialize iterator");

	return itr;
}


static const SObject *ListPyVal(const SIterator *iterator, s_erc *error)
{
	const SListPyIterator *self;
	const SObject *tmp;


	S_CLR_ERR(error);

	/* must cast this one to make sure */
	self = S_CAST(iterator, SListPyIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyVal",
				  "Failed to cast SIterator to SListPyIterator"))
		return NULL;


	tmp = s_list_element_get((s_list_element*)self->c_itr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyVal",
				  "Call to s_list_element_get failed"))
		return NULL;

	return tmp;
}


static SObject *ListPyUnlink(SIterator *iterator, s_erc *error)
{
	SListPyIterator *self;
	SObject *tmp;

	S_CLR_ERR(error);

	/* must cast this one */
	self = S_CAST(iterator, SListPyIterator, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyUnlink",
				  "Failed to cast SIterator to SListPyIterator"))
		return NULL;

	if (self->c_itr == NULL)
		return NULL;

	tmp = s_list_element_unlink((s_list_element*)self->c_itr, error);
	self->c_itr = NULL;
	if (S_CHK_ERR(error, S_CONTERR,
				  "ListPyVal",
				  "Call to \"s_list_element_unlink\" failed"))
		return NULL;

	/* remove reference to this container */
	SObjectDecRef(tmp);
	return tmp;
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
		/* No methods */
	},
	/* SListClass */
	ListPyIsEmpty,                /* is_empty      */
	ListPySize,                   /* size          */
	ListPyAppend,                 /* append        */
	ListPyPrepend,                /* prepend       */
	ListPyInsertBefore,           /* insert_before */
	ListPyInsertAfter,            /* insert_after  */
	ListPyMerge,                  /* merge         */
	ListPyCopy,                   /* copy          */
	ListPyPush,                   /* push          */
	ListPyPop,                    /* pop           */
	ListPyReverse,                /* reverse       */
	ListPyNth,                    /* nth           */
	ListPyValPresent,             /* val_present   */
	ListPyIterator,               /* iterator      */
	ListPyVal,                    /* value         */
	ListPyUnlink                  /* unlink        */
};
