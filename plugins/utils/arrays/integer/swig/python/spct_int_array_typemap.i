%typemap(in) (const sint32 *array, uint32 len)
{
	int i;
	int count;
	sint32 *ip;


	if (PyList_Check($input))
	{
		count = PyList_Size($input);
		ip = S_MALLOC(sint32, count);
		if (ip == NULL)
		{
			PyErr_SetString(PyExc_RuntimeError,
							"Failed to allocate memory for 'sint32*' object");
			return NULL;
		}

		for (i = 0; i < count; i++)
		{
			PyObject *o = PyList_GetItem($input,i);
			if (PyInt_Check(o))
			{
				ip[i] = (sint32)PyLong_AsLong(o);
			}
			else
			{
				PyErr_SetString(PyExc_TypeError,
								"not an integer");
				S_FREE(ip);
				return NULL;
			}
		}
	}
	else
	{
		PyErr_SetString(PyExc_TypeError,"not a list");
		return NULL;
	}
	$1 = ip;
	$2 = count;
 }


%typemap(out) int_array_t
{
	int_array_t ia;
	int i;
	PyObject *o;

	ia = $1;

	$result = PyList_New(ia.ia_count);
	for (i = 0; i < ia.ia_count; i++)
	{
		o = PyInt_FromLong((long)ia.ia_ip[i]);
		PyList_SetItem($result,i,o);
	}
 }
