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
## Python class for SRelation.                                                      ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


%pythoncode
%{

class SRelation(SObject):
    """
    A Relation is a named list of L{SItem}'s.
    An L{SUtterance} can hold an arbitrary number of Relations.
    """



    class _SRelationIterator(object):
        """
        An iterator class for L{SRelation} items.
        """
        
        def __init__(self, relation, forward):
            """
            Contructor.
            @param relation: The relation that must be iterated over.
            @type relation: L{SRelation}
            @param forward: I{True} if the iterator direction is forward
            over the relation items.
            @type forward: bool
            """

            if not isinstance(relation, SRelation):
                raise TypeError('Input argument \"relation\" is not an ' +
                                'instance of type \'SRelation\'')

            if not isinstance(forward, bool):
                raise TypeError('Input argument \"forward\" is not an ' +
                                'instance of type \'bool\'')

            if forward:
                self._item = relation._head()
            else:
                self._item = relation._tail()
            self._forward = forward
            

        def  __iter__(self):
            """
            Return the iterator itself (part of the Python iterator protocol).
            @return: self
            @rtype: L{_SRelationIterator}
            """

            return self


        def next(self):
            """
            Return the next item in the relation (part of the Python iterator protocol),
            raising the C{StopIteration} if at the end of the relation.
            @return: The next item in the relation.
            @rtype: L{SItem}
            """

            if not self._item:
                raise StopIteration
            else:
                item = self._item

                if self._forward:
                    self._item = self._item.next()
                else:
                    self._item = self._item.prev()
                
                return item
            

    # SRelation class
    def __init__(self, name="tmp", utt=None, object=None, owner=False):
        """
        Construct a new SRelation object. The input can either be:
           -   a name (default=C{\"tmp\"}) and Python SUtterance (default=C{None}), or a
           -   C Speect SRelation type SObject and ownership status
               flag (default=I{False}).
        @param name: The name of the relation (default = I{None}).
        @type name: str or unicode.
        @param utt: The utterance to which the relation belongs (default = I{None}).
        @type utt: L{SUtterance}
        @param object: A pointer to a C-type Speect SRelation type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SRelation is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: The newly created relation.
        @rtype: L{SRelation}
        """

        if object:
            if not py_sobject_is_type(object, "SRelation"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SRelation\'')
            else:
                super(SRelation, self).__init__(object, owner)

            return

        if not isinstance(name, str) and not isinstance(name, unicode):
            raise TypeError("name must be of type 'str' or 'unicode'")

        utt_object = None
        if utt:
            # test that it is an Python SUtterance object
            if not isinstance(utt, SUtterance):
                raise TypeError('Input argument \"utt\" must be ' +
                                'of type \'SUtterance\'')
            
            utt_object = utt._get_speect_object()

        relation_object = py_srelation_new(name.encode('utf8'), utt_object)
        if relation_object:
            super(SRelation, self).__init__(relation_object, True)
        else:
            raise RuntimeError('Failed to create new Speect SRelation object')


    def __del__(self):
        """
        Destructor. Overloads L{SObject.__del__} as relations must be
        I{force} deleted.
        Calls the Speect C{S_FORCE_DELETE} macro on the Speect SRelation*
        instance variable
        """

        spct_object = self._get_speect_object()
        if self._owner() and spct_object:
            py_srelation_delete(spct_object)
            self.__spct_object__ = None
        elif spct_object:
            self.__spct_object__ = None   # remove reference


    def to_string(self, prefix=""):
        """
        Returns string representation of L{SRelation} object.
        @returns: String representation of relation object.
        @rtype: str
        """
        
        str = "%sRelation \'%s\':\n" %(prefix, self.name())
        item_prefix = "%s    " %prefix
        for i in self:
            str += i.to_string(prefix=item_prefix)
            
        return str
        

    def __str__(self):
        """
        Overloaded 'print' method
        @returns: String representation of L{SRelation}.
        @rtype: str
        """
        
        return self.to_string()

    
    def name(self):
        """
        Get the relation's name.
        @return: The relation's name.
        @rtype: unicode
        """

        name = None
        spct_object = self._get_speect_object()
        if spct_object:
            name = py_srelaion_name(spct_object);

        return unicode(name)


    def utterance(self):
        """
        Get the relation's utterance.
        @return: The relation's utterance, or C{None} if no utterance.
        @rtype: L{SUtterance}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            utt_object = py_srelation_utterance(spct_object)
            if not utt_object:
                return None
            else:
                return SUtterance(object=utt_object, owner=False)
        else:
            return None


    def __iter__(self):
        """
        Underlying implementation of the C{__iter__} Python function for
        the L{SRelation} class.
        @return: A forward iterator to the L{SRelation} class.
        @rtype: L{_SRelationIterator}
        """

        return SRelation._SRelationIterator(self, forward=True)


    def __reversed__(self):
        """
        Underlying implementation of the C{__reversed__} Python function for
        the L{SRelation} class.
        @return: A reverse iterator to the L{SRelation} class.
        @rtype: L{_SRelationIterator}
        """

        return SRelation._SRelationIterator(self, forward=False)


    def __getitem__(self, key):
        """
        Underlying implementation of the C{__getitem__} Python function for
        the L{SRelation} class. Allows one to get an item of this
        relation with the key as an index to the items in this relation.
        @param key: The index of the required item.
           - if C{-1} then the last item in this relation is returned.
        @type key: int
        @return: The item in this relation associated with the index.
        @rtype: L{SItem}
        """

        if not isinstance(key, int):
            raise TypeError("key must be of type 'int'")

        if key < -1:
            raise IndexError('key must be equal or bigger than -1')

        if key == -1:
            # return tail
            return self._tail()

        if key == 0:
            # return head
            return self._head()

        # otherwise use a iterator
        count = 0
        item = None
        
        for i in self:
            if count == key:
                item = i
                break
            count += 1

        if not item:
            raise IndexError("key is out of range")
        else:
            return item


    def __delitem__(self, key):
        """
        Underlying implementation of the C{__delitem__} Python function for
        the L{SRelation} class. Allows one to delete an item from this
        relation with the key as an index to the items in this relation.
        @param key: The index of the required item.
           - if C{-1} then the last item in this relation is deleted.
        @type key: int
        """

        if not isinstance(key, int):
            raise TypeError("key must be of type 'int'")

        if key < -1:
            raise IndexError('key must be equal or bigger than -1')

        if key == -1:
            # return tail
            item = self._tail()
            if item:
                item.delete()
                return

        if key == 0:
            # return head
            item = self._head()
            if item:
                item.delete()
                return

        # otherwise use a iterator
        count = 0
        item = None
        
        for i in self:
            if count == key:
                item = i
                break
            count += 1

        if not item:
            raise IndexError("key is out of range")
        else:
            item.delete()
        
 
    def _head(self):
        """
        Get the item at the head of the relation.
        @return: The item at the head of the relation, or C{None} if no item.
        @rtype: L{SItem}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            item_object = py_srelation_head(spct_object)
            if not item_object:
                return None
            else:
                return SItem(object=item_object, owner=False)
        else:
            return None


    def _tail(self):
        """
        Get the item at the tail of the relation.
        @return: The item at the tail of the relation, or C{None} if no item.
        @rtype: L{SItem}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            item_object = py_srelation_tail(spct_object)
            if not item_object:
                return None
            else:
                return SItem(object=item_object, owner=False)
        else:
            return None


    def append(self, to_share=None):
        """
        Create a new item and append it after the items
        in this relation. The new item will share it's content
        with the given item I{to_share}.
        @param to_share: The item with which the newly created item will
        share it's content. If C{None} (default) then a new content will
        be created for the appended item.
        @type to_share: L{SItem}
        @return: Appended item
        @rtype: L{SItem}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            to_share_object = None
            if to_share != None:
                if not isinstance(to_share, SItem):
                    raise TypeError('Input argument \"to_share\" must be ' +
                                    'of type \'SItem\'')
                to_share_object = to_share._get_speect_object()

            item_object = py_srelation_append(spct_object, to_share_object)
            if item_object:
                return SItem(object=item_object, owner=False)
            else:
                raise RuntimeError('Failed to append Speect SItem object')
        else:
            return None


    def prepend(self, to_share=None):
        """
        Create a new item and prepend it before the items
        in this relation. The new item will share it's content
        with the given item I{to_share}.
        @param to_share: The item with which the newly created item will
        share it's content. If C{None} (default) then a new content will
        be created for the prepended item.
        @type to_share: L{SItem}
        @return: Prepended item
        @rtype: L{SItem}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            to_share_object = None
            if to_share != None:
                if not isinstance(to_share, SItem):
                    raise TypeError('Input argument \"to_share\" must be ' +
                                    'of type \'SItem\'')
                to_share_object = to_share._get_speect_object()

            item_object = py_srelation_prepend(spct_object, to_share_object)
            if item_object:
                return SItem(object=item_object, owner=False)
            else:
                raise RuntimeError('Failed to prepend Speect SItem object')
        else:
            return None
%}
