.. index:: 
   single: Simple Containers (C API); Buffer

.. _base_buffer:

======
Buffer
======

A buffer data structure with exponential growth that ensures that it
is always null padded.


Summary
=======

.. doxybridge-autosummary::
   :nosignatures:

   s_buffer_new
   s_buffer_delete
   s_buffer_append
   s_buffer_clear
   s_buffer_data
   s_buffer_size
   s_buffer_truncate


Definition
==========

.. doxybridge:: s_buffer


Create/Delete
=============

.. doxybridge:: s_buffer_new

.. doxybridge:: s_buffer_delete


Data Functions
==============

.. doxybridge:: s_buffer_append

.. doxybridge:: s_buffer_clear

.. doxybridge:: s_buffer_data


Miscellaneous
=============

.. doxybridge:: s_buffer_size

.. doxybridge:: s_buffer_truncate
