.. index:: ! Threads (C API)

.. _threads/main:

=======
Threads
=======

A set of macros to access multi-threaded functionality. The
Speect Engine itself is not multi-threaded, but it does need access
to mutex locks to be thread safe, and thread id's for logging
purposes. Different multi-threaded implementations are supported by
implementing the macros.


Summary
=======

.. doxybridge-autosummary::
   :nosignatures:

   macro S_DECLARE_MUTEX
   macro S_DECLARE_MUTEX_STATIC
   macro s_mutex_init
   macro s_mutex_destroy
   macro s_mutex_lock
   macro s_mutex_unlock
   macro s_thread_id


Definitions
===========

.. doxybridge:: s_mutex
   :type: typedef 


Declarations
============

.. doxybridge:: S_DECLARE_MUTEX
   :type: macro

.. doxybridge:: S_DECLARE_MUTEX_STATIC
   :type: macro


Initialize/Destroy
==================

.. doxybridge:: s_mutex_init
   :type: macro

.. doxybridge:: s_mutex_destroy
   :type: macro


Lock/Unlock
===========

.. doxybridge:: s_mutex_lock
   :type: macro

.. doxybridge:: s_mutex_unlock
   :type: macro


Miscellaneous
=============

.. doxybridge:: s_thread_id
   :type: macro

