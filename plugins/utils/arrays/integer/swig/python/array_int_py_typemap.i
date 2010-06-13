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
/* Typemaps to convert types between Python list of integers and SArrayInt.         */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


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
