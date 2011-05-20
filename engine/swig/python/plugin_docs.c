/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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
/* Python documentation strings for SPlugin.                                        */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%define plugin_ctor_DOCSTRING
"""
SPlugin(path)

Plug-in objects encapsulate the loading/unloading and resource acquisition
of *dynamic shared objects*.

Load a plug-in from the given path. If the plug-in at the given
path has already been loaded, then the plug-in's reference counter
is increased and a pointer to the loaded plug-in is returned. This
reduces the need for multiple redundant calls.

If the given path does not include any path separators (just a file name)
then the path is concatenated with the default plug-in path.

:param path: The full path and name of the plug-in to load.
:type path: string
:return: Plug-in object
:rtype: Swig Object of type SPlugin
:raises: RuntimeError if Speect was unable to load the plug-in.
"""
%enddef

%feature("autodoc", plugin_ctor_DOCSTRING) SPlugin;
