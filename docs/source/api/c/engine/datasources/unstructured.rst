.. _datasources/unstructered:

===========================
Unstructured Data Interface
===========================

Unstructured data interface for any type of data, for example reading
an integer from a file. There are currently two different
:ref:`implementations <implementations_datasource>` of the SDatasource
object.

Also see :doc:`../../../../examples/c/datasources_example`.


Definitions
===========

.. index::
   single: Data Sources (C API); SDatasource

.. doxybridge:: SDatasource
   :type: speect object
   :members: none
   :inheritance: SObject.SDatasource

.. toctree::
   :hidden:

   SDatasource_structure

:doc:`SDatasource_structure`

.. index:: 
   single: Data Sources (C API); SDatasourceClass

.. doxybridge:: SDatasourceClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SDatasourceClass

.. toctree::
   :hidden:

   SDatasourceClass_structure

:doc:`SDatasourceClass_structure`


.. doxybridge:: s_byte_order
   :type: typedef enum

.. doxybridge:: s_seek_mode
   :type: typedef enum



Macros
======

.. doxybridge:: S_DATASOURCE
  :type: macro



Functions
=========

Read
~~~~

.. doxybridge:: SDatasourceRead

.. doxybridge:: SDatasourceReadAt


Write
~~~~~

.. doxybridge:: SDatasourceWrite

.. doxybridge:: SDatasourceWriteAt


Position/Byte Order
~~~~~~~~~~~~~~~~~~~

.. doxybridge:: SDatasourceTell

.. doxybridge:: SDatasourceSeek

.. doxybridge:: SDatasourceSetByteOrder



Integer I/O
~~~~~~~~~~~

.. doxybridge:: s_uint8_read

.. doxybridge:: s_uint8_read_at

.. doxybridge:: s_uint8_write

.. doxybridge:: s_uint8_write_at

.. doxybridge:: s_uint16_read

.. doxybridge:: s_uint16_read_at

.. doxybridge:: s_uint16_write

.. doxybridge:: s_uint16_write_at

.. doxybridge:: s_uint32_read

.. doxybridge:: s_uint32_read_at

.. doxybridge:: s_uint32_write

.. doxybridge:: s_uint32_write_at

.. doxybridge:: s_sint8_read

.. doxybridge:: s_sint8_read_at

.. doxybridge:: s_sint8_write

.. doxybridge:: s_sint8_write_at

.. doxybridge:: s_sint16_read

.. doxybridge:: s_sint16_read_at

.. doxybridge:: s_sint16_write

.. doxybridge:: s_sint16_write_at

.. doxybridge:: s_sint32_read

.. doxybridge:: s_sint32_read_at

.. doxybridge:: s_sint32_write

.. doxybridge:: s_sint32_write_at


Floating Point I/O
~~~~~~~~~~~~~~~~~~

.. doxybridge:: s_float_read

.. doxybridge:: s_float_read_at

.. doxybridge:: s_float_write

.. doxybridge:: s_float_write_at

.. doxybridge:: s_double_read

.. doxybridge:: s_double_read_at

.. doxybridge:: s_double_write

.. doxybridge:: s_double_write_at


.. _implementations_datasource:

Implementations
===============

Two different implementations of the abstract SDatasource object:

.. index::
   single: Data Sources (C API); SFilesource

SFilesource
~~~~~~~~~~~

.. doxybridge:: SFilesource
   :type: speect object
   :members: none
   :inheritance: SObject.SDatasource.SFilesource
   

SFilesource Specific Functions
------------------------------

.. doxybridge:: SFilesourceOpenHandle
   
.. doxybridge:: SFilesourceOpenFile


.. index::
   single: Data Sources (C API); SMMapFilesource

SMMapFilesource
~~~~~~~~~~~~~~~

.. doxybridge:: SMMapFilesource
   :type: speect object
   :members: none
   :inheritance: SObject.SDatasource.SMMapFilesource

 

SMMapFilesource Specific Functions
----------------------------------

.. doxybridge:: SMMapFilesourceOpenFile
