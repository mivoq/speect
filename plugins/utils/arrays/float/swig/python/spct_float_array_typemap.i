%typemap(in) (const float *array, uint32 len)
{
	int i;
	int count;
	float *fp;


	if (PyList_Check($input))
	{
		count = PyList_Size($input);
		fp = S_MALLOC(float, count);
		if (fp == NULL)
		{
			PyErr_SetString(PyExc_RuntimeError,
							"Failed to allocate memory for 'float*' object");
			return NULL;
		}

		for (i = 0; i < count; i++)
		{
			PyObject *o = PyList_GetItem($input,i);
			if (PyFloat_Check(o))
			{
				fp[i] = PyFloat_AsDouble(o);
			}
			else
			{
				PyErr_SetString(PyExc_TypeError,
								"not a float");
				S_FREE(fp);
				return NULL;
			}
		}
	}
	else
	{
		PyErr_SetString(PyExc_TypeError,"not a list");
		return NULL;
	}
	$1 = fp;
	$2 = count;
 }


%typemap(out) float_array_t
{
	float_array_t fa;
	int i;
	PyObject *o;

	fa = $1;

	$result = PyList_New(fa.fa_count);
	for (i = 0; i < fa.fa_count; i++)
	{
		o = PyFloat_FromDouble((double)fa.fa_ip[i]);
		PyList_SetItem($result,i,o);
	}
 }
