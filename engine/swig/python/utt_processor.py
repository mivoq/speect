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
## Python class for SUttProcessor.                                                  ##
##                                                                                  ##
##                                                                                  ##
######################################################################################



%pythoncode
%{

class SUttProcessor(SObject):
    """
    The utterance processor class implementation.

    Utterance processors have features that are accessed as dictionary
    types. For example: C{self.features[\"name\"]} to get the utterance's \"name\"
    feature, and C{self.features[\"number\"] = speect.SInt(31)} to set the
    \"number\" feature of the utterance to a Speect L{SInt} object with a value of 31.
    """

    
    def __init__(self, class_name=None, object=None, owner=False):
        """
        @param class_name: An utterance processor class from which to create
        an utterance processor.
        @type class_name: str
        @param object: A pointer to a C-type Speect SUttProcessor type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SUttProcessor is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: The newly created utterance processor.
        @rtype: L{SUttProcessor}
        """

        if not class_name and not object:
            raise RuntimeError("Either \'class_name\' or \'object\' must not be None")

        if class_name:
            if not isinstance(class_name, str):
                raise RuntimeError("\'class_name\' must be of type \'str\'")
            
            utt_proc_object = py_suttproc_create(class_name)
            if not utt_proc_object:
                string = "Speect failed to create utterance processor of type \'" +\
                         class_name + "\'"
                raise RuntimeError(string)

            super(SUttProcessor, self).__init__(object=utt_proc_object, owner=True)
        else:
            if not py_sobject_is_type(object, "SUttProcessor"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SUttProcessor\'')
            else:
                super(SUttProcessor, self).__init__(object, owner)

        utt_proc_object = self._get_speect_object()

        # utterance processor features
        spct_utt_proc_feats_object = py_suttproc_features(utt_proc_object)
        self.features = SMapInternal(owner_object=utt_proc_object,
                                     spct_map_object=spct_utt_proc_feats_object,
                                     get_item_func=py_suttproc_get_feature,
                                     set_item_func=py_suttproc_set_feature,
                                     del_item_func=py_suttproc_del_feature)


    def run(self, utterance):
        """
        Run the utterance processor on the given utterance.
        @param utterance: The utterance which to process.
        @type utterance: L{SUtterance}
        """
        
        if not isinstance(utterance, SUtterance):
            raise TypeError('Input argument \"utterance\" must be ' +
                            'of type \'SUtterance\'')
        
        py_suttproc_run(self._get_speect_object(), utterance._get_speect_object())
      
%}
