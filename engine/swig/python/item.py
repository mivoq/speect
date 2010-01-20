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
## Python class for SItem.                                                          ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


%pythoncode
%{

class SItem(SObject):
    """
    A class for containing the items (arbitrary objects).
    
    Items are nodes in a relation which is again linked to an utterance.
    
    Items contain a key-value (string/object) set of features that
    can be accessed as a dictionary type. For example:
    C{self.features[\"name\"]} to get the item's \"name\" feature, and
    C{self.features[\"number\"] = speect.SInt(31)} to set the \"number\" feature
    of the item to a Speect L{SInt} object with a value of 31.

    An Item can have a list of daughter items as well.
    Items can be shared between relations.
    
    @group As/In Relation: as_in_relation, in_relation
    @group List Traversal: next, prev, append, prepend
    @group Tree Traversal: parent, daughter, add_daughter
    @group Item Paths: path_to_item, path_to_feature, path_to_processor
    @group Miscelanuous: relation, utterance, voice
    """


    def __init__(self, rel=None, to_share=None, object=None, owner=False):
        """
        Construct a new SItem object. The input can either be:
           -   Python SRelation and an SItem (shared contents,
               default=I{None}), or a
           -   C Speect SItem type SObject and ownership status
               flag (default=I{False}).
        @param rel: The relation that the newly created item belongs to
        (default = I{None}).
        @type rel: L{SRelation}
        @param to_share: The item with which the newly created one shares
        it's content, if C{None} (default) then a new content is created.
        @type to_share: L{SItem}
        @param object: A pointer to a C-type Speect SItem type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SItem is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: The newly created item.
        @rtype: L{SItem}
        """

        rel_object = None
        to_share_object = None
  
        if (rel and object) or (not rel and not object):
            raise RuntimeError('Either argument \"rel\" or \"object\" '+\
                               'must not be \'None\'')

        if rel:
            # test that it is an Python SRelation object
            if not isinstance(rel, SRelation):
                raise TypeError('Input argument \"rel\" must be ' +
                                'of type \'SRelation\'')

            rel_object = rel._get_speect_object()

            if to_share:
                # test that it is an Python SItem object
                if not isinstance(to_share, SItem):
                    raise TypeError('Input argument \"to_share\" must be ' +
                                    'of type \'SItem\'')

                to_share_object = to_share._get_speect_object()

            item_object = py_sitem_new(rel_object, to_share_object)
            if item_object:
                # it is assumed that a item never owns it's
                # speect object, therefore it cannot delete it.
                super(SItem, self).__init__(item_object, False)
            else:
                raise RuntimeError('Failed to create new Speect SItem object')
            
        else:
            if not py_sobject_is_type(object, "SItem"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SItem\'')
            else:
                super(SItem, self).__init__(object, owner)

        # item features
        item_object = self._get_speect_object()
        if not item_object:
           raise RuntimeError("Cannot find item's Speect object")
       
        spct_item_feats_object = py_sitem_features(item_object)
        if not spct_item_feats_object:
            raise RuntimeError("Item does not have a Speect features object")
    
        self.features = SMapInternal(owner_object=item_object,
                                     spct_map_object=spct_item_feats_object,
                                     get_item_func=py_sitem_get_feature,
                                     set_item_func=py_sitem_set_feature,
                                     del_item_func=py_sitem_del_feature)
   
    def delete(self):
        """
        Destructor
        Calls the Speect C{S_FORCE_DELETE} macro on the Speect SItem*
        instance variable
        """

        spct_object = self._get_speect_object()
        if spct_object:
            py_sobject_force_delete(spct_object)
            self.__spct_object__ = None
            del self
        else:
            del self

            
    def as_in_relation(self, relname):
        """
        Find the item in the given relation that has the same shared content as this item.
        @param relname: The name of the relation in which to look.
        @type relname: str or unicode
        @return: The item as it is in the given relation.
        @rtype: L{SItem}
        """

        if not isinstance(relname, str) and not isinstance(relname, unicode):
            raise TypeError("relname must be of type 'str' or 'unicode'")

        spct_object = self._get_speect_object()
        if spct_object:
            item_object = py_sitem_as(spct_object, relname.encode('utf8'))
            if not item_object:
                return None
            else:
                return SItem(object=item_object, owner=False)
        else:
            return None

    def in_relation(self, relname):
        """
        Query if the item is in the named relation. 
        @param relname: The name of the relation in which to look.
        @type relname: str or unicode
        @return: I{True} or I{False}
        @rtype: bool
        """

        if not isinstance(relname, str) and not isinstance(relname, unicode):
            raise TypeError("relname must be of type 'str' or 'unicode'")

        spct_object = self._get_speect_object()
        if spct_object:
            return py_sitem_in(spct_object, relname.encode('utf8'))
        else:
            return False


    def next(self):
        """
        Get the item next to this one in the current relation.
        @return: The next item.
        @rtype: L{SItem}
        """
        print("in item, name = ", self.features["name"])
        spct_object = self._get_speect_object()
        if spct_object:
            item_object = py_sitem_next(spct_object)
            if not item_object:
                return None
            else:
                return SItem(object=item_object, owner=False)
        else:
            return None


    def prev(self):
        """
        Get the item previous to this one in the current relation.
        @return: The previous item.
        @rtype: L{SItem}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            item_object = py_sitem_prev(spct_object)
            if not item_object:
                return None
            else:
                return SItem(object=item_object, owner=False)
        else:
            return None


    def append(self, to_share=None):
        """
        Create a new item and append it after this one.
        The new item will share it's content with the given
        item I{to_share}.
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

            item_object = py_sitem_append(spct_object, to_share_object)
            if item_object:
                return SItem(object=item_object, owner=False)
            else:
                raise RuntimeError('Failed to append Speect SItem object')
        else:
            return None


    def prepend(self, to_share=None):
        """
        Create a new item and prepend it before this one.
        The new item will share it's content with the given
        item I{to_share}.
        @param to_share: The item with which the newly created item will
        share it's content. If C{None} (default) then a new content will
        be created for the prepended item.
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

            item_object = py_sitem_prepend(spct_object, to_share_object)
            if item_object:
                return SItem(object=item_object, owner=False)
            else:
                raise RuntimeError('Failed to prepend Speect SItem object')
        else:
            return None         


    def parent(self):
        """
        Get this item's parent item.
        @return: The parent item.
        @rtype: L{SItem}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            item_object = py_sitem_parent(spct_object)
            if item_object:
                return SItem(object=item_object, owner=False)
            else:
                return None
        else:
            return None


    def daughter(self, nth=0):
        """
        Get this item's daughter item.
        @param nth: The I{nth} daughter to get of this item.
        (default = C{None}).
           - If C{0} (default) then the first daughter is returned.
           - If C{-1} then the last daughter is returned.
           - If any positive integer number then the numbered daughter is returned.
        @return: The daughter item.
        @rtype: L{SItem}
        """

        print "in daughter(item), my swig info = " + str(self._get_speect_object())

        if not isinstance(nth, int):
            raise TypeError('Input argument \"nth\" must be ' +
                                    'of type \'int\'')

        if nth < -1:
            raise TypeError('Input argument \"nth\" must be equal or bigger than -1')

        spct_object = self._get_speect_object()
        
        if spct_object:
            if nth == 0:
                item_object = py_sitem_daughter(spct_object)
            elif nth == -1:
                item_object = py_sitem_last_daughter(spct_object)
            else:
                item_object = py_sitem_nth_daughter(spct_object, nth)
                       
            if item_object:
                print "daughter swig info = " + str(item_object)
                print "making Python daughter"
                daughter = SItem(object=item_object, owner=False)
                print "---Python daughter = " + str(daughter)
                print "---end Python daughter"
                return SItem(object=item_object, owner=False)
            else:
                print "daughter swig info = NO DAUGHTER"
                return None
        else:
            print "shit happend"
            return None


    def add_daughter(self, to_share=None):
        """
        Create a new item and add it as a daughter of this one.
        The new item will share it's content with the given
        item I{to_share}.
        @param to_share: The item with which the newly created item will
        share it's content. If C{None} (default) then a new content will
        be created for the daughter item.
        @type to_share: L{SItem}
        @return: Daughter item
        @rtype: L{SItem}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            to_share_object = None
            if to_share:
                if not isinstance(to_share, SItem):
                    raise TypeError('Input argument \"to_share\" must be ' +
                                    'of type \'SItem\'')
                to_share_object = to_share._get_speect_object()

            item_object = py_sitem_add_daughter(spct_object, to_share_object)
            if item_object:
                return SItem(object=item_object, owner=False)
            else:
                return None
        else:
            return None


    def to_string(self, prefix="", label="Item"):
        """
        Returns string representation of L{SItem} object.
        @returns: String representation of the item object.
        @rtype: str
        """

        print "my swig info = " + str(self._get_speect_object())

        print "in to_string(item)"
        label_spacing = ''
        
        for s in range(0, len(label)):
            label_spacing += ' '

        print "getting num_features"
        num_features = len(self.features)
        print "got num_features"
        
        if num_features > 0:
            stri = "%s%s: [ " %(prefix,label)
                  
            first = True
            count = num_features

            for ik in self.features:
                if not first:
                    stri += '\n%s%s    ' %(prefix, label_spacing)
                first = False
                count -= 1
                if count != 0:
                    stri += '%15.15s => %s,' %(ik, repr(self.features[ik]))
                else:
                    stri += '%15.15s => %s' %(ik, repr(self.features[ik]))
                
            if  num_features > 0:
                stri += '    ]\n'

        else:
            stri = ""
            
        print "getting daughter"
        i = self.daughter()
        print "got daughter"

        if i:
            print "daughter not none"
        else:
            print "daughter is none"
        
        n = 0
        daughter_prefix = '%s    ' %prefix
        
        while i:
            stri += '%s' %i.to_string(prefix=daughter_prefix, 
                                     label='Daughter')
            n += 1
            i = self.daughter(n)

        return stri


    def __str__(self):
        """
        Overloaded 'print' method
        @returns: String representation of L{SItem}.
        @rtype: str
        """
        print "in __str__(item)"
        return self.to_string()


    def path_to_item(self, path):
        """
        Extract an I{item}, from the given path, relative to this item.
        @param path: The string path to the desired item.
        @type path: str
        @returns: The item from the given path, relative to this item.
        @rtype: L{SItem}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            item_object = py_sitem_path_to_item(spct_object, path)
            if item_object:
                return SItem(object=item_object, owner=False)
            else:
                return None
        else:
            return None


    def path_to_feature(self, path):
        """
        Extract a I{feature}, from the given path, relative to this item.
        @param path: The string path to the desired item feature.
        @type path: str
        @returns: The feature from the given path, relative to this item.
        @rtype: instance of Python L{SObject} or sub-class thereof.
        """

        spct_object = self._get_speect_object()
        if spct_object:
            feature_object = py_sitem_path_to_feature(spct_object, path)
            if feature_object:
                return create_py_sobject(feature_object, False)
            else:
                return None
        else:
            return None


    def path_to_processor(self, path):
        """
        Execute a I{feature processor}, on the item from the
        given path, relative to this item. The name of the feature
        processor to execute is the last element of the path.
        @param path: The string path to the desired item, with the I{feature
        processor} as the last element of the path.
        @type path: str
        @returns: The feature extracted with the feature processor,
        from the given path, relative to this item.
        @rtype: instance of Python L{SObject} or sub-class thereof.
        """

        spct_object = self._get_speect_object()
        if spct_object:
            feature_object = py_sitem_path_to_extracted_feature(spct_object, path)
            if feature_object:
                # True as this is a new feature object
                return create_py_sobject(feature_object, True)
            else:
                return None
        else:
            return None
 

    def relation(self):
        """
        Get the item's relation.
        @return: The item's relation.
        @rtype: L{SRelation}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            rel_object = py_sitem_relation(spct_object)
            if rel_object:
                return SRelation(object=rel_object, owner=False)
            else:
                return None
        else:
            return None


    def utterance(self):
        """
        Get the item's utterance. 
        @return: The item's utterance, or C{None} if no utterance.
        @rtype: L{SUtterance}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            utt_object = py_sitem_utterance(spct_object)
            if utt_object:
                return SUtterance(object=utt_object, owner=False)
            else:
                return None
        else:
            return None

    def voice(self):
        """
        Get the item's voice. 
        @return: The item's utterance, or C{None} if no voice.
        @rtype: L{SVoice}
        """

        spct_object = self._get_speect_object()
        if spct_object:
            voice_object = py_sitem_voice(spct_object)
            if voice_object:
                return SVoice(object=voice_object, owner=False)
            else:
                return None
        else:
            return None
%}
