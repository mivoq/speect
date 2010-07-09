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
/* Miscellaneous Python object functions.                                           */
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
#include "py_object.h"
#include "py_list.h"
#include "py_funcs.h"


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/* enum for SObject types */
typedef enum
{
	S_TYPE_SSINT = 0,    /*!< SInt, signed integer value.                */
	S_TYPE_SFLOAT,       /*!< SFloat, floating point value.              */
	S_TYPE_SSTRING,      /*!< SString, string value (char*).             */
	S_TYPE_SPYOBJECT,    /*!< SPyObject, Python object.                  */
	S_TYPE_SLISTPY,      /*!< SListPy, Python list.                      */
	S_TYPE_SMAPPY,       /*!< SMapPy, Python dictionary.                 */
	S_TYPE_SUNKNOWN,     /*!< Unknown type (not primitive), use SObject. */
	S_TYPE_SERROR        /*!< error, should not get here.                */
} s_sobject_type;


/* helper structure for defining a list of object types */
typedef struct
{
	const char     *class;
	s_sobject_type  type;
} s_sobject_defs;


/* enum for PyObject types */
typedef enum
{
	S_TYPE_INT = 0,     /*!< int, signed integer value.                */
#if PY_VERSION_HEX < 0x03000000 /* Python 2.x */
	S_TYPE_LONG,        /*!< long, long int.				 		   */
#endif /* PY_VERSION_HEX < 0x03000000 */
	S_TYPE_FLOAT,       /*!< float, floating point value.              */
	S_TYPE_STRING,      /*!< string, string value (char*).			   */
#if PY_VERSION_HEX < 0x03000000 /* Python 2.x */
	S_TYPE_UNICODE,     /*!< unicode, string value (char*).			   */
#endif /* PY_VERSION_HEX < 0x03000000 */
	S_TYPE_LIST,        /*!< list.			 						   */
 	S_TYPE_DICT,        /*!< dictionary (map in Speect).			   */
	S_TYPE_UNKNOWN,     /*!< Unknown type, use SPyObject.              */
	S_TYPE_ERROR        /*!< error, should not get here.               */
} s_pobject_type;


/* helper structure for defining a list of object types */
typedef struct
{
	const char     *class;
	s_pobject_type  type;
} s_pobject_defs;


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* a list of Speect object types with there enum types */
static const s_sobject_defs sobject_type_list[] =
{
	{ "SInt", S_TYPE_SSINT },
	{ "SFloat", S_TYPE_SFLOAT },
	{ "SString", S_TYPE_SSTRING },
	{ "SPyObject", S_TYPE_SPYOBJECT },
	{ "SListPy", S_TYPE_SLISTPY },
	{ "SMapPy", S_TYPE_SMAPPY },
	{ NULL, S_TYPE_UNKNOWN }
};


/* a list of Python object types with there enum types */
static const s_pobject_defs pobject_type_list[] =
{
	{ "int", S_TYPE_INT },
#if PY_VERSION_HEX < 0x03000000 /* Python 2.x */
	{ "long", S_TYPE_LONG },
#endif /* PY_VERSION_HEX < 0x03000000 */
	{ "float", S_TYPE_FLOAT },
	{ "str", S_TYPE_STRING },
#if PY_VERSION_HEX < 0x03000000 /* Python 2.x */
	{ "unicode", S_TYPE_UNICODE },
#endif /* PY_VERSION_HEX < 0x03000000 */
	{ "list", S_TYPE_LIST },
	{ "dict", S_TYPE_DICT },
	{ NULL, S_TYPE_UNKNOWN }
};


/* SObject SWIG type info */
static swig_type_info *SObject_p_swig_type_info = NULL;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

/* Python -> Speect */
static s_sobject_type get_sobject_type(const SObject *object, s_erc *error);

static PyObject *ssint_2_pyobject(const SObject *object, s_erc *error);

static PyObject *sfloat_2_pyobject(const SObject *object, s_erc *error);

static PyObject *sstring_2_pyobject(const SObject *object, s_erc *error);

static PyObject *swig_2_pyobject(const SObject *object, s_bool own, s_erc *error);


/* Speect -> Python */
static s_pobject_type get_pobject_type(PyObject *object, s_erc *error);

static SObject *pyobject_2_ssint(PyObject *pobject, s_erc *error);

static SObject *pyobject_2_sfloat(PyObject *pobject, s_erc *error);

static SObject *pyobject_2_sstring(PyObject *pobject, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_py_funcs_initialize(s_erc *error)
{
	S_CLR_ERR(error);

	SObject_p_swig_type_info = SWIG_TypeQuery("SObject *");
}


S_API PyObject *s_sobject_2_pyobject(const SObject *object, s_bool own, s_erc *error)
{
	s_sobject_type type;
	PyObject *pobject;


	S_CLR_ERR(error);

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_sobject_2_pyobject",
				  "Argument \"object\" is NULL");
		Py_RETURN_NONE;
	}

	type = get_sobject_type(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_sobject_2_pyobject",
				  "Call to \"get_sobject_type\" failed"))
		Py_RETURN_NONE;

	switch(type)
	{
	case S_TYPE_SSINT:
	{
		pobject = ssint_2_pyobject(object, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_sobject_2_pyobject",
					  "Call to \"ssint_2_pyobject\" failed"))
			Py_RETURN_NONE;

		goto check_owner;
	}
	case S_TYPE_SFLOAT:
	{
		pobject = sfloat_2_pyobject(object, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_sobject_2_pyobject",
					  "Call to \"sfloat_2_pyobject\" failed"))
			Py_RETURN_NONE;

		goto check_owner;
	}
	case S_TYPE_SSTRING:
	{
		pobject = sstring_2_pyobject(object, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_sobject_2_pyobject",
					  "Call to \"sstring_2_pyobject\" failed"))
			Py_RETURN_NONE;

		goto check_owner;
	}
	case S_TYPE_SPYOBJECT:
	{
		pobject = SPyObjectGet(S_PYOBJECT(object), error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_sobject_2_pyobject",
					  "Call to \"SPyObjectGet\" failed"))
			Py_RETURN_NONE;

		goto check_owner;
	}
	case S_TYPE_SLISTPY:
	{
		pobject = SPyObjectGet(((SListPy*)object)->pyObject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_sobject_2_pyobject",
					  "Call to \"SPyObjectGet\" failed"))
			Py_RETURN_NONE;

		goto check_owner;
	}
	case S_TYPE_SMAPPY:
	{
		pobject = SPyObjectGet(((SMapPy*)object)->pyObject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_sobject_2_pyobject",
					  "Call to \"SPyObjectGet\" failed"))
			Py_RETURN_NONE;

		goto check_owner;
	}
	case S_TYPE_SUNKNOWN:
	{
		pobject = swig_2_pyobject(object, own, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_sobject_2_pyobject",
					  "Call to \"swig_2_pyobject\" failed"))
			Py_RETURN_NONE;

		return pobject; /* ownership is done in swig_2_pyobject */
	}
	default:
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_sobject_2_pyobject",
				  "Failed to do type conversion");
		Py_RETURN_NONE;
	}
	}

check_owner:
	if (own == TRUE)
		S_DELETE(object, "s_sobject_2_pyobject", error);

	return pobject;
}


S_API SObject *s_pyobject_2_sobject(PyObject *pobject, s_erc *error)
{
	s_pobject_type type;
	SObject *object;
	int res;
	void *argp;


	S_CLR_ERR(error);

	if (pobject == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_pyobject_2_sobject",
				  "Argument \"pobject\" is NULL");
		return NULL;
	}

	/* first let SWIG try to do the conversion */
	res = SWIG_ConvertPtr(pobject, &argp, SObject_p_swig_type_info,
						  SWIG_POINTER_DISOWN|0);
	if (SWIG_IsOK(res))
	{
		/* it's a speect object */
		object = (SObject*)argp;
		return object;
	}

	type = get_pobject_type(pobject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_pyobject_2_sobject",
				  "Call to \"get_pobject_type\" failed"))
		return NULL;

	switch(type)
	{
	case S_TYPE_INT:
	{
		object = pyobject_2_ssint(pobject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pyobject_2_sobject",
					  "Call to \"pyobject_2_ssint\" failed"))
			return NULL;

		return object;
	}
#if PY_VERSION_HEX < 0x03000000 /* Python 2.x, also uses pyobject_2_ssint */
	case S_TYPE_LONG:
	{
		object = pyobject_2_ssint(pobject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pyobject_2_sobject",
					  "Call to \"pyobject_2_ssint\" failed"))
			return NULL;

		return object;
	}
#endif /* PY_VERSION_HEX < 0x03000000 */
	case S_TYPE_FLOAT:
	{
		object = pyobject_2_sfloat(pobject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pyobject_2_sobject",
					  "Call to \"pyobject_2_sfloat\" failed"))
			return NULL;

		return object;
	}
	case S_TYPE_STRING:
	{
		object = pyobject_2_sstring(pobject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pyobject_2_sobject",
					  "Call to \"pyobject_2_sstring\" failed"))
			return NULL;

		return object;
	}
#if PY_VERSION_HEX < 0x03000000 /* Python 2.x, also uses pyobject_2_sstring */
	case S_TYPE_UNICODE:
	{
		object = pyobject_2_sstring(pobject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pyobject_2_sobject",
					  "Call to \"pyobject_2_sstring (unicode)\" failed"))
			return NULL;

		return object;
	}
#endif /* PY_VERSION_HEX < 0x03000000 */
	case S_TYPE_LIST:
	{
		object = S_NEW("SListPy", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pyobject_2_sobject",
					  "Failed to create new 'SListPy' object"))
			return NULL;

		SListPyInit((SListPy**)&object, pobject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pyobject_2_sobject",
					  "Call to \"SListPyInit\" failed"))
			return NULL;

		return object;
	}
	case S_TYPE_DICT:
	{
		object = S_NEW("SMapPy", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pyobject_2_sobject",
					  "Failed to create new 'SMapPy' object"))
			return NULL;

		SMapPyInit((SMapPy**)&object, pobject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pyobject_2_sobject",
					  "Call to \"SMapPyInit\" failed"))
			return NULL;

		return object;
	}
	case S_TYPE_UNKNOWN:
	{
		object = S_NEW("SPyObject", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pyobject_2_sobject",
					  "Failed to create new 'SPyObject' object"))
			return NULL;

		SPyObjectSet((SPyObject*)object, pobject, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_pyobject_2_sobject",
					  "Call to \"SPyObjectSet\" failed"))
		{
			S_DELETE(object, "s_pyobject_2_sobject", error);
			return NULL;
		}

		return object;
	}
	default:
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_pyobject_2_sobject",
				  "Failed to do type conversion");
	}
	}

	return NULL;
}


S_API PyObject *s_set_pyobject_str(const char *cstr, s_erc *error)
{
	size_t slen;
	PyObject *pobject;


	S_CLR_ERR(error);

	slen = s_strsize(cstr, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_set_pyobject_str",
				  "Call to \"s_strsize\" failed"))
		return NULL;

#if PY_VERSION_HEX >= 0x03000000
	pobject = PyUnicode_FromStringAndSize(cstr, slen);
#define SPCT_S_SET_PYOBJECT_STR_FUNC "PyUnicode_FromStringAndSize"
#else /* ! PY_VERSION_HEX >= 0x03000000 */
	pobject = PyString_FromStringAndSize(cstr, slen);
#define SPCT_S_SET_PYOBJECT_STR_FUNC "PyString_FromStringAndSize"
#endif /* PY_VERSION_HEX >= 0x03000000 */

	if (pobject == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "s_set_pyobject_str",
					  "Call to \"%s\" failed. Reported error: %s",
					  SPCT_S_SET_PYOBJECT_STR_FUNC, py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "s_set_pyobject_str",
					  "Call to \"%s\" failed", SPCT_S_SET_PYOBJECT_STR_FUNC);
		}

		return NULL;
	}

	return pobject;
}


S_API char *s_get_pyobject_str(PyObject *pobject, s_erc *error)
{
	PyObject *strObject;
	PyObject *utf8Object;
	char *cstr;
	char *new_cstr;


	S_CLR_ERR(error);
	if (pobject == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_get_pyobject_str",
				  "Argument \"pobject\" is NULL");
		return NULL;
	}

#if PY_VERSION_HEX >= 0x03000000
	strObject = PyObject_Str(pobject);
	if (strObject == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_pyobject_str",
				  "Call to \"PyObject_Str\" failed");
		return NULL;
	}

	utf8Object = PyUnicode_AsUTF8String(strObject);
	Py_XDECREF(strObject);
	if (utf8Object == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_pyobject_str",
				  "Call to \"PyUnicode_AsUTF8String\" failed");
		return NULL;
	}

	cstr = PyBytes_AsString(utf8Object);
	if (cstr == NULL)
	{
		Py_XDECREF(utf8Object);
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_pyobject_str",
				  "Call to \"PyBytes_AsString\" failed");
		return NULL;
	}

	new_cstr = s_strdup(cstr, error);
	Py_XDECREF(utf8Object);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_pyobject_str",
				  "Call to \"s_strdup\" failed"))
		return NULL;
#else /* ! PY_VERSION_HEX >= 0x03000000 */
	strObject = PyObject_Unicode(pobject);
	if (strObject == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_pyobject_str",
				  "Call to \"PyObject_Unicode\" failed");
		return NULL;
	}

	utf8Object = PyUnicode_AsUTF8String(strObject);
	Py_XDECREF(strObject);
	if (utf8Object == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_pyobject_str",
				  "Call to \"PyUnicode_AsUTF8String\" failed");
		return NULL;
	}

	cstr = PyString_AsString(utf8Object);
	if (cstr == NULL)
	{
		Py_XDECREF(utf8Object);
		S_CTX_ERR(error, S_FAILURE,
				  "s_get_pyobject_str",
				  "Call to \"PyString_AsString\" failed");
		return NULL;
	}

	new_cstr = s_strdup(cstr, error);
	Py_XDECREF(utf8Object);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_get_pyobject_str",
				  "Call to \"s_strdup\" failed"))
		return NULL;
#endif /* PY_VERSION_HEX >= 0x03000000 */

	return new_cstr;
}


S_API char *s_get_python_error_str(void)
{
	PyObject *pyErr;
	PyObject *ptype;
	PyObject *pvalue;
	PyObject *ptraceback;
	char *terror = NULL; /* error type */
	char *verror = NULL; /* error value */
	char *this_error = NULL; /* all combined */
	s_erc error;


	S_CLR_ERR(&error);

	pyErr = PyErr_Occurred();

	/* return NULL if error is not set */
	if (pyErr == NULL)
		return NULL;

	PyErr_Fetch(&ptype, &pvalue, &ptraceback);
	if (ptype == NULL)
		goto error_cleanup;

	PyErr_NormalizeException(&ptype, &pvalue, &ptraceback);
	if (ptype == NULL)
		goto error_cleanup;

	terror = s_get_pyobject_str(ptype, &error);
	if (error != S_SUCCESS)
		goto error_cleanup;

	if (pvalue != NULL)
	{
		verror = s_get_pyobject_str(pvalue, &error);
		if (error != S_SUCCESS)
			goto error_cleanup;
	}

	/* combine them */
	s_asprintf(&this_error, &error, "%s, %s",
			   terror, verror? verror : "Unknown");
	if (error != S_SUCCESS)
		goto error_cleanup;

	/* all normal */
	/* clear */
	Py_XDECREF(ptype);
	Py_XDECREF(pvalue);
	Py_XDECREF(ptraceback);

	if (terror != NULL)
		S_FREE(terror);

	if (verror != NULL)
		S_FREE(verror);

	return this_error;

	/* errors */
error_cleanup:
	/* clear */
	Py_XDECREF(ptype);
	Py_XDECREF(pvalue);
	Py_XDECREF(ptraceback);

	if (terror != NULL)
		S_FREE(terror);

	if (verror != NULL)
		S_FREE(verror);

	this_error = s_strdup("Unknown error", &error);
	return this_error;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static s_sobject_type get_sobject_type(const SObject *object, s_erc *error)
{
	const char *class;
	const s_sobject_defs *itr;
	int s_comp;

	S_CLR_ERR(error);

	class = SObjectType(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "get_sobject_type",
				  "Call to \"SObjectType\" failed"))
		return S_TYPE_SERROR;

	/* iterate through object type list to get the
	 * right s_sobject_type enum
	 */
	for (itr = sobject_type_list; itr->class != NULL; itr++)
	{
		s_comp = s_strcmp(class, itr->class, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_sobject_type",
					  "Call to \"s_strcmp\" failed"))
			return S_TYPE_ERROR;

		if (s_comp == 0)
			return itr->type;
	}

	return S_TYPE_SUNKNOWN;
}


static s_pobject_type get_pobject_type(PyObject *object, s_erc *error)
{
	PyTypeObject *to;
	const char *type;
	const s_pobject_defs *itr;
	int s_comp;


	S_CLR_ERR(error);
	to = Py_TYPE(object);
	type = to->tp_name;

	/* iterate through object type list to get the
	 * right s_pobject_type enum
	 */
	for (itr = pobject_type_list; itr->class != NULL; itr++)
	{
		s_comp = s_strcmp(type, itr->class, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "get_pobject_type",
					  "Call to \"s_strcmp\" failed"))
			return S_TYPE_ERROR;

		if (s_comp == 0)
			return itr->type;
	}

	return S_TYPE_UNKNOWN;
}



static PyObject *ssint_2_pyobject(const SObject *object, s_erc *error)
{
	sint32 val;
	PyObject *pobject;


	S_CLR_ERR(error);

	val = SObjectGetInt(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "ssint_2_pyobject",
				  "Call to \"SObjectGetInt\" failed"))
		return NULL;

	pobject = PyLong_FromLong((long)val);
	if (pobject == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ssint_2_pyobject",
					  "Call to \"PyLong_FromLong\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "ssint_2_pyobject",
					  "Call to \"PyLong_FromLong\" failed");
		}
	}

	return pobject;
}


static PyObject *sfloat_2_pyobject(const SObject *object, s_erc *error)
{
	float val;
	PyObject *pobject;


	S_CLR_ERR(error);

	val = SObjectGetFloat(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "sfloat_2_pyobject",
				  "Call to \"SObjectGetFloat\" failed"))
		return NULL;

	pobject = PyFloat_FromDouble((double)val);
	if (pobject == NULL)
	{
		char *py_error = s_get_python_error_str();

		if (py_error)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "sfloat_2_pyobject",
					  "Call to \"PyFloat_FromDouble\" failed. Reported error: %s",
					  py_error);
			S_FREE(py_error);
		}
		else
		{
			S_CTX_ERR(error, S_FAILURE,
					  "sfloat_2_pyobject",
					  "Call to \"PyFloat_FromDouble\" failed");
		}
	}

	return pobject;
}


static PyObject *sstring_2_pyobject(const SObject *object, s_erc *error)
{
	const char *val;
	PyObject *pobject;


	S_CLR_ERR(error);

	val = SObjectGetString(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "sstring_2_pyobject",
				  "Call to \"SObjectGetString\" failed"))
		return NULL;

	pobject = s_set_pyobject_str(val, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "sstring_2_pyobject",
				  "Call to \"s_set_pyobject_str\" failed"))
		return NULL;

	return pobject;
}


static PyObject *swig_2_pyobject(const SObject *object, s_bool own, s_erc *error)
{
	PyObject       *pobject = NULL;
	char           *cname;
	char           *c;
	char           *cp;
	const char     *inherit_hier;
	size_t          len;
	char           *full_type_name;
	swig_type_info *info = NULL;


	S_CLR_ERR(error);

	inherit_hier = SObjectInheritance(object, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "swig_2_pyobject",
				  "Call to \"SObjectInheritance\" failed"))
		return NULL;

	cname = s_strdup(inherit_hier, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "swig_2_pyobject",
				  "Call to \"s_strdup\" failed"))
		return NULL;

	cp = cname;
	do
	{
		c = s_strtok_r(NULL, ":", &cp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "swig_2_pyobject",
					  "Call to \"s_strtok_r\" failed"))
		{
			S_FREE(cname);
			return NULL;
		}

		if (c == NULL)
		{
			S_FREE(cname);
			break;
		}

		len = s_strzsize(c, error) + 2;
		if (S_CHK_ERR(error, S_CONTERR,
					  "swig_2_pyobject",
					  "Call to \"s_strzsize\" failed"))
		{
			S_FREE(cname);
			return NULL;
		}

		full_type_name = S_CALLOC(char, len);
		if (full_type_name == NULL)
		{
			S_FREE(cname);
			S_FTL_ERR(error, S_MEMERROR,
					  "swig_2_pyobject",
					  "Failed to allocate memory for 'char' object");
			return NULL;
		}

		s_strcat(full_type_name, c, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "swig_2_pyobject",
					  "Call to \"s_strcat\" failed"))
		{
			S_FREE(full_type_name);
			S_FREE(cname);
			return NULL;
		}

		s_strcat(full_type_name, " *", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "swig_2_pyobject",
					  "Call to \"s_strcat\" failed"))
		{
			S_FREE(full_type_name);
			S_FREE(cname);
			return NULL;
		}

		info = SWIG_TypeQuery(full_type_name);
		S_FREE(full_type_name);

		if (info != NULL)
		{
			S_FREE(cname);
			break;
		}

	} while (c != NULL);


	if (info != NULL)
	{
		if (own == TRUE)
		{
			pobject = SWIG_NewPointerObj(S_VOIDPTR(object),
										 info, 1);
		}
		else
		{
			pobject = SWIG_NewPointerObj(S_VOIDPTR(object),
										 info, 0 |0);
		}
	}
	else
	{
		/* don't know this object type, use SObject */
		if (own == TRUE)
		{
			pobject = SWIG_NewPointerObj(S_VOIDPTR(object),
										 SObject_p_swig_type_info, 1);
		}
		else
		{
			pobject = SWIG_NewPointerObj(S_VOIDPTR(object),
										 SObject_p_swig_type_info, 0 |0);
		}
	}

	return pobject;
}


static SObject *pyobject_2_ssint(PyObject *pobject, s_erc *error)
{
	SObject *object;
	sint32 val;


	S_CLR_ERR(error);
	val = (sint32)PyLong_AsLong(pobject);
	object = SObjectSetInt(val, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "pyobject_2_ssint",
				  "Call to \"SObjectSetInt\" failed"))
		return NULL;

	return object;
}


static SObject *pyobject_2_sfloat(PyObject *pobject, s_erc *error)
{
	SObject *object;
	float val;


	S_CLR_ERR(error);
	val = (float)PyFloat_AsDouble(pobject);
	object = SObjectSetFloat(val, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "pyobject_2_sfloat",
				  "Call to \"SObjectSetFloat\" failed"))
		return NULL;

	return object;
}


static SObject *pyobject_2_sstring(PyObject *pobject, s_erc *error)
{
	SObject *object;
	char *val;


	S_CLR_ERR(error);
	val = s_get_pyobject_str(pobject, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "pyobject_2_sstring",
				  "Call to \"s_get_pyobject_str\" failed"))
		return NULL;

	object = SObjectSetString(val, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "pyobject_2_sstring",
				  "Call to \"SObjectSetString\" failed"))
	{
		S_FREE(val);
		return NULL;
	}

	S_FREE(val);
	return object;
}
