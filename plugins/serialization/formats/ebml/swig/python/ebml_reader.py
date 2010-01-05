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

class SEbmlRead(speect.SObject):
    """
    An EBML reader class implementation.
    """

    
    def __init__(self, path=None, object=None, owner=False):
        """
        Constructor.
        @param path: Full path and file name of the ebml file to read.
        @type path: str
        @param object: A pointer to a C-type Speect SEbmlRead type
        SObject (default = I{None}).
        @type object: I{PySwigObject SObject*}
        @param owner: I{True} if the Python SEbmlRead is the owner of the
        C-type Speect SObject (and can therefore delete it),
        otherwise I{False} (default = I{False}).
        @type owner: bool
        @return: The newly created EBML reader.
        @rtype: L{SEbmlRead}
        """

        if (path and object) or (not path and not object):
            raise RuntimeError('Either argument \"path\" or '+\
                               '\"object\" must not be \'None\'')

        if path:
            if not isinstance(path, str):
                raise RuntimeError('The path argument must be a \'str\' type')

            ebml_object = py_sebml_reader_new(path)
            if not ebml_object:
                raise RuntimeError('Speect failed to create new \'SEbmlRead\' object')
   
            super(SEbmlRead, self).__init__(ebml_object, True)
        else:
            if not py_sobject_is_type(object, "SEbmlRead"):
                raise TypeError('Input argument \"object\" must be ' +
                                'of type \'C Speect SEbmlRead\'')
            else:
                super(SEbmlRead, self).__init__(object, owner)

        # set header info
        self.ebml_version = py_sebml_reader_get_header_version(ebml_object)
        self.reader_version = py_sebml_reader_get_header_reader_version(ebml_object)
        self.max_id_width = py_sebml_reader_get_header_max_id_width(ebml_object)
        self.max_size_width = py_sebml_reader_get_header_max_size_width(ebml_object)
        self.doc_type = py_sebml_reader_get_header_doctype(ebml_object)
        self.doc_type_version = py_sebml_reader_get_header_doctype_version(ebml_object)
        self.doc_type_reader_version = py_sebml_reader_get_header_doctype_reader_version(ebml_object)


    def peak_id(self):
        """
        Peek the I{id} of the next element.
        @return: The id of the next element.
        @rtype: int
        """

        return py_sebml_reader_peek_id(self._get_speect_object())


    def skip_element(self):
        """
        Skip the next element in the ebml file.
        """

        return py_sebml_reader_element_skip(self._get_speect_object())


    def read_uint(self):
        """
        Read unsigned integer.
        @returns: tuple [ ebml id, unsigned integer ]
        @rtype: tuple [ int, int ]
        """
        
        ebml_id = 0
        val = py_sebml_reader_read_uint(self._get_speect_object(), ebml_id)
        return [ ebml_id, val ]


    def read_sint(self):
        """
        Read signed integer.
        @returns: tuple [ ebml id, signed integer ]
        @rtype: tuple [ int, int ]
        """
        
        ebml_id = 0
        val = py_sebml_reader_read_sint(self._get_speect_object(), ebml_id)
        return [ ebml_id, val ]


    def read_double(self):
        """
        Read double.
        @returns: tuple [ ebml id, double ]
        @rtype: tuple [ int, float ]
        """
        
        ebml_id = 0
        val = py_sebml_reader_read_double(self._get_speect_object(), ebml_id)
        return [ ebml_id, val ]

  
    def read_str(self):
        """
        Read unicode string.
        @returns: tuple [ ebml id, string ]
        @rtype: tuple [ int, unicode ]
        """
        
        ebml_id = 0
        val = py_sebml_reader_read_utf8(self._get_speect_object(), ebml_id)
        return [ ebml_id, unicode(val, 'utf-8') ]


    def read_object(self):
        """
        Read an SObject type instance.
        @returns: tuple [ ebml id, object ]
        @rtype: tuple [ int, L{SObject}/sub-class ]
        """
        
        ebml_id = 0
        object = py_sebml_reader_read_utf8(self._get_speect_object(), ebml_id)
        pyobject = create_py_sobject(item_object, True)
        return [ ebml_id, pyobject ]
    

    def open_container(self):
        """
        Open an EBML container element and return it's id.
        @return: Container id.
        @rtype: int
        """
        
        return py_sebml_reader_read_container(self._get_speect_object())


    def container_exhausted(self):
        """
        Query if the elements in the container have been exhausted.
        @return: I{True} or I{False}.
        @rtype: bool
        """
        
        return py_sebml_reader_container_at_end(self._get_speect_object())
         
  
# register class with Speect Engine
speect.register_class(SEbmlRead)

%}
