.. index:: Memory Allocation (C API)

.. _utils_alloc_c:

=================
Memory Allocation
=================

Macros for use in memory allocation and deallocation. These macros
wrap the standard ``malloc``, ``calloc``, ``realloc`` and ``free``
functions and should always be preferred over these functions.

Also see :ref:`examples/c/mem_example`.


Summary
=======

.. doxybridge-autosummary::
   :nosignatures:
   
   macro S_MALLOC
   macro S_MALLOC_SIZE
   macro S_CALLOC
   macro S_CALLOC_SIZE
   macro S_REALLOC
   macro S_REALLOC_SIZE
   macro S_FREE


Malloc
======

.. doxybridge:: S_MALLOC
   :type: macro

.. doxybridge:: S_MALLOC_SIZE
   :type: macro


Calloc
======

.. doxybridge:: S_CALLOC
   :type: macro

.. doxybridge:: S_CALLOC_SIZE
   :type: macro


Realloc
=======

.. doxybridge:: S_REALLOC
   :type: macro

.. doxybridge:: S_REALLOC_SIZE
   :type: macro


Free
====

.. doxybridge:: S_FREE
   :type: macro
