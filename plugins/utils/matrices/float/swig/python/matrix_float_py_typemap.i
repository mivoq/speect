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
/* and SMatrixFloat.                                                                */
/*                                                                                  */
/************************************************************************************/


%typemap(in) (const float **fm, uint32 row_count, uint32 col_count)
{
	int i;
	int j;
	int row_count = 0;
	int col_count = 0;
	float **fpp;


	if (PyList_Check($input))
	{
		row_count = PyList_Size($input);
		fpp = S_MALLOC(float*, row_count);
		if (fpp == NULL)
		{
			SWIG_exception_fail(SWIG_RuntimeError, "in method '" "$symname" "',	failed to allocate memory for 'float*' object");
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
					S_FREE(fpp); /* there will be some loss */
					SWIG_exception_fail(SWIG_RuntimeError, "in method '" "$symname" "',	failed to allocate memory for 'float' object");
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
						S_FREE(fpp); /* there will be some loss */
						SWIG_exception_fail(SWIG_TypeError, "in method '" "$symname" "',  element of list not a float");
					}
				}
			}
			else
			{
				S_FREE(fpp); /* there will be some loss */
				SWIG_exception_fail(SWIG_TypeError, "in method '" "$symname" "',  not a list of lists");
			}
		}
	}
	else
	{
		SWIG_exception_fail(SWIG_TypeError, "in method '" "$symname" "',  not a list");
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
