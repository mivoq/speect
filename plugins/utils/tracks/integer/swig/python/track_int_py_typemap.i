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
/* DATE    : February 2010                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Typemaps to convert types between Python track representation and STrackInt.     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


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


%typemap(in) (const sint32 **im, uint32 row_count, uint32 col_count)
{
	int i;
	int j;
	int row_count = 0;
	int col_count = 0;
	sint32 **ipp;


	if (PyList_Check($input))
	{
		row_count = PyList_Size($input);
		ipp = S_MALLOC(sint32*, row_count);
		if (ipp == NULL)
		{
			PyErr_SetString(PyExc_RuntimeError,
							"Failed to allocate memory for 'sint32**' object");
			return NULL;
		}

		for (i = 0; i < row_count; i++)
		{
			PyObject *o = PyList_GetItem($input,i);
			if (PyList_Check(o))
			{
				col_count = PyList_Size(o);
				ipp[i] = S_MALLOC(sint32, col_count);
				if (ipp[i] == NULL)
				{
					PyErr_SetString(PyExc_RuntimeError,
									"Failed to allocate memory for 'sint32*' object");
					S_FREE(ipp); /* there will be some loss */
					return NULL;
				}

				for (j = 0; j < col_count; j++)
				{
					PyObject *p = PyList_GetItem(o,j);
					if (PyInt_Check(p))
					{
						ipp[i][j] = (sint32)PyInt_AsLong(p);
					}
					else
					{
						PyErr_SetString(PyExc_TypeError,
										"not an integer");
						S_FREE(ipp); /* there will be some loss */
						return NULL;
					}
				}
			}
			else
			{
				PyErr_SetString(PyExc_TypeError, "not a list of lists");
				S_FREE(ipp); /* there will be some loss */
				return NULL;
			}
		}
	}
	else
	{
		PyErr_SetString(PyExc_TypeError,"not a list");
		return NULL;
	}

	$1 = ipp;
	$2 = row_count;
	$3 = col_count;
}


%typemap(out) int_track_t
{
    int_track_t it;
    int i;
	int j;
    PyObject *o;
	PyObject *times;
	PyObject *matrix;


    it = $1;

	$result = PyTuple_New(2);
	times = PyList_New(it.row_count);
	PyTuple_SET_ITEM($result, 0, times);

    for (i = 0; i < it.row_count; i++)
    {
		PyList_SetItem(times, i, PyFloat_FromDouble((double)it.times[i]));
    }


    matrix = PyList_New(it.row_count);
	PyTuple_SET_ITEM($result, 1, matrix);
    for (i = 0; i < it.row_count; i++)
    {
		o = PyList_New(it.col_count);
		for (j = 0; j < it.col_count; j++)
		{
			PyList_SetItem(o, j ,PyInt_FromLong((long)it.ipp[i][j]));
		}
		PyList_SetItem(matrix, i, o);
    }
}
