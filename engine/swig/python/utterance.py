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
## AUTHOR  : Richard Carlson, Aby Louw                                              ##
## DATE    : November 2009                                                          ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Python class for SUtterance.                                                     ##
##                                                                                  ##
##                                                                                  ##
######################################################################################



%pythoncode
%{

class SUtterance(SObject):
    """
    The utterance class implementation.

    A utterance consists of a set of L{SRelation} which in
    turn consists of a set of L{SItem}.

    Utterances have relations and features that are accessed as dictionary
    types. For example: C{self.features[\"name\"]} to get the utterance's \"name\"
    feature, and C{self.features[\"number\"] = speect.SInt(31)} to set the
    \"number\" feature of the utterance to a Speect L{SInt} object with a value of 31.
    """

    
    def __init__(self, voice=None, object=None, owner=False):
        """
        Construct a new SUtterance object. The input can either be:
           -   a voice (optional, default=I{None}), or a
           -   C Speect SUtterance type SObject and ownership status
               flag (default=I{False}).
        @param voice: The voice to which the utterance belongs (default = I{None}).
        @type voice: L{SVoice}
        @param object: A pointer to a C-type Speect SUtterance type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SUtterance is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: The newly created utterance.
        @rtype: L{SUtterance}
        """

        self._voice = None

        if not object:
            voice_object = None

            if voice:
                # test that it is an Python SVoice object
                if not isinstance(utt, speect.SVoice):
                    raise TypeError('Input argument \"voice\" must be ' +
                                    'of type \'SVoice\'')

                self._voice = voice
                voice_object = voice._get_speect_object()

            utterance_object = py_sutterance_new(voice_object)
            if utterance_object:
                super(SUtterance, self).__init__(utterance_object, True)
            else:
                raise RuntimeError('Failed to create new Speect SUtterance object')

        else :
            if not py_sobject_is_type(object, "SUtterance"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SUtterance\'')
            else:
                super(SUtterance, self).__init__(object, owner)

        utt_object = self._get_speect_object()

        # utterance features
        spct_utt_feats_object = py_sutterance_features(utt_object)
        self.features = SMapInternal(owner_object=utt_object,
                                     spct_map_object=spct_utt_feats_object,
                                     get_item_func=py_sutterance_get_feature,
                                     set_item_func=py_sutterance_set_feature,
                                     del_item_func=py_sutterance_del_feature)

        # utterance relations
        spct_utt_relations_object = py_sutterance_relations(utt_object)
        self.relations = SMapInternal(owner_object=utt_object,
                                      spct_map_object=spct_utt_relations_object,
                                      get_item_func=py_sutterance_get_relation,
                                      set_item_func=py_sutterance_set_relation,
                                      del_item_func=py_sutterance_del_relation)

        
    def __str__(self):
        """
        Overloaded 'print' method
        @returns: String representation of utterance object.
        @rtype: str
        """
        
        str = "Utterance:\n"
        for f in self.features:
            str += '    Feature: %20.20s => %s\n' %(f, repr(self.features[f]))

        for r in self.relations:
            str += self.relations[r].to_string(prefix="        ")
                       
        return str

        
    def get_voice(self):
        """
        Return the voice of this utterance.
        @return: The voice of this utterance or C{None} if no voice.
        @rtype: L{SVoice}
        """
        
        return self._voice
%}
