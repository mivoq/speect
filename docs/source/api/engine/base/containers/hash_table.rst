.. index:: 
   single: Simple Containers (C API); Hash Table

.. _base_hash_table:

==========
Hash Table
==========

Summary
=======

.. doxybridge-autosummary::
   :nosignatures:

   s_hash_table_new
   s_hash_table_delete
   s_hash_table_add
   s_hash_element_unlink
   s_hash_element_delete
   s_hash_table_size
   s_hash_table_resize
   s_hash_table_find
   s_hash_table_first
   s_hash_element_next
   s_hash_element_key
   s_hash_element_key_length	
   s_hash_element_get_data
   s_hash_element_set_data
   s_hash_table_stats
   s_hash_element_pos


Definition
==========

.. doxybridge:: s_hash_table

.. doxybridge:: s_hash_element
   

Create/Delete
=============

.. doxybridge:: s_hash_table_new

.. doxybridge:: s_hash_table_delete


Function Pointers
~~~~~~~~~~~~~~~~~

.. doxybridge:: s_hash_table_free_fp
   :type: typedef function pointer


Adding/Removing
===============

.. doxybridge:: s_hash_table_add

.. doxybridge:: s_hash_element_unlink
 
.. doxybridge:: s_hash_element_delete


Size/Resize
===========

.. doxybridge:: s_hash_table_size

.. doxybridge:: s_hash_table_resize


Accessing
=========

.. doxybridge:: s_hash_table_find

.. doxybridge:: s_hash_table_first

.. doxybridge:: s_hash_element_next


Element Key/Data
================

.. doxybridge:: s_hash_element_key

.. doxybridge:: s_hash_element_key_length

.. doxybridge:: s_hash_element_get_data

.. doxybridge:: s_hash_element_set_data


Miscellaneous
=============

.. doxybridge:: s_hash_table_stats

.. doxybridge:: s_hash_element_pos

