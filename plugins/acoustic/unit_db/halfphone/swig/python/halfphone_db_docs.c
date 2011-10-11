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
/* Python documentation strings for SHalfphoneDB.                                   */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%define halfphone_db_DOCSTRING
"""
A halfphone sized unit selection database class.
"""
%enddef

%feature("autodoc", halfphone_db_DOCSTRING) SHalfphoneDB;


%define halfphone_db_name_DOCSTRING
"""
name()

Get the name of the database.

:return: The database name.
:rtype: string
"""
%enddef

%feature("autodoc", halfphone_db_name_DOCSTRING) SHalfphoneDB::name;


%define halfphone_db_description_DOCSTRING
"""
description()

Get a description of the database.

:return: The database description.
:rtype: string
"""
%enddef

%feature("autodoc", halfphone_db_description_DOCSTRING) SHalfphoneDB::description;


%define halfphone_db_language_DOCSTRING
"""
language()

Get the database language.

:return: The database language.
:rtype: string
"""
%enddef

%feature("autodoc", halfphone_db_language_DOCSTRING) SHalfphoneDB::language;


%define halfphone_db_lang_code_DOCSTRING
"""
lang_code()

Get the database's ``ISO 639-2`` language code.

:return: The ``ISO 639-2`` language code of the database.
:rtype: string
"""
%enddef

%feature("autodoc", halfphone_db_lang_code_DOCSTRING) SHalfphoneDB::lang_code;


%define halfphone_db_version_DOCSTRING
"""
version()

Get the database's version.

:return: The version of the database (major, minor).
:rtype: tuple
"""
%enddef

%feature("autodoc", halfphone_db_version_DOCSTRING) SHalfphoneDB::version;


%define halfphone_db_features_DOCSTRING
"""
Get the features that are defined for the database.

:return: A map of the database features.
:rtype: SMap
"""
%enddef

%feature("autodoc", halfphone_db_features_DOCSTRING) features;
