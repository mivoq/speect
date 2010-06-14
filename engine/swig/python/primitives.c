/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* DATE    : November 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for primitives SInt, SFloat, SString.                    */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%define sobject_2_pyobject_DOCSTRING
"""
sobject_2_pyobject(object, own)

Convert a Speect ``SObject`` type object to a Python object. This can be
use full for conversions where either type is required an is used extensively
internally and in the plug-in implementations.

The ``own`` ownership flag specifies if Python must take ownership of the object,
and hence can delete it. If not then Speect controls the memory of the object.

:param object: The Speect object to convert.
:type key: SObject
:param own: Ownership flag.
:type own: bool
:return: A Python representation of the given object
:rtype: Python type object.
"""
%enddef

%feature("autodoc", sobject_2_pyobject_DOCSTRING) sobject_2_pyobject;


%define pyobject_2_sobject_DOCSTRING
"""
pyobject_2_sobject(pobject)

Convert a Python type object to a Speect ``SObject`` object. This can be
use full for conversions where either type is required an is used extensively
internally and in the plug-in implementations.

Note that for basic types (int, float, string) Speect creates a new type. For
other types, Speect creates an ``SObject``. This function will only work with
basic types if no external plug-ins are loaded.

:param pobject: The Python object to convert.
:return: A Speect representation of the given object
:rtype: ``SObject`` type object.
"""
%enddef

%feature("autodoc", pyobject_2_sobject_DOCSTRING) pyobject_2_sobject;



/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	PyObject *sobject_2_pyobject(const SObject *object, s_erc *error, s_bool own)
	{
		PyObject *pobject;
		const char *type;
		int s_comp;
		swig_type_info *info;


		if (object == NULL)
			Py_RETURN_NONE;

		type = SObjectType(object, error);
		if (*error != S_SUCCESS)
			return NULL;

		s_comp = s_strcmp(type, "SInt", error);
		if (*error != S_SUCCESS)
			return NULL;

		if (s_comp == 0)
		{
			sint32 val;


			val = SObjectGetInt(object, error);
			if (*error != S_SUCCESS)
				return NULL;

			pobject = PyInt_FromLong((long)val);
			if (own == TRUE)
				S_DELETE(object, "sobject_2_pyobject", error);

			return pobject;
		}

		s_comp = s_strcmp(type, "SFloat", error);
		if (*error != S_SUCCESS)
			return NULL;

		if (s_comp == 0)
		{
			float val;


			val = SObjectGetFloat(object, error);
			if (*error != S_SUCCESS)
				return NULL;

			pobject = PyFloat_FromDouble((double)val);
			if (own == TRUE)
				S_DELETE(object, "sobject_2_pyobject", error);

			return pobject;
		}

		s_comp = s_strcmp(type, "SString", error);
		if (*error != S_SUCCESS)
			return NULL;

		if (s_comp == 0)
		{
			const char *val;
			size_t slen;


			val = SObjectGetString(object, error);
			if (*error != S_SUCCESS)
				return NULL;

			slen = s_strsize(val, error);
			if (*error != S_SUCCESS)
				return NULL;

			pobject = PyString_FromStringAndSize(val, slen);
			if (own == TRUE)
				S_DELETE(object, "sobject_2_pyobject", error);

			return pobject;
		}

		s_comp = s_strcmp(type, "SPyObject", error);
		if (*error != S_SUCCESS)
			return NULL;

		if (s_comp == 0)
		{
			/* note that the get method implementation of the
			 * SPyObject increased the reference count of the returned
			 * python object */
			pobject = (PyObject*)SObjectGetVoid(object, error);
			if (*error != S_SUCCESS)
				return NULL;

			if (own == TRUE)
				S_DELETE(object, "sobject_2_pyobject", error);

			return pobject;
		}

		/* cycle through object inheritance hierarchy,
		 * maybe we find something
		 */
		{
			char               *cname;
			char               *c;
			char               *cp;
			const char         *inherit_hier;
			size_t             len;
			char               *full_type_name;

			inherit_hier = SObjectInheritance(object, error);
			if (*error != S_SUCCESS)
				return NULL;

			cname = s_strdup(inherit_hier, error);
			if (*error != S_SUCCESS)
				return NULL;

			cp = cname;
			do
			{
				c = s_strtok_r(NULL, ":", &cp, error);

				if (S_CHK_ERR(error, S_CONTERR,
							  "sobject_2_pyobject",
							  "Failed to extract token from class name"))
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
				if (*error != S_SUCCESS)
				{
					S_FREE(cname);
					return NULL;
				}

				full_type_name = S_CALLOC(char, len);
				if (full_type_name == NULL)
				{
					S_FREE(cname);
					S_FTL_ERR(error, S_MEMERROR,
							  "sobject_2_pyobject",
							  "Failed to allocate memory for 'char' object");
					return NULL;
				}

				s_strcat(full_type_name, c, error);
				if (*error != S_SUCCESS)
				{
					S_FREE(full_type_name);
					S_FREE(cname);
					return NULL;
				}

				s_strcat(full_type_name, " *", error);
				if (*error != S_SUCCESS)
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
		}

		if (info != NULL)
		{
			if (own == TRUE)
			{
				pobject = SWIG_NewPointerObj(SWIG_as_voidptr(object),
											 info, 1);
			}
			else
			{
				pobject = SWIG_NewPointerObj(SWIG_as_voidptr(object),
											 info, 0 |0);
			}
		}
		else
		{
			/* don't know this object type, use SObject */
			if (own == TRUE)
			{
				pobject = SWIG_NewPointerObj(SWIG_as_voidptr(object),
											 SWIGTYPE_p_SObject, 1);
			}
			else
			{
				pobject = SWIG_NewPointerObj(SWIG_as_voidptr(object),
											 SWIGTYPE_p_SObject, 0 |0);
			}
		}

		if (own == TRUE)
			S_DELETE(object, "sobject_2_pyobject", error);

		return pobject;
	}


	SObject *pyobject_2_sobject(PyObject *pobject, s_erc *error)
	{
		SObject *object;
		int res;
		void *argp;


		if (pobject == Py_None)
			return NULL;

		res = SWIG_ConvertPtr(pobject, &argp,SWIGTYPE_p_SObject, SWIG_POINTER_DISOWN|0);
		if (SWIG_IsOK(res))
		{
			/* it's a speect object */
			object = (SObject*)argp;
			return object;
		}

		/* it's a python object */
		if (PyObject_IsInstance(pobject, (PyObject*)&PyInt_Type))
		{
			sint32 tmp;


			tmp = (sint32)PyLong_AsLong(pobject);
			object = SObjectSetInt(tmp, error);
			if (*error != S_SUCCESS)
				return NULL;

			return object;
		}

		if (PyObject_IsInstance(pobject, (PyObject*)&PyFloat_Type))
		{
			float tmp;


			tmp = (float)PyFloat_AsDouble(pobject);
			object = SObjectSetFloat(tmp, error);
			if (*error != S_SUCCESS)
				return NULL;

			return object;
		}

		if (PyObject_IsInstance(pobject, (PyObject*)&PyString_Type))
		{
			const char *tmp;


			tmp = PyString_AsString(pobject);
			object = SObjectSetString(tmp, error);
			if (*error != S_SUCCESS)
				return NULL;

			return object;
		}

		if (PyObject_IsInstance(pobject, (PyObject*)&PyUnicode_Type))
		{
			PyObject *ustring;
			const char *tmp;


			ustring = PyUnicode_AsUTF8String(pobject);
			if (ustring == NULL)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "pyobject_2_sobject",
						  "Call to \"PyUnicode_AsUTF8String\" failed");
				return NULL;
			}

			tmp = PyString_AsString(ustring);
			object = SObjectSetString(tmp, error);
			if (*error != S_SUCCESS)
				return NULL;

			Py_XDECREF(ustring);
			return object;
		}

		if (PyObject_IsInstance(pobject, (PyObject*)&PyList_Type))
		{
			SList *list;
			int count;
			int i;


			count = PyList_Size(pobject);
			if (count == 0)
				return NULL;

			list = (SList*)S_NEW("SListList", error);
			if (*error != S_SUCCESS)
				goto slist_fail;

			SListListInit(&list, error);
			if (*error != S_SUCCESS)
				goto slist_fail;

			for (i = 0; i < count; i++)
			{
				PyObject *p = PyList_GetItem(pobject, i);


				if (PyString_Check(p))
				{
					const char *tmp;
					SObject *obj;


					tmp = PyString_AsString(p);
					obj = SObjectSetString(tmp, error);
					if (*error != S_SUCCESS)
						goto slist_fail;

					SListAppend(list, obj, error);
					if (*error != S_SUCCESS)
						goto slist_fail;
				}
				else if (PyUnicode_Check(p))
				{
					const char *tmp;
					SObject *obj;
					PyObject *ustring;


					ustring = PyUnicode_AsUTF8String(p);
					if (ustring == NULL)
					{
						S_CTX_ERR(error, S_FAILURE,
								  "pyobject_2_sobject",
								  "Call to \"PyUnicode_AsUTF8String\" failed");
						goto slist_fail;
					}

					tmp = PyString_AsString(p);
					obj = SObjectSetString(tmp, error);
					if (error != S_SUCCESS)
						goto slist_fail;

					SListAppend(list, obj, error);
					if (*error != S_SUCCESS)
						goto slist_fail;
				}
				else
				{
					PyErr_SetString(PyExc_TypeError,
									"object in list is not str/unicode type");
					goto slist_fail;
				}
			}

			goto slist_good;

		slist_fail:
			if (list != NULL)
				S_DELETE(list, "pyobject_2_sobject", error);
		slist_good:
			object = S_OBJECT(list);

			return object;
		}


		/* Differs from speect.sobject_2_pyobject
		 * Adds support for Python object
		 *
		 * Not a simple object, make a SPyObject SVoid type
		 * Needs speect.pyobject module to work.
		 *
		 */
		object = SObjectSetVoid("SPyObject", (void*)pobject, error);
		if (*error != S_SUCCESS)
			return NULL;

		return object;
	}


/*
 * Do not delete this delimiter, required for SWIG
 */
%}


