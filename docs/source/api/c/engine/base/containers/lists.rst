.. index:: 
   single: Simple Containers (C API); Doubly Linked List

.. _base_lists:

==================
Doubly Linked List
==================

A doubly linked list data structure definition and functions.


Summary
=======

.. doxybridge-autosummary::
   :nosignatures:

   s_list_new
   s_list_delete
   s_list_find_index
   macro s_list_find
   macro s_list_index
   s_list_nth
   s_list_first
   s_list_last
   s_list_element_get
   s_list_element_replace
   s_list_element_unlink
   s_list_element_delete
   s_list_element_next
   s_list_element_prev
   s_list_isempty
   s_list_size
   s_list_push
   s_list_pop
   s_list_reverse
   s_list_prepend
   s_list_append
   s_list_insert_before
   s_list_insert_after



Definition
==========

.. doxybridge:: s_list

.. doxybridge:: s_list_element


Create/Delete
=============

.. doxybridge:: s_list_new

.. doxybridge:: s_list_delete


Function Pointers
~~~~~~~~~~~~~~~~~

.. doxybridge:: s_list_compare_fp
   :type: typedef function pointer

.. doxybridge:: s_list_free_fp
   :type: typedef function pointer


Accessing
=========

.. doxybridge:: s_list_find_index

.. doxybridge:: s_list_find
   :type: macro

.. doxybridge:: s_list_index
   :type: macro

.. doxybridge:: s_list_nth

.. doxybridge:: s_list_first

.. doxybridge:: s_list_last


Element functions
=================

.. doxybridge:: s_list_element_get

.. doxybridge:: s_list_element_replace

.. doxybridge:: s_list_element_unlink

.. doxybridge:: s_list_element_delete

.. doxybridge:: s_list_element_next

.. doxybridge:: s_list_element_prev


Query
=====

.. doxybridge:: s_list_isempty

.. doxybridge:: s_list_size


Queue
=====

.. doxybridge:: s_list_push

.. doxybridge:: s_list_pop

.. doxybridge:: s_list_reverse


Insertion
=========

.. doxybridge:: s_list_prepend

.. doxybridge:: s_list_append

.. doxybridge:: s_list_insert_before

.. doxybridge:: s_list_insert_after


