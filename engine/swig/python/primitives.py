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
## Python classes for primitives SInt, SFloat, SString and SVoid.                   ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


%pythoncode
%{

class SInt(SObject):
    """
    A class for the I{int} primitive data type.
    """

    def __init__(self, val=None, object=None, owner=False):
        """
        Construct a new SInt object. The input can either be a
        Python int type, or a C Speect SInt type SObject.
        @param val: Integer value for new SInt object.
        @type val: int
        @param object: A pointer to a C-type Speect SInt type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SInt is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: A new SInt instance.
        @rtype: L{SInt}
        """

        if val is not None:
            # test if val is an int
            if isinstance(val, int):
                new_spct_object = py_sint_set(val)

                # this is a new object, so this instance is
                # the owner for now
                super(SInt, self).__init__(new_spct_object, True) 
            else:
                raise TypeError('Input argument \"val\" must be ' +
                                'of type \'int\'')
        elif object:
            if not py_sobject_is_type(object, "SInt"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SInt\'')
            else:
                super(SInt, self).__init__(object, owner)
        else:
            raise RuntimeError('Invalid input arguments')


    def __repr__(self):
        """
        Return a string representation of the object.
        Calls SInt.get() internally.
        @return: A string representation of the object.
        @rtype: str
        """
        return str(self.get())


    def get(self):
        """
        Get the value of this SInt object.
        @return: The integer value of this SInt object instance.
        @rtype: int
        """

        spct_object = self._get_speect_object()
        if spct_object:
            return py_sint_get(spct_object)
        else:
            return None



class SFloat(SObject):
    """
    A class for the I{float} primitive data type.
    """

    def __init__(self, val=None, object=None, owner=False):
        """
        Construct a new SFloat object. The input can either be a
        Python int type, or a C Speect SFloat type SObject.
        @param val: Float value for new SFloat object.
        @type val: float
        @param object: A pointer to a C-type Speect SFloat type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SFloat is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: A new SFloat instance.
        @rtype: L{SFloat}
        """

        if val:
            # test if val is a float
            if isinstance(val, float) or isinstance(val, int):
                new_spct_object = py_sfloat_set(float(val))

                # this is a new object, so this instance is
                # the owner for now
                super(SFloat, self).__init__(new_spct_object, True) 
            else:
                raise TypeError('Input argument \"val\" must be ' +
                                'of type \'float\'')
        elif object:
            if not py_sobject_is_type(object, "SFloat"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SFloat\'')
            else:
                super(SFloat, self).__init__(object, owner) 
        else:
            raise RuntimeError('Invalid input arguments')


    def __repr__(self):
        """
        Return a string representation of the object.
        Calls SFloat.get() internally.
        @return: A string representation of the object.
        @rtype: str
        """
        return str(self.get())


    def get(self):
        """
        Get the value of this SFloat object.
        @return: The float value of this SFloat object instance.
        @rtype: float
        """

        spct_object = self._get_speect_object()
        if spct_object:
            return py_sfloat_get(spct_object)
        else:
            return None        



class SString(SObject):
    """
    A class for the I{char*} primitive data type.
    """

    def __init__(self, val=None, object=None, owner=False):
        """
        Construct a new SString object. The input can either be a
        Python str/unicode type, or a C Speect SString type SObject.
        @param val: String value for new SString object.
        @type val: unicode or str
        @param object: A pointer to a C-type Speect SString type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SString is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: A new SString instance.
        @rtype: L{SString}
        """

        if val:
            # test if val is a string type
            if isinstance(val, str) or isinstance(val, unicode):
                new_spct_object = py_sstring_set(val.encode('utf-8'))

                # this is a new object, so this instance is
                # the owner for now
                super(SString, self).__init__(new_spct_object, True) 
            else:
                raise TypeError('Input argument \"val\" must be ' +
                                'of type \'unicode\' or \'str\'')
        elif object:
            if not py_sobject_is_type(object, "SString"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SString\'')
            else:
                super(SString, self).__init__(object, owner) 
        else:
            raise RuntimeError('Invalid input arguments')


    def __repr__(self):
        """
        Return a string representation of the object.
        Calls SString.get() internally.
        @return: A string representation of the object.
        @rtype: str
        """
        return self.get()


    def get(self):
        """
        Get the value of this SString object.
        @return: The string value of this SString object instance.
        @rtype: unicode
        """

        spct_object = self._get_speect_object()
        if spct_object:
            string = py_sstring_get(spct_object)
            if string:
                return unicode(string, 'utf-8')
            else:
                return None
        else:
            return None



class SVoid(SObject):
    """
    A class for the I{void*} primitive data type.
    """

    def __init__(self, val=None, object=None, owner=False):
        """
        Construct a new SVoid object. The input can either be a
        Python object (anytype), or a C Speect SVoid type SObject.
        @param val: Any Python type object.
        @param object: A pointer to a C-type Speect SVoid type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SVoid is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: A new SVoid instance.
        @rtype: L{SVoid}
        """

        if val and not object:
            new_spct_object = py_svoid_set(val)
            # this is a new object, so this instance is
            # the owner for now
            super(SVoid, self).__init__(new_spct_object, True) 

        elif object and not val:
            if not py_sobject_is_type(object, "SVoid"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SVoid\'')
            else:
                super(SVoid, self).__init__(object, owner) 
        else:
            raise RuntimeError('Invalid input arguments')


    def get(self):
        """
        Get the value of this SVoid object.
        @return: The Python object value of this SVoid object
        instance.
        @rtype: Python object
        """

        spct_object = self._get_speect_object()
        if spct_object:
            return py_svoid_get(spct_object)
        else:
            return None
%}
