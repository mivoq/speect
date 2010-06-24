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
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Python documentation strings for SAudio.                                         */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%define audio_DOCSTRING
"""
A container class for audio objects. Provides a Speect native class for
creating/adding audio objects. The audio samples are internally represented as floats.
"""
%enddef

%feature("autodoc", audio_DOCSTRING) SAudio;


%define audio_num_samples_DOCSTRING
"""
num_samples()

Return the number of samples of this audio object.

:return: Number of samples of this audio object.
:rtype: int
"""
%enddef

%feature("autodoc", audio_num_samples_DOCSTRING) SAudio::num_samples;


%define audio_sample_rate_DOCSTRING
"""
sample_rate()

Return the sample rate of the samples of this audio object.

:return: Sample rate of this audio object (Hertz).
:rtype: int
"""
%enddef

%feature("autodoc", audio_sample_rate_DOCSTRING) SAudio::sample_rate;


%define audio_resize_DOCSTRING
"""
resize(new_size)

Resize (number of samples) the audio object to the given size.

:param new_size: The new size of the audio object (number of samples).
:type new_size: int
"""
%enddef

%feature("autodoc", audio_resize_DOCSTRING) SAudio::resize;


%define audio_scale_DOCSTRING
"""
scale(factor)

Scale the given audio object (samples) with the given factor.

:param factor: The factor with which to scale the samples.
:type factor: float
"""
%enddef

%feature("autodoc", audio_scale_DOCSTRING) SAudio::scale;

