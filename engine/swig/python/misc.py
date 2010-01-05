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
## Python wrappers miscellaneous Speect functions and helper functions.             ##
##                                                                                  ##
##                                                                                  ##
######################################################################################


%pythoncode
%{
import sys

# Doesn't work correctly, Python calls atexit before
# deleting Speect objects!
    
# register the speect quit function
#import atexit
#atexit.register(py_speect_quit)

def register_class(class_to_register):
    """
    Register a plug-in class with the Speect Engine
    Python interface. Call this function I{after} the
    class definition.
    @note: The class name must be unique, Speect classes
    should not be replaced.
    @param class_to_register: The class to register.
    @type class_to_register: sub-class of Python L{SObject}.
    """

    class_name = class_to_register.__name__
    
    if hasattr(sys.modules['speect'], class_name):
        raise RuntimeError('Class name is already present in Speect')
    
    if not issubclass(class_to_register, SObject):
        raise RuntimeError('Class must be a sub-class of Python SObject')

    setattr(sys.modules['speect'], class_name, class_to_register)

    
def is_py_sobject(object):
    """
    Query if the given object is an instance of
    the Python L{SObject} class or subclass thereof.
    @param object: The object to check
    @return: I{True} or I{False}
    @rtype: bool
    """

    return isinstance(object, SObject)


def create_py_sobject(object, owner):
    """
    Create the appropriate Python Speect object instance for
    the given raw Speect object. The object's Speect type is queried
    to get the correct Python Speect class.
    @param object: Pointer to the raw Speect object
    @type object: I{PySwigObject}
    @return: instance of Python SObject or sub-class thereof.
    @rtype: Python L{SObject}
    @note: This depends on the fact that the C Speect classes and the
    Python Speect classes have the same names.
    @note: If the appropriate Python Speect class is not found then
    the Python SObject base class is used.
    """

    found_class = False

    # get the speect object type name
    spct_type = str(py_sobject_get_type(object))

    try:
        py_spct_class = getattr(sys.modules['speect'], spct_type)
        found_class = True
    except NameError:
        found_class = False

    if not found_class:
        # try to find SObject class
        try:
            py_spct_class = getattr(sys.modules['speect'], 'SObject')
            spct_type = 'SObject'
        except NameError:
            failure_str = 'Failed to find Python Speect class \'' + spct_type \
                          + '\', then failed to find \'SObject\' class'
            raise RuntimeError(failure_str)

    py_spct_object = py_spct_class(object=object, owner=owner)
    if not py_spct_object:
        failure_str = 'Failed to create Python Speect object instance ' +\
        ' of class \'' + spct_type + '\''
        raise RuntimeError(failure_str)
    else:
        return py_spct_object
%}
