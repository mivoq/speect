######################################################################################
## Copyright (c) 2009 The Department of Arts and Culture,                           ##
## The Government of the Republic of South Africa.                                  ##
##                                                                                  ##
## Contributors:  Meraka Institute, CSIR, South Africa.                             ##
##                                                                                  ##
## Permission is hereby granted, free of charge, to any person obtaining a copy     ##
## of this software and associated documentation files (the "Software"), to deal    ##
## in the Software without restriction, including without limitation the rights     ##
## to use, copy, modify, merge, publish, distribute, sublicense, and#or sell        ##
## copies of the Software, and to permit persons to whom the Software is            ##
## furnished to do so, subject to the following conditions:                         ##
## The above copyright notice and this permission notice shall be included in       ##
## all copies or substantial portions of the Software.                              ##
##                                                                                  ##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       ##
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         ##
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      ##
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           ##
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    ##
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN        ##
## THE SOFTWARE.                                                                    ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## AUTHOR  : Aby Louw                                                               ##
## DATE    : December 2009                                                          ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Python class for SAudio.                                                         ##
##                                                                                  ##
##                                                                                  ##
######################################################################################



%pythoncode
%{

class SAudio(speect.SObject):
    """
    The Python class for access to utterance SAudio objects.
    """

    
    def __init__(self, object=None, owner=False):
        """
        Constructor.
        @param object: A pointer to a C-type Speect SAudio type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SAudio is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: The newly created audio object.
        @rtype: L{SAudio}
        """

        if not object:
            raise RuntimeError('Argument \"object\" must not be \'None\'')

        if not speect.py_sobject_is_type(object, "SAudio"):
            raise TypeError('Input argument \"object\" must be ' +
                            'of type \'C Speect SAudio\'')
        else:
            super(SAudio, self).__init__(object, owner)


    def num_samples(self):
        """
        Get the number of samples of the given SAudio object.
        @return: The number of samples of the audio object.
        @rtype: int
        """

        spct_object = self._get_speect_object()
        if spct_object:
            return py_saudio_num_samples(spct_object)
        return 0


    def sample_rate(self):
        """
        Get the sample rate of of the given SAudio object.
        @return: The sample rate of the audio object.
        @rtype: int
        """

        spct_object = self._get_speect_object()
        if spct_object:
            return py_saudio_sample_rate(spct_object)
        return 0

    
    def get_audio_waveform(self):
        """
        Return the audio waveform in Python dict  e.g.:
        {\"sampletype\": \"int16\", \"samplerate\": 16000,
        \"samples\": \"...bytestring...samples....\"}
        """
        sample_type = "int16"   # currently only option

        spct_object = self._get_speect_object()
        if spct_object:
            sample_rate = py_saudio_sample_rate(spct_object)
            samples = py_saudio_samples(spct_object)
            wave_dict = {
                "sampletype" : sample_type,
                "samplerate" : sample_rate,
                "samples" : samples
                }
            return wave_dict
        return None


# register class with Speect Engine
speect.register_class(SAudio)

%}
