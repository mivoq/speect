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
## Python class for SFeatProcessor.                                                 ##
##                                                                                  ##
##                                                                                  ##
######################################################################################



%pythoncode
%{

class SFeatProcessor(SObject):
    """
    The feature processor class implementation.
    """

    
    def __init__(self, object, owner=False):
        """
        @param object: A pointer to a C-type Speect SFeatProcessor type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SFeatProcessor is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: The newly created feature processor.
        @rtype: L{SFeatProcessor}
        """

        if not py_sobject_is_type(object, "SFeatProcessor"):
            raise TypeError('Input argument \"object\" must be ' +
                            'of type \'C Speect SFeatProcessor\'')
        else:
            super(SFeatProcessor, self).__init__(object, owner)


    def run(self, item):
        """
        Run the feature processor on the given item.
        @param item: The item from which to extract features with the
        feature processor.
        @type item: L{SItem}
        @return: A Python L{SObject} or sub-class thereof feature object.
        """

        if not isinstance(item, SItem):
            raise TypeError('Input argument \"item\" must be ' +
                            'of type \'SItem\'')
        
        feat_object = py_sfeatproc_run(self._get_speect_object(),
                                       item._get_speect_object())
        if feat_object:
            return create_py_sobject(feat_object, True)
        else:
            raise RuntimeError("Failed to get feature object from feature processor")
        
%}
