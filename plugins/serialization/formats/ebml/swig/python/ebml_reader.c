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
/* AUTHOR  : Richard Carlson, Aby Louw                                              */
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SEbmlRead Python wrapper.                            */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%apply unsigned int *INOUT { uint32 *id };

%inline
%{
	SObject *sebml_reader_read_object(SEbmlRead *self, uint32 *id, s_erc *error)
	{
		SObject *val;


		val = S_EBMLREAD_CALL(self, read_object)(self, id, error);
		if (*error != S_SUCCESS)
			return NULL;

		return val;
	}

	PyObject *ebml_sobject_2_pyobject(SObject *object, s_erc *error)
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

			S_DELETE(object, "ebml_sobject_2_pyobject", error);
			pobject = PyInt_FromLong((long)val);
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

			S_DELETE(object, "ebml_sobject_2_pyobject", error);
			pobject = PyFloat_FromDouble((double)val);
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

			S_DELETE(object, "ebml_sobject_2_pyobject", error);
			pobject = PyString_FromStringAndSize(val, slen);
			return pobject;
		}

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
			pobject = SWIG_NewPointerObj(SWIG_as_voidptr(object),
										 info, 1);
		}
		else
		{
			/* don't know this object type, use SObject */
			pobject = SWIG_NewPointerObj(SWIG_as_voidptr(object),
										 SWIGTYPE_p_SObject, 1);
		}

		return pobject;
	}
%}


typedef struct
{
	%extend
	{
		const int ebml_version;
		const int reader_version;
		const int max_id_width;
		const int max_size_width;
		const int doc_type_version;
		const int doc_type_reader_version;
	}
} SEbmlRead;


%types(SEbmlRead = SObject);


%extend SEbmlRead
{
	SEbmlRead(const char *path, s_erc *error)
	{
		SDatasource *ds;
		SEbmlRead *ebmlReader;


		/*
		 * create a data source, "rb" = read, binary
		 */
		ds = SFilesourceOpenFile(path, "rb", error);
		if (*error != S_SUCCESS)
			return NULL;

		/*
		 * create ebml reader object
		 */
		ebmlReader = (SEbmlRead*)S_NEW("SEbmlRead", error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(ds, "SEbmlRead()", error);
			return NULL;
		}

		/*
		 * initialize ebml reader object
		 */
		S_EBMLREAD_CALL(ebmlReader, read_init)(&ebmlReader, ds, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(ds, "SEbmlRead()", error);
			return NULL;
		}

		return ebmlReader;
	}

	const char *doctype(void)
	{
		return (const char*)$self->header->doctype;
	}

	~SEbmlRead()
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE($self, "~SEbmlRead()", &error);
	}


	uint32 peek_id(s_erc *error)
	{
		uint32 id;


		id = S_EBMLREAD_CALL($self, peek_id)($self, error);
		if (*error != S_SUCCESS)
			return 0;

		return id;
	}


	void skip_element(s_erc *error)
	{
		S_EBMLREAD_CALL($self, element_skip)($self, error);
	}


	PyObject *read_uint(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		uint32 val;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_uint)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_uint",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyInt_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);

		object = PyInt_FromLong((ulong) val);
		PyTuple_SET_ITEM(tuple, 1, object);

		return tuple;
	}


	PyObject *read_sint(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		sint32 val;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_sint)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_sint",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyInt_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);

		object = PyInt_FromLong((long) val);
		PyTuple_SET_ITEM(tuple, 1, object);

		return tuple;
	}


	PyObject *read_double(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		double val;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_double)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_double",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyInt_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);

		object = PyFloat_FromDouble(val);
		PyTuple_SET_ITEM(tuple, 1, object);

		return tuple;
	}


	PyObject *read_str(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		char *val;
		size_t len;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_utf8)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		len = s_strsize(val, error);
		if (*error != S_SUCCESS)
			return NULL;

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_str",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyInt_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);

		object = PyString_FromStringAndSize(val, len);
		PyTuple_SET_ITEM(tuple, 1, object);
		S_FREE(val);

		return tuple;
	}


	PyObject *read_object(s_erc *error)
	{
		PyObject *tuple;
		PyObject *object;
		SObject *val;
		PyObject *sobject;
		uint32 id;


		val = S_EBMLREAD_CALL($self, read_object)($self, &id, error);
		if (*error != S_SUCCESS)
			return NULL;

		sobject = ebml_sobject_2_pyobject(val, error);
		if (*error != S_SUCCESS)
		{
			S_DELETE(sobject, "SEbmlRead::read_object()", error);
			return NULL;
		}

		tuple = PyTuple_New(2);
		if (tuple == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "read_str",
					  "Call to \"PyTuple_New\" failed");
			return NULL;
		}

		object = PyInt_FromLong((ulong) id);
		PyTuple_SET_ITEM(tuple, 0, object);
		PyTuple_SET_ITEM(tuple, 1, sobject);


		return tuple;
	}


	uint32 open_container(s_erc *error)
	{
		uint32 id;


		id = S_EBMLREAD_CALL($self, container)($self, error);
		if (*error != S_SUCCESS)
			return 0;

		return id;
	}


	s_bool container_exhausted(s_erc *error)
	{
		s_bool query;


		query = S_EBMLREAD_CALL($self, container_at_end)($self, error);
		if (*error != S_SUCCESS)
			return FALSE;

		return query;
	}
}


%{
	const int SEbmlRead_ebml_version_get(SEbmlRead *self)
	{
		return (int)self->header->ebml_version;
	}

	const int SEbmlRead_reader_version_get(SEbmlRead *self)
	{
		return (int)self->header->ebml_read_version;
	}

	const int SEbmlRead_max_id_width_get(SEbmlRead *self)
	{
		return (int)self->header->max_id_width;
	}

	const int SEbmlRead_max_size_width_get(SEbmlRead *self)
	{
		return (int)self->header->max_size_width;
	}

	const int SEbmlRead_doc_type_version_get(SEbmlRead *self)
	{
		return (int)self->header->doctype_version;
	}

	const int SEbmlRead_doc_type_reader_version_get(SEbmlRead *self)
	{
		return (int)self->header->doctype_read_version;
	}
%}
