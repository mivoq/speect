######################################################################################
## Copyright (c) 2008-2009 The Department of Arts and Culture,                      ##
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
## DATE    : March 2009                                                             ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Python class for SObject.                                                        ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


%pythoncode
%{

class SObject(object):
    """
    The base Speect Engine class.
    All Speect Engine classes inherit from this class.
    """

    def __init__(self, object, owner):
        """
        Constructor.
        Create an instance of the SObject class. An SObject instance
        can only be initialized with a pointer to a C Speect SObject.
        @param object: A pointer to a C-type Speect SObject.
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SObject is the owner of the
        C-type Speect SObject (and can therefore delete it), otherwise I{False}.
        @type owner: bool
        @return: A new SObject instance.
        @rtype: L{SObject}
        """

        # class members
        self.__spct_object__ = object
        self.__this_own__ = owner

        # test if object is a Speect SObject
        try:
            is_spct_object = py_is_sobject(self.__spct_object__)
        except TypeError: # TypeError from SWIG
            self.__spct_object__ = None
            self.__this_own__ = False
            is_spct_object = False
            raise TypeError('Input argument \"object\" must be a ' +
                            'Speect C SObject pointer (SWIG) type')


    def __del__(self):
        """
        Destructor
        Calls the Speect C{S_DELETE} macro on the Speect SObject*
        instance variable
        """

#        print "del called on " + type(self)
#        spct_object = self._get_speect_object()
#        if self._owner() and spct_object:
#            py_sobject_delete(spct_object)
 #       elif spct_object:
#            self.__spct_object__ = None   # remove reference



    def __str__(self):
        """
        Get the Speect representation of this object's C Speect SObject.
        @return: The C Speect SObject representation.
        @rtype: unicode
        """
        spct_object = self._get_speect_object()
        if spct_object:
            return py_sobject_print(spct_object)
        else:
            return None


    def __eq__(self, other):
        """
        Compare two Python SObject's C Speect SObject pointers.
        @param other: An instance of the Python SObject (or sub-class thereof).
        @type other: L{SObject} instance (or sub-class).
        @return: I{True} or I{False}
        @rtype: bool
        """

        if isinstance(other, type(self)):
            other_spct_object = other.__spct_object__
            self_spct_object = self.__spct_object__
            if other_spct_object and self_spct_object:
                return py_sobject_compare(self_spct_object, other_spct_object)
        return False


    def __nonzero__(self):
        """
        Query the validity of the object. Checks if the object
        has a reference to a C Speect SObject.
        @return: I{True} or I{False}
        @rtype: bool
        """

        spct_object = self.__spct_object__
        if spct_object:
            return True
        else:
            return False


    def is_speect_type(self, type):
        """
        Query if given Speect object type is in this instance's
        inheritance tree.
        @param type: The Speect object type to check for.
        @type type: unicode or str
        @return: I{True} or I{False}
        @rtype: bool
        """

        if not isinstance(type, str) and not isinstance(type, unicode):
            raise TypeError('Input argument \"type\" is not of type ' +
                            '\'unicode\' or \'str\'')

        spct_object = self.__spct_object__
        if spct_object:
            return py_sobject_is_type(spct_object, type)
        else:
            return False


    def get_speect_type(self):
        """
        Get the Speect type this object
        @return: The C Speect SObject type.
        @rtype: unicode
        """

        spct_object = self.__spct_object__
        if spct_object:
            return py_sobject_get_type(spct_object)
        else:
            return None


    def _copy(self):
        """
        Copy this object. This operation depends on the C Speect object's
        implementation or lack thereof (some objects do not provide a copy
        method implementation), and therefore I{None} can be returned. 
        @note: This is a deep copy.
        @return: Copy of this object.
        @rtype: L{SObject}
        """

        spct_object = self.__spct_object__
        if spct_object:
            new_spct_object = py_sobject_copy(spct_object)
            if new_spct_object:
                return SObject(new_spct_object, True)

        return None


    def _owner(self):
        """
        Query the ownership of the C Speect SObject. If this object
        owns the C Speect SObject then it can edit it, and delete it,
        otherwise not.
        @return: I{True} or I{False}
        @rtype: bool
        """

        return self.__this_own__


    def _disown(self):
        """
        Remove the ownership status of this object's C Speect SObject.
        """

        self.__this_own__ = False


    def _get_speect_object(self):
        """
        Return the C Speect SObject assosiated with this object.
        If this object does not own the C Speect SObject then
        it returns I{None}.
        @return: A pointer to a C-type Speect SObject.
        @rtype: I{PySwigObject SObject*}
        """

        return self.__spct_object__


    def _dec_ref(self):
        """
        Decrement the object's C Speect SObject reference count.
        @warning: Dont use this if you're not sure what you are doing.
        """

        spct_object = self.__spct_object__
        owner = self.__this_own__
        if spct_object and owner:
            py_sobject_dec_ref(spct_object)


    def _inc_ref(self):
        """
        Increment the object's C Speect SObject reference count.
        @warning: Dont use this if you're not sure what you are doing.
        """

        spct_object = self.__spct_object__
        owner = self.__this_own__
        if spct_object and owner:
            py_sobject_inc_ref(spct_object)
%}
