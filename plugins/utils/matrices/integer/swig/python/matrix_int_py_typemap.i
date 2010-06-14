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
/* Typemaps to convert types between Python list of lists of floats                 */
/* and SMatrixInt.                                                                  */
/*                                                                                  */
/************************************************************************************/


%typemap(in) (const float **fm, uint32 row_count, uint32 col_count)
{
	int i;
	int j;
	int row_count;
	int col_count;
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


%typemap(out) int_matrix_t
{
    int_matrix_t im;
    int i;
	int j;
    PyObject *o;

    im = $1;

    $result = PyList_New(im.row_count);
    for (i = 0; i < im.row_count; i++)
    {
		o = PyList_New(im.col_count);
		for (j = 0; j < im.col_count; j++)
		{
			PyList_SetItem(o,j,PyInt_FromLong((long)im.ipp[i][j]));
		}
		PyList_SetItem($result,i,o);
    }
}
