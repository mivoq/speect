.. index:: 
   single: Strings (C API); String Lists

.. _string_lists:

============
String Lists
============


Definition
==========

Internally a string list is just an :c:type:`s_list` that has strings as list
elements. 

.. doxybridge:: s_str_list

.. doxybridge:: s_str_list_element




Create/Delete
=============

.. doxybridge:: s_str_list_new

.. doxybridge:: s_str_list_split

.. doxybridge:: s_str_list_delete


Accessing
=========

.. doxybridge:: s_str_list_find_index

.. doxybridge:: s_str_list_find
   :type: macro

.. doxybridge:: s_str_list_index
   :type: macro

.. doxybridge:: s_str_list_nth

.. doxybridge:: s_str_list_nth_string

.. doxybridge:: s_str_list_first

.. doxybridge:: s_str_list_last


Element functions
=================

.. doxybridge:: s_str_list_element_get

.. doxybridge:: s_str_list_element_replace

.. doxybridge:: s_str_list_element_unlink

.. doxybridge:: s_str_list_element_delete

.. doxybridge:: s_str_list_element_next

.. doxybridge:: s_str_list_element_prev


Query
=====

.. doxybridge:: s_str_list_isempty

.. doxybridge:: s_str_list_size


Queue
=====

.. doxybridge:: s_str_list_push

.. doxybridge:: s_str_list_pop

.. doxybridge:: s_str_list_reverse


Insertion
=========

.. doxybridge:: s_str_list_prepend

.. doxybridge:: s_str_list_append

.. doxybridge:: s_str_list_insert_before

.. doxybridge:: s_str_list_insert_after


Copy
====

.. doxybridge:: s_str_list_dup

.. doxybridge:: s_str_list_slice


Miscellaneous
=============

.. doxybridge:: s_str_list_cmp

.. doxybridge:: s_str_list_merge

.. doxybridge:: s_str_list_to_string
