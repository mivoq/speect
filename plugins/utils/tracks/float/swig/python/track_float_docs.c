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
/* Python documentation strings for STrackFloat.                                    */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%define track_float_DOCSTRING
"""
STrackFloat(times, mlist)

A native Speect class for containing an track of floating point values. A track
is a class for storing time aligned coefficients. Each row of a track
corresponds to a specific time instant. A column of a track is known as a
channel, while track rows are knows as frames, for example:

+-------------+---------+------------+------------+------------+
|             | Times   | Channel 0  | Channel 1  | Channel 2  |
+=============+=========+============+============+============+
| **Frame 1** | 0.131   | 0.2        | 2.1        | 8.6        |
+-------------+---------+------------+------------+------------+
| **Frame 2** | 0.254   | 5.22       | 675.32     | 12.55      |
+-------------+---------+------------+------------+------------+
| **Frame 3** | 0.428   | 0.453      | 0.54       | 9.6        |
+-------------+---------+------------+------------+------------+

Create a new object, that contains an track of floating point point values.

The call to create the above track one would be::

    track = speect.track_float.STrackFloat([0.131, 0.254, 0.428], [[0.2, 2.1, 8.6], [5.22, 675.32, 12.55], [0.453, 0.54, 9.6]])

:param times: A Python list of the time values, floating point.
:type times: list
:param mlist: A Python list of lists containing only floating point values (frames and channels).
:type mlist: list
:return: Newly created STrackFloat object.
:rtype: STrackFloat
"""
%enddef

%feature("autodoc", track_float_DOCSTRING) STrackFloat;


%define get_DOCSTRING
"""
get()

Return a copy of this STrackFloat object as a Python tuple, with the first
element being a list of the track times, and the second element being
the frame and channel values.

:return: A tuple, with the first element being a list of the track times, and the second element being the frame and channel values.
:rtype: tuple
"""
%enddef

%feature("autodoc", get_DOCSTRING) STrackFloat::get;


%define row_count_DOCSTRING
"""
Return a count of the number of rows (frames) in the STrackFloat object.

:return: Number of rows (frames) in the STrackFloat object.
:rtype: int
"""
%enddef

%feature("autodoc", row_count_DOCSTRING) row_count;


%define col_count_DOCSTRING
"""
Return a count of the number of columns (channels) in the STrackFloat object.

:return: Number of columns (channels) in the STrackFloat object.
:rtype: int
"""
%enddef

%feature("autodoc", col_count_DOCSTRING) col_count;

