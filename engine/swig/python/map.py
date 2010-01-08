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
## Python class for SMap container.                                                 ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


%pythoncode
%{

class SMap(SContainer):
    """
    A dictionary data container for C Speect SObjects.
    Keys into the dictionary are strings (encoded internally as utf8).
    L{SIterator} is the iterator for the SMap class,
    sub-classes that want to use SIterator must define C{iterator_new_func} and
    C{iterator_get_item_func} attributes. See L{SIterator}.
    Iteration over the map is with keys, which are returned in
    unicode format.
    Emulates Python dictionary behaviour.
    """


    # SMap class
    def __init__(self, object=None, owner=False):
        """
        Construct a new SMap object. If no input is given then a
        new C Speect SMap (C{SMapMap}) is created, otherwise this
        class is an interface to an existing C Speect SMap type SObject.
        @note: If the C Speect SMap is an C{SMapHashTable} implementation
        (check with C{map.is_speect_type(\"SMapHashTable\")}), then the map
        can only be iterated in a forward direction.
        @param object: A pointer to a C-type Speect SMap type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SMap is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: A new SMap instance.
        @rtype: L{SMap}
        """

        if object:
            if not py_sobject_is_type(object, "SMap"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SMap\'')
            else:
                super(SMap, self).__init__(object, owner)
        else:
            new_spct_object = py_smap_new()
            if new_spct_object:
                # this is a new object, so this instance is
                # the owner for now
                super(SMap, self).__init__(new_spct_object, True)
            else:
                raise RuntimeError('Failed to create new Speect SMap object')

        # iterator functions
        self.new_iterator = py_smap_iterator_new
        self.iterator_get_item = py_smap_iterator_get_item


    def __len__(self):
        """
        Underlying implementation of the C{len()} Python function for
        the L{SMap} container.
        @return: The number of items in the container.
        @rtype: int
        """

        spct_object = self._get_speect_object()
        if spct_object:
            return py_smap_len(spct_object)
        return 0


    def __repr__(self):
        """
        Return a string representation of the object.
        @return: A string representation of the object.
        @rtype: str
        """

        string = "{"
        length = len(self)
        if length == 0:
            string += "}"
            return string
        else:
            for i in self:
                string += '\'' + str(i) + '\': ' + str(self.get(i))
                length -= 1
                if length != 0:
                    string += ", "
            string += "}";
            return string;


    def __getitem__(self, key):
        """
        Underlying implementation of the C{__getitem__} Python function for
        the L{SMap} container.
        @param key: The string key into the map.
        @type key: str or unicode
        @return: The appropriate Python Speect object instance for
        the map item (calls L{create_py_sobject}).
        @rtype: instance of Python L{SObject} or sub-class thereof.
        """

        if not isinstance(key, str) and not isinstance(key, unicode):
            raise TypeError("key must be of type 'str' or 'unicode'")

        key = key.encode('utf8')

        spct_object = self._get_speect_object()
        if spct_object:
            if not py_smap_key_present(spct_object, key):
                string = "key \'" + key + "\' is not present in map"
                raise KeyError(string)

            item_object = py_smap_get_item(spct_object, key);
            if item_object:
                return create_py_sobject(item_object, False)
            else:
                raise RuntimeError("Failed to get item from Speect")
        else:
            return None


    def __setitem__(self, key, value):
        """
        Underlying implementation of the C{__setitem__} Python function for
        the L{SMap} container. Does nothing if the L{SMap} is not
        the owner of the C Speect object.
        @param key: The string key into the map.
        @type key: str or unicode
        @param value: The item to insert into the map.
        @type value: Python L{SObject} instance or sub-class thereof.
        """

        if not self._owner():
            return

        if not isinstance(key, str) and not isinstance(key, unicode):
            raise TypeError("key must be of type 'str' or 'unicode'")

        # value must be a Python SObject instance
        if not is_py_sobject(value):
            raise TypeError('Input argument \"value\" is not an ' +
                            'instance of type \'SObject\'')
        
        key = key.encode('utf8')

        # value must be a Python SObject instance
        if not is_py_sobject(value):
            raise TypeError('Input argument \"value\" is not an ' +
                            'instance of type \'SObject\'')

        item_object = value._get_speect_object()
        spct_object = self._get_speect_object()

        if item_object and spct_object:
            # map is now the owner of the value's Speect object
            value._disown()
            py_smap_set_item(spct_object, key, item_object)


    def __delitem__(self, key):
        """
        Underlying implementation of the C{__delitem__} Python function for
        the L{SMap} container. Does nothing if the L{SMap} is not
        the owner of the C Speect object.
        @param key: The string key into the map.
        @type key: str or unicode
        """

        if not self._owner():
            return

        if not isinstance(key, str) and not isinstance(key, unicode):
            raise TypeError("key must be of type 'str' or 'unicode'")

        key = key.encode('utf8')

        spct_object = self._get_speect_object()
        if spct_object:
            if not py_smap_key_present(spct_object, key):
                string = "key \'" + key + "\' is not present in map"
                raise KeyError(string)

            py_smap_del_item(spct_object, key)


    def __contains__(self, item):
        """
        Underlying implementation of the C{__contains__} Python function for
        the L{SMap} container.
        @param item: The string key to query if it is contained in the map.
        @type item: str or unicode
        @return: I{True} or I{False}.
        @rtype: bool
        """

        if not isinstance(item, str) and not isinstance(item, unicode):
            raise TypeError("key must be of type 'str' or 'unicode'")

        item = item.encode('utf8')
        spct_object = self._get_speect_object()

        if spct_object:
            return py_smap_key_present(spct_object, item)
        else:
            return False


    def clear(self):
        """
        Implementation of the C{clear} Python function for
        the L{SMap} container. Does nothing if the L{SMap} is not
        the owner of the C Speect object.
        """

        if not self._owner():
            return

        for i in self:
            del self[i]


    def copy(self):
        """
        Implementation of the C{copy} Python function for
        the L{SMap} container.
        @return: A new Python L{SMap} container with shallow copies
        of the items in this container.
        @rtype: L{SMap}
        @note: The item's C Speect SObjects in the new map will be
        invalid as soon as the originals are deleted.
        """

        spct_object = self._get_speect_object()
        if spct_object:
            new_map_object = py_smap_copy(spct_object)
            if new_map_object:
                # True because the map is new. Objects will be ref
                # counted by Speect.
                return SMap(object=new_map_object, owner=True)
            else:
                raise RuntimeError('Failed to copy Speect SMap object')
        else:
            return None


    def get(self, key, default=None):
        """
        Implementation of the C{get} Python function for
        the L{SMap} container.
        @param key: The string key into the map.
        @type key: str or unicode
        @return: The appropriate Python Speect object instance for
        the map item (calls L{create_py_sobject}).
        @rtype: instance of Python L{SObject} or sub-class thereof.
        """

        if not isinstance(key, str) and not isinstance(key, unicode):
            return default

        key = key.encode('utf8')

        spct_object = self._get_speect_object()
        if spct_object:
            if not py_smap_key_present(spct_object, key):
                return default
            else:
                item_object = py_smap_get_item(spct_object, key)
                if item_object:
                    return create_py_sobject(item_object, False)
                else:
                    raise RuntimeError("Failed to get item from Speect")
        else:
            return default


    def keys(self):
        """
        Implementation of the C{keys} Python function for
        the L{SMap} container.
        @return: A list of the keys in the map.
        @rtype: L{SList}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            keys_object = py_smap_get_keys(spct_object)
            if keys_object:
                key_list = SList(object=keys_object, owner=True)
                return key_list
            else:
                return None
        else:
            return None


    def pop(self, key, default=None):
        """
        Implementation of the C{pop} Python function for
        the L{SMap} container. Returns I{default} if
        the L{SMap} is not the owner of the C Speect object.
        @param key: The string key into the map.
        @type key: str or unicode
        @return: The appropriate Python Speect object instance for
        the map item (calls L{create_py_sobject}) that has been poped.
        @rtype: Python L{SObject} instance or sub-class thereof.
        """

        if not self._owner():
            return default

        if not isinstance(key, str) and not isinstance(key, unicode):
            raise TypeError("key must be of type 'str' or 'unicode'")

        key = key.encode('utf8')

        spct_object = self._get_speect_object()
        if spct_object:
            if not py_smap_key_present(spct_object, key):
                if not default:
                    string = "key \'" + key + "\' is not present in map"
                    raise KeyError(string)
                else:
                    return default
            else:
                item_object = py_smap_unlink_item(spct_object, key)

                if item_object:
                    # returned item is no longer in map, so it owns the Speect object
                    return create_py_sobject(item_object, True)
                else:
                    return default
        else:
            return default
%}
