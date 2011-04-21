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
/* DATE    : November 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Python documentation strings for miscellaneous Speect functions.                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%feature("autodoc", "Request a graceful shutdown of the Speect Engine.") quit;

%define version_DOCSTRING
"""
Return the Speect Engine library version in a tuple, (major version number, minor
version number, patch number, release name).

:rtype: tuple
"""
%enddef

%feature("autodoc", version_DOCSTRING) version;


%define setDebugLevel_DOCSTRING
"""
setDebugLevel(level)

Set the debugging level of the Speect Engine logging system.
The levels are:
    :S_DBG_NONE:   0, No debugging output.
	:S_DBG_INFO:   1, Basic debug level.
	:S_DBG_TRACE:  2, Verbose debug level.
	:S_DBG_ALL:    3, All debugging logged.

:param v: The debugging level.
"""
%enddef

%feature("autodoc", setDebugLevel_DOCSTRING) setDebugLevel;


%define getDebugLevel_DOCSTRING
"""
Get the debugging level of the Speect Engine logging system.
The levels are:
    :S_DBG_NONE:   0, No debugging output.
	:S_DBG_INFO:   1, Basic debug level.
	:S_DBG_TRACE:  2, Verbose debug level.
	:S_DBG_ALL:    3, All debugging logged.

:rtype: int
"""
%enddef

%feature("autodoc", getDebugLevel_DOCSTRING) getDebugLevel;
