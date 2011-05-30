
.. index:: 
   single: Data Sources (C API); SDatasource


===========
SDatasource
===========

.. toctree::
   :hidden:

   SDatasource_detail

   
Definitions
===========

.. doxybridge:: SDatasource
   :type: speect object
   :members: none
   :inheritance: SObject.SDatasource

.. doxybridge:: SDatasourceClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SDatasourceClass
 
.. seealso:: :doc:`SDatasource_detail`.


.. doxybridge:: s_byte_order
   :type: typedef enum


.. doxybridge:: s_seek_mode
   :type: typedef enum


Functions
=========

Read
----

.. doxybridge:: SDatasourceRead

.. doxybridge:: SDatasourceReadAt


Write
-----

.. doxybridge:: SDatasourceWrite

.. doxybridge:: SDatasourceWriteAt


Position/Byte Order
-------------------

.. doxybridge:: SDatasourceTell

.. doxybridge:: SDatasourceSeek

.. doxybridge:: SDatasourceSetByteOrder



Integer I/O
-----------

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
------------------

.. doxybridge:: s_float_read

.. doxybridge:: s_float_read_at

.. doxybridge:: s_float_write

.. doxybridge:: s_float_write_at

.. doxybridge:: s_double_read

.. doxybridge:: s_double_read_at

.. doxybridge:: s_double_write

.. doxybridge:: s_double_write_at


Macros
======

.. doxybridge:: S_DATASOURCE
  :type: macro

