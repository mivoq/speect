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


%typemap(out) float_matrix_t
{
    float_matrix_t fm;
    int i;
	int j;
    PyObject *o;

    fm = $1;

    $result = PyList_New(fm.row_count);
    for (i = 0; i < fm.row_count; i++)
    {
		o = PyList_New(fm.col_count);
		for (j = 0; j < fm.col_count; j++)
		{
			PyList_SetItem(o,j,PyFloat_FromDouble((double)fm.fpp[i][j]));
		}
		PyList_SetItem($result,i,o);
    }
}
