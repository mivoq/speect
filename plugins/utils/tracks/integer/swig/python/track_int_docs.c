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
/* DATE    : February 2010                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Python documentation strings for STrackInt.                                      */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%define track_int_DOCSTRING
"""
STrackInt(times, mlist)

A native Speect class for containing an track of integer values. A track
is a class for storing time aligned coefficients. Each row of a track
corresponds to a specific time instant. A column of a track is known as a
channel, while track rows are knows as frames, for example:

+-------------+---------+------------+------------+------------+
|             | Times   | Channel 0  | Channel 1  | Channel 2  |
+=============+=========+============+============+============+
| **Frame 1** | 0.131   | 12         | 2          | 8          |
+-------------+---------+------------+------------+------------+
| **Frame 2** | 0.254   | 52         | 201        | 43         |
+-------------+---------+------------+------------+------------+
| **Frame 3** | 0.428   | 21         | 134        | 78         |
+-------------+---------+------------+------------+------------+

Create a new object, that contains an track of integer values. Note
that the time values are still floating point.

The call to create the above track one would be::

    track = speect.track_int.STrackInt([0.131, 0.254, 0.428], [[12, 2, 8], [52, 201, 43], [21, 134, 78]])

:param times: A Python list of the time values, floating point.
:type times: list
:param mlist: A Python list of lists containing only integer values (frames and channels).
:type mlist: list
:return: Newly created STrackInt object.
:rtype: STrackInt
"""
%enddef

%feature("autodoc", track_int_DOCSTRING) STrackInt;


%define get_DOCSTRING
"""
get()

Return a copy of this STrackInt object as a Python tuple, with the first
element being a list of the track times, and the second element being
the frame and channel values.

:return: A tuple, with the first element being a list of the track times, and the second element being the frame and channel values.
:rtype: tuple
"""
%enddef

%feature("autodoc", get_DOCSTRING) STrackInt::get;


%define row_count_DOCSTRING
"""
Return a count of the number of rows (frames) in the STrackInt object.

:return: Number of rows (frames) in the STrackInt object.
:rtype: int
"""
%enddef

%feature("autodoc", row_count_DOCSTRING) row_count;


%define col_count_DOCSTRING
"""
Return a count of the number of columns (channels) in the STrackInt object.

:return: Number of columns (channels) in the STrackInt object.
:rtype: int
"""
%enddef

%feature("autodoc", col_count_DOCSTRING) col_count;

