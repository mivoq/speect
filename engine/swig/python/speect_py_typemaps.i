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
/* DATE    : March 2009                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Typemaps to convert types between Python and C, for the Speect Engine.           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/*** sint32 ***/
%typemap(in) sint32
{
	$1 = (sint32) PyLong_AsLong($input);
}

%typemap(out) sint32
{
	$result = PyInt_FromLong((long) $1);
}


/*** uint32 ***/
%typemap(in) uint32
{
	$1 = (uint32) PyLong_AsUnsignedLong($input);
}

%typemap(out) uint32
{
	$result = PyInt_FromLong((ulong) $1);
}


/*** s_bool ***/
%typemap(in) s_bool
{
	if ($input == Py_False)
		$1 = 0;
	else
		$1 = 1;
}

%typemap(out) s_bool
{
	if ($1 == 0)
		$result = Py_False;
	else
		$result = Py_True;
}


/*** const char ***/
/* %typemap(in) const char* */
/* { */
/*  	if (PyObject_IsInstance($input, (PyObject*)&PyString_Type)) */
/*  	{ */
/*  		$1 = PyString_AsString($input); */
/* 	} */
/* 	else */
/* 	{ */
/* 		PyObject *ustring; */


/* 		ustring = PyUnicode_AsUTF8String($input); */
/* 		$1 = PyString_AsString(ustring); */
/* 	} */
/* } */



/*** version ***/
%typemap(out) s_version*
{
	PyObject *versionTuple;
	PyObject *versionMajor;
	PyObject *versionMinor;
	s_version *output = $1;


	versionTuple = PyTuple_New(2);
	versionMajor = PyInt_FromLong((long)output->major);
	versionMinor = PyInt_FromLong((long)output->minor);

	PyTuple_SetItem(versionTuple, 0, versionMajor);
	PyTuple_SetItem(versionTuple, 1, versionMinor);

	$result = versionTuple;
}


