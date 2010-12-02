/************************************************************************************/
/* LICENSE INFO                                                                     */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* AUTHOR  :                                                                        */
/* DATE    :                                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Description, ...                                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/* see TODO:  add documentation for Python class from
 * swig/common/source_common.c and swig/python/source_python.c, see
 * examples below
 */


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

