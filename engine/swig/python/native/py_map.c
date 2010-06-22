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
/* SMap wrappers for Python dictionaries.                                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "py_funcs.h"
#include "py_map_itr.h"
#include "py_map.h"
#include "py_list.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/* get the Python dict object */
#define S_PY_DICT(SELF)							\
	SELF->pyObject->pobject


/* check that the SMapPy object has a non-NULL Python dict object */
#define S_CHECK_PY_MAP(SELF, FUNCNAME)									\
	do{																	\
		if (SELF->pyObject == NULL)										\
		{																\
			S_CTX_ERR(error, S_FAILURE,									\
					  FUNCNAME,											\
					  "SMapPy object's SPyObject is NULL");				\
			goto failure;												\
		}																\
		if (SELF->pyObject->pobject == NULL)							\
		{																\
			S_CTX_ERR(error, S_FAILURE,									\
					  FUNCNAME,											\
					  "SMapPy object's SPyObject's Python object is NULL"); \
			goto failure;												\
		}																\
	} while (0)


/************************************************************************************/
/*                                                                                  */
/*  Static variables                                                                */
/*                                                                                  */
/************************************************************************************/

static SMapPyClass MapPyClass; /* SMapPy class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Function implementations                                                        */
/*                                                                                  */
/************************************************************************************/

S_API void SMapPyInit(SMapPy **self, PyObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if ((*self) == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapPyInit",
				  "Argument \"self\" is NULL");
		goto error_clean_up;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapPyInit",
				  "Argument \"self\" is NULL");
		goto error_clean_up;
	}

	/* set this map's Python object */
	SPyObjectSet((*self)->pyObject, object, error);
	if (S_CHK_ERR(error, S_ARGERROR,
				  "SMapPyInit",
				  "Call to \"SPyObjectSet\" failed"))
		goto error_clean_up;

	/* all OK */
	return;

	/* clean up code */
error_clean_up:
	S_DELETE((*self), "SMapPyInit", error);
	(*self) = NULL;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_map_py_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&MapPyClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_map_py_class_reg",
			  "Failed to add SMapPyClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitMapPy(void *obj, s_erc *error)
{
	SMapPy *self = obj;


	S_CLR_ERR(error);
	self->tmp = NULL;
	self->pyObject = (SPyObject*)S_NEW("SPyObject", error);
	S_CHK_ERR(error, S_CONTERR,
			  "InitMapPy",
			  "Failed to create new 'SPyObject' object");
}


static void DestroyMapPy(void *obj, s_erc *error)
{
	SMapPy *self = obj;


	S_CLR_ERR(error);

	if (self->pyObject != NULL)
		S_DELETE(self->pyObject, "DestroyMapPy", error);

	if (self->tmp != NULL)
		S_DELETE(self->tmp, "DestroyMapPy", error);
}


static void DisposeMapPy(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static const SObject *MapPyValGet(const SMap *self, const char *key,
								  s_erc *error)
{
	SMapPy *pMap = (SMapPy*)self;
	PyObject *pKey;
	PyObject *pobject;


	S_CLR_ERR(error);
	S_CHECK_PY_MAP(pMap, "MapPyValGet");

	pKey = s_set_pyobject_str(key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapPyValGet",
				  "Call to \"s_set_pyobject_str\" failed"))
		return NULL;

	pobject = PyObject_GetItem(S_PY_DICT(pMap), pKey);
	if (pobject == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValGet",
					  "Call to \"PyObject_GetItem\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValGet",
					  "Call to \"PyObject_GetItem\" failed");
		}

		/* decrement reference of pKey */
		Py_XDECREF(pKey);
		return NULL;
	}

	/* decrement reference of pKey */
	Py_XDECREF(pKey);

	if (pMap->tmp != NULL)
		S_DELETE(pMap->tmp, "MapPyValGet", error);

	/* convert to Speect object type */
	pMap->tmp = s_pyobject_2_sobject(pobject, error);

	return (const SObject*)pMap->tmp;

	/* for S_CHECK_PY_MAP */
failure:
	return NULL;
}


static void MapPyValSet(SMap *self, const char *key, const SObject *val,
						s_erc *error)
{
	SMapPy *pMap = (SMapPy*)self;
	PyObject *pKey = NULL;
	PyObject *pobject;
	SObject *tmp;


	S_CLR_ERR(error);
	S_CHECK_PY_MAP(pMap, "MapPyValSet");

	/* cast away const */
 	tmp = (SObject*)val;

	/* increment reference count */
	SObjectIncRef(tmp);

	/* convert to Python object type, list is now owner of object (TRUE) */
	pobject = s_sobject_2_pyobject(tmp, TRUE, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapPyValSet",
				  "Call to \"s_sobject_2_pyobject\" failed"))
		goto error;

	pKey = s_set_pyobject_str(key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapPyValSet",
				  "Call to \"s_set_pyobject_str\" failed"))
		goto error;

	if (!PyObject_SetItem(S_PY_DICT(pMap), pKey, pobject))
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValSet",
					  "Call to \"PyObject_SetItem\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValSet",
					  "Call to \"PyObject_SetItem\" failed");
		}

		goto error;
	}

	/* decrement reference of pobject */
	Py_XDECREF(pobject);

	/* all OK */
	return;

error:
	/* decrement the reference counting on error as the object
	 * has not successfully been set in the map
	 */
	SObjectDecRef(tmp);

	/* decrement reference of pobject */
	Py_XDECREF(pobject);

	/* decrement reference of pkey */
	Py_XDECREF(pKey);

	/* for S_CHECK_PY_MAP */
failure:
	return;
}


static void MapPyValDelete(SMap *self, const char *key, s_erc *error)
{
	SMapPy *pMap = (SMapPy*)self;
	PyObject *pKey = NULL;


	S_CLR_ERR(error);
	S_CHECK_PY_MAP(pMap, "MapPyValDelete");

	pKey = s_set_pyobject_str(key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapPyValDelete",
				  "Call to \"s_set_pyobject_str\" failed"))
		return;

	if (!PyObject_DelItem(S_PY_DICT(pMap), pKey))
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValDelete",
					  "Call to \"PyObject_DelItem\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValDelete",
					  "Call to \"PyObject_DelItem\" failed");
		}
	}

	/* decrement reference of pkey */
	Py_XDECREF(pKey);

	/* for S_CHECK_PY_MAP */
failure:
	return;
}


static SObject *MapPyValUnlink(SMap *self, const char *key, s_erc *error)
{
	const SMapPy *pMap = (const SMapPy *)self;
	PyObject *pKey;
	PyObject *pobject = NULL;
	SObject *tmp;


	S_CLR_ERR(error);
	S_CHECK_PY_MAP(pMap, "MapPyValUnlink");

	pKey = s_set_pyobject_str(key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapPyValUnlink",
				  "Call to \"s_set_pyobject_str\" failed"))
		goto error;

	pobject = PyObject_GetItem(S_PY_DICT(pMap), pKey);
	if (pobject == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValUnlink",
					  "Call to \"PyObject_GetItem\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValUnlink",
					  "Call to \"PyObject_GetItem\" failed");
		}

		goto error;
	}

	if (!PyObject_DelItem(S_PY_DICT(pMap), pKey))
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValUnlink",
					  "Call to \"PyObject_DelItem\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValUnlink",
					  "Call to \"PyObject_DelItem\" failed");
		}

		goto error;
	}

	/* decrement reference of pKey */
	Py_XDECREF(pKey);

	/* convert to Speect object type */
	tmp = s_pyobject_2_sobject(pobject, error);

	/* decrement reference of pobject */
	Py_XDECREF(pobject);

	/* all ok */
	return tmp;


error:
	/* decrement reference of pKey */
	Py_XDECREF(pKey);

	/* decrement reference of pobject */
	Py_XDECREF(pobject);

	/* for S_CHECK_PY_MAP */
failure:
	return NULL;
}


static s_bool MapPyValPresent(const SMap *self, const char *key, s_erc *error)
{
	SMapPy *pMap = (SMapPy*)self;
	PyObject *pKey = NULL;
	int rv;


	S_CLR_ERR(error);
	S_CHECK_PY_MAP(pMap, "MapPyValPresent");

	pKey = s_set_pyobject_str(key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapPyValPresent",
				  "Call to \"s_set_pyobject_str\" failed"))
		return FALSE;

	rv = PyDict_Contains(S_PY_DICT(pMap), pKey);
	if (rv == -1)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValPresent",
					  "Call to \"PyDict_Contains\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValPresent",
					  "Call to \"PyDict_Contains\" failed");
		}

		/* decrement reference of pKey */
		Py_XDECREF(pKey);

		return FALSE;
	}

	/* decrement reference of pKey */
	Py_XDECREF(pKey);

	if (rv == 1)
		return TRUE;

	return FALSE;

	/* for S_CHECK_PY_MAP */
failure:
	return FALSE;
}


static SList *MapPyValKeys(const SMap *self, s_erc *error)
{
	SMapPy *pMap = (SMapPy*)self;
	SListPy *pyList;
	PyObject *keys;


	S_CLR_ERR(error);
	S_CHECK_PY_MAP(pMap, "MapPyValKeys");

	keys = PyDict_Keys(S_PY_DICT(pMap));
	if (keys == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValKeys",
					  "Call to \"PyDict_Keys\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "MapPyValKeys",
					  "Call to \"PyDict_Keys\" failed");
		}

		return NULL;
	}

	pyList = (SListPy*)S_NEW("SListPy", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapPyValKeys",
				  "Failed to create new 'SListPy' object"))
	{
		/* decrement reference of keys */
		Py_XDECREF(keys);
		return NULL;
	}

	SListPyInit(&pyList, keys, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapPyValKeys",
				  "Call to \"SListPyInit\" failed"))
	{
		/* decrement reference of keys */
		Py_XDECREF(keys);
		return NULL;
	}

	/* decrement reference of keys */
	Py_XDECREF(keys);

	return S_LIST(pyList);


	/* for S_CHECK_PY_MAP */
failure:
	return NULL;
}


/* number of elements, not the table size. */
static size_t MapPySize(const SMap *self, s_erc *error)
{
	SMapPy *pMap = (SMapPy*)self;
	size_t size;


	S_CLR_ERR(error);
	S_CHECK_PY_MAP(pMap, "MapPyValKeys");

	size = PyDict_Size(S_PY_DICT(pMap));
	return size;

	/* for S_CHECK_PY_MAP */
failure:
	return 0;
}


static SIterator *MapPyIterator(const SContainer *self, s_erc *error)
{
	SMapPyIterator *itr;


	S_CLR_ERR(error);

	itr = (SMapPyIterator*)S_NEW("SMapPyIterator", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "MapPyIterator",
				  "Failed to create new iterator"))
		return NULL;

	SMapPyIteratorInit(&itr, S_MAPPY(self), error);
	S_CHK_ERR(error, S_CONTERR,
			  "MapPyIterator",
			  "Failed to initialize iterator");

	return S_ITERATOR(itr);
}


/************************************************************************************/
/*                                                                                  */
/* SMapPy class initialization                                                      */
/*                                                                                  */
/************************************************************************************/

static SMapPyClass MapPyClass =
{
	{
		/* SObjectClass */
		{
			"SContainer:SMap:SMapPy",
			sizeof(SMapPy),
			{ 0, 1},
			InitMapPy,    /* init    */
			DestroyMapPy, /* destroy */
			DisposeMapPy, /* dispose */
			NULL,         /* compare */
			NULL,         /* print   */
			NULL,         /* copy    */
		},
		/* SContainerClass */
		MapPyIterator,    /* get_iterator */
	},
	/* SMapClass */
	MapPyValGet,            /* val_get      */
	MapPyValSet,            /* val_set      */
	MapPyValDelete,         /* val_delete   */
	MapPyValUnlink,         /* val_unlink   */
	MapPyValPresent,        /* val_present  */
	MapPyValKeys,           /* val_keys     */
	MapPySize,              /* size         */
	NULL                    /* copy         */
};
