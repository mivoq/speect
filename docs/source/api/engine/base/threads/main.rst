.. index:: ! Threads (C API)

.. _threads/main:

=======
Threads
=======

Defines a set of macros to access multi-threaded functionality. The Speect Engine itself is
not multi-threaded, but it does need access to mutex locks to be thread safe, and thread id numbers
for logging purposes. Different multi-threaded implementations are supported by implementing the
following macros:

.. doxybridge:: S_DECLARE_MUTEX_STATIC
   :type: macro

.. doxybridge:: S_DECLARE_MUTEX
   :type: macro

.. doxybridge:: s_mutex_init
   :type: macro

.. doxybridge:: s_mutex_destroy
   :type: macro

.. doxybridge:: s_mutex_lock
   :type: macro

.. doxybridge:: s_mutex_unlock
   :type: macro

.. doxybridge:: s_thread_id
   :type: macro

