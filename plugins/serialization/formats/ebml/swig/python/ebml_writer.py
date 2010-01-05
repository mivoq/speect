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
## DATE    : December 2009                                                          ##
##                                                                                  ##
######################################################################################
##                                                                                  ##
## Python class for SEbmlRead plug-in.                                              ##
##                                                                                  ##
##                                                                                  ##
######################################################################################



%pythoncode
%{

class SEbmlWrite(speect.SObject):
    """
    An EBML writer class implementation.
    """

    
    def __init__(self,
                 path=None,
                 ebml_version=1,
                 ebml_read_version=1,
                 max_id_width=4,
                 max_size_width=4,
                 doc_type=None,
                 doc_type_version=None,
                 doc_type_read_version=None,
                 object=None, owner=False):
        """
        Constructor.
        @param path: Full path and file name of the ebml file to write.
        @type path: str
        @param ebml_version: EBML version
        @type ebml_version: str
        @param ebml_read_version: EBML reader version.
        @type ebml_read_version: str
        @param max_id_width: Maximum width of ID fields.
        @type max_id_width: int
        @param max_size_width: Maximum width of size fields.
        @type max_size_width: int
        @param doc_type: Document type, eg. 'spct_phoneset'
        @type doc_type: str
        @param doc_type_version: Document type version.
        @type doctype_version: int
        @param doctype_read_version: Document type read version.
        @type doctype_read_version: int
        @param object: A pointer to a C-type Speect SEbmlWrite type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SEbmlWrite is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: The newly created EBML reader.
        @rtype: L{SEbmlWrite}
        """

        if (path and object) or (not path and not object):
            raise RuntimeError('Either argument \"path\" or '+\
                               '\"object\" must not be \'None\'')

        if path:
            if not isinstance(path, str):
                raise RuntimeError('The \'path\' argument must be a \'str\' type')

            if not isinstance(ebml_version, int):
                raise RuntimeError('The \'ebml_version\' argument must be an \'int\' type')

            if ebml_version <= 0:
                raise RuntimeError('The \'ebml_version\' argument must be >= 1')

            if not isinstance(ebml_read_version, int):
                raise RuntimeError('The \'ebml_read_version\' argument must be an \'int\' type')

            if ebml_read_version <= 0:
                raise RuntimeError('The \'ebml_read_version\' argument must be >= 1')

            if not isinstance(max_id_width, int):
                raise RuntimeError('The \'max_id_width\' argument must be an \'int\' type')
            
            if max_id_width <= 0 or max_id_width > 5:
                raise RuntimeError('The \'max_id_width\' argument must be >= 1 and <= 4')
            
            if not isinstance(max_size_width, int):
                raise RuntimeError('The \'max_size_width\' argument must be an \'int\' type')
            
            if max_size_width <= 0 or max_size_width > 5:
                raise RuntimeError('The \'max_size_width\' argument must be >= 1 and <= 4')

            if not isinstance(doc_type, str):
                raise RuntimeError('The \'doc_type\' argument must be a \'str\' type')

            if not isinstance(doc_type_version, int):
                raise RuntimeError('The \'doc_type_version\' argument must be an \'int\' type')
            
            if doc_type_version <= 0:
                raise RuntimeError('The \'doc_type_version\' argument must be >= 1')

            if not isinstance(doc_type_read_version, int):
                raise RuntimeError('The \'doc_type_read_version\' argument must be an \'int\' type')
            
            if doc_type_read_version <= 0:
                raise RuntimeError('The \'doc_type_read_version\' argument must be >= 1')
            
            ebml_object = py_sebml_writer_new(path, ebml_version, ebml_read_version,
                                              max_id_width, max_size_width, doc_type,
                                              doc_type_version, doc_type_read_version)
            if not ebml_object:
                raise RuntimeError('Speect failed to create new \'SEbmlWrite\' object')
   
            super(SEbmlWrite, self).__init__(ebml_object, True)
        else:
            if not py_sobject_is_type(object, "SEbmlWrite"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SEbmlWrite\'')
            else:
                super(SEbmlWrite, self).__init__(object, owner)


    def write_uint(self, id, val):
        """
        Write an unsigned integer.
        @param id: The id of the unsigned integer to write.
        @type id: int
        @param val: Unsigned integer to write.
        @type val: int
        """

        if not isinstance(id, int):
            raise RuntimeError('The \'id\' argument must be an \'int\' type')

        if id < 0:
            raise RuntimeError('The \'val\' argument must be > 0')

        if not isinstance(val, int):
            raise RuntimeError('The \'val\' argument must be an \'int\' type')
        
        if val < 0:
            raise RuntimeError('The \'val\' argument must be >= 0')
       
        py_sebml_writer_write_uint(self._get_speect_object(), id, val)
    

    def write_sint(self, id, val):
        """
        Write a signed integer.
        @param id: The id of the signed integer to write.
        @type id: int
        @param val: signed integer to write.
        @type val: int
        """

        if not isinstance(id, int):
            raise RuntimeError('The \'id\' argument must be an \'int\' type')

        if id < 0:
            raise RuntimeError('The \'val\' argument must be > 0')

        if not isinstance(val, int):
            raise RuntimeError('The \'val\' argument must be an \'int\' type')
      
        py_sebml_writer_write_suint(self._get_speect_object(), id, val)
   
        
    def write_double(self, id, val):
        """
        Write a double.
        @param id: The id of the double to write.
        @type id: int
        @param val: double to write.
        @type val: float
        """
        
        if not isinstance(id, int):
            raise RuntimeError('The \'id\' argument must be an \'int\' type')

        if id < 0:
            raise RuntimeError('The \'val\' argument must be > 0')

        if not isinstance(val, int) or not isinstance(val, float):
            raise RuntimeError('The \'val\' argument must be an \'int\' or \'float\' type')

   
        py_sebml_writer_write_double(self._get_speect_object(), id, val)
 
  
    def write_str(self, id, val):
        """
        Write a unicode string.
        @param id: The id of the string to write.
        @type id: int
        @param val: string to write.
        @type val: unicode or str
        """
        
        if not isinstance(id, int):
            raise RuntimeError('The \'id\' argument must be an \'int\' type')

        if id < 0:
            raise RuntimeError('The \'val\' argument must be > 0')

        if not isinstance(val, str) or not isinstance(val, unicode):
            raise RuntimeError('The \'val\' argument must be an \'str\' or \'unicode\' type')

        py_sebml_writer_write_utf8(self._get_speect_object(), id, val.encode('utf8'))


    def write_object(self, id, val):
        """
        Write an L{SObject} type. 
        @param id: The id of the object to write.
        @type id: int
        @param val: SObject to write
        @type val: L{SObject} or sub-class
        @note: The object must have a defined serialization class for
        the \'spct_ebml\' format or this method will raise an error.
        """
        
        if not isinstance(id, int):
            raise RuntimeError('The \'id\' argument must be an \'int\' type')

        if id < 0:
            raise RuntimeError('The \'val\' argument must be > 0')

        if not issubclass(val, speect.SObject):
            string = 'The \'val\' argument must be an instance of SObject, '+\
                     'or a sub-class thereof'
            raise RuntimeError(string)
        
        py_sebml_writer_write_object(self._get_speect_object(), id,
                                     val._get_speect_object())

        
    def start_container(self, id):
        """
        Start an EBML container element with the given id.
        @param id: The id of the object to write.
        @type id: int
        """

        if not isinstance(id, int):
            raise RuntimeError('The \'id\' argument must be an \'int\' type')

        if id < 0:
            raise RuntimeError('The \'val\' argument must be > 0')
      
        py_sebml_writer_start_container(self._get_speect_object(), id)


    def end_container(self):
        """
        End an already started container.
        """

        py_sebml_writer_stop_container(self._get_speect_object())
         
  
# register class with Speect Engine
speect.register_class(SEbmlWrite)

%}
