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
/* DATE    : June 2010                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Native interfaces for Speect <-> Python objects.                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "py_native.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static sint initialized = 0;


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_erc s_python_native_objects_init(void)
{
	s_erc local_err;

	S_CLR_ERR(&local_err);

	if (initialized++ == 1)
		return S_SUCCESS;

	_s_py_funcs_initialize(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "s_python_native_objects_init",
				  "Failed to initialize Python functions"))
		return local_err;

	_s_pyobject_class_reg(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "s_python_native_objects_init",
				  "Failed to initialize SPyObject"))
		return local_err;

	_s_list_py_class_reg(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "s_python_native_objects_init",
				  "Failed to initialize SListPy"))
		return local_err;

	_s_list_py_iterator_class_reg(&local_err);
	if (S_CHK_ERR(&local_err, S_CONTERR,
				  "s_python_native_objects_init",
				  "Failed to initialize SListPyIterator"))
		return local_err;

	return S_SUCCESS;
}

