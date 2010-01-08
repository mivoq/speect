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
## Python class for SIterator.                                                      ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


%pythoncode
%{
    class SIterator(SObject):
        """
        An iterator for L{SContainer}.
        Implements the Python iterator protocol.
        """


        def __init__(self, container, forward):
            """
            Constructor.
            Initialized with an instance of L{SContainer}.
            Expects the following functions to be attributes of the
            L{SContainer} object:
               - C{iterator_new_func}, create a new iterator for the container.
                 Takes as input the C Speect SContainer object and returns a C
                 Speect SIterator object.
               - C{iterator_get_item_func}, get the item of the iterator from the container.
                 Takes as input the C Speect SIterator object and returns a
                 Python L{SObject} instance or sub-class thereof.
            @param container: The container over which to iterate.
            @type container: L{SContainer}
            @param forward: I{True} if the iterator direction is forward over the
            container items.
            @type forward: bool
            @return: An iterator implemantation of the Python iterator protocol
            for an L{SContainer} type container.
            @rtype: L{SIterator}
            """

            if not isinstance(container, SContainer):
                raise TypeError('Input argument \"container\" is not an ' +
                                'instance of type \'SContainer\'')

            if not isinstance(forward, bool):
                raise TypeError('Input argument \"forward\" is not an ' +
                                'instance of type \'bool\'')

            # get the function to create a new C Speect SIterator object
            try:
                iterator_new_func = container.__getattribute__('new_iterator')
            except AttributeError:
                raise AttributeError("'new_iterator' function not implemented for given container")

            # get the function to get an item from the C Speect SContainer
            try:
                self.iterator_get_item_func = container.__getattribute__('iterator_get_item')
            except AttributeError:
                raise AttributeError("'iterator_get_item' function not implemented for given container")

            # create a new iterator
            iterator_object = iterator_new_func(container._get_speect_object())
            self._forward = forward

            # initialize the iterator direction
            if iterator_object:
                if forward:
                    iterator_object = py_siterator_first(iterator_object)
                else:
                    iterator_object = py_siterator_last(iterator_object)
                super(SIterator, self).__init__(iterator_object, True)
                self._fin = 0
            else:
                self._fin = 1
                self._disown()
                self._container_object = None
                self.__spct_object__ = None


        def __iter__(self):
            """
            Return the iterator itself (part of the Python iterator protocol).
            @return: self
            @rtype: L{SIterator}
            """

            return self


        def next(self):
            """
            Return the next item in the container (part of the Python iterator protocol),
            raising the C{StopIteration} if at the end of the container.
            @return: The next item in the container.
            """

            if self._fin == 1:
                raise StopIteration

            iterator_object = self._get_speect_object()
            if not iterator_object:
                raise StopIteration
            
            item = self.iterator_get_item_func(iterator_object)
            
            if not item:
                self._fin = 1
                self._disown()
                self.__spct_object__ = None
                raise StopIteration

            if self._forward:
                next_itr = py_siterator_next(iterator_object)
            else:
                next_itr = py_siterator_prev(iterator_object)
                       
            if not next_itr:
                self._fin = 1
                self._disown()
                self.__spct_object__ = None
            else:
                self.__spct_object__ = next_itr

            return item
%}
