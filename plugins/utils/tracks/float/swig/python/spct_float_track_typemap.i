%typemap(in) (const float *times, uint32 len)
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


%typemap(in) (const float **fm, uint32 row_count, uint32 col_count)
{
	int i;
	int j;
	int row_count;
	int col_count;
	float **fpp;


	if (PyList_Check($input))
	{
		row_count = PyList_Size($input);
		fpp = S_MALLOC(float*, row_count);
		if (fpp == NULL)
		{
			PyErr_SetString(PyExc_RuntimeError,
							"Failed to allocate memory for 'float**' object");
			return NULL;
		}

		for (i = 0; i < row_count; i++)
		{
			PyObject *o = PyList_GetItem($input,i);
			if (PyList_Check(o))
			{
				col_count = PyList_Size(o);
				fpp[i] = S_MALLOC(float, col_count);
				if (fpp[i] == NULL)
				{
					PyErr_SetString(PyExc_RuntimeError,
									"Failed to allocate memory for 'float*' object");
					S_FREE(fpp); /* there will be some loss */
					return NULL;
				}

				for (j = 0; j < col_count; j++)
				{
					PyObject *p = PyList_GetItem(o,j);
					if (PyFloat_Check(p))
					{
						fpp[i][j] = (float)PyFloat_AsDouble(p);
					}
					else
					{
						PyErr_SetString(PyExc_TypeError,
										"not a float");
						S_FREE(fpp); /* there will be some loss */
						return NULL;
					}
				}
			}
			else
			{
				PyErr_SetString(PyExc_TypeError, "not a list of lists");
				S_FREE(fpp); /* there will be some loss */
				return NULL;
			}
		}
	}
	else
	{
		PyErr_SetString(PyExc_TypeError,"not a list");
		return NULL;
	}

	$1 = fpp;
	$2 = row_count;
	$3 = col_count;
}


%typemap(out) float_track_t
{
    float_track_t ft;
    int i;
	int j;
    PyObject *o;
	PyObject *times;
	PyObject *matrix;


    ft = $1;

	$result = PyTuple_New(2);
	times = PyList_New(ft.row_count);
	PyTuple_SET_ITEM($result, 0, times);

    for (i = 0; i < ft.row_count; i++)
    {
		PyList_SetItem(times, i, PyFloat_FromDouble((double)ft.times[i]));
    }


    matrix = PyList_New(ft.row_count);
	PyTuple_SET_ITEM($result, 1, matrix);
    for (i = 0; i < ft.row_count; i++)
    {
		o = PyList_New(ft.col_count);
		for (j = 0; j < ft.col_count; j++)
		{
			PyList_SetItem(o, j ,PyFloat_FromDouble((double)ft.fpp[i][j]));
		}
		PyList_SetItem(matrix, i, o);
    }
}
