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
## Python class for SMapInternal container.                                         ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


%pythoncode
%{

class SMapInternal(SMap):
    """
    A dictionary data container for C Speect SMap containers that are
    internal to other C Speect SObjects, for example the I{features} SMap
    container in L{SItem}. These dictionaries can be edited, but not deleted.
    """


    # SMapInternal class
    def __init__(self, owner_object, spct_map_object,
                 get_item_func=None, set_item_func=None,
                 del_item_func=None):
        """
        Construct a new SMapInternal object.
        @param owner_object: The C-type Speect SObject owner of this
        internal SMap container. For example the I{features} SMap
        container in L{SItem}, then the C Speect SItem is the owner.
        @type owner_object: I{PySwigObject SObject*}
        @param spct_map_object: The owner object's C Speect type map container.
        @param get_item_func: A function to get an item from the owner object
        (default=C{None}). Arguments are the owner object and a key. Returns
        an item that can be passed to L{create_py_sobject}.
        @param set_item_func: A function to set an item in the owner object's
        map container (default=C{None}). Arguments are the owner object,
        a key and a Python L{SObject} type value.
        @param del_item_func: A function to delete an item from the owner
        object's map container default=C{None}). Arguments are the owner
        object and a key.
        @return: A new SMapInternal instance.
        @rtype: L{SMapInternal}
        """
        
        if not py_is_sobject(owner_object):
            raise TypeError('Input argument \"owner_object\" must be ' +
                            'of type \'C Speect SObject\'')

        # a dummy class to act like a "struct"
        class _dummy:
            pass

        self._internal = _dummy()
        self._internal.owner = owner_object
        self._internal.get_item = get_item_func
        self._internal.set_item = set_item_func
        self._internal.del_item = del_item_func
        super(SMapInternal, self).__init__(spct_map_object, False)


    def __del__(self):
        """
        Destructor
        Remove local references
        """

 #       self._internal.owner = None
 #       self._internal.get_item = None
 #       self._internal.set_item = None
 #      self._internal.del_item = None
 #      super(SMapInternal, self).__del__()

        
    def __len__(self):
        """
        Underlying implementation of the C{len()} Python function for
        the L{SMapInternal} container.
        @return: The number of items in the container.
        @rtype: int
        """

        return py_smap_len(self._get_speect_object())
    

    def __getitem__(self, key):
        """
        Underlying implementation of the C{__getitem__} Python function for
        the L{SMapInternal} container.
        @param key: The string key into the map.
        @type key: str or unicode
        @return: The appropriate Python Speect object instance for
        the map item (calls L{create_py_sobject}).
        @rtype: instance of Python L{SObject} or sub-class thereof.
        """

        if not self._internal.get_item:
            return None

        if not isinstance(key, str) and not isinstance(key, unicode):
            raise TypeError("key must be of type 'str' or 'unicode'")

        key = key.encode('utf8')

        if not py_smap_key_present(self._get_speect_object(), key):
            string = "key \'" + key + "\' is not present in map"
            raise KeyError(string)

        item_object = self._internal.get_item(self._internal.owner, key);
        if item_object:
            return create_py_sobject(object=item_object, owner=False)
        else:
            raise RuntimeError("Failed to get item from Speect")


    def __setitem__(self, key, value):
        """
        Underlying implementation of the C{__setitem__} Python function for
        the L{SMapInternal} container.
        @param key: The string key into the map.
        @type key: str or unicode
        @param value: The item to insert into the map.
        @type value: Python L{SObject} instance or sub-class thereof.
        """

        if not self._internal.set_item:
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
        if item_object:
            # map is now the owner of the value's Speect object
            value._disown()
            self._internal.set_item(self._internal.owner, key, item_object)


    def __delitem__(self, key):
        """
        Underlying implementation of the C{__delitem__} Python function for
        the L{SMapInternal} container.
        @param key: The string key into the map.
        @type key: str or unicode
        """

        if not self._internal.del_item:
            return

        if not isinstance(key, str) and not isinstance(key, unicode):
            raise TypeError("key must be of type 'str' or 'unicode'")

        key = key.encode('utf8')

        if not py_smap_key_present(self._get_speect_object(), key):
            string = "key \'" + key + "\' is not present in map"
            raise KeyError(string)

        self._internal.del_item(self._internal.owner, key)
            
  
    def __contains__(self, item):
        """
        Underlying implementation of the C{__contains__} Python function for
        the L{SMapInternal} container.
        @param item: The string key to query if it is contained in the map.
        @type item: str or unicode
        @return: I{True} or I{False}.
        @rtype: bool
        """

        if not isinstance(item, str) and not isinstance(item, unicode):
            raise TypeError("key must be of type 'str' or 'unicode'")

        item = item.encode('utf8')
        return py_smap_key_present(self._get_speect_object(), key)


    def clear(self):
        """
        Implementation of the C{clear} Python function for
        the L{SMapInternal} container. 
        """

        for i in self:
            del self[i]


    def copy(self):
        pass
    

    def get(self, key, default=None):
        """
        Implementation of the C{get} Python function for
        the L{SMapInternal} container.
        @param key: The string key into the map.
        @type key: str or unicode
        @return: The appropriate Python Speect object instance for
        the map item (calls L{create_py_sobject}).
        @rtype: instance of Python L{SObject} or sub-class thereof.
        """

        if not self._internal.get_item:
            return None

        if not isinstance(key, str) and not isinstance(key, unicode):
            return default

        key = key.encode('utf8')

        if not py_smap_key_present(self._get_speect_object(), key):
            return default

        item_object = self._internal.get_item(self._internal.owner, key);
        if item_object:
            return create_py_sobject(item_object, False)
        else:
            raise RuntimeError("Failed to get item from Speect")
        

    def keys(self):
        pass


    def pop(self, key, default=None):
        pass
%}
