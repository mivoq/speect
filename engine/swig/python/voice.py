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
## DATE    : November 2009                                                          ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Python class for SVoice.                                                         ##
##                                                                                  ##
##                                                                                  ##
######################################################################################



%pythoncode
%{

class SVoice(SObject):
    """
    The voice class implementation.
    @group Info: get_name, get_description, get_gender,
    get_language, get_lang_code, get_version
    @group Synthesis: synth
    """

    
    def __init__(self, file_name=None, load_data=True, object=None, owner=False):
        """
        Construct a new SVoice object. The input can either be:
           -   a voice configuration file path (JSON-format) and a load data flag
               (to load the voice data, default=I{True}), or a
           -   C Speect SVoice type SObject and ownership status
               flag (default=I{False}).
        
        @param file_name: The full path to the voice configuration file.
        @type file_name: str
        @param load_data: Flag to specify if the voice data must be loaded or
        not (default=I{True}).
        @type load_data: bool
        @param object: A pointer to a C-type Speect SVoice type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SVoice is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: The newly created voice.
        @rtype: L{SVoice}
        """

        self._voice = None

        if not object:
            if not isinstance(file_name, str):
                raise TypeError("file_name must be of type 'str'")

            if not isinstance(load_data, bool):
                raise TypeError("load_data must be of type 'bool'")

            voice_object = py_svoice_load(file_name, load_data)
            if voice_object:
                super(SVoice, self).__init__(voice_object, True)
            else:
                raise RuntimeError('Failed to create Speect SVoice object')

        else :
            if not py_sobject_is_type(object, "SVoice"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SVoice\'')
            else:
                super(SVoice, self).__init__(object, owner)

        
    def get_name(self):
        """
        Get the voice's name.
        @return: The voice's name.
        @rtype: unicode
        """
        
        return py_svoice_get_name(self._get_speect_object())


    def get_description(self):
        """
        Get the voice's description.
        @return: The voice's description.
        @rtype: unicode
        """
        
        return py_svoice_get_description(self._get_speect_object())


    def get_gender(self):
        """
        Get the voice's gender.
        @return: The voice's gender.
        @rtype: unicode
        """
        
        return py_svoice_get_gender(self._get_speect_object())


    def get_language(self):
        """
        Get the voice's language.
        @return: The voice's language.
        @rtype: unicode
        """
        
        return py_svoice_get_language(self._get_speect_object())


    def get_lang_code(self):
        """
        Get the voice's I{ISO 639-2} language code. 
        @return: The voice's language code.
        @rtype: unicode
        """
        
        return py_svoice_get_lang_code(self._get_speect_object())


    def get_version(self):
        """
        Get the voice's version.
        @return: The voice's version C{(major, minor)}.
        @rtype: tuple
        """
        
        return py_svoice_get_version(self._get_speect_object())


    def synth(self, utt_type, input):
        """
        Synthesize an utterance with the given utterance type and input.
        @param utt_type: The utterance type to apply for synthesis.
        @type utt_type: str or unicode
        @param input: A Python L{SObject} instance input that is appropriate
        for the given utterance type.
        @return: The synthesized utterance.
        @rtype: L{SUtterance}
        """

        if not isinstance(utt_type, str) and not isinstance(utt_type, unicode):
            raise TypeError("utt_type must be of type 'str' or 'unicode'")

        # input must be a Python SObject instance
        if not is_py_sobject(input):
            raise TypeError('Input argument \"input\" is not an ' +
                            'instance of type \'SObject\'')

        # utterance will take hold of input, disown it.
        input._disown()

        utt_object = py_svoice_synth_utt(self._get_speect_object(),
                                         utt_type, input._get_speect_object())
        if utt_object:
            return create_py_sobject(object=utt_object, owner=True)
        else:
            raise RuntimeError("Failed to synthesize utterance")
%}
