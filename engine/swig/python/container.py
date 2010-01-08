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
## Python class for SContainer.                                                     ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


%pythoncode
%{

class SContainer(SObject):
    """
    An abstract data container class for C Speect SObjects. All container type
    classes can inherit from this class, and implement the container type specific
    functions. L{SIterator} is the default iterator for the SContainer class,
    sub-classes that want to use SIterator must define C{iterator_new_func} and
    C{iterator_get_item_func} attributes. See L{SIterator}.
    """

    # SContainer class
    def __init__(self, object, owner=False):
        """
        Construct a new SContainer object.
        @param object: A pointer to a C-type Speect SContainer type
        SObject.
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SContainer is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: A new SContainer instance.
        @rtype: L{SContainer}
        """

        if not py_sobject_is_type(object, "SContainer"):
            raise TypeError('Input argument \"object\" must be ' +
                            'of type \'C Speect SContainer\'')
        else:
            super(SContainer, self).__init__(object, owner)


    def __iter__(self):
        """
        Underlying implementation of the C{__iter__} Python function for
        the L{SContainer} container.
        @return: A forward iterator to the L{SContainer} container.
        @rtype: L{SIterator}
        """

        return SIterator(self, forward=True)


    def __reversed__(self):
        """
        Underlying implementation of the C{__reversed__} Python function for
        the L{SContainer} container.
        @return: A reverse iterator to the L{SContainer} container.
        @rtype: L{SIterator}
        """
        
        return SIterator(self, forward=False)
%}
