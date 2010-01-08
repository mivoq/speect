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
## Python class for SList container.                                                ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


%pythoncode
%{

class SList(SContainer):
    """
    A list data container for C Speect SObjects. Emulates
    Python list behaviour, except for list slicing (C{self[i:j]}).
    L{SIterator} is the iterator for the SList class,
    sub-classes that want to use SIterator must define C{iterator_new_func} and
    C{iterator_get_item_func} attributes. See L{SIterator}.
    """


    # SList class
    def __init__(self, object=None, owner=False):
        """
        Construct a new SList object. If no input is given then a
        new C Speect SList (C{SListList}) is created, otherwise this
        class is an interface to an existing C Speect SList type SObject.
        @param object: A pointer to a C-type Speect SList type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SList is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: A new SList instance.
        @rtype: L{SList}
        """

        if object:
            if not py_sobject_is_type(object, "SList"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SList\'')
            else:
                super(SList, self).__init__(object, owner)
        else:
            new_spct_object = py_slist_new()
            if new_spct_object:
                # this is a new object, so this instance is
                # the owner for now
                super(SList, self).__init__(new_spct_object, True)
            else:
                raise RuntimeError('Failed to create new Speect SList object')

        # iterator functions
        self.new_iterator = py_slist_iterator_new
        self.iterator_get_item = self._iterator_get_item


    def _iterator_get_item(self, itr):
        """
        A convenience function to get a Python L{SObject} instance
        from a list item. Used by L{SIterator}.
        @param itr: C Speect SIterator object
        @return: Python L{SObject} instance of the list item pointer to the
        iterator object.
        """
        item_object = py_slist_iterator_get_item(itr)
        if item_object:
            return create_py_sobject(item_object, False)
        else:
            return None


    def __len__(self):
        """
        Underlying implementation of the C{len()} Python function for
        the L{SList} container.
        """

        spct_object = self._get_speect_object()
        if spct_object:
            return py_slist_len(spct_object)
        return 0


    def __repr__(self):
        """
        Return a string representation of the object.
        @return: A string representation of the object.
        @rtype: str
        """

        string = "["
        length = len(self)
        if length == 0:
            string += "]"
            return string
        else:
            string += " "
            for i in self:
                string += str(i)
                length -= 1
                if length != 0:
                    string += ", "
            string += " ]";
            return string;


    def __getitem__(self, key):
        """
        Underlying implementation of the C{__getitem__} Python function for
        the L{SList} container.
        @param key: index into the list. Indexing starts at 0.
        @type key: int
        @return: The appropriate Python Speect object instance for
        the list item (calls L{create_py_sobject}).
        @rtype: instance of Python L{SObject} or sub-class thereof.
        """

        if not isinstance(key, int):
            raise TypeError("list index must be of type 'int'")

        slen = self.__len__()
        if key >= slen:
            raise IndexError("list index out of range")

        spct_object = self._get_speect_object()
        if spct_object:
            item_object = py_slist_nth(spct_object, key);
            if not item_object:
                return None
            else:
                return create_py_sobject(item_object, False)
        else:
            return None


    def __setitem__(self, key, value):
        """
        Underlying implementation of the C{__setitem__} Python function for
        the L{SList} container. Does nothing if the L{SList} is not
        the owner of the C Speect object.
        @param key: index into the list. Indexing starts at 0.
        @type key: int
        @param value: The item to insert into the list.
        @type value: Python L{SObject} instance or sub-class thereof.
        """

        if not self._owner():
            return

        if not isinstance(key, int):
            raise TypeError("list index must be of type 'int'")

        slen = self.__len__()
        if key >= slen:
            raise IndexError("list index out of range")

        # value must be a Python SObject instance
        if not is_py_sobject(value):
            raise TypeError('Input argument \"value\" is not an ' +
                            'instance of type \'SObject\'')

        item_object = value._get_speect_object()
        spct_object = self._get_speect_object()

        if item_object and spct_object:
            # list is now the owner of the value's Speect object
            value._disown()
            py_slist_set_item(spct_object, item_object, key)


    def __delitem__(self, key):
        """
        Underlying implementation of the C{__delitem__} Python function for
        the L{SList} container. Does nothing if the L{SList} is not
        the owner of the C Speect object.
        @param key: index into the list. Indexing starts at 0.
        @type key: int
        """

        if not self._owner():
            return

        if not isinstance(key, int):
            raise TypeError("list index must be of type 'int'")

        slen = self.__len__()
        if key >= slen:
            raise IndexError("list index out of range")

        spct_object = self._get_speect_object()
        if spct_object:
            py_slist_del_item(spct_object, key)


    def __contains__(self, item):
        """
        Underlying implementation of the C{__contains__} Python function for
        the L{SList} container.
        @param item: Item to query if it is contained in the list.
        @type item: instance of Python L{SObject} or sub-class thereof.
        @return: I{True} or I{False}.
        @rtype: bool
        """        

        # item must be a Python SObject instance
        if not is_py_sobject(item):
            raise TypeError('Input argument \"item\" is not an ' +
                            'instance of type \'SObject\'')

        spct_object = self._get_speect_object()
        item_object = item._get_speect_object()

        if spct_object and item_object:
            return py_slist_constains(spct_object, item_object)
        else:
            return False


    def append(self, item):
        """
        Implementation of the C{append} Python function for
        the L{SList} container. Does nothing if the L{SList} is not
        the owner of the C Speect object.
        @param item: Item to append to the end of the list.
        @type item: instance of Python L{SObject} or sub-class thereof.
        """

        if not self._owner():
            return

        # item must be a Python SObject instance
        if not is_py_sobject(item):
            raise TypeError('Input argument \"item\" is not an ' +
                            'instance of type \'SObject\'')

        spct_object = self._get_speect_object()
        item_object = item._get_speect_object()

        if spct_object and item_object:
            # list is now the owner of the item's Speect object
            item._disown()
            py_slist_append(spct_object, item_object)


    def extend(self, list):
        """
        Implementation of the C{extend} Python function for
        the L{SList} container. Does nothing if the L{SList} is not
        the owner of the C Speect object.
        @param list: The list with which to extend this list.
        @type list: L{SList}.
        """

        if not self._owner():
            return

        # list must be a Python SList instance
        if not isinstance(list, SList):
            raise TypeError('Input argument \"list\" is not an ' +
                            'instance of type \'SList\'')

        spct_object = self._get_speect_object()
        list_object = list._get_speect_object()

        if spct_object and item_object:
            # note that list still owns it's Speect object
            py_slist_merge(spct_object, item_object)


    def insert(self, index, item):
        """
        Implementation of the C{insert} Python function for
        the L{SList} container. Does nothing if the L{SList} is not
        the owner of the C Speect object.
        @param index: index into the list. Indexing starts at 0.
        @type index: int
        @param item: The item to insert into the list.
        @type item: Python L{SObject} instance or sub-class thereof.
        """

        if not self._owner():
            return

        if not isinstance(index, int):
            raise TypeError("list index must be of type 'int'")

        slen = self.__len__()
        if index > slen:
            index = index;

        # item must be a Python SObject instance
        if not is_py_sobject(item):
            raise TypeError('Input argument \"item\" is not an ' +
                            'instance of type \'SObject\'')

        item_object = item._get_speect_object()
        spct_object = self._get_speect_object()

        if item_object and spct_object:
            # list is now the owner of the item's Speect object
            item._disown()

            if index == 0:
                py_slist_prepend(spct_object, item_object)
            elif index == slen:
                py_slist_append(spct_object, item_object)
            else:
                py_slist_insert_item(spct_object, item_object, index)


    def pop(self, index=None):
        """
        Implementation of the C{pop} Python function for
        the L{SList} container. Returns I{None} if the L{SList} is not
        the owner of the C Speect object.
        @param index: index into the list. Indexing starts at 0 (default is last item).
        @type index: int
        @return: The appropriate Python Speect object instance for
        the list item (calls L{create_py_sobject}) that has been poped.
        @rtype: Python L{SObject} instance or sub-class thereof.
        """

        if not self._owner():
            return None

        spct_object = self._get_speect_object()
        item_object = None

        if spct_object:
            if not index:
                # pop last item in list
                item_object = py_slist_pop_last_item(spct_object)
            else:
                if not isinstance(index, int):
                    raise TypeError("list index must be of type 'int'")

                slen = self.__len__()
                if index >= slen:
                    raise IndexError("list index out of range")

                item_object = py_slist_pop_item(spct_object, index)

            if not item_object:
                return None
            else:
                # returned item is no longer in list, so it owns the Speect object
                return create_py_sobject(item_object, True)
        else:
            return None


    def reverse(self):
        """
        Implementation of the C{reverse} Python function for
        the L{SList} container. Reverses the list in place.
        Does nothing if the L{SList} is not the owner of
        the C Speect object.
        """

        if not self._owner():
            return 

        spct_object = self._get_speect_object()
        if spct_object:
            py_slist_reverse(spct_object)
%}
