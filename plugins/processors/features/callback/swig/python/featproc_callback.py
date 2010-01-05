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
## Python class for SFeatProcessorCB Python callback feature processor.             ##
##                                                                                  ##
##                                                                                  ##
######################################################################################



%pythoncode
%{

class SFeatProcessorCB(speect.SFeatProcessor):
    """
    The Python callback feature processor class implementation.
    """

    
    def __init__(self, callback_func=None, object=None, owner=False):
        """
        Constructor.
        @param callback_func: A Python function that takes a Python object
        of type L{SItem} (get the SItem object with
        C{speect.SItem(object=item, owner=False)}) and returns a C Speect SObject
        feature.
        @param object: A pointer to a C-type Speect SFeatProcessorCB type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SFeatProcessorCB is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: The newly created feature processor.
        @rtype: L{SFeatProcessorCB}
        """

        if (callback_func and object) or (not callback_func and not object):
            raise RuntimeError('Either argument \"callback_func\" or '+\
                               '\"object\" must not be \'None\'')

        if callback_func:
            if not callable(callback_func):
                raise RuntimeError('Given callback function is not callable')

            feat_proc_object = py_sfeatproc_cb_new(callback_func)
            if not feat_proc_object:
                raise RuntimeError('Speect failed to create new \'SFeatProcessorCB\' object')

            super(SFeatProcessorCB, self).__init__(feat_proc_object, True)
        else:
            if not py_sobject_is_type(object, "SFeatProcessorCB"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SFeatProcessorCB\'')
            else:
                super(SFeatProcessorCB, self).__init__(object, owner)


    def __del__(self):
        """
        Destructor. Overloads L{SObject.__del__} to decrement the reference count
        of the callback function.
        """
        
        spct_object = self._get_speect_object()
        if self._owner() and spct_object:
            py_sfeatproc_cb_decrement_func(spct_object)

        # now call super's .__del__
        super(SFeatProcessorCB, self).__del__()

# register class with Speect Engine
speect.register_class(SFeatProcessorCB)

%}
